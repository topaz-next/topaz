-----------------------------------
-- Area: Empyreal Paradox
--  Mob: Promathia
-- Note: Phase 2
-----------------------------------
local ID = require("scripts/zones/Empyreal_Paradox/IDs")
require("scripts/globals/status")
require("scripts/globals/titles")
require("scripts/globals/magic")
-----------------------------------
local entity = {}

entity.onMobInitialize = function(mob)
    mob:addMod(tpz.mod.REGAIN, 50)
    mob:addMod(tpz.mod.UFASTCAST, 50)
end

entity.onMobSpawn = function(mob)
    local battlefield = mob:getBattlefield()
    if GetMobByID(ID.mob.PROMATHIA_OFFSET + (battlefield:getArea() - 1) * 2):isDead() then
        battlefield:setLocalVar("phaseChange", 0)
    end
end

entity.onMobEngaged = function(mob, target)
    local bcnmAllies = mob:getBattlefield():getAllies()
    for i, v in pairs(bcnmAllies) do
        if v:getName() == "Prishe" then
            if not v:getTarget() then
                v:entityAnimationPacket("prov")
                v:showText(v, ID.text.PRISHE_TEXT + 1)
                v:setLocalVar("ready", mob:getID())
            end
        else
            v:addEnmity(mob, 0, 1)
        end
    end
end

entity.onMobFight = function(mob, target)
    if mob:getAnimationSub() == 3 and not mob:hasStatusEffect(tpz.effect.STUN) then
        mob:setAnimationSub(0)
        mob:stun(1500)
    elseif mob:getAnimationSub() == 2 and not mob:hasStatusEffect(tpz.effect.MAGIC_SHIELD) then
        mob:setAnimationSub(0)
    elseif mob:getAnimationSub() == 1 and not mob:hasStatusEffect(tpz.effect.PHYSICAL_SHIELD) then
        mob:setAnimationSub(0)
    end

    local bcnmAllies = mob:getBattlefield():getAllies()
    for i, v in pairs(bcnmAllies) do
        if not v:getTarget() then
            v:addEnmity(mob, 0, 1)
        end
    end
end

entity.onSpellPrecast = function(mob, spell)
    if spell:getID() == 218 then
        spell:setAoE(tpz.magic.aoe.RADIAL)
        spell:setFlag(tpz.magic.spellFlag.HIT_ALL)
        spell:setRadius(30)
        spell:setAnimation(280)
        spell:setMPCost(1)
    elseif spell:getID() == 219 then
        spell:setMPCost(1)
    end
end

entity.onMagicCastingCheck = function(mob, target, spell)
    if math.random() > 0.75 then
        return 219
    else
        return 218
    end
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity
