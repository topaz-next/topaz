-----------------------------------
-- Area: Bhaflau Thickets
--  Mob: Olden Treant
-- Note: Place holder Emergent Elm
-----------------------------------
local ID = require("scripts/zones/Bhaflau_Thickets/IDs")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    tpz.mob.phOnDespawn(mob, ID.mob.EMERGENT_ELM_PH, 5, 14400) -- 4 hours
end

return entity
