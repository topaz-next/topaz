-----------------------------------
-- Area: Ilrusi Atoll (Extermination)
--  Mob: Undead Leech
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    local instance = mob:getInstance()

    instance:setProgress(instance:getProgress() + 1)
end

return entity
