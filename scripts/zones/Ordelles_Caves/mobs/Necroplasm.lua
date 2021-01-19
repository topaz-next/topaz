-----------------------------------
-- Area: Ordelle's Caves
--   NM: Necroplasm
-- Involved in Eco Warrior (San d'Oria)
-----------------------------------
local ID = require("scripts/zones/Ordelles_Caves/IDs")
require("scripts/globals/quests")
require("scripts/globals/status")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMobMod(tpz.mobMod.ADD_EFFECT, 1)
end

entity.onAdditionalEffect = function(mob, target, damage)
    return tpz.mob.onAddEffect(mob, target, damage, tpz.mob.ae.PARALYZE)
end

entity.onMobDeath = function(mob, player, isKiller)
    if player:getCharVar("EcoStatus") == 1 and player:hasStatusEffect(tpz.effect.LEVEL_RESTRICTION) then
        player:setCharVar("EcoStatus", 2)
    end
end

return entity
