 -- Assume the GUID of the scripting zone is known and set it here
 local scriptingZoneGUID = '1b7a19'

 function onload()
  
 end
 
 function onObjectEnterScriptingZone(zone, enter_object)
     if zone.getGUID() == scriptingZoneGUID then
         enter_object.setInvisibleTo({"Red","Yellow","White","Green"})
     end
 end
 
 
 
 function onObjectLeaveScriptingZone(zone, leave_object)
     if zone.getGUID() == scriptingZoneGUID then
         leave_object.setInvisibleTo({})
     end
 end
 