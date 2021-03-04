-----------------------------------
-- Area: Periqia (Requiem)
--  Mob: Putrid Immortal Guard
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller, firstCall)
    if firstCall then
        local instance = mob:getInstance()
        instance:setProgress(instance:getProgress() + 1)
    end
end

return entity
