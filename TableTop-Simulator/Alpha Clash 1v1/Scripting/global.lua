--[[ Lua code. See documentation: https://api.tabletopsimulator.com/ --]]

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    --[[ print('onLoad!') --]]
    local seatedPlayers = Player.getPlayers()
    local colorToChange = ""
    --print("seatedPlayers " .. #seatedPlayers)
    if #seatedPlayers == 1 then
        for _, playerObject in ipairs(seatedPlayers) do
            playerObject.changeColor("Red")
        end
    end

    deckZones = {
        Blue = getObjectFromGUID("5cea21"),
        Red = getObjectFromGUID("e232e2"),
    }

    untapZones = {
        Blue = getObjectFromGUID("4c000b"),
        Red = getObjectFromGUID("ed21c6"),
    }

    energyZones = {
        Blue = getObjectFromGUID("3ed145"),
        Red = getObjectFromGUID("e9e9d5"),
    }

    portalZone = getObjectFromGUID("caa372")


    checkFlip = true
end

--[[ The onUpdate event is called once per frame. --]]
function onUpdate()
    --[[ print('onUpdate loop!') --]]
end

function onPlayerConnect(player)
    local seatedPlayers = Player.getPlayers()
    local colorToChange = ""
    --print("seatedPlayers " .. #seatedPlayers)

    if #seatedPlayers == 2 then
        -- If a 2nd person joins, seat them at the proper seat.
        for _, playerObject in ipairs(seatedPlayers) do
            if player.color ~= playerObject.color then
                if playerObject.color == "Red" then
                    colorToChange = "Blue"
                else
                    colorToChange = "Red"
                end

                -- Apply camera view after changing color
                Wait.time(function()
                    player.changeColor(colorToChange)
                end, 1) -- Wait a very small bit to ensure the color change has been processed and does not interfere with the camera change.
            end
        end
    end
end

function onPlayerChangeColor(player_color)
    if player_color == "Blue" or player_color == "Red" or player_color == "White" then
        -- print("onPlayerChangeColor called " .. player_color)
        local seatedPlayers = Player.getPlayers()
        local position = { x = 0, y = 0, z = 0 }
        local yaw = 360
        local distance = 50
        local textLabel = ""
        local color = ""

        if player_color == "Red" then
            yaw = 180
        elseif player_color == "White" then
            yaw = 180
            position = { x = 0, y = 0, z = 0 }
            distance = 32
        end

        -- Loop through all players and output their name to proper the mat
        print("Players " .. #seatedPlayers)
        -- for _, playerObject in ipairs(seatedPlayers) do
        --     color = playerObject.color
        --     if color ~= "White" then
        --         textLabel.TextTool.setValue(playerObject.steam_name)
        --     end
        -- end

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

-- Shuffles Player Deck
function shuffleDeck(player, value, id)
    if checkInSeat(player.color, "shuffle deck") then
        local zoneObjects = deckZones[player.color].getObjects()
        for _, item in ipairs(zoneObjects) do
            if item.tag == 'Deck' then
                print("Player " .. player.color .. " shuffled deck")
                item.shuffle()
            end
        end
    end
end

-- Untap All
function untapAll(player, value, id)
    if checkInSeat(player.color, "untap all") then
        local objectsToUntap = untapZones[player.color].getObjects()
        local energyToUntap = energyZones[player.color].getObjects()

        removeSubsetFromCollection(objectsToUntap, energyToUntap)

        local rightSideUp = player.color == "Red" and 360 or 180
        local rightSideDown = player.color == "Red" and 180 or 360

        print("Untapping cards for Player " .. player.color)
        for _, obj in ipairs(objectsToUntap) do
            if (obj.tag == "Card") then
                local objRotation = obj.getRotation()
                if objRotation.y ~= 0 or objRotation.y ~= 360 then
                    local finalRotation = { 0, rightSideUp, obj.getRotation().z }
                    obj.setRotationSmooth(finalRotation)
                end
            end
        end

        Wait.time(checkFlipTrue, 1.5)


        for _, obj in ipairs(energyToUntap) do
            if (obj.tag == "Card") then
                local objRotation = obj.getRotation()
                if objRotation.y ~= 180 then
                    local finalRotation = { 0, rightSideDown, obj.getRotation().z }
                    obj.setRotationSmooth(finalRotation)
                end
            end
        end
    end
end

-- Activate Portal
function activatePortal(player, value, id)
    if checkInSeat(player.color, "Activate Portal") then
        print("Player " .. player.color .. " activated the portal")
        local portalCard = portalZone.getObjects()
        for _, obj in ipairs(portalCard) do
            if (obj.type == "Card") then
                local objRotation = obj.getRotation()
                if objRotation.z == 180 then
                    local finalRotation = { objRotation.x, objRotation.y, 0 }
                    obj.setRotationSmooth(finalRotation)
                else
                    local finalRotation = { objRotation.x, objRotation.y, 180 }
                    obj.setRotationSmooth(finalRotation)
                end
            end
        end
    end
end

--#region Utility functions

function checkInSeat(playerColor, buttonName)
    if playerColor then
        if deckZones[playerColor] then
            return true
        else
            broadcastToColor("You must sit at a colored seat to " .. buttonName .. "!", playerColor,
                { r = 1, g = 0, b = 0 })
            return false
        end
    else
        broadcastToAll("Player must be seated to " .. buttonName .. "!", { r = 1, g = 0, b = 0 })
        return false
    end
end

function checkFlipTrue()
    checkFlip = true
end

-- Function to remove objects in the subset from the main collection
function removeSubsetFromCollection(mainCollection, subset)
    -- Create a lookup table for the subset
    local lookup = {}
    for _, obj in ipairs(subset) do
        lookup[obj] = true
    end

    -- Remove objects from the main collection
    for i = #mainCollection, 1, -1 do
        if lookup[mainCollection[i]] then
            table.remove(mainCollection, i)
        end
    end
end

function onObjectSearchStart(object, player_color)
    UI.setAttribute("customUI", "active", "false")
    -- UI.setAttribute("customUI_wait", "active", "false")
end

function onObjectSearchEnd(object, player_color)
    UI.setAttribute("customUI", "active", "true")
    -- UI.setAttribute("customUI_wait", "active", "false")
end

--#endregion
