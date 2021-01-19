-----------------------------------
-- Area: Balga's Dais
--  Mob: Bitoso
-- BCNM: Creeping Doom
-----------------------------------
require("scripts/globals/status")
require("scripts/globals/magic")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMobMod(tpz.mobMod.HP_HEAL_CHANCE, 90)
    mob:setMobMod(tpz.mobMod.HEAL_CHANCE, 100)
    mob:setMod(tpz.mod.REGEN, 0)
end

entity.onMobSpawn = function(mob)
    mob:setMobMod(tpz.mobMod.MAGIC_COOL, 20)
    mob:setMobMod(tpz.mobMod.SOUND_RANGE, 13)
end

entity.onMobFight = function(mob, target)
end

entity.onMagicHit = function(caster, target, spell)
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity
