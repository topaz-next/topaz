-----------------------------------
-- Area: Maze of Shakhrami
--  NPC: qm2
-- Note: Spawns Wyrmflies for Eco-Warrior (Windurst)
-- !pos 143 9 -219 198
-----------------------------------
local ID = require("scripts/zones/Maze_of_Shakhrami/IDs")
require("scripts/globals/keyitems")
require("scripts/globals/npc_util")
require("scripts/globals/quests")
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
end

entity.onTrigger = function(player, npc)
    local wyrmfly = ID.mob.WYRMFLY_OFFSET

    if player:getCharVar("EcoStatus") == 201 and player:hasStatusEffect(tpz.effect.LEVEL_RESTRICTION) then
        npcUtil.popFromQM(player, npc, {wyrmfly, wyrmfly + 1, wyrmfly + 2}, {claim=true, look=true, hide = 0})
    elseif player:getCharVar("EcoStatus") == 202 and not player:hasKeyItem(tpz.ki.INDIGESTED_MEAT) then
        npcUtil.giveKeyItem(player, tpz.ki.INDIGESTED_MEAT)
    else
        player:messageSpecial(ID.text.NOTHING_OUT_OF_ORDINARY)
    end
end

entity.onEventUpdate = function(player, csid, option)
end

entity.onEventFinish = function(player, csid, option)
end

return entity
