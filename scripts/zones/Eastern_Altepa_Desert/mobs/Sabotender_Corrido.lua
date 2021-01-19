-----------------------------------
-- Area: Eastern Altepa Desert
--   NM: Sabotender Corrido
-----------------------------------
require("scripts/globals/hunts")
mixins = {require("scripts/mixins/job_special")}
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMobMod(tpz.mobMod.ADD_EFFECT, 1)
    mob:setMod(tpz.mod.DOUBLE_ATTACK, 25)
    mob:setMod(tpz.mod.MOVE, 25)
end

entity.onMobSpawn = function(mob)
    tpz.mix.jobSpecial.config(mob, {
        specials =
        {
            {id = tpz.jsa.HUNDRED_FISTS, hpp = 25},
        },
    })
end

entity.onAdditionalEffect = function(mob, target, damage)
    return tpz.mob.onAddEffect(mob, target, damage, tpz.mob.ae.SLOW)
end

entity.onMobDeath = function(mob, player, isKiller)
    tpz.hunts.checkHunt(mob, player, 411)
end

return entity
