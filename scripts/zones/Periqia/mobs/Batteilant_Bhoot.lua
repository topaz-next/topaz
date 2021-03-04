-----------------------------------
-- Area: Periqia (Requiem)
--  Mob: Batteilant Bhoot
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobSpawn = function(mob)
    mob:addMod(tpz.mod.FASTCAST, 50)
end

entity.onMobDeath = function(mob, player, isKiller, firstCall)
    if firstCall then
        local instance = mob:getInstance()
        instance:setProgress(instance:getProgress() + 1)
    end
end

return entity
