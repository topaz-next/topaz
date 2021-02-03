-----------------------------------
-- Area: Lebros Cavern (Troll Fugitives)
--  MOB: Troll Fugitive
-----------------------------------
mixins = {require("scripts/mixins/job_special")}
require("scripts/globals/status")
-----------------------------------

function onMobInitialize(mob)
    mob:addStatusEffect(dsp.effect.NO_REST,1,0,0);
end

function onMobSpawn(mob)
    local MaxHP = mob:getHP()
    local bonus = math.random(2,6)
    
    mob:setHP(MaxHP / (8/(bonus)))
end

function onMobDeath(mob, player, isKiller, firstCall)
    local instance = mob:getInstance()
    
    if firstCall then
        instance:setProgress(instance:getProgress() + 1)
    end
end
