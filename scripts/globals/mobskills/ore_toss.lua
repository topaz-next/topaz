-----------------------------------
--  Ore Toss
--
--  Description: Deals high damage in a ranged attack.
--  Type: Ranged
--  Utsusemi/Blink absorb: Yes
--  Range: Unknown range
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
    local numhits = 1
    local accmod = 1
    local dmgmod = math.random(3, 6)
    local info = MobPhysicalMove(mob, target, skill, numhits, accmod, dmgmod, TP_DMG_VARIES, 1, 2, 3)
    local dmg = MobFinalAdjustments(info.dmg, mob, skill, target, tpz.attackType.RANGED, tpz.damageType.BLUNT, MOBPARAM_1_SHADOW)
    target:takeDamage(dmg, mob, tpz.attackType.RANGED, tpz.damageType.BLUNT)
    return dmg
end

return mobskill_object
