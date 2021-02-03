-----------------------------------
-- Area: Periqia (Requiem)
--  MOB: Putrid Immortal Guard
-----------------------------------

function onMobDeath(mob, player, isKiller, firstCall)
    if firstCall then
        local instance = mob:getInstance()
        instance:setProgress(instance:getProgress() + 1)
    end
end
