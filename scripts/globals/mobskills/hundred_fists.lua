-----------------------------------
-- Hundred Fists
-----------------------------------
require("scripts/globals/monstertpmoves")
require("scripts/globals/status")
require("scripts/globals/msg")
-----------------------------------
local mobskill_object = {}

mobskill_object.onMobSkillCheck = function(target, mob, skill)
    return 0
end

mobskill_object.onMobWeaponSkill = function(target, mob, skill)
    MobBuffMove(mob, tpz.effect.HUNDRED_FISTS, 1, 0, 30)

    skill:setMsg(tpz.msg.basic.USES)

    return tpz.effect.HUNDRED_FISTS
end

return mobskill_object
