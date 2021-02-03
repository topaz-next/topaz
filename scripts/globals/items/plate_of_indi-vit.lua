-----------------------------------------
-- ID: 4743
-- Indi-Vit
-- Teaches Indi-Vit
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(774)
end

function onItemUse(target)
    target:addSpell(774)
end