-- Assume the GUID of the scripting zone is known and set it here
local scriptingZoneGUID = "e232e2"
local selfObject = getObjectFromGUID("e232e2")

function onload()
    selfObject.setSnapPoints({
        {
            position = {0,0,0}
        }
    })
end

function onObjectEnterScriptingZone(zone, enter_object)
    if zone.getGUID() == scriptingZoneGUID then
        updateCardCount(zone)
    end
end

function onObjectLeaveScriptingZone(zone, leave_object)
    if zone.getGUID() == scriptingZoneGUID then
        updateCardCount(zone)
    end
end

function updateCardCount(zone)
    local objects = zone.getObjects()
    local cardCount = 0

    for _, obj in ipairs(objects) do
        if obj.type == "Deck" then
            cardCount = cardCount + obj.getQuantity()
        elseif obj.type == "Card" then
            cardCount = cardCount + 1
        end
    end

    displayCount(cardCount)
end

function displayCount(count)
    UI.setValue("deck_Top", "<b>" .. count .. "</b>")

end
