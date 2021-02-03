-----------------------------------
-- Area: Lebros Cavern
-----------------------------------
local ID = require("scripts/zones/Lebros_Cavern/IDs")
require("scripts/globals/utils/assault")
require("scripts/globals/zone")
-----------------------------------

function onTrade(player, npc, trade)
end

function onTrigger(player, npc)
    local instance = npc:getInstance()

    if instance:completed() then
        player:startEvent(100)
    end

    return 1
end

function onEventUpdate(player, csid, option)
end

function onEventFinish(player, csid, option)
    if csid == 100 and option == 1 then
        assaultUtil.runeReleaseFinish(player, LEBROS_ASSAULT_POINT, ID.text)
    elseif csid == 102 then
        local instance = player:getInstance()
        local chars = instance:getChars()
        
        for _,v in pairs(chars) do
            v:setPos(0,0,0,0,dsp.zone.MOUNT_ZHAYOLM)
        end
    end
end