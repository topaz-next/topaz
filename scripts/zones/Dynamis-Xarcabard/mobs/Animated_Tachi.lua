-----------------------------------
-- Area: Dynamis - Xarcabard
--  Mob: Animated Tachi
-----------------------------------
require("scripts/globals/status")
local ID = require("scripts/zones/Dynamis-Xarcabard/IDs")
-----------------------------------
local entity = {}

entity.onMobEngaged = function(mob, target)

    if (mob:getAnimationSub() == 3) then
        SetDropRate(117, 1580, 1000)
    else
        SetDropRate(117, 1580, 0)
    end

    target:showText(mob, ID.text.ANIMATED_TACHI_DIALOG)

    SpawnMob(17330445):updateEnmity(target)
    SpawnMob(17330446):updateEnmity(target)
    SpawnMob(17330447):updateEnmity(target)
    SpawnMob(17330457):updateEnmity(target)
    SpawnMob(17330458):updateEnmity(target)
    SpawnMob(17330459):updateEnmity(target)

end

entity.onMobFight = function(mob, target)
    -- TODO: add battle dialog
end

entity.onMobDisengage = function(mob)
    mob:showText(mob, ID.text.ANIMATED_TACHI_DIALOG+2)
end

entity.onMobDeath = function(mob, player, isKiller)

    player:showText(mob, ID.text.ANIMATED_TACHI_DIALOG+1)

    DespawnMob(17330445)
    DespawnMob(17330446)
    DespawnMob(17330447)
    DespawnMob(17330457)
    DespawnMob(17330458)
    DespawnMob(17330459)

end

return entity
