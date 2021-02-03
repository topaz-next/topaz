-----------------------------------
-- Area: Mamool Ja Training Grounds (Imperial Agent Rescue)
--  MOB: Mamool Ja Lizard
-----------------------------------
mixins = {require("scripts/mixins/pet_instanced")}
require("scripts/globals/status")
-----------------------------------

function onMobSpawn(mob)
    assaultUtil.adjustMobLevel(mob, mob:getID())
    mob:setLocalVar("masterID", mob:getID() -1)
end

function onMobDeath(mob, player, isKiller)
end
