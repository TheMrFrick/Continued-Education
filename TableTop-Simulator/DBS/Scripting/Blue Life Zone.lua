-- Global variable to track if the "lost game" message has been announced
local hasLostGameBeenAnnounced = false
local scriptingZoneGUID = '0ea49b' -- Assume the GUID is known and set

-- This function runs when the game starts or the script is reloaded
function onload()
    hasLostGameBeenAnnounced = false
end

-- Triggered when an object enters the scripting zone
function onObjectEnterScriptingZone(zone, enter_object)
    if zone.getGUID() == scriptingZoneGUID and Global.call('getMastersSelected') then
        updateDisplayCount(zone) -- Immediate display update
    end
end

-- Triggered when an object leaves the scripting zone
function onObjectLeaveScriptingZone(zone, leave_object)
    if zone.getGUID() == scriptingZoneGUID and Global.call('getMastersSelected') then
        updateDisplayCount(zone) -- Immediate display update
        Wait.time(function() checkForLoss(zone) end, 2.0) -- Delayed check for loss
    end
end

-- Updates the displayed count of cards/decks immediately
function updateDisplayCount(zone)
    local currentCardCount = getCurrentCardCount(zone)
    displayCount(currentCardCount)
end

-- Retrieves the current card count in the zone
function getCurrentCardCount(zone)
    local objects = zone.getObjects()
    local cardCount = 0

    for _, obj in ipairs(objects) do
        if obj.tag == "Deck" or obj.tag == "Card" then
            cardCount = cardCount + (obj.tag == "Deck" and obj.getQuantity() or 1)
        end
    end

    return cardCount
end

-- Displays the current count on a 3D text object or similar
function displayCount(count)
    UI.setValue("life_Bottom", "<b>" .. count .. "</b>")
end

-- Checks if the player has lost the game (i.e., no more cards in the zone)
function checkForLoss(zone)
    local currentCardCount = getCurrentCardCount(zone)
    if currentCardCount == 0 and not hasLostGameBeenAnnounced then
        broadcastToAll("Player has lost the game, no more life", {r = 1, g = 0, b = 0})
        Global.call("changeUI","Setup")
        hasLostGameBeenAnnounced = true
    elseif currentCardCount > 0 then
        hasLostGameBeenAnnounced = false -- Reset flag if cards are back in the zone
    end
end

