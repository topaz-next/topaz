-----------------------------------
-- Area: Dynamis - Xarcabard
--  Mob: Animated Gun
-----------------------------------
require("scripts/globals/status")
local ID = require("scripts/zones/Dynamis-Xarcabard/IDs")
-----------------------------------
local entity = {}

entity.onMobEngaged = function(mob, target)

    if (mob:getAnimationSub() == 3) then
        SetDropRate(105, 1585, 1000)
    else
        SetDropRate(105, 1585, 0)
    end

    target:showText(mob, ID.text.ANIMATED_GUN_DIALOG)

    SpawnMob(17330513):updateEnmity(target)
    SpawnMob(17330514):updateEnmity(target)
    SpawnMob(17330515):updateEnmity(target)
    SpawnMob(17330516):updateEnmity(target)
    SpawnMob(17330517):updateEnmity(target)
    SpawnMob(17330518):updateEnmity(target)

end

entity.onMobFight = function(mob, target)
    -- TODO: add battle dialog
end

entity.onMobDisengage = function(mob)
    mob:showText(mob, ID.text.ANIMATED_GUN_DIALOG+2)
end

entity.onMobDeath = function(mob, player, isKiller)

    player:showText(mob, ID.text.ANIMATED_GUN_DIALOG+1)

    DespawnMob(17330513)
    DespawnMob(17330514)
    DespawnMob(17330515)
    DespawnMob(17330516)
    DespawnMob(17330517)
    DespawnMob(17330518)

end

return entity
