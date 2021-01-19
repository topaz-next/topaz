-----------------------------------
-- Area: Vunkerl Inlet [S]
--   NM: Warabouc
-----------------------------------
require("scripts/globals/hunts")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.hunts.checkHunt(mob, player, 487)
end

entity.onMobDespawn = function(mob)
    mob:setRespawnTime(7200 + math.random(0, 600)) -- 2 hours, then 10 minute window
end

return entity
