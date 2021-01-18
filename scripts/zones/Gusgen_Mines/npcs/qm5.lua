-----------------------------------
-- Area: Gusgen Mines
--  NPC: qm5
-- Note: Spawns Puddings for Eco-Warrior (Bastok)
-- !pos 22.796 -61.156 -19.687 196
-----------------------------------
local ID = require("scripts/zones/Gusgen_Mines/IDs")
require("scripts/globals/keyitems")
require("scripts/globals/npc_util")
require("scripts/globals/quests")
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
end

entity.onTrigger = function(player, npc)
    local pudding = ID.mob.PUDDING_OFFSET

    if player:getCharVar("EcoStatus") == 101 and player:hasStatusEffect(tpz.effect.LEVEL_RESTRICTION) then
        npcUtil.popFromQM(player, npc, {pudding, pudding + 1}, {claim=true, look=true, hide = 0})
    elseif player:getCharVar("EcoStatus") == 102 and not player:hasKeyItem(tpz.ki.INDIGESTED_ORE) then
        npcUtil.giveKeyItem(player, tpz.ki.INDIGESTED_ORE)
    else
        player:messageSpecial(ID.text.NOTHING_OUT_OF_ORDINARY)
    end
end

entity.onEventUpdate = function(player, csid, option)
end

entity.onEventFinish = function(player, csid, option)
end
return entity
