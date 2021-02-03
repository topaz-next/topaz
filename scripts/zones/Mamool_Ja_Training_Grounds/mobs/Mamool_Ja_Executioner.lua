-----------------------------------
-- Area: Mamool Ja Training Grounds (Preemptive Strike)
--  MOB: Mamool Ja Executioner (BLU/WHM/NIN)
-----------------------------------
mixins = {require("scripts/mixins/weapon_break")}
-----------------------------------

function onMobSpawn(mob)
    if mob:getMainJob() ~= dsp.job.WHM then
        mob:setLocalVar("BreakChance", 0) -- Only whm mobs have weapons to break
    end
end

function onMobDeath(mob, player, isKiller, firstCall)
    if firstCall then
        local instance = mob:getInstance()
        instance:setProgress(instance:getProgress() + 1)
    end
end
