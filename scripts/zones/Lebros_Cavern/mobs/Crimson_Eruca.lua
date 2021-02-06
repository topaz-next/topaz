-----------------------------------
-- Area: Lebros Cavern
--  Mob: Crimson Eruca
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobSpawn = function(mob)
    -- TODO: Handle resists
	-- mob:addResist({ tpz.resist.ENFEEBLING_LIGHTSLEEP, 25, 0 })
    -- mob:addResist({ tpz.resist.ENFEEBLING_DARKSLEEP, 25, 0 })
    -- mob:addResist({ tpz.resist.ENFEEBLING_GRAVITY, 25, 0 })
    -- mob:addResist({ tpz.resist.ENFEEBLING_BIND, 25, 0 })
end

entity.onMobDeath(mob, player, isKiller, firstCall)
end

return entity
