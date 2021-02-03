-----------------------------------
-- Area: Nashmau
--  NPC: Memeroon
-- Type: Appraiser
-- !pos -26.3 0 -40.6 48
-----------------------------------
require("scripts/globals/utils/appraisal")
require("scripts/globals/npc_util")
local ID = require("scripts/zones/Nashmau/IDs")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
    -- TODO: Check this ID (273)
    appraisalUtil.appraiseItem(player, npc, trade, 500, 273)
end

entity.onTrigger = function(player, npc)
    player:startEvent(272, 500)
end

entity.onEventUpdate = function(player, csid, option)
end

entity.onEventFinish = function(player, csid, option, npc)
    -- TODO: Check this ID (273)
    appraisalUtil.appraisalOnEventFinish(player, csid, option, 500, 273, npc)
end

return entity
