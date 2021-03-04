-----------------------------------
-- Area: Leujaoam Sanctum (Leujaoam Cleansing)
--  Mob: Leujaoam Worm
-- Immune to Charm, strong to Magical Attacks
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobSpawn = function(mob)
    -- TODO: mob:setMod(tpz.mod.UDMGMAGIC, -50)
    mob:addMod(tpz.mod.DEF, 100)
    --mob:setMobMod(tpz.mobMod.MAGIC_STANDBACK, 1) -- TODO: Only will cast when out of combat range
    --mob:setMobMod(tpz.mobMod.STANDBACK_COOL, 15) -- TODO: Doesn't work
end

entity.onMobDeath = function(mob, player, isKiller)
    local instance = mob:getInstance()

    if mob:getLocalVar("Killed") == 0 then
        instance:setProgress(instance:getProgress() + 1)
        mob:setLocalVar("Killed", 1)
    end
end

return entity
