-----------------------------------
-- Sandspin
-- Deals earth damage to enemies within range. Additional Effect: Accuracy Down.
-- Area of Effect is centered around caster.
-- The Additional Effect: Accuracy Down may not always process.
-- Duration: Three minutes ?
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
    local typeEffect = tpz.effect.ACCURACY_DOWN

    MobStatusEffectMove(mob, target, typeEffect, 50, 0, 120)

    local dmgmod = 1
    local info = MobMagicalMove(mob, target, skill, mob:getWeaponDmg()*2.3, tpz.magic.ele.EARTH, dmgmod, TP_MAB_BONUS, 1)
    local dmg = MobFinalAdjustments(info.dmg, mob, skill, target, tpz.attackType.MAGICAL, tpz.damageType.EARTH, MOBPARAM_IGNORE_SHADOWS)
    target:takeDamage(dmg, mob, tpz.attackType.MAGICAL, tpz.damageType.EARTH)
    return dmg

end

return mobskill_object
