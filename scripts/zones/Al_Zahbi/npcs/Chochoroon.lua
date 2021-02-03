-----------------------------------
-- Area: Al Zahbi
--  NPC: Chochoroon
-- Type: Appraiser
-- !pos -42.739 -1 -45.987 48
-----------------------------------
local ID = require("scripts/zones/Al_Zahbi/IDs")
require("scripts/globals/utils/appraisal")
require("scripts/globals/npc_util")
-----------------------------------

function onTrade(player, npc, trade)
    appraisalUtil.appraiseItem(player, npc, trade, 50, 261)
end

function onTrigger(player, npc)
    player:startEvent(260, 50)
end

function onEventUpdate(player, csid, option)
end

function onEventFinish(player, csid, option, npc)
    appraisalUtil.appraisalOnEventFinish(player, csid, option, 50, 261, npc)
end
