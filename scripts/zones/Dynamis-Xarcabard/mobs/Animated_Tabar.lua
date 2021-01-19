-----------------------------------
-- Area: Dynamis - Xarcabard
--  Mob: Animated Tabar
-----------------------------------
require("scripts/globals/status")
local ID = require("scripts/zones/Dynamis-Xarcabard/IDs")
-----------------------------------
local entity = {}

entity.onMobEngaged = function(mob, target)

    if (mob:getAnimationSub() == 3) then
        SetDropRate(116, 1575, 1000)
    else
        SetDropRate(116, 1575, 0)
    end

    target:showText(mob, ID.text.ANIMATED_TABAR_DIALOG)

    SpawnMob(17330380):updateEnmity(target)
    SpawnMob(17330381):updateEnmity(target)
    SpawnMob(17330382):updateEnmity(target)
    SpawnMob(17330392):updateEnmity(target)
    SpawnMob(17330393):updateEnmity(target)
    SpawnMob(17330394):updateEnmity(target)

end

entity.onMobFight = function(mob, target)
    -- TODO: add battle dialog
end

entity.onMobDisengage = function(mob)
    mob:showText(mob, ID.text.ANIMATED_TABAR_DIALOG+2)
end

entity.onMobDeath = function(mob, player, isKiller)

    player:showText(mob, ID.text.ANIMATED_TABAR_DIALOG+1)

    DespawnMob(17330380)
    DespawnMob(17330381)
    DespawnMob(17330382)
    DespawnMob(17330392)
    DespawnMob(17330393)
    DespawnMob(17330394)

end

return entity
