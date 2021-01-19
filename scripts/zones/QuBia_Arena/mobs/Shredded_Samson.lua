-----------------------------------
-- Area: Qu'Bia Arena
--  Mob: Shredded Samson
-- BCNM: Celery
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobEngaged = function(mob, target)
    local mobId = mob:getID()
    DespawnMob(mobId - 1)
    DespawnMob(mobId + 1)
    DespawnMob(mobId + 2)
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity
