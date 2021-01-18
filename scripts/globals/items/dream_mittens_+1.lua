-----------------------------------
-- ID: 10383
-- Item: Dream Mittens +1
-- Enchantment: Invisible
-- Duration: 3 Mins 20 Secs
-- TODO: Enhances duration of Invisible Effect
-----------------------------------
require("scripts/globals/settings")
require("scripts/globals/status")
-----------------------------------
local item_object = {}

item_object.onItemCheck = function(target)
    return 0
end

item_object.onItemUse = function(target)
    if (not target:hasStatusEffect(tpz.effect.INVISIBLE)) then
        target:addStatusEffect(tpz.effect.INVISIBLE, 0, 10, math.floor(200 * SNEAK_INVIS_DURATION_MULTIPLIER))
    end
end

return item_object
