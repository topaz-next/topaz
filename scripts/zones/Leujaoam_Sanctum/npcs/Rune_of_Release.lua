-----------------------------------
-- Area: Leujaoam Sanctum
-----------------------------------
local ID = require("scripts/zones/Leujaoam_Sanctum/IDs")
require("scripts/globals/utils/assault")
require("scripts/globals/zone")
-----------------------------------

function onTrade(player, npc, trade)
end;

function onTrigger(player,npc)

    local instance = npc:getInstance();

    if (instance:completed()) then
        player:startEvent(100,0);
    end

    return 1;

end;

function onEventUpdate(player, csid, option)
end;

function onEventFinish(player, csid, option)
    if csid == 100 and option == 1 then
        assaultUtil.runeReleaseFinish(player, LEUJAOAM_ASSAULT_POINT, ID.text)
    elseif csid == 102 then
        local instance = player:getInstance()
        local chars = instance:getChars()
        
        for i,v in pairs(chars) do
            v:setPos(0,0,0,0,dsp.zone.CAEDARVA_MIRE)
        end
    end
end
