--[[ Lua code. See documentation: https://api.tabletopsimulator.com/ --]]
function onLoad(player, value, id)
    mastersSelected = ""
    blueLastCardHovered = nil
    redLastCardHovered = nil
    greenLastCardHovered = nil
    yellowLastCardHovered = nil
    deckBuilderSelected = false

    -- Deck Zones
    deckZones = {
        Blue = getObjectFromGUID("a06676"),
        Red = getObjectFromGUID("1f6d92"),
        Green = getObjectFromGUID("751f1a"),
        Yellow = getObjectFromGUID("5b503c")
    }

    -- Life Zones
    lifeZones = {
        Blue = getObjectFromGUID("0ea49b"),
        Red = getObjectFromGUID("711931"),
        Green = getObjectFromGUID("d498a2"),
        Yellow = getObjectFromGUID("fd84b1")
    }
    -- Drop Zones
    dropZones = {
        Blue = getObjectFromGUID("ce2e21"),
        Red = getObjectFromGUID("7734e0"),
        Green = getObjectFromGUID("6f2071"),
        Yellow = getObjectFromGUID("329ea0")
    }

    removeZones = {
        Blue = getObjectFromGUID("3a8519"),
        Red = getObjectFromGUID("f6946f"),
        Green = getObjectFromGUID("334e67"),
        Yellow = getObjectFromGUID("a34166")
    }

    -- Leader Zones
    leaderZones = {
        Blue = getObjectFromGUID("e06cbb"),
        Red = getObjectFromGUID("5942ee"),
        Green = getObjectFromGUID("0b4c6c"),
        Yellow = getObjectFromGUID("2aae75")
    }

    -- Z-Energy Zones
    zEnergyZones = {
        Blue = getObjectFromGUID("07d638"),
        Red = getObjectFromGUID("0ad75d")
    }

    -- Z-Deck Zones
    zDeckZones = {
        Blue = getObjectFromGUID("c834e9"),
        Red = getObjectFromGUID("f81c0c")
    }

    -- Warp Zones
    warpZones = {
        Blue = getObjectFromGUID("520c9f"),
        Red = getObjectFromGUID("feb895")
    }

    -- Main Zones
    mainZones = {
        Blue = getObjectFromGUID("663c0d"),
        Red = getObjectFromGUID("124816"),
        Green = getObjectFromGUID("c519e6"),
        Yellow = getObjectFromGUID("4fbdf0")
    }

    -- Energy Zones
    energyZones = {
        Blue = getObjectFromGUID("1c9eea"),
        Red = getObjectFromGUID("c2bdcf"),
        Green = getObjectFromGUID("e562a1"),
        Yellow = getObjectFromGUID("e11034")
    }

    -- Hand Zones
    handZones = {
        Blue = getObjectFromGUID("fe420e"),
        Red = getObjectFromGUID("d32ad0"),
        Green = getObjectFromGUID("c381dc"),
        Yellow = getObjectFromGUID("5adc90")
    }
    -- Shuffle buttons after search
    shuffleDeckButtons = {
        Blue = getObjectFromGUID("a7b432"),
        Red = getObjectFromGUID("a2d058"),
        Green = getObjectFromGUID("99e21e"),
        Yellow = getObjectFromGUID("c8f96f")
    }
    -- Shuffle buttons after search
    shuffleLifeButtons = {
        Blue = getObjectFromGUID("902683"),
        Red = getObjectFromGUID("fa6882"),
        Green = getObjectFromGUID("0a3566"),
        Yellow = getObjectFromGUID("be4be7")
    }

    nameTags = {
        Blue = getObjectFromGUID("1b9c1a"),
        Red = getObjectFromGUID("9d0390"),
        Green = getObjectFromGUID("fd70f6"),
        Yellow = getObjectFromGUID("606665")
    }

    checkFlip = true
    showSplash()
end

function onObjectEnterZone(zone, object)
    -- Ensure cards are face up when up in hand.
    local rotation = ""

    if (zone.guid == "b14b41" or zone.guid == "b0fac3") then
        rotation = { 0, 0, 0 }
    end
    if (zone.guid == "842be0" or zone.guid == "2b22c3") then
        rotation = { 0, 180, 0 }
    end
    if rotation ~= "" then
        object.setRotation(rotation)
    end
end

function showSplash()
    local seatedPlayers = Player.getPlayers()

    -- Check if the player joining is the first player
    if #seatedPlayers == 1 then
        -- This is the first player joining, so show the splash screen
        UI.setAttribute("splashScreen", "active", "true")
    end
end

function onUpdate()
    if checkFlip == true then
        -- Find the decks in the zones
        local redDeckZone = deckZones["Red"]
        local blueDeckZone = deckZones["Blue"]
        local redLifeZone = lifeZones["Red"]
        local blueLifeZone = lifeZones["Blue"]
        local greenDeckZone = deckZones["Green"]
        local yellowDeckZone = deckZones["Yellow"]
        local greenLifeZone = lifeZones["Green"]
        local yellowLifeZone = lifeZones["Yellow"]
        local redZDeckZone = zDeckZones["Red"]
        local blueZDeckZone = zDeckZones["Blue"]
        local redWarpZone = warpZones["Red"]
        local blueWarpZone = warpZones["Blue"]
        local redDropZone = dropZones["Red"]
        local blueDropZone = dropZones["Blue"]
        local greenDropZone = dropZones["Green"]
        local yellowDropZone = dropZones["Yellow"]
        local redZEnergyZone = zEnergyZones["Red"]
        local blueZEnergyZone = zEnergyZones["Blue"]

        local redDeckObjects = redDeckZone.getObjects()
        local blueDeckObjects = blueDeckZone.getObjects()
        local redLifeObjects = redLifeZone.getObjects()
        local blueLifeObjects = blueLifeZone.getObjects()
        local greenDeckObjects = greenDeckZone.getObjects()
        local yellowDeckObjects = yellowDeckZone.getObjects()
        local greenLifeObjects = greenLifeZone.getObjects()
        local yellowLifeObjects = yellowLifeZone.getObjects()
        local redZDeckObjects = redZDeckZone.getObjects()
        local blueZDeckObjects = blueZDeckZone.getObjects()
        local redWarpObjects = redWarpZone.getObjects()
        local blueWarpObjects = blueWarpZone.getObjects()
        local redDropObjects = redDropZone.getObjects()
        local blueDropObjects = blueDropZone.getObjects()
        local greenDropObjects = greenDropZone.getObjects()
        local yellowDropObjects = yellowDropZone.getObjects()
        local redZEnergyObjects = redZEnergyZone.getObjects()
        local blueZEnergyObjects = blueZEnergyZone.getObjects()

        -- Ensure the decks cannot be flipped.
        checkIfDeckFlipped(redDeckObjects, 0, 180)
        checkIfDeckFlipped(blueDeckObjects, 180, 180)
        checkIfDeckFlipped(redLifeObjects, 0, 180)
        checkIfDeckFlipped(blueLifeObjects, 180, 180)
        checkIfDeckFlipped(greenDeckObjects, 180, 180)
        checkIfDeckFlipped(yellowDeckObjects, 0, 180)
        checkIfDeckFlipped(greenLifeObjects, 180, 180)
        checkIfDeckFlipped(yellowLifeObjects, 0, 180)
        checkIfDeckFlipped(redZDeckObjects, 0, 180)
        checkIfDeckFlipped(blueZDeckObjects, 180, 180)
        checkIfDeckFlipped(redWarpObjects, 0, 0)
        checkIfDeckFlipped(blueWarpObjects, 180, 0)
        checkIfDeckFlipped(redDropObjects, 0, 0)
        checkIfDeckFlipped(blueDropObjects, 180, 0)
        checkIfDeckFlipped(greenDropObjects, 180, 0)
        checkIfDeckFlipped(yellowDropObjects, 0, 0)
        checkIfDeckFlipped(redZEnergyObjects, 90, 0)
        checkIfDeckFlipped(blueZEnergyObjects, 270, 0)
    end

    if mastersSelected then
        if Player["Blue"] then
            checkHoverObject("Blue")
        end

        if Player["Red"] then
            checkHoverObject("Red")
        end
    else
        if Player["Green"] then
            checkHoverObject("Green")
        end
        if Player["Yellow"] then
            checkHoverObject("Yellow")
        end
    end
end

function checkHoverObject(playerColor)
    local player = Player[playerColor]
    local cardObject

    if playerColor == "Blue" then
        cardObject = blueLastCardHovered
    elseif playerColor == "Red" then
        cardObject = redLastCardHovered
    elseif playerColor == "Green" then
        cardObject = greenLastCardHovered
    elseif playerColor == "Yellow" then
        cardObject = yellowLastCardHovered
    else
        return -- Invalid player color
    end

    if player.getHoverObject() == nil or cardObject ~= player.getHoverObject() then
        if cardObject ~= nil and cardObject.getButtons() then
            cardObject.clearButtons()
            cardObject.UI.setXml("")
        end
    end
end

-- This function is used for the on hover feature on battle cards.
function onObjectHover(player_color, object)
    local validColors = { Blue = true, Red = true, Yellow = true, Green = true }

    if validColors[player_color] and object then
        local playerZones = {
            leader = leaderZones[player_color],
            deck = deckZones[player_color],
            life = lifeZones[player_color],
            main = mainZones[player_color],
            energy = energyZones[player_color]
        }

        if not isObjectInZone(object, playerZones.leader) and not isObjectInZone(object, playerZones.deck) and
            not isObjectInZone(object, playerZones.life) and (isObjectInZone(object, playerZones.main) or
                isObjectInZone(object, playerZones.energy)) then
            if object.tag == "Card" or object.tag == "CardCustom" or object.tag == "Deck" then
                handleCardHover(player_color, object)
            else
                clearLastCardHovered(player_color, object)
            end
        end
    end
end

function handleCardHover(player_color, object)
    local currentRotation = object.getRotation()
    local positions = getPositions(currentRotation, player_color)

    clearLastCardHovered(player_color, object)
    setLastCardHovered(player_color, object)
    createHoverButtons(object, positions, currentRotation, player_color)
end

function getPositions(rotation, player_color)
    local x1, x2, x3 = 0.37, 0, -0.37
    local y1, y2, y3 = -0.5, -0.5, -0.5
    local z1, z2, z3 = 1.9, 1.9, 1.9
    local dropOffset = "-37 0"
    local bottomOffset = mastersSelected and "" or "37 0"
    local warpOffset = mastersSelected and "75 0" or ""
    local rotateY, rotateZ = "0", "180"
    local buttonPosition = "0 190"

    if mastersSelected then
        x1, x3 = 0.75, -0.75
        dropOffset = "-75 0"
    end

    local function setRotationValues(yValue, yRot, zRot, zPos1, zPos2, zPos3, dropOffsetVal, warpOffsetVal,
                                     bottomOffsetVal)
        rotateY, rotateZ = yRot, zRot
        y1, y2, y3 = yValue, yValue, yValue
        z1, z2, z3 = zPos1, zPos2, zPos3
        dropOffset = dropOffsetVal
        if mastersSelected then
            warpOffset = warpOffsetVal
        else
            bottomOffset = bottomOffsetVal
        end
    end

    local function setButtonPosition(xPos, yPos)
        buttonPosition = xPos .. " " .. yPos
    end

    if rotation.z < 190 and rotation.z > 170 then
        setRotationValues(0.5, "180", "180", 1.9, 1.9, 1.9, mastersSelected and "75 0" or "37 0", "-75 0", "-37 0")
        if (rotation.y < 190 and rotation.y > 170 and (player_color == "Blue" or player_color == "Green")) or (rotation.y < 10 and (player_color == "Red" or player_color == "Yellow")) then
            setButtonPosition("0", "190")
        else
            setButtonPosition("0", "-190")
            setRotationValues(0.5, "180", "0", -1.9, -1.9, -1.9, mastersSelected and "-75 0" or "-37 0", "75 0", "37 0")
        end
    end

    if (rotation.y < 275 and rotation.y > 265 and rotation.z ~= 180) or (rotation.y > 85 and rotation.y < 95 and rotation.z < 185 and rotation.z > 175) then
        if rotation.z < 190 and rotation.z > 170 then
            if player_color == "Blue" or player_color == "Green" then
                rotateZ = "90"
                setRotationValues(-0.5, "0", "90", -0.75, 0, 0.75, mastersSelected and "0 75" or "0 37", "0 -75", "0 -37")
            else
                rotateZ = "270"
                setRotationValues(-0.5, "0", "270", -0.75, 0, 0.75, mastersSelected and "0 -75" or "0 -37", "0 75",
                    "0 37")
            end
        else
            if player_color == "Blue" or player_color == "Green" then
                rotateZ = "270"
                setRotationValues(-0.5, "0", "270", 0.75, 0, -0.75, mastersSelected and "0 -75" or "0 -37", "0 75",
                    "0 37")
            else
                rotateZ = "90"
                setRotationValues(-0.5, "0", "90", 0.75, 0, -0.75, mastersSelected and "0 75" or "0 37", "0 -75", "0 -37")
            end
        end
        y1, y2, y3 = -0.5, -0.5, -0.5
        if player_color == "Blue" or player_color == "Green" then
            x1, x2, x3 = 1.5, 1.5, 1.5
            setButtonPosition("-150", "0")
        else
            x1, x2, x3 = -1.5, -1.5, -1.5
            setButtonPosition("150", "0")
        end
    end

    if ((rotation.y > 80 and rotation.y < 100) and rotation.z ~= 180) or (rotation.y < 275 and rotation.y > 265 and rotation.z < 185 and rotation.z > 175) then
        y1, y2, y3 = -0.5, -0.5, -0.5
        if rotation.z < 190 and rotation.z > 170 then
            if player_color == "Blue" or player_color == "Green" then
                rotateZ = "270"
                setRotationValues(-0.5, "0", "270", -0.75, 0, 0.75, mastersSelected and "0 -75" or "0 -37", "0 75",
                    "0 37")
            else
                rotateZ = "90"
                setRotationValues(-0.5, "0", "90", -0.75, 0, 0.75, mastersSelected and "0 75" or "0 37", "0 -75", "0 -37")
            end
        else
            if player_color == "Blue" or player_color == "Green" then
                rotateZ = "90"
                setRotationValues(-0.5, "0", "90", -0.75, 0, 0.75, mastersSelected and "0 75" or "0 37", "0 -75", "0 -37")
            else
                rotateZ = "270"
                setRotationValues(-0.5, "0", "270", -0.75, 0, 0.75, mastersSelected and "0 -75" or "0 -37", "0 75",
                    "0 37")
            end
        end
        if player_color == "Blue" or player_color == "Green" then
            setButtonPosition("150", "0")
            x1, x2, x3 = -1.5, -1.5, -1.5
        else
            setButtonPosition("-150", "0")
            x1, x2, x3 = 1.5, 1.5, 1.5
        end
    end

    if (rotation.y < 10 and (rotation.z > 350 or rotation.z < 10)) and (player_color == "Blue" or player_color == "Green") then
        setButtonPosition("0", "-190")
        z1, z2, z3 = -1.9, -1.9, -1.9
        rotateZ = "0"
        if mastersSelected then
            dropOffset = "75 0"
            warpOffset = "-75 0"
        else
            dropOffset = "37 0"
            bottomOffset = "-37 0"
        end
    end

    if (rotation.y < 190 and rotation.y > 170 and (rotation.z > 350 or rotation.z < 10)) and (player_color == "Red" or player_color == "Yellow") then
        setButtonPosition("0", "-190")
        z1, z2, z3 = -1.9, -1.9, -1.9
        rotateZ = "0"
        if mastersSelected then
            dropOffset = "75 0"
            warpOffset = "-75 0"
        else
            dropOffset = "37 0"
            bottomOffset = "-37 0"
        end
    end

    if rotation.z == 180 then
        rotateY = "180"
        y1, y2, y3 = 0.5, 0.5, 0.5
    end

    return { x1, x2, x3, y1, y2, y3, z1, z2, z3, dropOffset, bottomOffset, warpOffset, rotateY, rotateZ, buttonPosition }
end

function createHoverButtons(object, positions, rotation, player_color)
    local x1, x2, x3, y1, y2, y3, z1, z2, z3, dropOffset, bottomOffset, warpOffset, rotateY, rotateZ, buttonPosition =
    table.unpack(positions)
    local name = object.getName()
    local description = object.getDescription()

    if mastersSelected then
        if description:find("Type: Z%-") or name:find("Token") or name:find("Energy Marker") then
            object.UI.setXml('<Panel color="#00000000" width="225" height="75" rotation="0 0 0" position="' ..
                buttonPosition .. '">' ..
                '<Button id="sendtoRemoveButton" width="75" height="75" image="https://steamusercontent-a.akamaihd.net/ugc/2369543762731611462/554E4EE2D620AFC5C2BCF3A8CAD0445B8E23428A/" onClick="Global/clickSendToRemove" colors="#ffffff, #FFBF00, #A4A4A4" rotation="0 ' ..
                rotateY .. ' ' .. rotateZ .. '" active="true" />' ..
                '</Panel>')
        else
            object.UI.setXml('<Panel color="#00000000" width="225" height="75" rotation="0 0 0" position="' ..
                buttonPosition .. '">' ..
                '<Button id="sendToWarpButton" offsetXY="' ..
                warpOffset ..
                '" width="75" height="75" image="https://steamusercontent-a.akamaihd.net/ugc/2461857409029743484/5087B0DD5F128BD7F213515B2B7E1CEDDB411790/" onClick="Global/clickSendToWarp" colors="#ffffff, #FFBF00, #A4A4A4" rotation="0 ' ..
                rotateY .. ' ' .. rotateZ .. '" active="true" />' ..
                '<Button id="sendtoBottomButton" width="75" height="75" image="https://steamusercontent-a.akamaihd.net/ugc/2461857409029743381/9F5A4D3B73188E6C9062DF89F5380EBC170B76BD/" onClick="Global/clickSendToBottom" colors="#ffffff, #FFBF00, #A4A4A4" rotation="0 ' ..
                rotateY .. ' ' .. rotateZ .. '" active="true" />' ..
                '<Button id="sendtoDropButton" offsetXY="' ..
                dropOffset ..
                '" width="75" height="75" image="https://steamusercontent-a.akamaihd.net/ugc/2461857409029743430/E4D0FF09B51053E9B0ACCD5160909AF7F727625C/" onClick="Global/clickSendToDrop" colors="#ffffff, #FFBF00, #A4A4A4" rotation="0 ' ..
                rotateY .. ' ' .. rotateZ .. '" active="true" />' ..
                '</Panel>')
        end
    else
        object.UI.setXml('<Panel color="#00000000" width="225" height="75" rotation="0 0 0" position="' ..
            buttonPosition .. '">' ..
            '<Button id="sendtoBottomButton" offsetXY="' ..
            bottomOffset ..
            '" width="75" height="75" image="https://steamusercontent-a.akamaihd.net/ugc/2461857409029743381/9F5A4D3B73188E6C9062DF89F5380EBC170B76BD/" onClick="Global/clickSendToBottom" colors="#ffffff, #FFBF00, #A4A4A4" rotation="0 ' ..
            rotateY .. ' ' .. rotateZ .. '" active="true" />' ..
            '<Button id="sendtoDropButton" offsetXY="' ..
            dropOffset ..
            '" width="75" height="75" image="https://steamusercontent-a.akamaihd.net/ugc/2461857409029743430/E4D0FF09B51053E9B0ACCD5160909AF7F727625C/" onClick="Global/clickSendToDrop" colors="#ffffff, #FFBF00, #A4A4A4" rotation="0 ' ..
            rotateY .. ' ' .. rotateZ .. '" active="true" />' ..
            '</Panel>')
    end

    createDummyButtons(object, { x1, y1, z1 }, { x2, y2, z2 }, { x3, y3, z3 }, rotation, player_color)
end

function createDummyButtons(object, pos1, pos2, pos3, rotation, player_color)
    local positions = { pos1, pos2, pos3 }
    local middleButtonLabel = "Bottom Deck"
    local name = object.getName()
    local description = object.getDescription()
    local dropButtonLabel = "Drop"
    local warpButtonLabel = "Warp"

    if description:find("Type: Z%-") or name:find("Token") or name:find("Energy Marker") then
        middleButtonLabel = "Remove from game"
        dropButtonLabel = ""
        warpButtonLabel = ""
    end

    local labels = { dropButtonLabel, middleButtonLabel, warpButtonLabel }

    if ((rotation.y > 350 or rotation.y < 10) and (player_color == "Blue" or player_color == "Green")) or ((rotation.y > 170 and rotation.y < 190) and (player_color == "Red" or player_color == "Yellow")) or (((rotation.y > 260 and rotation.y < 280) or (rotation.y > 80 and rotation.y < 100)) and (player_color == "Blue" or player_color == "Green")) then
        labels = { warpButtonLabel, middleButtonLabel, dropButtonLabel }
    end

    for i, pos in ipairs(positions) do
        object.createButton({
            click_function = "dummyClicked",
            label = labels[i],
            tooltip = labels[i],
            color = { r = 1, g = 1, b = 1, a = 0 },
            position = pos,
            rotation = { 0, 0, 0 },
            width = 357,
            height = 357,
            font_size = 200,
            interactable = false,
            active = false
        })
    end
end

function clearLastCardHovered(player_color, object)
    local lastCard = {
        Blue = blueLastCardHovered,
        Red = redLastCardHovered,
        Green = greenLastCardHovered,
        Yellow = yellowLastCardHovered
    }

    if lastCard[player_color] and lastCard[player_color] ~= object then
        lastCard[player_color].clearButtons()
        lastCard[player_color].UI.setXml("")
    end
end

function setLastCardHovered(player_color, object)
    if player_color == "Blue" then
        blueLastCardHovered = object
    elseif player_color == "Red" then
        redLastCardHovered = object
    elseif player_color == "Green" then
        greenLastCardHovered = object
    elseif player_color == "Yellow" then
        yellowLastCardHovered = object
    end
end

function onObjectRotate(object, spin, flip, player_color, old_spin, old_flip)
    --print('onRotate called')
    object.clearButtons()
    object.UI.setXml("")

    Wait.time(function()
        onObjectHover(player_color, object)
    end, .75)
end

function dummyClicked(object, player_clicker_color, alt_click)
    -- This is a hback so the buttons that send to warp, bottom deck and drop don't execute code twice.
end

-- This function sends a card to a specified zone, and you can customize the y-offset and rotation.
function sendToZone(object, player_color, zoneType, yOffset, zRotation)
    local zones = {
        deck = deckZones,
        drop = dropZones,
        warp = warpZones,
        remove = removeZones
    }

    local deck = findDeckInZone(zones[zoneType][player_color])
    local zonePosition = zones[zoneType][player_color].getPosition()
    local zoneRotation = zones[zoneType][player_color].getRotation()
    local cardName = object.getName()

    if object.tag == "Deck" then
        cardName = object.getQuantity() .. " cards"
    end

    if deck and zoneType == "deck" then
        local deckPosition = deck.getPosition()
        -- Set the rotation of the card based on input parameter
        object.setRotation({ x = 0, y = 0, z = zRotation })

        -- Temporarily position the card relative to the deck before adding it
        object.setPosition({ x = deckPosition.x, y = deckPosition.y + yOffset, z = deckPosition.z })

        -- Delay the putObject to allow physics to update the object's position
        Wait.frames(function()
            deck.putObject(object)
        end, 1) -- 1 frame delay should be enough
        print("Player " .. player_color .. " has sent a card to the bottom of their deck.")
        object.clearButtons()
        object.UI.setXml("")
    elseif zoneType == "drop" or zoneType == "warp" then
        -- Set the rotation of the card to the rotation of zone
        object.setRotationSmooth({ x = 0, y = zoneRotation.y, z = 0 })

        -- Set position of the card to the position of zone
        object.setPositionSmooth({ x = zonePosition.x, y = zonePosition.y + yOffset, z = zonePosition.z })
        print("Player " .. player_color .. " has sent " .. cardName .. " to the their " .. zoneType .. ".")

        Wait.time(function()
            object.clearButtons()
            object.UI.setXml("")
        end, 0.1)
    elseif zoneType == "remove" then
        -- Set the rotation of the card to the rotation of zone
        object.setRotationSmooth({ x = 0, y = zoneRotation.y, z = 0 })
        -- Set position of the card to the position of zone
        object.setPositionSmooth({ x = zonePosition.x, y = zonePosition.y + yOffset, z = zonePosition.z })
        print("Player " .. player_color .. " has removed " .. cardName .. " from the game.")

        object.clearButtons()
        object.UI.setXml("")
    else
        print("No deck found in zone for player: " .. player_color)
    end
end

function clickSendToWarp(player, value, id)
    local player_color = player.color
    local lastCardHovered = nil

    local switch = {
        ["Blue"] = function()
            lastCardHovered = blueLastCardHovered
        end,
        ["Red"] = function()
            lastCardHovered = redLastCardHovered
        end,
        ["Green"] = function()
            lastCardHovered = greenLastCardHovered
        end,
        ["Yellow"] = function()
            lastCardHovered = yellowLastCardHovered
        end
    }
    switch[player_color]()

    sendToWarp(lastCardHovered, player_color, false)
end

function clickSendToRemove(player, value, id)
    local player_color = player.color
    local lastCardHovered = nil

    local switch = {
        ["Blue"] = function()
            lastCardHovered = blueLastCardHovered
        end,
        ["Red"] = function()
            lastCardHovered = redLastCardHovered
        end,
        ["Green"] = function()
            lastCardHovered = greenLastCardHovered
        end,
        ["Yellow"] = function()
            lastCardHovered = yellowLastCardHovered
        end
    }
    switch[player_color]()

    sendToRemove(lastCardHovered, player_color, false)
end

function clickSendToBottom(player, value, id)
    local player_color = player.color
    local shuffleButtonActive = shuffleDeckButtons[player_color].UI.getAttribute(player_color .. "DeckShuffle", "active")
    local switch = {
        ["Blue"] = function()
            lastCardHovered = blueLastCardHovered
        end,
        ["Red"] = function()
            lastCardHovered = redLastCardHovered
        end,
        ["Green"] = function()
            lastCardHovered = greenLastCardHovered
        end,
        ["Yellow"] = function()
            lastCardHovered = yellowLastCardHovered
        end
    }
    switch[player_color]()
    sendToBottom(lastCardHovered, player_color, false)

    if shuffleButtonActive then
        -- Correctly set the UI attribute to make the panel inactive
        shuffleDeckButtons[player_color].UI.setAttribute(player_color .. "DeckShuffle", "active", "false")

        -- Adjust the position of the checker object
        local position = shuffleDeckButtons[player_color].getPosition()
        shuffleDeckButtons[player_color].setPosition({ position.x, position.y + 100, position.z })
    end
end

function clickSendToDrop(player, value, id)
    local player_color = player.color
    local switch = {
        ["Blue"] = function()
            lastCardHovered = blueLastCardHovered
        end,
        ["Red"] = function()
            lastCardHovered = redLastCardHovered
        end,
        ["Green"] = function()
            lastCardHovered = greenLastCardHovered
        end,
        ["Yellow"] = function()
            lastCardHovered = yellowLastCardHovered
        end
    }
    switch[player_color]()
    sendToDrop(lastCardHovered, player_color, false)
end

-- Usage examples
function sendToBottom(object, player_clicker_color, alt_click)
    sendToZone(object, player_clicker_color, 'deck', -1, 180)
end

function sendToDrop(object, player_clicker_color, alt_click)
    sendToZone(object, player_clicker_color, 'drop', 2, 0)
end

function sendToWarp(object, player_clicker_color, alt_click)
    sendToZone(object, player_clicker_color, 'warp', 2, 0)
end

function sendToRemove(object, player_clicker_color, alt_click)
    sendToZone(object, player_clicker_color, 'remove', 2, 0)
end

-- onObjectDrop and onObjectPickUp fixes issue with onHover when a card is dropped/picked up on the battle field.
-- - The buttons on the card would not show up even though you were hovering over it.
function onObjectDrop(player_color, object)
    --print("onObjectDrop called")
    Wait.time(function()
        if Player[player_color].getHoverObject() == object then
            onObjectHover(player_color, object)
        end
    end, .3)
end

function onObjectPickUp(player_color, object)
    --print("onObjectPickUp called")
    if nil ~= object.getButtons() then
        object.clearButtons()
        object.UI.setXml("")
    end
end

function checkIfDeckFlipped(objectsInZone, yRotation, zRotation)
    for _, object in ipairs(objectsInZone) do
        -- Check if the object is a deck
        if object.tag == "Deck" or object.tag == "Card" or object.tag == "CardCustom" then
            local rotation = object.getRotation()
            -- Check if the Z rotation is not 180
            if rotation.z ~= zRotation then
                -- Correct the Z rotation to 180
                rotation.x = 0
                rotation.y = yRotation
                rotation.z = zRotation
                object.setRotation(rotation)
            end
        end
    end
end

function onSelectDeckImport(player, value, id)
    deckBuilderSelected = true

    Player[player.color].changeColor("Red")

    UI.setAttribute("splashScreen", "active", "false")
    changeUI("Setup")
end

function onSelectVersus(player, value, id)
    if mastersSelected then
        Player[player.color].changeColor("Red")
    else
        Player[player.color].changeColor("Yellow")
    end

    UI.setAttribute("splashScreen", "active", "false")
    changeUI("Setup")
end

function onSelectBack(player, value, id)
    -- Go back to first splash screen
    UI.setAttribute("splash1", "active", "true")
    UI.setAttribute("splash1Menu", "active", "true")
    UI.setAttribute("splashMasters", "active", "false")
    UI.setAttribute("splashFW", "active", "false")
    UI.setAttribute("splash2Menu", "active", "false")
end

function onSelectMasters(player, value, id)
    mastersSelected = true
    print("Masters selected.")
    -- Show 2nd splash screen
    UI.setAttribute("splash1", "active", "false")
    UI.setAttribute("splash1Menu", "active", "false")
    UI.setAttribute("splashMasters", "active", "true")
    UI.setAttribute("splash2Menu", "active", "true")
end

function onSelectFW(player, value, id)
    mastersSelected = false
    print("Fusion World selected.")
    -- Show 2nd splash screen
    UI.setAttribute("splash1", "active", "false")
    UI.setAttribute("splash1Menu", "active", "false")
    UI.setAttribute("splashFW", "active", "true")
    UI.setAttribute("splash2Menu", "active", "true")
end

function onObjectLeaveZone(zone, object)
    local topColor = ""
    local bottomColor = ""

    if mastersSelected then
        topColor = "Red"
        bottomColor = "Blue"
    else
        topColor = "Green"
        bottomColor = "Yellow"
    end
    -- Check if the object is in the deck or life zones for the specified player color
    local isInBottomDeckZone = isObjectInZone(object, deckZones[bottomColor])
    local isInBottomLifeZone = isObjectInZone(object, lifeZones[bottomColor])
    local isInTopDeckZone = isObjectInZone(object, deckZones[topColor])
    local isInTopLifeZone = isObjectInZone(object, lifeZones[topColor])

    local buttonToActivate = ""

    if isInBottomDeckZone or isInBottomLifeZone or isInTopDeckZone or isInTopLifeZone then
        local buttonObject = ""
        local color = bottomColor
        local deckType = "Deck"
        local buttonObject = ""

        if isInTopDeckZone or isInTopLifeZone then
            color = topColor
        end

        buttonObject = shuffleDeckButtons[color]

        if isInTopLifeZone or isInBottomLifeZone then
            deckType = "Life"
            buttonObject = shuffleLifeButtons[color]
        end

        buttonToActivate = color .. deckType .. "Shuffle"

        currentState = buttonObject.UI.getAttribute(buttonToActivate, "active")

        if buttonObject and currentState == "true" then
            local position = buttonObject.getPosition()

            -- Activate the shuffle button UI and update its position
            buttonObject.UI.setAttribute(buttonToActivate, "active", "false") -- Assuming this is correct, might need to adjust based on actual UI setup
            buttonObject.setPosition({ position.x, position.y + 100, position.z })
            print("Removed the shuffle button from " ..
            color .. "'s " .. deckType .. " since their " .. deckType .. " was moved.")
        end
    end
end

function onObjectSearchStart(object, player_color)
    local topColor = ""
    local bottomColor = ""

    if mastersSelected then
        topColor = "Red"
        bottomColor = "Blue"
    else
        topColor = "Green"
        bottomColor = "Yellow"
    end
    -- Check if the object is in the deck or life zones for the specified player color
    local isInBottomDeckZone = isObjectInZone(object, deckZones[bottomColor])
    local isInBottomLifeZone = isObjectInZone(object, lifeZones[bottomColor])
    local isInTopDeckZone = isObjectInZone(object, deckZones[topColor])
    local isInTopLifeZone = isObjectInZone(object, lifeZones[topColor])

    UI.setAttribute("customUI", "active", "false")
    UI.setAttribute("customUI_wait", "active", "true")

    local buttonToActivate = ""

    if isInBottomDeckZone or isInBottomLifeZone or isInTopDeckZone or isInTopLifeZone then
        local buttonObject = ""
        local color = bottomColor
        local deckType = "Deck"
        local buttonObject = ""

        if isInTopDeckZone or isInTopLifeZone then
            color = topColor
        end

        buttonObject = shuffleDeckButtons[color]

        if isInTopLifeZone or isInBottomLifeZone then
            deckType = "Life"
            buttonObject = shuffleLifeButtons[color]
        end

        buttonToActivate = color .. deckType .. "Shuffle"

        currentState = buttonObject.UI.getAttribute(buttonToActivate, "active")

        if buttonObject and currentState == "false" then
            local position = buttonObject.getPosition()

            -- Activate the shuffle button UI and update its position
            buttonObject.UI.setAttribute(buttonToActivate, "active", "true") -- Assuming this is correct, might need to adjust based on actual UI setup
            buttonObject.setPosition({ position.x, position.y - 100, position.z })

            print("Player " .. player_color .. " is searching a through " .. color .. "'s " .. deckType)
        end
    else
        print("Player " .. player_color .. " is searching a through some cards.")
    end
end

-- Function to check if the object is within the specified zone
function isObjectInZone(object, zone)
    local objectsInZone = zone.getObjects()
    for _, obj in ipairs(objectsInZone) do
        if obj == object then
            return true
        end
    end
    return false
end

function shuffleDeck(deckInfo)
    local zone = deckZones[deckInfo.Color]

    if deckInfo.DeckType == "Life" then
        zone = lifeZones[deckInfo.Color]
    end

    if not zone then
        print("Zone not found for color:", deckInfo.Color)
        return
    end

    local deckObjects = zone.getObjects()
    for _, item in ipairs(deckObjects) do
        if item.tag == 'Deck' then
            for i = 1, 10 do
                item.shuffle()
            end
            break
        end
    end
    print(deckInfo.Color .. "'s " .. deckInfo.DeckType .. " has been shuffled 10 times.")
end

function onObjectSearchEnd(object, player_color)
    UI.setAttribute("customUI", "active", "true")
    UI.setAttribute("customUI_wait", "active", "false")
end

function table.indexOf(table, value)
    for index, val in ipairs(table) do
        if val == value then
            return index
        end
    end
    return nil
end

function checkInSeat(playerColor, buttonName)
    if playerColor then
        if deckZones[playerColor] then
            return true
        else
            broadcastToColor("You must sit at a colored seat to " .. buttonName .. "!", playerColor, { r = 1, g = 0, b = 0 })
            return false
        end
    else
        broadcastToAll("Player must be seated to " .. buttonName .. "!", { r = 1, g = 0, b = 0 })
        return false
    end
end

-- Draws the player a card from their deck
function drawCard(player, value, id)
    checkFlip = false
    if checkInSeat(player.color, "draw a card") then
        local zoneObjects = deckZones[player.color].getObjects()
        for _, item in ipairs(zoneObjects) do
            if item.tag == 'Deck' then
                print("Player " .. player.color .. " drew 1 card.")
                item.deal(1, player.color)
            end
        end
        for _, item in ipairs(zoneObjects) do
            if item.tag == 'Card' then
                print("Player " .. player.color .. " drew 1 card.")
                item.deal(1, player.color)
            end
        end
    end
    Wait.time(checkFlipTrue, 1.5)
end

-- Draws the players intial starting hand from their deck
function drawStartingHand(player, value, id)
    checkFlip = false
    local playerColor = player.color
    if checkInSeat(playerColor, "draw starting hand") then
        -- Check the player's hand for cards
        local handObjects = Player[playerColor].getHandObjects()
        if #handObjects > 0 then
            print('Player ' .. playerColor .. ' already has cards in hand. No cards dealt.')
            return -- Exit the function early to avoid dealing more cards
        end

        local zoneObjects = deckZones[playerColor].getObjects()
        local deckFound = "false"
        for _, item in ipairs(zoneObjects) do
            if item.tag == 'Deck' then
                deckFound = "true"
                for i = 1, 10 do
                    item.shuffle()
                end
                print('Player ' .. playerColor .. ' has shuffled their deck 10 times.')

                Wait.time(function()
                    -- Deal the same number of cards that was put back into the deck
                    item.deal(6, playerColor)
                    print('Player ' .. playerColor .. ' has drawn their starting hand of 6 cards.')
                end, 1)
            end
        end

        if deckFound == "false" then
            print("There was no deck found for " .. playerColor .. ", not drawing starting hand.")
        end
    end
    Wait.time(checkFlipTrue, 2.0)
end

-- Set initial life to 8, will not place life if cards exist in life.
function setLife(player, value, id)
    if checkInSeat(player.color, "set initial life") then
        local playerColor = player.color
        local lifeZonePos = lifeZones[playerColor].getPosition()
        local deckObjects = deckZones[playerColor].getObjects()

        local lifeFound = false

        for _, item in ipairs(lifeZones[playerColor].getObjects()) do
            if item.tag == 'Deck' or item.tag == 'Card' or item.tag == 'CardCustom' then
                lifeFound = true
                break -- Stop the loop once a card, custom card, or deck is found
            end
        end

        if not lifeFound and checkInSeat(playerColor, "set initial life") then
            for _, item in ipairs(deckObjects) do
                if item.tag == 'Deck' then
                    for i = 1, 8 do
                        item.takeObject({ position = { lifeZonePos.x, lifeZonePos.y + 1, lifeZonePos.z } })
                    end
                    print("Player " .. playerColor .. " has set their 8 initial life.")
                    break -- Assuming you only want to take life from the first deck found
                end
            end
        else
            print("Player " .. playerColor .. " already has life! Not setting initial life.")
        end

        -- Check life in both zones after any life setting attempt or check
        Wait.time(checkAndUpdateUI, 2.5)
    end
end

function checkAndUpdateUI()
    local lifeFoundBottom = false
    local lifeFoundTop = false
    local topColor = ""
    local bottomColor = ""

    if mastersSelected then
        topColor = "Red"
        bottomColor = "Blue"
    else
        topColor = "Green"
        bottomColor = "Yellow"
    end

    for _, item in ipairs(lifeZones[topColor].getObjects()) do
        if item.tag == 'Deck' or item.tag == 'Card' or item.tag == 'CardCustom' then
            lifeFoundTop = true
            break
        end
    end

    for _, item in ipairs(lifeZones[bottomColor].getObjects()) do
        if item.tag == 'Deck' or item.tag == 'Card' or item.tag == 'CardCustom' then
            lifeFoundBottom = true
            break
        end
    end

    print("Checking life for UI update...")
    print("Life Found - Blue: ", lifeFoundBottom, ", Red: ", lifeFoundTop)

    if lifeFoundTop and lifeFoundBottom then
        changeUI("In Game")
    else
        print("Waiting for both players to have life before changing the UI for after setup phase.")
    end
end

function changeUI(phase)
    local setupButtonState = ""
    local inGameButtonState = ""
    local image = ""
    local overlay = ""
    local height = ""
    local handSizePanelOffset = ""
    local deckPanelOffset = ""
    local dropPanelOffset = ""
    local energyPanelOffset = ""

    if phase == "Setup" then
        setupButtonState = "true"
        inGameButtonState = "false"

        if mastersSelected then
            overlay = "hud_overlay_masters"
            height = "460"
            image =
            "https://steamusercontent-a.akamaihd.net/ugc/2457352616125671834/1A73639E4E402236B3DE2C13752ED5F50507B663/"
            handSizePanelOffset = "80 450"
            deckPanelOffset = "80 380"
            dropPanelOffset = "80 310"
            energyPanelOffset = ""
            UI.setAttribute("gainLife_button", "active", "false")
            UI.setAttribute("mill_button", "active", "false")
            UI.setAttribute("overrealm_button", "active", "false")
            UI.setAttribute("switchFW_button", "active", "true")
            UI.setAttribute("switchMasters_button", "active", "false")
            UI.setAttribute("hud_overlay_FW", "active", "false")
        else
            overlay = "hud_overlay_FW"
            height = "460"
            image =
            "https://steamusercontent-a.akamaihd.net/ugc/2461856773364733755/FCAE0A954F55608DC755E5EF4705234207EF6A4B/"
            handSizePanelOffset = "80 430"
            deckPanelOffset = "80 360"
            dropPanelOffset = "107 330"
            UI.setAttribute("gainLife_button", "active", "false")
            UI.setAttribute("switchFW_button", "active", "false")
            UI.setAttribute("mill_button", "active", "false")
            UI.setAttribute("overrealm_button", "active", "false")
            UI.setAttribute("switchMasters_button", "active", "true")
            UI.setAttribute("hud_overlay_masters", "active", "false")
        end
    else
        setupButtonState = "false"
        inGameButtonState = "true"

        if mastersSelected then
            overlay = "hud_overlay_masters"
            height = "554"
            image =
            "https://steamusercontent-a.akamaihd.net/ugc/2457352616125734975/5F522183F51ACC52B552261119A1CC9A965F2809/"
            handSizePanelOffset = "80 450"
            deckPanelOffset = "80 380"
            dropPanelOffset = "80 310"
            UI.setAttribute("overRealm_button", "active", "true")
            UI.setAttribute("mill_button", "active", "true")
            UI.setAttribute("gainLife_button", "active", "false")
            UI.setAttribute("drawCard_panel", "offsetXY", "-40 515")
            UI.setAttribute("untapAll_panel", "offsetXY", "-40 480")
            UI.setAttribute("drawLife_panel", "offsetXY", "-40 420")
        else
            overlay = "hud_overlay_FW"
            height = "499"
            image =
            "https://steamusercontent-a.akamaihd.net/ugc/2461856773364733065/7B244C68D21AE22BCDC3D6461079879A5FD13CED/"
            handSizePanelOffset = "80 430"
            deckPanelOffset = "80 360"
            dropPanelOffset = "107 330"
            UI.setAttribute("overRealm_button", "active", "false")
            UI.setAttribute("mill_button", "active", "false")
            UI.setAttribute("gainLife_button", "active", "true")
            UI.setAttribute("hud_overlay_masters", "active", "false")
            UI.setAttribute("drawCard_panel", "offsetXY", "-40 465")
            UI.setAttribute("untapAll_panel", "offsetXY", "-40 430")
            UI.setAttribute("drawLife_panel", "offsetXY", "-40 385")
        end
    end

    -- Update Overlay to use the larger version for 4 buttons
    UI.setAttribute(overlay, "active", "true")
    UI.setAttribute(overlay, "image", image)
    UI.setAttribute(overlay, "height", height)
    UI.setAttribute("hud_overlay_panel", "height", height)
    UI.setAttribute("zDeckPanel", "active", mastersSelected)
    UI.setAttribute("zEnergyPanel", "active", mastersSelected)
    UI.setAttribute("warpPanel", "active", mastersSelected)
    UI.setAttribute("handSizePanel", "offsetXY", handSizePanelOffset)
    UI.setAttribute("deckPanel", "offsetXY", deckPanelOffset)
    UI.setAttribute("dropPanel", "offsetXY", dropPanelOffset)

    UI.setAttribute("setLife_button", "active", setupButtonState)
    UI.setAttribute("mulligan_button", "active", setupButtonState)
    UI.setAttribute("drawStartingHand_button", "active", setupButtonState)
    UI.setAttribute("drawCard_button", "active", inGameButtonState)
    UI.setAttribute("untapAll_button", "active", inGameButtonState)
    UI.setAttribute("drawLife_button", "active", inGameButtonState)
    UI.setAttribute("takeCrit_button", "active", inGameButtonState)
    print("Updating UI for the " .. phase .. " phase.")
end

-- Draw 1 card from the players life
function drawLife(player, value, id)
    checkFlip = false
    local playerColor = player.color
    if checkInSeat(playerColor, "take a life") then
        local zoneObjects = lifeZones[playerColor].getObjects()
        for _, item in ipairs(zoneObjects) do
            if item.tag == 'Deck' then
                item.deal(1, playerColor)
                print("Player " .. playerColor .. " took 1 life.")
            end
        end
        for _, item in ipairs(zoneObjects) do
            if item.tag == 'Card' then
                item.deal(1, playerColor)
                print("Player " .. playerColor .. " took 1 life.")
            end
        end
    end

    Wait.time(checkFlipTrue, 1.0)
end

function checkFlipTrue()
    checkFlip = true
end

-- Takes a critical hit for the player
function takeCrit(player, value, id)
    if checkInSeat(player.color, "take a critical hit") then
        checkFlip = false
        local dropZonePos = dropZones[player.color].getPosition()
        local lifeObjects = lifeZones[player.color].getObjects()
        local dropObjects = dropZones[player.color].getObjects()

        -- This block will execute if no life is found, i.e., lifeFound is false
        for _, item in ipairs(lifeObjects) do
            if item.tag == 'Deck' then
                -- Adjusted to use a callback function for flipping
                item.takeObject({
                    position = { dropZonePos.x, dropZonePos.y, dropZonePos.z },
                    callback_function = function(obj)
                        obj.flip()
                        print("Player " .. player.color .. " has taken a CRITICAL hit.")
                    end
                })

                break -- Assuming you only want to take life from the first deck found
            end
        end
        Wait.time(checkFlipTrue, 2.0)
    end
end

-- Mill a card from players deck
function clickedMill(player, value, id)
    if checkInSeat(player.color, "take a critical hit") then
        checkFlip = false
        local dropZonePos = dropZones[player.color].getPosition()
        local deckObjects = deckZones[player.color].getObjects()
        local dropObjects = dropZones[player.color].getObjects()

        -- This block will execute if no life is found, i.e., lifeFound is false
        for _, item in ipairs(deckObjects) do
            if item.tag == 'Deck' then
                -- Adjusted to use a callback function for flipping
                item.takeObject({
                    position = dropZonePos,
                    callback_function = function(obj)
                        obj.flip()
                        print("Player " .. player.color .. " has milled 1 card.")
                    end
                })

                break -- Assuming you only want to take life from the first deck found
            end

            if item.tag == 'Card' or item.tag == 'Card' then
                item.setPositionSmooth(dropZonePos)
                Wait.time(function()
                    item.flip()
                end, 0.5)
                break
            end
        end
        Wait.time(checkFlipTrue, 2.0)
    end
end

-- Move entire drop to warp
function clickedOverRealm(player, value, id)
    if checkInSeat(player.color, "take a critical hit") then
        local warpZonePos = warpZones[player.color].getPosition()
        local warpObjects = warpZones[player.color].getObjects()
        local dropObjects = dropZones[player.color].getObjects()

        -- This block will execute if no life is found, i.e., lifeFound is false
        for _, item in ipairs(dropObjects) do -- Make sure this is the correct variable holding your deck
            if item.tag == 'Deck' then
                -- Directly set the deck's position to the drop zone
                item.setPositionSmooth(warpZonePos)
                print("Player " .. player.color .. " activated Over Realm and placed their drop in their warp.")
                break -- Assuming you only want to move the first deck found
            end
        end
    end
end

-- Used to mulligan a players hand
function mulligan(player, value, id)
    if checkInSeat(player.color, "mulligan") then
        local playerColor = player.color
        local deckZone = deckZones[playerColor]
        local objectsInZone = handZones[playerColor].getObjects()
        local cardCount = 0
        local cardsToRecycle = {}
        local lifeObjects = lifeZones[playerColor].getObjects()
        local lifeFound = false

        -- Check if any cards in life
        for _, item in ipairs(lifeObjects) do
            if item.tag == 'Deck' or item.tag == 'Card' or item.tag == 'CardCustom' then
                lifeFound = true
                break -- Stop the loop once a card, custom card, or deck is found
            end
        end

        -- Exit the function early to avoid doing a mulligan during an on going game
        if lifeFound then
            print('Player ' ..
            playerColor ..
            ' already has cards in their life.  Assuming game is currently in progress and not performing mulligan.')
            return
        end

        checkFlip = false

        for _, obj in ipairs(objectsInZone) do
            if obj.tag == 'Card' or obj.tag == 'CardCustom' then
                cardCount = cardCount + 1
                table.insert(cardsToRecycle, obj)
            end
        end

        -- If cards are found flipped face down, perform mulligan
        if cardCount > 0 then
            for _, card in ipairs(cardsToRecycle) do
                card.setRotation({ 0, 0, 180 })
                deckPosition = deckZone.getPosition()
                deckPosition.y = deckPosition.y + 1
                card.setPosition(deckPosition)
            end

            print("Player " .. playerColor .. " took a muligan and drew 6 new cards.")

            -- Add a delay to ensure cards are absorbed into the deck
            Wait.time(function()
                local mainDeck = findDeckInZone(deckZones[playerColor])

                if mainDeck then
                    print("Shuffling Player " .. playerColor .. "'s deck 10 times.")
                    for i = 1, 10 do
                        mainDeck.shuffle()
                    end
                    Wait.time(function()
                        -- Deal the same number of cards that was put back into the deck
                        mainDeck.deal(cardCount, playerColor)
                        print('Player ' ..
                        playerColor .. ' has successfully took a mulligan and drew ' .. cardCount .. ' cards.')
                        print('Setting life in 2 seconds to ensure you draw your cards...')
                        Wait.time(function()
                            setLife(player)
                        end, 2)
                    end, 1)
                else
                    print('No deck found in the specified deck zone.')
                end
            end, 2) -- Wait for 0.5 seconds before shuffling and dealing
        else
            print('Player ' .. playerColor .. ' attempted to mulligan, but they have no cards in their hand.')
        end
        Wait.time(checkFlipTrue, 4.0)
    end
end

-- Used in the mulligan function
function findDeckInZone(deckZone)
    local objectsInZone = deckZone.getObjects()

    for _, obj in ipairs(objectsInZone) do
        if obj.tag == 'Deck' or obj.tag == 'DeckCustom' or obj.tag == 'Card' or obj.tag == 'CardCustom' then
            return obj
        end
    end

    return nil
end

-- This function is used to when an untap all button is clicked
function untapAll(player, value, id)
    if checkInSeat(player.color, "untap all") then
        local energyObjects = energyZones[player.color].getObjects()
        local mainObjects = mainZones[player.color].getObjects()

        print("Untapping cards for Player " .. player.color)
        if player.color == "Red" or player.color == "Yellow" then
            untapCardsInScriptingZone(mainObjects, 360, player.color, "BATTLE")
            untapCardsInScriptingZone(energyObjects, 180, player.color, "ENERGY")
        else
            untapCardsInScriptingZone(mainObjects, 180, player.color, "BATTLE")
            untapCardsInScriptingZone(energyObjects, 360, player.color, "ENERGY")
        end
    end
end

-- This function is used to untap all cards in the specified zone
function untapCardsInScriptingZone(uptapZone_objects, rotationY, playerColor, untapType)
    local zone_objects = uptapZone_objects

    for _, obj in ipairs(zone_objects) do
        local cardName = obj.getName()
        local cardDescription = obj.getDescription()

        if (obj.tag == "Card" or obj.tag == "CardCustom" or obj.tag == "Deck") and
            not cardName:find("Energy Marker") and
            not cardName:find("P%-270") and
            not cardName:find("P%-271") and
            not cardDescription:find("Z%-EXTRA") and -- Adjusted for exact match
            (untapType == "ENERGY" or untapType == "BATTLE") then
            local finalRotation = { 0, rotationY, obj.getRotation().z }
            obj.setRotationSmooth(finalRotation)
        end
    end
end

function clickedEndTurn(player)
    if player.color == "Blue" then
        broadcastToAll('PASS TURN', { 0, 0, 1 })
    end
    if player.color == "Red" then
        broadcastToAll('PASS TURN', { 1, 0, 0 })
    end
end

function clickedNoResponse(player)
    if player.color == "Blue" then
        broadcastToAll('NO RESPONSE', { 0, 0, 1 })
    end
    if player.color == "Red" then
        broadcastToAll('NO RESPONSE', { 1, 0, 0 })
    end
end

function clickedNoCombos(player)
    if player.color == "Blue" then
        broadcastToAll('NO COMBOS', { 0, 0, 1 })
    end
    if player.color == "Red" then
        broadcastToAll('NO COMBOS', { 1, 0, 0 })
    end
end

function getMastersSelected()
    return mastersSelected
end

function clickedSwitchGames()
    local seatedPlayers = Player.getPlayers()
    local colorToChange = ""
    local zoneToCount = ""
    mastersSelected = not mastersSelected

    local game = "Fusion World"

    if mastersSelected then
        game = "Masters"
    end

    broadcastToAll("Switching to " .. game .. ".")
    print("Changing player seats to proper table.")
    for _, playerObject in ipairs(seatedPlayers) do
        -- Determine new color and position based on game mode
        if mastersSelected then
            if playerObject.color == "Green" then
                colorToChange = "Blue"
            else
                colorToChange = "Red"
            end
        else
            if playerObject.color == "Blue" then
                colorToChange = "Green"
            else
                colorToChange = "Yellow"
            end
        end

        -- Change player color first
        playerObject.changeColor(colorToChange)
    end
    changeUI("Setup")
    updateCardCountOnSwitch()
end

function onPlayerChangeColor(player_color)
    if player_color == "Blue" or player_color == "Red" or player_color == "Green" or player_color == "Yellow" or player_color == "White" then
        -- print("onPlayerChangeColor called " .. player_color)
        local seatedPlayers = Player.getPlayers()
        local position = { x = -30, y = 0, z = 0 }
        local yaw = 360
        local distance = 50
        local textLabel = ""
        local color = ""

        if player_color == "Red" then
            if deckBuilderSelected then
                position = { x = 0, y = 0, z = 0 }
                deckBuilderSelected = false
            end
            yaw = 180
        elseif player_color == "Yellow" then
            yaw = 180
            position = { x = 30, y = 0, z = 0 }
        elseif player_color == "Green" then
            position = { x = 30, y = 0, z = 0 }
        elseif player_color == "White" then
            yaw = 180
            position = { x = 0, y = 0, z = 0 }
            distance = 32
        end

        -- Reset player names on mats to empty
        nameTags["Blue"].TextTool.setValue(" ")
        nameTags["Red"].TextTool.setValue(" ")
        nameTags["Green"].TextTool.setValue(" ")
        nameTags["Yellow"].TextTool.setValue(" ")

        -- Loop through all players and output their name to proper the mat
        for _, playerObject in ipairs(seatedPlayers) do
            color = playerObject.color

            if color ~= "White" then
                textLabel = nameTags[color]
                textLabel.TextTool.setValue(playerObject.steam_name)
            end
        end

        -- Apply camera view after changing color
        Wait.time(function()
            Player[player_color].lookAt({
                position = position,
                pitch    = 45,
                yaw      = yaw,
                distance = distance,
                rotation = rotation
            })
        end, .01) -- Wait a very small bit to ensure the color change has been processed and does not interfere with the camera change.
    end
end

function onPlayerConnect(player)
    local seatedPlayers = Player.getPlayers()
    local colorToChange = ""

    if #seatedPlayers == 2 then
        -- If a 2nd person joins, seat them at the proper seat.
        for _, playerObject in ipairs(seatedPlayers) do
            if player.color ~= playerObject.color then
                if mastersSelected then
                    if playerObject.color == "Red" then
                        colorToChange = "Blue"
                    else
                        colorToChange = "Red"
                    end
                else
                    if playerObject.color == "Yellow" then
                        colorToChange = "Green"
                    else
                        colorToChange = "Yellow"
                    end
                end

                -- Apply camera view after changing color
                Wait.time(function()
                    player.changeColor(colorToChange)
                end, 1) -- Wait a very small bit to ensure the color change has been processed and does not interfere with the camera change.
            end
        end
    end
end

function updateCardCountOnSwitch()
    local colorsToProcess = {}

    local zones = {
        deckZonesUI = {
            Red = { zone = deckZones["Red"], uiElement = "deck_Top" },
            Blue = { zone = deckZones["Blue"], uiElement = "deck_Bottom" },
            Green = { zone = deckZones["Green"], uiElement = "deck_Top" },
            Yellow = { zone = deckZones["Yellow"], uiElement = "deck_Bottom" }
        },
        lifeZonesUI = {
            Red = { zone = lifeZones["Red"], uiElement = "life_Top" },
            Blue = { zone = lifeZones["Blue"], uiElement = "life_Bottom" },
            Green = { zone = lifeZones["Green"], uiElement = "life_Top" },
            Yellow = { zone = lifeZones["Yellow"], uiElement = "life_Bottom" }
        },
        energyZonesUI = {
            Red = { zone = energyZones["Red"], uiElement = "energy_Top" },
            Blue = { zone = energyZones["Blue"], uiElement = "energy_Bottom" },
            Green = { zone = energyZones["Green"], uiElement = "energy_Top" },
            Yellow = { zone = energyZones["Yellow"], uiElement = "energy_Bottom" }
        },
        dropZonesUI = {
            Red = { zone = dropZones["Red"], uiElement = "drop_Top" },
            Blue = { zone = dropZones["Blue"], uiElement = "drop_Bottom" },
            Green = { zone = dropZones["Green"], uiElement = "drop_Top" },
            Yellow = { zone = dropZones["Yellow"], uiElement = "drop_Bottom" }
        },
        handZonesUI = {
            Red = { zone = handZones["Red"], uiElement = "handSize_Top" },
            Blue = { zone = handZones["Blue"], uiElement = "handSize_Bottom" },
            Green = { zone = handZones["Green"], uiElement = "handSize_Top" },
            Yellow = { zone = handZones["Yellow"], uiElement = "handSize_Bottom" }
        },
        warpZonesUI = {
            Red = { zone = warpZones["Red"], uiElement = "warp_Top" },
            Blue = { zone = warpZones["Blue"], uiElement = "warp_Bottom" }
        },
        zDeckZonesUI = {
            Red = { zone = zDeckZones["Red"], uiElement = "zDeck_Top" },
            Blue = { zone = zDeckZones["Blue"], uiElement = "zDeck_Bottom" }
        },
        zEnergyZonesUI = {
            Red = { zone = zEnergyZones["Red"], uiElement = "zEnergy_Top" },
            Blue = { zone = zEnergyZones["Blue"], uiElement = "zEnergy_Bottom" }
        }
    }

    if mastersSelected then
        colorsToProcess = { Red = true, Blue = true }
        updateCardCountForZones(zones.zDeckZonesUI, colorsToProcess)
        updateCardCountForZones(zones.zEnergyZonesUI, colorsToProcess)
        updateCardCountForZones(zones.warpZonesUI, colorsToProcess)
    else
        colorsToProcess = { Green = true, Yellow = true }
    end

    updateCardCountForZones(zones.deckZonesUI, colorsToProcess)
    updateCardCountForZones(zones.lifeZonesUI, colorsToProcess)
    updateCardCountForZones(zones.energyZonesUI, colorsToProcess)
    updateCardCountForZones(zones.dropZonesUI, colorsToProcess)
    updateCardCountForZones(zones.handZonesUI, colorsToProcess)
end

-- Function to update card count for all zones of a given type
function updateCardCountForZones(zoneTable, colors)
    for color, info in pairs(zoneTable) do
        if colors[color] then
            updateCardCount(info.zone, info.uiElement)
        end
    end
end

-- Modified function to update card count and display in UI
function updateCardCount(zone, uiElement)
    local objects = zone.getObjects()
    local cardCount = 0

    for _, obj in ipairs(objects) do
        if obj.tag == "Deck" then
            cardCount = cardCount + obj.getQuantity()
        elseif obj.tag == "Card" then
            cardCount = cardCount + 1
        end
    end

    displayCount(cardCount, uiElement)
end

-- Modified displayCount function to update specific UI elements
function displayCount(count, uiElement)
    UI.setValue(uiElement, "<b>" .. count .. "</b>")
end

-- Delay between processing each card

function scoopCards(params)
    color = params.color
    removeCards = params.removeCards

    -- Define zones for the specified color
    local zones = {
        deck = deckZones[color],
        zDeck = zDeckZones[color],
        leader = leaderZones[color],
        removed = removeZones[color],
        life = lifeZones[color],
        drop = dropZones[color],
        warp = warpZones[color],
        main = mainZones[color],
        energy = energyZones[color],
        zEnergy = zEnergyZones[color],
        hand = handZones[color]
    }

    -- Get the GUID of the deck in the deck zone, if any
    local deckZoneObjects = zones.deck.getObjects()
    local deckInDeckZone = nil
    local deckProcessingDelay = 0.2

    for _, obj in ipairs(deckZoneObjects) do
        if obj.tag == "Deck" then
            deckInDeckZone = obj.getGUID()
            break
        end
    end

    -- Phase 2: Process deck objects, one card at a time, rechecking the deck after each move
    processNextDeckCard(color, zones, deckInDeckZone, removeCards)

    -- Phase 1: Process all single card objects first, ignore decks
    Wait.time(function()
        local zonesToSearch = {}
        if color == "Red" or color == "Blue" then
            zonesToSearch = { zones.energy, zones.hand, zones.removed, zones.drop, zones.warp, zones.zEnergy, zones
                .zDeck, zones.life, zones.main }
        else
            zonesToSearch = { zones.energy, zones.hand, zones.removed, zones.drop, zones.life, zones.main }
        end

        for _, zone in pairs(zonesToSearch) do
            local objects = zone.getObjects()

            for _, obj in ipairs(objects) do
                if obj.tag == "Card" then
                    local description = obj.getDescription()

                    if not (obj.getName():find("Token") or obj.getName():find("Energy Marker")) and removeCards then
                        destroyObject(obj)
                    else
                        if description:find("Type: Z%-") and (color == "Red" or color == "Blue") then
                            -- Move Z cards to the Z deck zone
                            moveCardIfNotInPosition(obj, zones.zDeck.getPosition())
                        elseif description:find("Type: LEADER") then
                            -- Move leader cards to the leader zone
                            local rotation = zones.leader.getRotation()
                            rotation.z = 0
                            moveCardIfNotInPosition(obj, zones.leader.getPosition(), rotation)
                        elseif obj.getName():find("Token") or obj.getName():find("Energy Marker") then
                            -- Remove tokens and energy markers from the game
                            local rotation = { 0, 90, 0 }

                            moveCardIfNotInPosition(obj, zones.removed.getPosition(), rotation)
                        else
                            -- Otherwise, add to the deck zone
                            moveCardIfNotInPosition(obj, zones.deck.getPosition())
                        end
                    end
                end
            end
        end
    end, deckProcessingDelay)
end

function processNextDeckCard(color, zones, deckInDeckZone, removeCards)
    local zonesToSearch = {}
    if color == "Red" or color == "Blue" then
        zonesToSearch = { zones.energy, zones.hand, zones.removed, zones.drop, zones.warp, zones.zEnergy, zones.zDeck,
            zones.life, zones.main }
    else
        zonesToSearch = { zones.energy, zones.hand, zones.removed, zones.drop, zones.life, zones.main }
    end

    -- Get all objects from the zones again, re-fetch decks after every card move
    for _, zone in pairs(zonesToSearch) do
        local objects = zone.getObjects()

        for _, obj in ipairs(objects) do
            if obj.tag == "Deck" then
                -- Use pcall to safely get the cards in the deck
                local success, cards = pcall(function() return obj.getObjects() end)

                if success and cards then
                    -- Loop through each card in the deck
                    for _, card in ipairs(cards) do
                        local description = card.description

                        -- Check if we need to destroy the card
                        if not (card.name:find("Token") or card.name:find("Energy Marker")) and removeCards then
                            -- Take the card object out of the deck and destroy it
                            obj.takeObject({
                                index = card.index,
                                callback_function = function(takenCard)
                                    -- Make sure the card is a valid object and destroy it
                                    destroyObject(takenCard)
                                end,
                                smooth = false
                            })
                            processNextDeckCard(color, zones, deckInDeckZone, removeCards)
                        else
                            -- Move the card based on its type
                            if description:find("Type: Z%-") and (color == "Red" or color == "Blue") then
                                -- Move Z cards to the Z deck zone
                                takeCardIfNotInPosition(obj, card.index, zones.zDeck.getPosition())
                            elseif description:find("Type: LEADER") then
                                -- Move leader cards to the leader zone
                                local rotation = zones.leader.getRotation()
                                rotation.z = 0
                                takeCardIfNotInPosition(obj, card.index, zones.leader.getPosition(), rotation)
                            elseif card.name:find("Token") or card.name:find("Energy Marker") then
                                -- Remove tokens and energy markers from the game
                                takeCardIfNotInPosition(obj, card.index, zones.removed.getPosition())
                            else
                                -- Otherwise, add to the deck zone
                                takeCardIfNotInPosition(obj, card.index, zones.deck.getPosition())
                            end

                            -- After processing the current card, search all decks again
                            processNextDeckCard(color, zones, deckInDeckZone, removeCards)

                            -- Stop after processing the first card in the deck
                            return
                        end
                    end
                else
                    -- If pcall failed, log the issue and continue to the next deck
                    print("Error accessing deck contents in zone: " .. zone.getName())
                end
            end
        end
    end
end

-- Moves a card if it’s not already in the target position
function moveCardIfNotInPosition(cardObj, targetPosition, targetRotation)
    if not positionsAreEqual(cardObj.getPosition(), targetPosition) then
        cardObj.setPosition(targetPosition)
        if targetRotation then
            cardObj.setRotation(targetRotation)
        end
    end
end

-- Takes a card from a deck if it's not already in the target position
function takeCardIfNotInPosition(deckObj, cardIndex, targetPosition, targetRotation)
    local cardPosition = deckObj.getPosition()

    if not positionsAreEqual(cardPosition, targetPosition) then
        deckObj.takeObject({
            index = cardIndex,
            position = targetPosition,
            rotation = targetRotation or nil,
            smooth = false
        })
    end
end

-- Helper function to compare positions with some tolerance for floating point differences
function positionsAreEqual(pos1, pos2, tolerance)
    tolerance = tolerance or 0.01
    return math.abs(pos1.x - pos2.x) < tolerance and math.abs(pos1.y - pos2.y) < tolerance and
    math.abs(pos1.z - pos2.z) < tolerance
end
