-----------------------------------
-- Area: Spire of Vahzl
--  Mob: Procreator
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMobMod(tpz.mobMod.LINK_RADIUS, 50)
end

function onMobSpawn(mob)
    mob:setLocalVar("maxBabies", 4)
end

function onMobEngaged(mob, target)
end

function onMobWeaponSkill(target, mob, skill)
end

entity.onMobFight = function(mob, target)
    if mob:getHPP() < 20 then
        local nextMob = GetMobByID(mob:getID() - 1) --Agonizer aggros at <20%
        if not nextMob:isEngaged() then
            nextMob:updateEnmity(target)
        end
    end
end

entity.onMobDeath = function(mob, player, isKiller)
    local momma = mob:getID()
    for i = momma + 1, momma + mob:getLocalVar("maxBabies") do
        local baby = GetMobByID(i)
        if baby:isSpawned() then
            baby:setHP(0)
        end
    end
end

return entity
