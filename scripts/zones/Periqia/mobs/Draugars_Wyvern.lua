-----------------------------------
-- Area: Periqia (Requiem)
--  MOB: Draugars Wyvern
-----------------------------------
mixins = {require("scripts/mixins/pet_instanced")}
require("scripts/globals/status")
require("scripts/globals/utils/assault")
-----------------------------------

function onMobSpawn(mob)
    assaultUtil.adjustMobLevel(mob, mob:getID())
    mob:addImmunity(tpz.immunity.SLEEP)
end

function onMobDeath(mob, player, isKiller, firstCall)
end
