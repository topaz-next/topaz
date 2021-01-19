-----------------------------------
-- Area: Wajaom Woodlands
--  ZNM: Iriz Ima
-----------------------------------
mixins = {require("scripts/mixins/rage")}
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMobMod(tpz.mobMod.IDLE_DESPAWN, 300)
end

entity.onMobSpawn = function(mob)
    mob:setLocalVar("[rage]timer", 3600) -- 60 minutes
    mob:setLocalVar("BreakChance", 5)
end

entity.onCriticalHit = function(mob, attacker)
    if math.random(100) <= mob:getLocalVar("BreakChance") then
        local animationSub = mob:getAnimationSub()
        if animationSub == 4 then
            mob:setAnimationSub(1) -- 1 horn broken
        elseif animationSub == 1 then
            mob:setAnimationSub(2) -- both horns broken
        end
    end
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity
