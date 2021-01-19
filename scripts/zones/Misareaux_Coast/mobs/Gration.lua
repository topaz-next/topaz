-----------------------------------
-- Area: Misareaux Coast
--   NM: Gration
-----------------------------------
mixins = {require("scripts/mixins/fomor_hate")}
require("scripts/globals/status")
-----------------------------------
local entity = {}

entity.onMobSpawn = function(mob)
    mob:addStatusEffect(tpz.effect.KILLER_INSTINCT, 40, 0, 0)
    mob:setLocalVar("fomorHateAdj", -2)
end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity
