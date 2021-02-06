-----------------------------------
-- Area: Periqia (Requiem)
--  Mob: Darkling Draugar (DRK)
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobSpawn = function(mob)
    mob:setMod(tpz.mod.SLEEPEVA_DARK, 9999)
end

entity.onMobDeath = function(mob, player, isKiller, firstCall)
    if firstCall then
        local instance = mob:getInstance()
        instance:setProgress(instance:getProgress() + 1)
    end
end

return entity
