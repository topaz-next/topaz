-----------------------------------
-- Area: Arrapago Remnants
--  Mob: Archaic Rampart
-----------------------------------
mixins = {require("scripts/mixins/families/rampart")}
local ID = require("scripts/zones/Arrapago_Remnants/IDs")
require("scripts/globals/instance")
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobSpawn = function(mob)
end

entity.onMobFight = function(mob, target)
    local instance = mob:getInstance()
    local popTime = mob:getLocalVar("lastPetPop")
    local POS = mob:getPos()
    local PET1 = GetMobByID((mob:getID() +1), instance)
    local PET2 = GetMobByID((mob:getID() +2), instance)
    local PET3 = GetMobByID((mob:getID() +3), instance)


    if os.time() - popTime > 15 then
        if (not PET1:isSpawned()) then
            PET1:setSpawn(POS.x, POS.y, POS.z, POS.rot)
            mob:useMobAbility(2034)
            mob:setLocalVar("lastPetPop", os.time())
            mob:timer(2500, function(mob)
            SpawnMob((mob:getID() +1), instance)
            end)
        elseif (not PET2:isSpawned()) then
            PET2:setSpawn(POS.x, POS.y, POS.z, POS.rot)
            mob:useMobAbility(2034)
            mob:setLocalVar("lastPetPop", os.time())
            mob:timer(2500, function(mob)
            SpawnMob((mob:getID() +2), instance)
            end)
        elseif (not PET3:isSpawned()) then
            PET3:setSpawn(POS.x, POS.y, POS.z, POS.rot)
            mob:useMobAbility(2034)
            mob:setLocalVar("lastPetPop", os.time())
            mob:timer(2500, function(mob)
            SpawnMob((mob:getID() +3), instance)
            end)
        end
    end
    if PET1:isSpawned() then
        PET1:updateEnmity(target)
    end
    if PET2: isSpawned() then
        PET2:updateEnmity(target)
    end
    if PET3: isSpawned() then
        PET3:updateEnmity(target)
    end
end

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
end

return entity
