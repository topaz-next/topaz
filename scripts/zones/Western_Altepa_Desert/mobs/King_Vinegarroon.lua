-----------------------------------
-- Area: Western Altepa Desert
--   NM: King Vinegarroon
-----------------------------------
require("scripts/globals/titles")
require("scripts/globals/world")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMobMod(tpz.mobMod.ADD_EFFECT, 1)
end

entity.onMobDrawIn = function(mob, target)
    -- todo make him use AoE tp move
    mob:addTP(3000)
end

entity.onAdditionalEffect = function(mob, target, damage)
    return tpz.mob.onAddEffect(mob, target, damage, tpz.mob.ae.PETRIFY, {chance = 100})
end

entity.onMobDisengage = function(mob, weather)
    if weather ~= tpz.weather.DUST_STORM and weather ~= tpz.weather.SAND_STORM then
        DespawnMob(mob:getID())
    end
end

entity.onMobDeath = function(mob, player, isKiller)
    player:addTitle(tpz.title.VINEGAR_EVAPORATOR)
end

entity.onMobDespawn = function(mob)
    UpdateNMSpawnPoint(mob:getID())
    mob:setRespawnTime(math.random(75600, 86400)) -- 21 to 24 hours
end

return entity
