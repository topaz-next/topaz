-----------------------------------
-- Area: Outer Horutoto Ruins (194)
--   NM: Ah Puch
-----------------------------------
require("scripts/globals/hunts")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.hunts.checkHunt(mob, player, 291)
end

return entity
