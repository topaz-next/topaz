-----------------------------------
-- Area: Dynamis - Xarcabard
--   NM: Ying
-----------------------------------
local ID = require("scripts/zones/Dynamis-Xarcabard/IDs")
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob, target)
end

entity.onMobSpawn = function(mob)
    local dynaLord = GetMobByID(ID.mob.DYNAMIS_LORD)
    if (dynaLord:getLocalVar("magImmune") < 2) then -- both dragons have not been killed initially
        dynaLord:setMod(tpz.mod.UDMGMAGIC, -100)
        dynaLord:setMod(tpz.mod.UDMGBREATH, -100)
        dynaLord:setLocalVar("magImmune", 0)
        mob:setSpawn(-364, -35.661, 17.254) -- Reset Ying's spawn point to initial spot.
    else
        mob:setSpawn(-414.282, -44, 20.427) -- Spawned by DL, reset to DL's spawn point.
    end
end

entity.onMobFight = function(mob, target)
    -- Repop Yang every 30 seconds if Ying is up and Yang is not.
    local yang = GetMobByID(ID.mob.YANG)
    local YangToD = mob:getLocalVar("YangToD")
    if yang:getCurrentAction() == tpz.act.NONE and os.time() > YangToD+30 then
        yang:setSpawn(mob:getXPos(), mob:getYPos(), mob:getZPos())
        yang:spawn()
        yang:updateEnmity(target)
    end
end

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    local Yang = GetMobByID(ID.mob.YANG)
    local dynaLord = GetMobByID(ID.mob.DYNAMIS_LORD)
    -- localVars clear on death, so setting it on its partner
    Yang:setLocalVar("YingToD", os.time())
    if (dynaLord:getLocalVar("magImmune") == 0) then
        dynaLord:setMod(tpz.mod.UDMGMAGIC, 0)
        dynaLord:setMod(tpz.mod.UDMGBREATH, 0)
        if (dynaLord:getLocalVar("physImmune") == 1) then -- other dragon is also dead
            dynaLord:setLocalVar("physImmune", 2)
            dynaLord:setLocalVar("magImmune", 2)
        else
            dynaLord:setLocalVar("magImmune", 1)
        end
    end
end

return entity
