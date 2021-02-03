-----------------------------------
-- Area: Periqia (Requiem)
--  MOB: Draconic Draugar (DRG)
-----------------------------------
mixins =
{
    require("scripts/mixins/master_instanced"),
    require("scripts/mixins/job_special"),
}
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
