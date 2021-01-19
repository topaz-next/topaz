-----------------------------------
-- Area: Sacrificial Chamber
--  Mob: Grav'iton
-- Zilart Mission 4 BCNM Fight
-----------------------------------
mixins =
{
    require("scripts/mixins/families/tonberry"),
    require("scripts/mixins/job_special")
}
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobSpawn = function(mob)
    mob:addMod(tpz.mod.SLEEPRES, 50)
    mob:addMod(tpz.mod.LULLABYRES, 50)
    mob:setLocalVar("everyonesRancorHPP", math.random(20, 30))
end

entity.onMobFight = function(mob, target)
    if mob:getLocalVar("everyonesRancorUsed") == 0 and mob:getHPP() <= mob:getLocalVar("everyonesRancorHPP") then
        mob:setLocalVar("everyonesRancorUsed", 1)
        mob:useMobAbility(921)
    end
end

entity.onMobDisengage = function(mob, weather)
    mob:setLocalVar("everyonesRancorUsed", 0)
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity
