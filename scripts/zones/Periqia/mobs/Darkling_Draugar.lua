-----------------------------------
-- Area: Periqia (Requiem)
--  MOB: Darkling Draugar (DRK)
-----------------------------------
require("scripts/globals/status")
-----------------------------------

function onMobSpawn(mob)
    mob:setMod(dsp.mod.SLEEPEVA_DARK, 9999)
end

function onMobDeath(mob, player, isKiller, firstCall)
    if firstCall then
        local instance = mob:getInstance()
        instance:setProgress(instance:getProgress() + 1)
    end
end
