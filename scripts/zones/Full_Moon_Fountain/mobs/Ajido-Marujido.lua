-----------------------------------
-- Area: Full Moon Fountain
--  Mob: Ajido-Marujido
-- Ally during Windurst Mission 9-2
-----------------------------------
local ID = require("scripts/zones/Full_Moon_Fountain/IDs")
require("scripts/globals/status")
require("scripts/globals/magic")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:setMod(tpz.mod.REFRESH, 1)
    mob:setMobMod(tpz.mobMod.TELEPORT_CD, 30)
end

entity.onMobSpawn = function(mob)
    mob:addListener("MAGIC_START", "MAGIC_MSG", function(mob, spell, action)
        -- Burst
        if spell:getID() == 212 then
            mob:showText(mob, ID.text.PLAY_TIME_IS_OVER)
        -- Flood
        elseif spell:getID() == 214 then
            mob:showText(mob, ID.text.YOU_SHOULD_BE_THANKFUL)
        end
    end)
end

entity.onMobRoam = function(mob)
    local wait = mob:getLocalVar("wait")
    if wait > 40 then
        -- pick a random living target from the two enemies
        local inst = mob:getBattlefield():getArea()
        local instOffset = ID.mob.MOON_READING_OFFSET + (6 * (inst - 1))
        local target = GetMobByID(instOffset + math.random(4, 5))
        if not target:isDead() then
            mob:addEnmity(target, 0, 1)
            mob:setLocalVar("wait", 0)
        end
    else
        mob:setLocalVar("wait", wait+3)
    end
end

entity.onMobEngaged = function(mob, target)
    mob:setMobMod(tpz.mobMod.TELEPORT_TYPE, 0)
end

entity.onMobFight = function(mob, target)
    if mob:getHPP() < 50 and mob:getLocalVar("saidMessage") == 0 then
        mob:showText(mob, ID.text.DONT_GIVE_UP)
        mob:setLocalVar("saidMessage", 1)
    end
    if target:isEngaged() then
        mob:setMobMod(tpz.mobMod.TELEPORT_TYPE, 1)
    end
end

entity.onMobDisengage = function(mob)
    mob:setLocalVar("wait", 0)
end

entity.onMobDeath = function(mob, player, isKiller)
    mob:getBattlefield():lose()
    local players = mob:getBattlefield():getPlayers()
    for _, player in pairs(players) do
        player:messageSpecial(ID.text.UNABLE_TO_PROTECT)
    end
end

return entity
