function onLoad()
    self.UI.setXml([[
        <HorizontalLayout width="3200" height="2225" position="0 0 -50">
            <!-- Panel for Multi-line Text Box -->
            <VerticalLayout width="1600" height="2000" position="500 0 -50">
                <Panel color="#00000000" width="1600" height="2000">
                    <InputField id="multilineInputField" fontSize="72" placeholder="- HOW TO IMPORT YOUR DECK -&#xA;&#xA;- In your internet browser, open Deck Planet&#xA;- Select your deck and click 'Export Deck'&#xA;- Click 'Copy to Clipboard'&#xA;- Paste copied deck here (Left Click on this box, CTRL+V)&#xA;- Click 'Import deck'&#xA;- Grab a deck box and put your cards into it&#xA;- Right click deck box and select 'Save Object'"
                        multiline="true" width="1600" height="1950" lineType="MultiLineNewLine"
                        textAlignment="UpperLeft" padding="20" textColor="#FFFFFF" backgroundColor="#000000" onValueChanged="InputValueChanged"/>
                </Panel>
                <!-- Panel for New Action Button -->
                <Panel color="#00000000" width="1600" height="200">
                    <Button id="newActionButton" width="1600" height="250" onClick="onImportDeck" color="#00000000">
                        <Image image="https://steamusercontent-a.akamaihd.net/ugc/2461857409029504801/13EFC5679841F2A1908B09668338A07BFC3A0B45/" />
                    </Button>
                </Panel>
            </VerticalLayout>
        </HorizontalLayout>
    ]])
end

function InputValueChanged(player, value, id)
    self.UI.setAttribute(id, "value", value)
end

local zoneGUIDs = {
    Leader = "5d10ec",
    ["Main Deck"] = "95b211",
    ["Z Deck"] = "33ad5f",
    ["Side Deck"] = "37a4a8",
    ["Default"] = "f9c685"
}

local boxGUIDs = {"58c569", "efcda3"}

local cardQueue = {}
local importInProgress = false
local currentCardIndex = 1
local importDelay = 0 -- Add delay between each card if needed

function onImportDeck(player, value, id)
    if importInProgress then
        broadcastToAll("Import already in progress, please wait.", {r=1, g=0, b=0})
        return
    end

    local deckListText = self.UI.getAttribute("multilineInputField", "value")
    if not deckListText or deckListText == "" then
        broadcastToAll("Please enter a valid deck list.", {r=1, g=0, b=0})
        return
    end

    importInProgress = true

    UI.setAttribute("customUI_import", "active", "true")
    
    cardQueue = parseDeckList(deckListText)
    currentCardIndex = 1
    Wait.time(processCardQueue, 0.001)
end

function importStart()
    broadcastToAll("Import started, please wait.", {r=0, g=1, b=0})
end

function parseDeckList(deckListText)
    local queue = {}
    local currentSection = "Main Deck"  -- Default section
    local isNextLineLeader = false  -- Flag to treat the next line as a leader card

    for line in string.gmatch(deckListText, "[^\r\n]+") do
        if line:match("^[Nn]ame") then
            isNextLineLeader = true  -- The next line is a leader card
        elseif isNextLineLeader then
            -- This line should be processed as the leader card
            local quantity, cardName, cardNumber = parseLine("1 " .. line)
            if quantity and cardName and cardNumber then
                table.insert(queue, {
                    name = cardName,
                    quantity = quantity,
                    cardNumber = cardNumber,
                    section = "Leader"
                })
            end
            isNextLineLeader = false
        else
            local sectionHeaderMain = line:match("^_____([^_]+)______$")
            local sectionHeader = line:match("^_____([^_]+)_____$")
            if sectionHeaderMain then
                currentSection = sectionHeaderMain:gsub("%s+", " "):trim()
            elseif sectionHeader then
                currentSection = sectionHeader:gsub("%s+", " "):trim()
            else
                local quantity, cardName, cardNumber = parseLine(line)
                if quantity and cardName and cardNumber and zoneGUIDs[currentSection] then
                    table.insert(queue, {
                        name = cardName,
                        quantity = quantity,
                        cardNumber = cardNumber,
                        section = currentSection
                    })
                end
            end
        end
    end
    return queue
