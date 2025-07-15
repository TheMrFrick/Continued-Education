-- API Info
apiUrl =
"https://op-api.carde.io/api/v1/decks/acceb67abe37a187f66a12f8a9ba4c6b1f8e9a054136469d1eb8c9d17e5b3dd23702f0f2d9f99ec40d09fce033a35238"
apiUrlEnd = "?formatResponse=tts&lookup=hash"

deckplanetUrl = "https://api.deckplanet.net/items/alpha_clash_decks/"
deckplanetPlayTestUrl = "https://api.deckplanet.net/items/alpha_clash_playtest_decks/"
endUrl = "?fields=*,deck_contender.*"

deckplanetAssetUrl = "https://multi-deckplanet.us-southeast-1.linodeobjects.com/alpha_clash/pngs/"
filetype = ".png"
cardbackUrl = "https://multi-deckplanet.us-southeast-1.linodeobjects.com/alpha_clash/pngs/default_card_back.png"


-- Default Settings
scriptName = "[1E87FF][Deckplanet Import][-] [i]"

-- Temp Settings
storaedID = ""
storedStarter = -1
storedDecks = {}
dropdownShown = false

debug = false

local contender_cards = {}
local main_cards = {}
local side_cards = {}

local zoneGUIDs = {
    Contender = "29ea1a",
    Deck = "5cea21"
}

local noteCardGUIDs = {
    Red = "423b37",
    Blue = "4e7f29"
}

function onLoad()
    local obj = getObjectFromGUID("116475")
    local fullXML = obj.UI.getXml()
    -- local uiXml = fullXML:match("<UI>(.-)</UI>") or ""
    local newUIAppend = [[
        <Panel width="500" height="50" position="-150 -150 -5" rotation="180 180 0" color="#00000000">
                <VerticalLayout>
                    <Toggle id="isPlaytestToggle" value="false" textColor="#FFFFFF" toggleWidth="40" toggleHeight="40" onValueChanged="setToggle">
                        <Text fontSize="30">
                            Is Playtester
                        </Text>
                    </Toggle>
                </VerticalLayout>
        </Panel>
    ]]

    self.UI.setXml(fullXML .. newUIAppend)
end

local isPlaytester = false

function setToggle(_, value, id)
    self.UI.setAttribute(id, "value", value)

    if id == "isPlaytestToggle" then
        isPlaytester = not isPlaytester
    end
end

--[[----------------------------------------------------------------------------

    API Handling
    Gets information from the API and spawns 2 decks with that information

------------------------------------------------------------------------------]]

