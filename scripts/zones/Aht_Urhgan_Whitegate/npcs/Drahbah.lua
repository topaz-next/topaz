-----------------------------------
-- Area: Aht Urhgan Whitegate
-- NPC: Drahbah
-- Type: Appraiser
-- !pos -86 0 83 50
-----------------------------------
local ID = require("scripts/zones/Aht_Urhgan_Whitegate/IDs")
require("scripts/globals/utils/appraisal")
require("scripts/globals/npc_util")
-----------------------------------

function onTrade(player, npc, trade)
    appraisalUtil.appraiseItem(player, npc, trade, 500, 679)
end 

function onTrigger(player, npc)
    player:startEvent(678, 500)
end 

function onEventUpdate(player, csid, option)
end

function onEventFinish(player, csid, option, npc)
    appraisalUtil.appraisalOnEventFinish(player, csid, option, 500, 679, npc)
end
