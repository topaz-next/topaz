-----------------------------------
-- Area: Beaucedine Glacier [S]
--   NM: Becut
-----------------------------------
require("scripts/globals/hunts")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.hunts.checkHunt(mob, player, 537)
end

return entity
