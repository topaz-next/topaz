-----------------------------------
-- Area: Qu'Bia Arena
--  Mob: Trion
-- Ally during San d'Oria Mission 9-2
-----------------------------------
local ID = require("scripts/zones/QuBia_Arena/IDs")
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:addMod(tpz.mod.REGAIN, 30)
end

entity.onMobSpawn = function(mob)
    mob:addListener("WEAPONSKILL_STATE_ENTER", "WS_START_MSG", function(mob, skillID)
        -- Red Lotus Blade
        if skillID == 968 then
            mob:showText(mob, ID.text.RLB_PREPARE)
        -- Flat Blade
        elseif skillID == 969 then
            mob:showText(mob, ID.text.FLAT_PREPARE)
        -- Savage Blade
        elseif skillID == 970 then
            mob:showText(mob, ID.text.SAVAGE_PREPARE)
        end
    end)
end

entity.onMobDisengage = function(mob)
    mob:setLocalVar("wait", 0)
end

entity.onMobRoam = function(mob)
    local wait = mob:getLocalVar("wait")
    if wait > 40 then
        -- pick a random living target from the three enemies
        local inst = mob:getBattlefield():getArea()
        local instOffset = ID.mob.HEIR_TO_THE_LIGHT_OFFSET + (14 * (inst-1))
        local target = GetMobByID(instOffset + math.random(0, 2))
        if not target:isDead() then
            mob:addEnmity(target, 0, 1)
            mob:setLocalVar("wait", 0)
        end
    else
        mob:setLocalVar("wait", wait+3)
    end
end

entity.onMobDeath = function(mob, player, isKiller)
    mob:getBattlefield():lose()
end

return entity
