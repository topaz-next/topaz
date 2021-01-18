-----------------------------------
-- Dissipation
-- Dispels all buffs add terror effect
-----------------------------------
require("scripts/globals/monstertpmoves")
require("scripts/globals/settings")
require("scripts/globals/status")
require("scripts/globals/msg")
-----------------------------------
local mobskill_object = {}

mobskill_object.onMobSkillCheck = function(target, mob, skill)
    return 1
end

mobskill_object.onMobWeaponSkill = function(target, mob, skill)
    local typeEffect = tpz.effect.TERROR
    MobStatusEffectMove(mob, target, typeEffect, 1, 0, 10)

    local count = target:dispelAllStatusEffect()

    if (count == 0) then
        skill:setMsg(tpz.msg.basic.SKILL_NO_EFFECT)
    else
        skill:setMsg(tpz.msg.basic.DISAPPEAR_NUM)
    end

    return count
end

return mobskill_object
