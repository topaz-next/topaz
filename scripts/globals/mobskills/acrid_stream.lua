-----------------------------------
--  Acrid Stream
--
--  Description: Deals water damage to enemies within a fan-shaped area originating from the caster. Additional effect: Lowers target's Magic Defense.
--  Type: Magical (Water)
--
--
-----------------------------------
require("scripts/globals/settings")
require("scripts/globals/status")
require("scripts/globals/monstertpmoves")
-----------------------------------
local mobskill_object = {}

mobskill_object.onMobSkillCheck = function(target, mob, skill)
    return 0
end

mobskill_object.onMobWeaponSkill = function(target, mob, skill)
    local typeEffect = tpz.effect.MAGIC_DEF_DOWN
    local power = 20

    local duration = 120

    MobStatusEffectMove(mob, target, typeEffect, power, 0, duration)

    local dmgmod = 1
    local info = MobMagicalMove(mob, target, skill, mob:getWeaponDmg()*3.5, tpz.magic.ele.WATER, dmgmod, TP_NO_EFFECT)
    local dmg = MobFinalAdjustments(info.dmg, mob, skill, target, tpz.attackType.MAGICAL, tpz.damageType.WATER, MOBPARAM_IGNORE_SHADOWS)
    target:takeDamage(dmg, mob, tpz.attackType.MAGICAL, tpz.damageType.WATER)
    return dmg
end

return mobskill_object
