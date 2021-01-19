-----------------------------------
-- Area: Promyvion - Vahzl
--   NM: Provoker
-----------------------------------
require("scripts/globals/status")
require("scripts/globals/magic")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:addMod(tpz.mod.ACC, 50)
    mob:setMobMod(tpz.mobMod.IDLE_DESPAWN, 180)
    mob:setMobMod(tpz.mobMod.ADD_EFFECT, 1)
end

entity.onMobFight = function(mob, target)

    local changeTime = mob:getLocalVar("changeTime")
    local element = mob:getLocalVar("element")

    if changeTime == 0 then
        mob:setLocalVar("changeTime", math.random(2, 3)*15)
        return
    end
    if mob:getBattleTime() >= changeTime then
        local newElement = element
        while newElement == element do
            newElement = math.random(1, 8)
        end
        if element ~= 0 then
            mob:delMod(tpz.magic.absorbMod[element], 100)
        end

        mob:useMobAbility(624)
        mob:addMod(tpz.magic.absorbMod[newElement], 100)
        mob:setLocalVar("changeTime", mob:getBattleTime() + math.random(2, 3)*15)
        mob:setLocalVar("element", newElement)
    end
end

entity.onAdditionalEffect = function(mob, target, damage)
    local element = mob:getLocalVar("element")
    if element > 0 then
        return tpz.mob.onAddEffect(mob, target, damage, tpz.mob.ae.ENFIRE + element - 1, {chance = 1000})
    else
        return 0, 0, 0 -- Just in case its somehow not got a variable set
    end
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity
