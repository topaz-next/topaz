-----------------------------------
-- Area: RuAun Gardens
--  Mob: Groundskeeper
-- Note: Place holder Despot
-----------------------------------
local ID = require("scripts/zones/RuAun_Gardens/IDs")
require("scripts/globals/regimes")
require("scripts/globals/mobs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.regime.checkRegime(player, mob, 143, 2, tpz.regime.type.FIELDS)
    tpz.regime.checkRegime(player, mob, 144, 1, tpz.regime.type.FIELDS)
    if isKiller then
        mob:setLocalVar("killer", player:getID())
    end
end

entity.onMobDespawn = function(mob)
    if tpz.mob.phOnDespawn(mob, ID.mob.DESPOT_PH, 5, 7200, true) then -- 2 hours
        local phId = mob:getID()
        local nmId = ID.mob.DESPOT_PH[phId]
        GetMobByID(nmId):addListener("SPAWN", "PH_VAR", function(m)
            m:setLocalVar("ph", phId)
        end)
    end
end

return entity
