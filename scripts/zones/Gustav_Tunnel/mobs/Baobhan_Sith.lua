-----------------------------------
-- Area: Gustav Tunnel
--   NM: Baobhan Sith
-----------------------------------
require("scripts/globals/hunts")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.hunts.checkHunt(mob, player, 425)
end

return entity
