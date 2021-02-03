-----------------------------------
-- Area: Lebros Cavern (Excavation Duty)
-- MOB: Brittle Rock
-----------------------------------
local ID = require("scripts/zones/Lebros_Cavern/IDs")
require("scripts/globals/missions")
require("scripts/globals/status")
-----------------------------------

function onMobInitialize(mob)
    mob:addStatusEffect(dsp.effect.NO_REST,1,0,0)
    mob:addImmunity(dsp.immunity.POISON)
    mob:addImmunity(dsp.immunity.LIGHTSLEEP)
    mob:addImmunity(dsp.immunity.DARKSLEEP)
    mob:setMobMod(dsp.mobMod.EXP_BONUS, -100)
    mob:setMobMod(dsp.mobMod.NO_MOVE, 1)
    mob:SetAutoAttackEnabled(false)
    mob:setMod(dsp.mod.UDMGPHYS,-75)
    mob:setMod(dsp.mod.UDMGBREATH,-95)
    mob:setMod(dsp.mod.UDMGRANGE,-95)
    mob:setMod(dsp.mod.UDMGMAGIC, -90)
    mob:setMod(dsp.mod.EVA, 0)
    mob:setMod(dsp.mod.CURSEEVA, 9999)
end

function onMobSpawn(mob)
    mob:setMod(dsp.mod.NO_DROPS, 1)
    mob:addListener("WEAPONSKILL_TAKE", "BRITTLE_ROCK_WEAPONSKILL_TAKE", function(mob, user, wsid)
        if wsid == 1838 then
            mob:setHP(0)
        end
    end)
end

function onMobFight(mob, target)
end

function onMobDeath(mob, player, isKiller)
    local instance = mob:getInstance()
    if mob:getID() == ID.mob[EXCAVATION_DUTY].MOBS_START.BRITTLE_ROCK1 then
        instance:getEntity(bit.band(ID.npc._1rx, 0xFFF), dsp.objType.NPC):setAnimation(8)
    elseif mob:getID() == ID.mob[EXCAVATION_DUTY].MOBS_START.BRITTLE_ROCK2 then
        instance:getEntity(bit.band(ID.npc._1ry, 0xFFF), dsp.objType.NPC):setAnimation(8)
    elseif mob:getID() == ID.mob[EXCAVATION_DUTY].MOBS_START.BRITTLE_ROCK3 then
        instance:getEntity(bit.band(ID.npc._1rz, 0xFFF), dsp.objType.NPC):setAnimation(8)
    elseif mob:getID() == ID.mob[EXCAVATION_DUTY].MOBS_START.BRITTLE_ROCK4 then
        instance:getEntity(bit.band(ID.npc._jr0, 0xFFF), dsp.objType.NPC):setAnimation(8)
    elseif mob:getID() == ID.mob[EXCAVATION_DUTY].MOBS_START.BRITTLE_ROCK5 then
        instance:getEntity(bit.band(ID.npc._jr1, 0xFFF), dsp.objType.NPC):setAnimation(8)
    end
    if mob:getLocalVar("Killed") == 0 then
        instance:setProgress(instance:getProgress() + 1)
        mob:setLocalVar("Killed", 1)
    end
end
