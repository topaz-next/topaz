-----------------------------------------
-- ID: 4743
-- Indi-Slow
-- Teaches Indi-Slow
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(832)
end

function onItemUse(target)
    target:addSpell(832)
end