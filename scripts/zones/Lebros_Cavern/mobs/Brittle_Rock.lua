-----------------------------------
-- Area: Lebros Cavern (Excavation Duty)
-- Mob: Brittle Rock
-----------------------------------
local ID = require("scripts/zones/Lebros_Cavern/IDs")
require("scripts/globals/missions")
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    --[[ TODO: Handle these
    mob:addStatusEffect(tpz.effect.NO_REST, 1, 0, 0)
    mob:addImmunity(tpz.immunity.POISON)
    mob:addImmunity(tpz.immunity.LIGHTSLEEP)
    mob:addImmunity(tpz.immunity.DARKSLEEP)
    mob:setMobMod(tpz.mobMod.EXP_BONUS, -100)
    mob:setMobMod(tpz.mobMod.NO_MOVE, 1)
    mob:SetAutoAttackEnabled(false)
    mob:setMod(tpz.mod.UDMGPHYS,-75)
    mob:setMod(tpz.mod.UDMGBREATH,-95)
    mob:setMod(tpz.mod.UDMGRANGE,-95)
    mob:setMod(tpz.mod.UDMGMAGIC, -90)
    mob:setMod(tpz.mod.EVA, 0)
    mob:setMod(tpz.mod.CURSEEVA, 9999)
    ]]
end

entity.onMobSpawn(mob)
    mob:setMod(tpz.mod.NO_DROPS, 1)
    mob:addListener("WEAPONSKILL_TAKE", "BRITTLE_ROCK_WEAPONSKILL_TAKE", function(mob, user, wsid)
        if wsid == 1838 then
            mob:setHP(0)
        end
    end)
end

entity.onMobFight(mob, target)
end

entity.onMobDeath(mob, player, isKiller)
    local instance = mob:getInstance()
    if mob:getID() == ID.mob[EXCAVATION_DUTY].MOBS_START.BRITTLE_ROCK1 then
        instance:getEntity(bit.band(ID.npc._1rx, 0xFFF), tpz.objType.NPC):setAnimation(8)
    elseif mob:getID() == ID.mob[EXCAVATION_DUTY].MOBS_START.BRITTLE_ROCK2 then
        instance:getEntity(bit.band(ID.npc._1ry, 0xFFF), tpz.objType.NPC):setAnimation(8)
    elseif mob:getID() == ID.mob[EXCAVATION_DUTY].MOBS_START.BRITTLE_ROCK3 then
        instance:getEntity(bit.band(ID.npc._1rz, 0xFFF), tpz.objType.NPC):setAnimation(8)
    elseif mob:getID() == ID.mob[EXCAVATION_DUTY].MOBS_START.BRITTLE_ROCK4 then
        instance:getEntity(bit.band(ID.npc._jr0, 0xFFF), tpz.objType.NPC):setAnimation(8)
    elseif mob:getID() == ID.mob[EXCAVATION_DUTY].MOBS_START.BRITTLE_ROCK5 then
        instance:getEntity(bit.band(ID.npc._jr1, 0xFFF), tpz.objType.NPC):setAnimation(8)
    end

    if mob:getLocalVar("Killed") == 0 then
        instance:setProgress(instance:getProgress() + 1)
        mob:setLocalVar("Killed", 1)
    end
end

return entity
