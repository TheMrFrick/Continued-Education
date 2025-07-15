local scriptingZoneGUID = "29ea1a"
local selfObject = getObjectFromGUID("29ea1a")

function onload()
    selfObject.setSnapPoints({
        {
            position = {0,0,0}
        }
    })
end