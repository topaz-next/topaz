-----------------------------------
-- Area: Maze of Shakhrami
--   NM: Pudding
-- Involved in Eco Warrior (Bastok)
-----------------------------------
local ID = require("scripts/zones/Gusgen_Mines/IDs")
require("scripts/globals/quests")
require("scripts/globals/status")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMobMod(tpz.mobMod.ADD_EFFECT, 1)
end

entity.onAdditionalEffect = function(mob, target, damage)
    return tpz.mob.onAddEffect(mob, target, damage, tpz.mob.ae.SLOW)
end

entity.onMobDeath = function(mob, player, isKiller)
    if player:getCharVar("EcoStatus") == 101 and player:hasStatusEffect(tpz.effect.LEVEL_RESTRICTION) then
        local bothDead = true
        for i = ID.mob.PUDDING_OFFSET, ID.mob.PUDDING_OFFSET + 1 do
            if i ~= mob:getID() and GetMobByID(i):isAlive() then
                bothDead = false
            end
        end
        if bothDead then
            player:setCharVar("EcoStatus", 102)
        end
    end
end

return entity
