-----------------------------------
-- Area: Wajaom Woodlands
--  ZNM: Vulpangue
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
    mod:addMod((tpz.mod.FIRE_ABSORB + VanadielDayElement() - 1), 100)
    mob:addMod(tpz.mod.WIND_ABSORB, 100)
    mob:setLocalVar("HPP", 90)
end

entity.onMobFight = function(mob, target)
    local defUpHPP = mob:getLocalVar("HPP")
    if mob:getHPP() <= defUpHPP then
        if mob:getHPP() > 10 then
            mob:addMod(tpz.mod.ACC, 10)
            mob:addMod(tpz.mod.ATT, 10)
            mob:setLocalVar("HPP", mob:getHPP() - 10)
        end
    end
end

entity.onMobDeath = function(mob, killer)
end

return entity