-- Function that takes a given code and spawns both the decks, then announces it to the given player
function spawnDecks(code, pColor)
    if pColor == nil then pColor = "White" end
    if code == "" or code == nil then
        broadcastToColor(scriptName .. "No deck has been selected.[/i]", pColor)
        return false
    end

    if isPlaytester then
        local noteCard = getObjectFromGUID(noteCardGUIDs[pColor])
        local jwt = noteCard.getDescription():gsub("%s+", "")

        local headers = {
            Authorization = "Bearer" .. " " .. jwt,
            ["Content-Type"] = "application/json"
        }

        local TestCode = deckplanetPlayTestUrl .. code .. endUrl

        -- Makes the GET request
        WebRequest.custom(TestCode, "GET", true, JSON.encode({}), headers, function(request)
            -- Waits for download to be done
            if not webReqError(request) then
                Wait.condition(function()
                        local apiInfo = JSON.decode(request.text).data
                        --local pos1 = self.positionToWorld({x= 0,    y= 0.75, z=-0.092})
                        --local pos2 = self.positionToWorld({x=-1.83, y= 0.75, z=-0.092})
                        local pos = vector(-0.126, 0.75, 0)
                        local xOffset = 1.73

                        if apiInfo.deck_contender ~= nil then
                            table.insert(contender_cards, {
                                name = apiInfo.deck_contender.card_name,
                                imageUrl = deckplanetAssetUrl .. apiInfo.deck_contender.card_number .. filetype,
                                description = apiInfo.deck_contender.card_skill
                            })
                        end

                        if apiInfo.deck_cards ~= nil then
                            for i, card in pairs(apiInfo.deck_cards) do
                                local t = 0
                                while t < card.amount_in_deck do
                                    table.insert(main_cards, {
                                        name = card.card_name,
                                        imageUrl = deckplanetAssetUrl .. card.card_number .. filetype,
                                        description = card.card_skill
                                    })
                                    t = t + 1
                                end
                            end
                        end

                        if apiInfo.side_deck_cards ~= nil then
                            for _, card in pairs(apiInfo.side_deck_cards) do
                                local t = 0
                                while t < card.amount_in_deck do
                                    table.insert(side_cards, {
                                        name = card.card_name,
                                        imageUrl = deckplanetAssetUrl .. card.card_number .. filetype,
                                        description = card.card_skill
                                    })
                                    t = t + 1
                                end
                            end
                        end


                        -- Spawns the decks
                        local wait = 0
                        -- for i, deckList in pairs(apiInfo.decks) do
                        --     pos.x = pos.x - ((i-1)*xOffset)
                        --     local spawnPos = self.positionToWorld(pos)
                        --     wait = generateDeck(deckList, apiInfo.cardBackImageUrl, spawnPos, wait)
                        --     Wait.frames(function()
                        --         renameDeck(spawnPos, "")
                        --     end, 20 + wait*5)
                        -- end

                        local spawnPos = getObjectFromGUID(zoneGUIDs.Contender).getPosition()
                        wait = generateDeck(contender_cards, cardbackUrl, spawnPos, wait)


                        spawnPos = getObjectFromGUID(zoneGUIDs.Deck).getPosition()
                        wait = generateDeck(main_cards, cardbackUrl, spawnPos, wait)


                        spawnPos = self.positionToWorld(pos)
                        wait = generateDeck(side_cards, cardbackUrl, spawnPos, wait)


                        -- Waits for decks to be spawned, then renames them and announces
                        Wait.frames(function()
                            broadcastToColor(scriptName .. "Deck generation complete.[/i]", pColor)
                        end, 20 + wait * 5)
                    end,
                    -- Wait condition
                    function()
                        return request.download_progress == 1
                    end)
            end
        end)
    else
        -- Makes sure that the code is formatted correctly
        code = deckplanetUrl .. code .. endUrl

        -- Makes the GET request
        WebRequest.get(code, function(request)
            -- Waits for download to be done
            if not webReqError(request) then
                Wait.condition(function()
                        local apiInfo = JSON.decode(request.text).data
                        --local pos1 = self.positionToWorld({x= 0,    y= 0.75, z=-0.092})
                        --local pos2 = self.positionToWorld({x=-1.83, y= 0.75, z=-0.092})
                        local pos = vector(-0.126, 0.75, 0)
                        local xOffset = 1.73

                        if apiInfo.deck_contender ~= nil then
                            table.insert(contender_cards, {
                                name = apiInfo.deck_contender.card_name,
                                imageUrl = deckplanetAssetUrl .. apiInfo.deck_contender.card_number .. filetype,
                                description = apiInfo.deck_contender.card_skill
                            })
                        end

                        if apiInfo.deck_cards ~= nil then
                            for i, card in pairs(apiInfo.deck_cards) do
                                local t = 0
                                while t < card.amount_in_deck do
                                    table.insert(main_cards, {
                                        name = card.card_name,
                                        imageUrl = deckplanetAssetUrl .. card.card_number .. filetype,
                                        description = card.card_skill
                                    })
                                    t = t + 1
                                end
                            end
                        end

                        if apiInfo.side_deck_cards ~= nil then
                            for _, card in pairs(apiInfo.side_deck_cards) do
                                local t = 0
                                while t < card.amount_in_deck do
                                    table.insert(side_cards, {
                                        name = card.card_name,
                                        imageUrl = deckplanetAssetUrl .. card.card_number .. filetype,
                                        description = card.card_skill
                                    })
                                    t = t + 1
                                end
                            end
                        end


                        -- Spawns the decks
                        local wait = 0
                        -- for i, deckList in pairs(apiInfo.decks) do
                        --     pos.x = pos.x - ((i-1)*xOffset)
                        --     local spawnPos = self.positionToWorld(pos)
                        --     wait = generateDeck(deckList, apiInfo.cardBackImageUrl, spawnPos, wait)
                        --     Wait.frames(function()
                        --         renameDeck(spawnPos, "")
                        --     end, 20 + wait*5)
                        -- end

                        local spawnPos = getObjectFromGUID(zoneGUIDs.Contender).getPosition()
                        wait = generateDeck(contender_cards, cardbackUrl, spawnPos, wait)


                        spawnPos = getObjectFromGUID(zoneGUIDs.Deck).getPosition()
                        wait = generateDeck(main_cards, cardbackUrl, spawnPos, wait)


                        spawnPos = self.positionToWorld(pos)
                        wait = generateDeck(side_cards, cardbackUrl, spawnPos, wait)


                        -- Waits for decks to be spawned, then renames them and announces
                        Wait.frames(function()
                            broadcastToColor(scriptName .. "Deck generation complete.[/i]", pColor)
                        end, 20 + wait * 5)
                    end,
                    -- Wait condition
                    function()
                        return request.download_progress == 1
                    end)
            end
        end)
    end
    contender_cards = {}
    main_cards = {}
    side_cards = {}
end

function webReqError(request)
    if request.is_error or (request.response_code >= 400 and request.response_code < 600) then
        log(request.error)
        printToAll(scriptName .. "Error retrieving cards from API. (" .. request.response_code .. ")[/i]", "Red")
        return true
    else
        return false
    end
end

-- Generates a given deck at a given position, returns amount of time necessary to wait
function generateDeck(deckJSON, cardBack, pos, wait)
    if wait == nil then wait = 0 end
    local rot = {
        x = self.getRotation().x,
        y = self.getRotation().y,
        z = 180
    }

    for _, card in pairs(deckJSON) do
        local name = card.name
        local imgUrl = card.imageUrl
        local description = card.description

        Wait.frames(function()
            spawnObject({
                type              = "Card",
                position          = pos,
                rotation          = rot,
                scale             = { 1.25, 1.25, 1.25 },
                sound             = false,
                callback_function = function(obj)
                    obj.setName(name)
                    obj.addTag("CardeioCard")
                    obj.addTag("Card")
                    obj.setCustomObject({
                        face = imgUrl,
                        back = cardBack
                    })
                    obj.setDescription(description)
                end
            })
        end, 5 * wait)

        wait = wait + 1
    end

    return wait
end

-- Finds and renames a deck in a given position
function renameDeck(pos, name)
    for _, castInfo in pairs(Physics.cast({
        origin       = pos,
        direction    = { 0, -1, 0 },
        type         = 2,
        size         = { 1, 1, 1 },
        max_distance = 1,
        debug        = debug
    })) do
        local obj = castInfo.hit_object
        if obj.type == "Deck" then
            obj.setName(name)
        end
    end
end

--[[----------------------------------------------------------------------------

    Button Handling
    Determines what each UI button does

------------------------------------------------------------------------------]]

-- Handles the various UI elements
function handleUI(pColor, input, id)
    -- Saves the given URL
    if id == "URL" then
        storedID = input
    end

    -- Spawns decks based off which button was clicked
    if id:sub(1, 6) == "Import" then
        spawnDecks(storedID, pColor.color)
    end
end

--[[----------------------------------------------------------------------------

    Helper Functions

------------------------------------------------------------------------------]]

-- Returns true if a given table contains the given value
function tableContains(tbl, val)
    for _, v in pairs(tbl) do
        if v == val then
            return true
        end
    end
    return false
end