end

function processCardQueue()
    if currentCardIndex <= #cardQueue then
        local cardInfo = cardQueue[currentCardIndex]
        if cardInfo then
            processCard(cardInfo.name, cardInfo.quantity, cardInfo.cardNumber, cardInfo.section)
        end
        currentCardIndex = currentCardIndex + 1
        Wait.time(processCardQueue, importDelay) -- Wait before processing the next card
    else
        importInProgress = false
        UI.setAttribute("customUI_import", "active", "false")
        print("Import complete.")
    end
end

function processCard(cardName, quantity, cardNumber, section)
    local zoneGUID = zoneGUIDs[section]
    local position = getObjectFromGUID(zoneGUID).getPosition()

    local cardObject, cardContainer = searchForCardInBags(cardNumber)

    if cardObject then
        for i = 1, quantity do
            cloneCard(cardObject, position)
        end
        cardContainer.putObject(cardObject)
    else
        print("Card not found with cardNumber:", cardNumber)
    end
end

function searchForCardInBags(cardNumber)
    local searchText = escapeSpecialCharacters(cardNumber):lower()
    for _, guid in ipairs(boxGUIDs) do
        local container = getObjectFromGUID(guid)
        if container then
            for _, item in ipairs(container.getObjects()) do
                if item.name:lower():find(searchText) then
                    return takeAndClone(item, container), container
                end
            end
        end
    end
    return nil
end

function takeAndClone(item, container)
    local position = container.getPosition()
    local tempPosition = {x = position.x, y = position.y + 0.15, z = position.z}
    return container.takeObject({
        guid = item.guid,
        position = tempPosition,
        smooth = false
    })
end

function cloneCard(cardObject, position)
    cardObject.clone({
        position = {x = position.x, y = 6, z = position.z},
        rotation = {x = 0, y = 0, z = 180},
        sound = false,
        smooth = false
    })
end

function escapeSpecialCharacters(s)
    return (s:gsub("([%(%)%.%%%+%-%*%?%[%^%$])", "%%%1"))
end

-- Helper function to trim strings (add this if you don't already have it)
function string:trim()
    return self:match("^%s*(.-)%s*$")
end

function parseLine(line)
    local quantity = 1  -- Default quantity to 1 for leader cards or similar cases
    local cardName = nil
    local cardNumber = nil

    -- Extract the quantity if the line starts with a number followed by a space
    local parts = {}
    for part in string.gmatch(line, "%S+") do
        table.insert(parts, part)
    end

    -- If the first part is numeric, it is the quantity
    if tonumber(parts[1]) then
        quantity = tonumber(table.remove(parts, 1))
    end

    -- Identify and extract the card number and card name
    cardName = {}  -- To accumulate parts of the card name
    local cardNumberFound = false
    for i = #parts, 1, -1 do  -- Iterate from the end to the beginning
        if parts[i]:match("^%[%w+%-%d+%]$") then  -- Matches card numbers in brackets like [FB01-035]
            cardNumber = parts[i]:match("%w+%-%d+")
            cardNumberFound = true
            break  -- Stop processing parts once card number is found
        elseif parts[i]:match("^%w+%-%d+$") then  -- Matches card numbers like BT1-004
            cardNumber = parts[i]
            cardNumberFound = true
            break  -- Stop processing parts once card number is found
        else
            table.insert(cardName, 1, parts[i])  -- Assume part of the card name until card number is found
        end
    end

    -- Concatenate remaining parts for the card name if needed
    if not cardNumberFound then
        cardName = table.concat(parts, " ")
    else
        cardName = table.concat(cardName, " ")
    end

    return quantity, cardName, cardNumber
end
