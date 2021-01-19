-----------------------------------
-- Area: Gusgen Mines
--   NM: Juggler Hecatomb
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    mob:setRespawnTime(math.random(75600, 86400)) -- 21 to 24 hours
end


return entity
