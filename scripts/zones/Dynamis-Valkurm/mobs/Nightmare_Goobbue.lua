-----------------------------------
-- Area: Dynamis - Valkurm
--  Mob: Nightmare Goobbue
-----------------------------------
mixins = {require("scripts/mixins/dynamis_dreamland")}
-----------------------------------
local entity = {}

entity.onMobSpawn = function(mob)
    mob:setLocalVar("dynamis_currency", 1449)
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity
