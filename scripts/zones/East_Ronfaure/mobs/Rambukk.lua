-----------------------------------
-- Area: East Ronfaure
--  Mob: Rambukk
-----------------------------------
require("scripts/globals/hunts")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.hunts.checkHunt(mob, player, 152)
end

function onMobDespawn(mob)
    UpdateNMSpawnPoint(mob:getID())
end

return entity
