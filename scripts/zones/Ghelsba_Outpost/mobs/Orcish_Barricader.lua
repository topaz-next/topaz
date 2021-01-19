-----------------------------------
-- Area: Ghelsba Outpost (140)
--  Mob: Orcish Barricader
-----------------------------------
require("scripts/globals/hunts")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.hunts.checkHunt(mob, player, 171)
end

entity.onMobDespawn = function(mob)
    mob:setRespawnTime(math.random(4200, 5700)) -- 70 to 95 min
end

return entity
