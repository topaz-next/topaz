-----------------------------------
-- Area: RuAun Gardens
--   NM: Despot
-----------------------------------
local entity = {}

entity.onMobSpawn = function(mob)
    local ph = GetMobByID(mob:getLocalVar("ph"))
    if ph then
        local pos = ph:getPos()
        mob:setPos(pos.x, pos.y, pos.z, pos.r)
        local killerId = ph:getLocalVar("killer")
        if killerId ~= 0 then
            local killer = GetPlayerByID(killerId)
            if not killer:isEngaged() and killer:checkDistance(mob) <= 50 then
                mob:updateClaim(killer)
            end
        end
    end
end

entity.onMobWeaponSkill = function(target, mob, skill)
    if skill:getID() == 536 then
        local panzerfaustCounter = mob:getLocalVar("panzerfaustCounter")
        local panzerfaustMax = mob:getLocalVar("panzerfaustMax")

        if panzerfaustCounter == 0 and panzerfaustMax == 0 then
            panzerfaustMax = math.random(2, 5)
            mob:setLocalVar("panzerfaustMax", panzerfaustMax)
        end

        panzerfaustCounter = panzerfaustCounter + 1
        mob:setLocalVar("panzerfaustCounter", panzerfaustCounter)

        if panzerfaustCounter > panzerfaustMax then
            mob:setLocalVar("panzerfaustCounter", 0)
            mob:setLocalVar("panzerfaustMax", 0)
        else
            mob:useMobAbility(536)
        end
    end
end

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    mob:removeListener("PH_VAR")
end

return entity
