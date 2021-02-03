-----------------------------------
-- Area: Illrusi Atoll
--  MOB: Cursed Chest
-----------------------------------
local ID = require("scripts/zones/Ilrusi_Atoll/IDs")
require("scripts/globals/status")
require("scripts/globals/utils/assault")
-----------------------------------

function onMobSpawn(mob)
    local instance = mob:getInstance()
    local figureheadChest = instance:getProgress()
    
    if mob:getID() ~= figureheadChest then
        mob:setMobMod(dsp.mobMod.SOUND_RANGE, 2)
    else
        mob:setMobMod(dsp.mobMod.SOUND_RANGE, 0)
        mob:setMobMod(dsp.mobMod.NO_AGGRO, 1)
    end
    mob:setLocalVar("despawn", 0)
    mob:setStatus(dsp.status.NORMAL)
    mob:hideName(false)
end

function onTrigger(player, mob)
    if player:checkDistance(mob) >= 2 then
        player:messageSpecial(ID.text.CHEST_GET_CLOSER)
        return
    end
    
    if mob:getLocalVar("Complete") == 0 then
        player:messageSpecial(ID.text.CHEST)
        local mobID = mob:getID()
        local instance = mob:getInstance()
        local figureheadChest = instance:getProgress()

        if mobID == figureheadChest then
            mob:setLocalVar("Complete", 1)
            mob:entityAnimationPacket("open")
            player:messageSpecial(ID.text.GOLDEN)
            instance:setProgress(1)
            mob:timer(1000, function(mob) mob:entityAnimationPacket("open") end)
            mob:timer(15000, function(mob) mob:entityAnimationPacket("kesu") end)
            mob:timer(16000, function(mob) mob:setStatus(dsp.status.DISAPPEAR) end)
        else
            mob:setStatus(1)
            mob:updateClaim(player)
        end
    end
end

function onMobEngaged(mob, target)
    mob:setStatus(1)
    mob:hideName(false)
    mob:setModelId(258)
    mob:AnimationSub(0)
end

function onMobFight(mob, target)
    if mob:AnimationSub() ~= 1 then
        mob:AnimationSub(1)
    end
    if mob:checkDistance(target) < 21.6 then
        mob:setMobMod(dsp.mobMod.DRAW_IN, 3)
        mob:setLocalVar("despawn", 0)
    else
        mob:setMobMod(dsp.mobMod.DRAW_IN, 0)
        if mob:getLocalVar("despawn") == 0 then
            mob:setLocalVar("despawn", os.time() + 30)
        end
    end
    if mob:getLocalVar("despawn") ~= 0 then
        if mob:getLocalVar("despawn") < os.time() then
            mob:setStatus(dsp.status.NORMAL)
            mob:disengage()
            mob:AnimationSub(0)
            mob:setHP(mob:getMaxHP())
            mob:setModelId(960)
            mob:hideName(true)
        end
    end
end

function onMobDespawn(mob)
end

function onMobDeath(mob, player, isKiller)
end
