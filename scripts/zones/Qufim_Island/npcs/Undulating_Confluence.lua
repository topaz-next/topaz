-----------------------------------
-- Area: Qufim Island (126)
--  NPC: Undulating Confluence
-- !pos -204.531 -20.027 75.318 126
-----------------------------------
local ID = require("scripts/zones/Qufim_Island/IDs")
require("scripts/globals/missions")
require("scripts/globals/npc_util")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
end

entity.onTrigger = function(player, npc)
    if player:getCurrentMission(ROV) == tpz.mission.id.rov.AT_THE_HEAVENS_DOOR then
        player:startEvent(63)
    elseif player:getCurrentMission(ROV) == tpz.mission.id.rov.THE_LIONS_ROAR then
        npcUtil.popFromQM(player, npc, ID.mob.OPHIOTAURUS, { look=true, hide=0 })
    elseif player:getCurrentMission(ROV) == tpz.mission.id.rov.EDDIES_OF_DESPAIR_I then
        player:startEvent(64)
    elseif player:getCurrentMission(ROV) >= tpz.mission.id.rov.SET_FREE then
        player:startEvent(65)
    end
end

entity.onEventUpdate = function(player, csid, option)
end

entity.onEventFinish = function(player, csid, option)
    if csid == 63 then
        player:completeMission(tpz.mission.log_id.ROV, tpz.mission.id.rov.AT_THE_HEAVENS_DOOR)
        player:addMission(tpz.mission.log_id.ROV, tpz.mission.id.rov.THE_LIONS_ROAR)
    elseif csid == 64 then
        tpz.teleport.to(player, tpz.teleport.id.ESCHA_ZITAH)
    elseif csid == 65 and option == 1 then
        tpz.teleport.to(player, tpz.teleport.id.ESCHA_ZITAH)
    end
end

return entity
