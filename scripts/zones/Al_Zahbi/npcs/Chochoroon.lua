-----------------------------------
-- Area: Al Zahbi
--  NPC: Chochoroon
-- Type: Appraiser
-- !pos -42.739 -1 -45.987 48
-----------------------------------
local ID = require("scripts/zones/Al_Zahbi/IDs")
require("scripts/globals/appraisal")
require("scripts/globals/npc_util")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
    appraisalUtil.appraiseItem(player, npc, trade, 50, 261)
end

entity.onTrigger = function(player, npc)
    player:startEvent(260, 50)
end

entity.onEventUpdate = function(player, csid, option)
end

entity.onEventFinish = function(player, csid, option, npc)
    appraisalUtil.appraisalOnEventFinish(player, csid, option, 50, 261, npc)
end

return entity
