-----------------------------------
-- Area: Leujaoam Sanctum (Orichalcum Survey)
--  MOB: Mineral Eater
-- Immune to Charm
-----------------------------------
require("scripts/globals/status")
-----------------------------------

function onMobSpawn(mob)
    assaultUtil.adjustMobLevel(mob, mob:getID())
end

function onMobDeath(mob, player, isKiller)
end
