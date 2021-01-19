-----------------------------------
-- Area: Caedarva Mire
--   NM: Aynu-kaysey
-----------------------------------
require("scripts/globals/hunts")
mixins = {require("scripts/mixins/families/qutrub")}
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    tpz.hunts.checkHunt(mob, player, 470)
end

entity.onMobDespawn = function(mob)
    mob:setRespawnTime(math.random(7200, 14400)) -- 2 to 4 hours
end

return entity
