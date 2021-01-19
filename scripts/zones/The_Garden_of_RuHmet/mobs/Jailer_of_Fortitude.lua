-----------------------------------
-- Area: The Garden of Ru'Hmet
--   NM: Jailer of Fortitude
-----------------------------------
local ID = require("scripts/zones/The_Garden_of_RuHmet/IDs")
mixins = {require("scripts/mixins/job_special")}
require("scripts/globals/settings")
require("scripts/globals/limbus")
require("scripts/globals/status")
require("scripts/globals/magic")
-----------------------------------
local entity = {}

entity.onMobSpawn = function(mob)
    tpz.mix.jobSpecial.config(mob, {
        specials =
        {
            {id = tpz.jsa.INVINCIBLE, cooldown = 180, hpp = math.random(90, 95)}, -- "Has access to Invincible, which it may use several times."
        },
    })

    -- Change animation to humanoid w/ prismatic core
    mob:setAnimationSub(1)
    mob:setModelId(1169)
end

entity.onMobFight = function(mob, target)
    local delay = mob:getLocalVar("delay")
    local LastCast = mob:getLocalVar("LAST_CAST")
    local spell = mob:getLocalVar("COPY_SPELL")

    if (mob:getBattleTime() - LastCast > 30) then
        mob:setLocalVar("COPY_SPELL", 0)
        mob:setLocalVar("delay", 0)
    end

    if (not GetMobByID(ID.mob.KFGHRAH_WHM):isDead() or not GetMobByID(ID.mob.KFGHRAH_BLM):isDead()) then -- check for kf'ghrah
        if (spell > 0 and not mob:hasStatusEffect(tpz.effect.SILENCE)) then
            if (delay >= 3) then
                mob:castSpell(spell)
                mob:setLocalVar("COPY_SPELL", 0)
                mob:setLocalVar("delay", 0)
            else
                mob:setLocalVar("delay", delay+1)
            end
        end
    end
end

entity.onMagicHit = function(caster, target, spell)
    if (spell:tookEffect() and (caster:isPC() or caster:isPet()) and spell:getSpellGroup() ~= tpz.magic.spellGroup.BLUE ) then
        -- Handle mimicked spells
        target:setLocalVar("COPY_SPELL", spell:getID())
        target:setLocalVar("LAST_CAST", target:getBattleTime())
        target:setLocalVar("reflectTime", target:getBattleTime())
        target:setAnimationSub(1)
    end

    return 1
end

entity.onMobDeath = function(mob, player, isKiller)
    -- Despawn the pets if alive
    DespawnMob(ID.mob.KFGHRAH_WHM)
    DespawnMob(ID.mob.KFGHRAH_BLM)
end

entity.onMobDespawn = function(mob)
    -- Move QM to random location
    local pos = math.random(1, 5)
    GetNPCByID(ID.npc.JAILER_OF_FORTITUDE_QM):setPos(ID.npc.JAILER_OF_FORTITUDE_QM_POS[pos][1], ID.npc.JAILER_OF_FORTITUDE_QM_POS[pos][2], ID.npc.JAILER_OF_FORTITUDE_QM_POS[pos][3])
end

return entity
