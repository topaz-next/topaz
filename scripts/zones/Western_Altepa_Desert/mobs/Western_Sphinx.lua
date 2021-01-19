-----------------------------------
-- Area: Western Altepa Desert
--  Mob: Western Sphinx
-----------------------------------
local ID = require("scripts/zones/Western_Altepa_Desert/IDs")
require("scripts/globals/missions")
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMobMod(tpz.mobMod.IDLE_DESPAWN, 180)
end

entity.onMobSpawn = function(mob)
    DespawnMob(mob:getID(), 180)
end

entity.onMobDeath = function(mob, player, isKiller)
    if
        GetMobByID(ID.mob.EASTERN_SPHINX):isDead() and
        GetMobByID(ID.mob.WESTERN_SPHINX):isDead() and
        player:getCurrentMission(BASTOK) == tpz.mission.id.bastok.RETURN_OF_THE_TALEKEEPER and
        player:getCharVar("MissionStatus") == 2
    then
        player:setCharVar("Mission6-1MobKilled", 1)
    end
end

return entity
