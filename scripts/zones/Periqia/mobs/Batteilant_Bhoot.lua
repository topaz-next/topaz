-----------------------------------
-- Area: Periqia (Requiem)
--  MOB: Batteilant Bhoot
-----------------------------------
require("scripts/globals/status")
-----------------------------------

function onMobSpawn(mob)
    mob:addMod(dsp.mod.FASTCAST, 50)
end

function onMobDeath(mob, player, isKiller, firstCall)
    if firstCall then
        local instance = mob:getInstance()
        instance:setProgress(instance:getProgress() + 1)
    end
end
