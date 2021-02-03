-----------------------------------
-- Area: Ilrusi Atoll
--  NPC: Rune of Release
-- !pos 412 -9 54 55
-----------------------------------
local ID = require("scripts/zones/Ilrusi_Atoll/IDs")
require("scripts/globals/utils/assault")
require("scripts/globals/zone")
-----------------------------------

function onTrade(player, npc, trade)
end

function onTrigger(player, npc)
    local instance = npc:getInstance()

    if (instance:completed()) then
        player:startEvent(100,4)
    end

    return 1
end

function onEventUpdate(player, csid, option)
end

function onEventFinish(player, csid, option)
    if csid == 100 and option == 1 then
        assaultUtil.runeReleaseFinish(player, ILRUSI_ASSAULT_POINT, ID.text)
    elseif csid == 102 then
        local instance = player:getInstance()
        local chars = instance:getChars()
        
        for i,v in pairs(chars) do
            v:setPos(0,0,0,0,dsp.zone.ARRAPAGO_REEF)
        end
    end
end