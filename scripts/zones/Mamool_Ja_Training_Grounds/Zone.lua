-----------------------------------
--
-- Zone: Mamool_Ja_Training_Grounds
--
-----------------------------------
local ID = require("scripts/zones/Mamool_Ja_Training_Grounds/IDs")
require("scripts/globals/zone")
-----------------------------------

function onInitialize(zone)
end;

function onInstanceZoneIn(player,instance)
    local cs = -1
    local pos = player:getPos()
    
    if (pos.x == 0 and pos.y == 0 and pos.z == 0) then
        local entrypos = instance:getEntryPos();
        player:setPos(entrypos.x, entrypos.y, entrypos.z, entrypos.rot);
    end
    if player:getInstance() ~= nil then
        player:setVar("assaultEntered", 2)
    end

    return cs
end;

function onRegionEnter(player,region)
end;

function onEventUpdate(player,csid,option)
end;

function onEventFinish(player,csid,option)
    local instance = player:getInstance()
    local chars = instance:getChars()
    
    if csid == 102 then
        for i,v in pairs(chars) do
            v:setPos(0,0,0,0,dsp.zone.BHAFLAU_THICKETS)
        end
    end
end;

function onInstanceLoadFailed()
    return 79;
end;