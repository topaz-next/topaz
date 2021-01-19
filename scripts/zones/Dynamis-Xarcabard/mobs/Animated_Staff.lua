-----------------------------------
-- Area: Dynamis - Xarcabard
--  Mob: Animated Staff
-----------------------------------
require("scripts/globals/status")
local ID = require("scripts/zones/Dynamis-Xarcabard/IDs")
-----------------------------------
local entity = {}

entity.onMobEngaged = function(mob, target)

    if (mob:getAnimationSub() == 3) then
        SetDropRate(115, 1582, 1000)
    else
        SetDropRate(115, 1582, 0)
    end

    target:showText(mob, ID.text.ANIMATED_STAFF_DIALOG)

    SpawnMob(17330337):updateEnmity(target)
    SpawnMob(17330338):updateEnmity(target)
    SpawnMob(17330339):updateEnmity(target)
    SpawnMob(17330347):updateEnmity(target)
    SpawnMob(17330348):updateEnmity(target)
    SpawnMob(17330349):updateEnmity(target)

end

entity.onMobFight = function(mob, target)
    -- TODO: add battle dialog
end

entity.onMobDisengage = function(mob)
    mob:showText(mob, ID.text.ANIMATED_STAFF_DIALOG+2)
end

entity.onMobDeath = function(mob, player, isKiller)

    player:showText(mob, ID.text.ANIMATED_STAFF_DIALOG+1)

    DespawnMob(17330337)
    DespawnMob(17330338)
    DespawnMob(17330339)
    DespawnMob(17330347)
    DespawnMob(17330348)
    DespawnMob(17330349)

end

return entity
