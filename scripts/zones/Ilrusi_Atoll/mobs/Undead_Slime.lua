-----------------------------------
-- Area: Ilrusi Atoll (Extermination)
--  Mob: Undead Slime
-----------------------------------
require("scripts/globals/assault")
-----------------------------------
local entity = {}

entity.onMobSpawn = function(mob)
    assaultUtil.adjustMobLevel(mob, mob:getID())
end

entity.onMobDeath = function(mob, player, isKiller, firstCall)
    if firstCall then
        local instance = mob:getInstance()
        instance:setProgress(instance:getProgress() + 1)
    end
end

return entity
