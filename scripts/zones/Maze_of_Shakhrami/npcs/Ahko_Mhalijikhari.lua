-----------------------------------
-- Area: Maze of Shakhrami
--  NPC: Ahko Mhalijikhari
-- Type: Quest NPC
-- !pos -344.617 -12.226 -166.233 198
-----------------------------------
require("scripts/globals/keyitems")
require("scripts/globals/quests")
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
end

entity.onTrigger = function(player, npc)
    if player:getCharVar("EcoStatus") == 201 then
        if not player:hasStatusEffect(tpz.effect.LEVEL_RESTRICTION) then
            player:startEvent(62) -- Apply ointment option
        else
            player:startEvent(64) -- Remove ointment option
        end
    elseif player:hasKeyItem(tpz.ki.INDIGESTED_MEAT) then
            player:startEvent(65) -- After receiving KI, Ahko sends the player to Lumomo
    else
        player:startEvent(61) -- Default dialogue
    end
end

entity.onEventUpdate = function(player, csid, option)
end

entity.onEventFinish = function(player, csid, option)
    if csid == 62 and option == 1 then
        player:addStatusEffect(tpz.effect.LEVEL_RESTRICTION, 25, 0, 0)
    elseif csid == 65 then
        player:delStatusEffect(tpz.effect.LEVEL_RESTRICTION)
        player:setCharVar("EcoStatus", 203)
    elseif csid == 64 and option == 0 then
        player:delStatusEffect(tpz.effect.LEVEL_RESTRICTION)
    end
end

return entity
