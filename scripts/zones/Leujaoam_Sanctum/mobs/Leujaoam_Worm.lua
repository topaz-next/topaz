-----------------------------------
-- Area: Leujaoam Sanctum (Leujaoam Cleansing)
--  MOB: Leujaoam Worm
-- Immune to Charm, strong to Magical Attacks
-----------------------------------
require("scripts/globals/status")
-----------------------------------

function onMobSpawn(mob)
    mob:setMod(dsp.mod.UDMGMAGIC, -50)
    mob:addMod(dsp.mod.DEF, 100)
    mob:setMobMod(dsp.mobMod.MAGIC_STANDBACK, 1) -- Only will cast when out of combat range
    mob:setMobMod(dsp.mobMod.STANDBACK_COOL, 15) -- Doesnt work
end

function onMobDeath(mob, player, isKiller)
    local instance = mob:getInstance()
    
    if mob:getLocalVar("Killed") == 0 then
        instance:setProgress(instance:getProgress() + 1)
        mob:setLocalVar("Killed", 1)
    end
end
