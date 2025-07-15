-- Assume the GUID of the scripting zone is known and set it here
local scriptingZoneGUID = "5cea21"
local selfObject = getObjectFromGUID("5cea21")

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
        if obj.tag == "Deck" then
            cardCount = cardCount + obj.getQuantity()
        elseif obj.tag == "Card" then
            cardCount = cardCount + 1
        end
    end

    displayCount(cardCount)
end

function displayCount(count)
    UI.setValue("deck_Bottom", "<b>" .. count .. "</b>")

end
