-----------------------------------
--  Dark Mist
--  Family: Gargouille
--  Description: Deals dark damage to an enemy. Additional effect: Weight
--  Type: Magical (dark)
--  Utsusemi/Blink absorb: Ignores shadows
--  Range: Radial
--  Notes: Only used when flying
-----------------------------------
require("scripts/globals/settings")
require("scripts/globals/status")
require("scripts/globals/monstertpmoves")
-----------------------------------
local mobskill_object = {}

mobskill_object.onMobSkillCheck = function(target, mob, skill)
    if (mob:getAnimationSub() ~=1) then
        return 1
    else
        return 0
    end
end

mobskill_object.onMobWeaponSkill = function(target, mob, skill)
    local numhits = 1
    local dmgmod = 1
    local typeEffect = tpz.effect.WEIGHT

    local info = MobMagicalMove(mob, target, skill, mob:getWeaponDmg()*4.0, tpz.magic.ele.DARK, dmgmod, TP_MAB_BONUS)
    local dmg = MobFinalAdjustments(info.dmg, mob, skill, target, tpz.attackType.MAGICAL, tpz.damageType.DARK, MOBPARAM_IGNORE_SHADOWS)
    target:takeDamage(dmg, mob, tpz.attackType.MAGICAL, tpz.damageType.DARK)
    MobStatusEffectMove(mob, target, typeEffect, 50, 0, 60)

    return dmg
end

return mobskill_object
