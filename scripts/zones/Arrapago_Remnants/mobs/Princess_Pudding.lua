-----------------------------------
-- Area: Arrapago Remnants
--  Mob: Princess Pudding
-----------------------------------
local ID = require("scripts/zones/Arrapago_Remnants/IDs")
require("scripts/globals/status")
-----------------------------------
local entity = {}

function onMobSpawn(mob)
    local instance = mob:getInstance()
    local slot = GetNPCByID(ID.npc[2][2].SLOT, instance)
        slot:setStatus(tpz.status.DISAPPEAR)
end

entity.onMobDeath = function(mob, player, isKiller)
end

function onMobDespawn(mob)
end

return entity
