 -- Assume the GUID of the scripting zone is known and set it here
 local scriptingZoneGUID = '1c9eea'

 -- Position to place the text object relative to the scripting zone
 local textPositionOffset = {x = -0.2, y = -1.0, z = 1.65}
 
 function onload()
  
 end
 
 function onObjectEnterScriptingZone(zone, enter_object)
     if zone.getGUID() == scriptingZoneGUID and Global.call('getMastersSelected') then
         updateCardCount(zone)
     end
 
 end
 
 
 
 function onObjectLeaveScriptingZone(zone, leave_object)
     if zone.getGUID() == scriptingZoneGUID and Global.call('getMastersSelected') then
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
     UI.setValue("energy_Bottom", "<b>" .. count .. "</b>")
 
 end