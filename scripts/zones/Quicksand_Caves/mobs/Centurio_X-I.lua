-----------------------------------
-- Area: Quicksand Caves
--   NM: Centurio X-I
-----------------------------------
require("scripts/globals/hunts")
mixins = {require("scripts/mixins/job_special")}
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMobMod(tpz.mobMod.ALWAYS_AGGRO, 1)
end

entity.onMobSpawn = function(mob)
    mob:addMod(tpz.mod.SILENCERES, 35)
    mob:addMod(tpz.mod.SLEEPRES, 50)
    mob:addMod(tpz.mod.LULLABYRES, 50)
    mob:addMod(tpz.mod.SPELLINTERRUPT, 25)
end

entity.onMobDeath = function(mob, player, isKiller)
    tpz.hunts.checkHunt(mob, player, 426)
end

return entity
