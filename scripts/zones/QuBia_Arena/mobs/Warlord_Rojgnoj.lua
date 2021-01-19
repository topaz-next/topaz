-----------------------------------
-- Area: QuBia_Arena
--  Mob: Warlord Rojgnoj
-- Mission 9-2 SANDO
-----------------------------------
mixins = {require("scripts/mixins/job_special")}
require("scripts/globals/status")
local ID = require("scripts/zones/QuBia_Arena/IDs")
-----------------------------------
local entity = {}

local function phaseChangeReady(battlefield)
    local inst = battlefield:getArea()
    local instOffset = ID.mob.HEIR_TO_THE_LIGHT_OFFSET + (14 * (inst-1))
    for i = instOffset + 3, instOffset + 13 do
        if not GetMobByID(i):isDead() then
            return false
        end
    end
    return true
end

entity.onMobInitialize = function(mob)
    mob:addMod(tpz.mod.SLEEPRES, 50)
end

entity.onMobSpawn = function(mob)
    local battlefield = mob:getBattlefield()
    if battlefield and phaseChangeReady(battlefield) then
        battlefield:setLocalVar("phaseChange", 0)
    end
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity
