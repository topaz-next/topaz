-----------------------------------------
-- ID: 4743
-- Indi-Slow
-- Teaches Indi-Slow
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(839)
end

function onItemUse(target)
    target:addSpell(839)
end