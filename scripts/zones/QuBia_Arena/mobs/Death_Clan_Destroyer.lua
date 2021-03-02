-----------------------------------
-- Area: QuBia_Arena
--  Mob: Death Clan Destroyer
-----------------------------------
local global = require("scripts/zones/QuBia_Arena/Globals")
local ID = require("scripts/zones/QuBia_Arena/IDs")
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMobMod(tpz.mobMod.HP_STANDBACK, 60)
end

entity.onMobFight = function(mob, target)
    local inst = mob:getBattlefield():getArea()
    local instOffset = ID.mob.HEIR_TO_THE_LIGHT_OFFSET + (14 * (inst-1))
    mob:setMP(9999)

    -- queue curaga II on any sleeping ally
    for i = instOffset + 3, instOffset + 12 do
        if GetMobByID(i):getCurrentAction() == tpz.act.SLEEP then
            if mob:actionQueueEmpty() then
                if mob:getLocalVar("cooldown") == 0 then
                    mob:castSpell(8, GetMobByID(i))
                    mob:setLocalVar("cooldown", 20)
                end
            else
                mob:setLocalVar("cooldown", 20)
            end
        end
    end
    if mob:getLocalVar("cooldown") > 0 then
        mob:setLocalVar("cooldown", mob:getLocalVar("cooldown") - 1)
    end
end

entity.onMobDeath = function(mob, player, isKiller)
    local battlefield = player:getBattlefield()
    if battlefield and global.phaseChangeReady(battlefield) then
        player:release() -- prevents event collision if player kills multiple remaining mobs with an AOE move/spell
        player:startEvent(32004, 0, 0, 4)
    end
end

return entity
