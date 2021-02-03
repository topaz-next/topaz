-----------------------------------------
-- ID: 4743
-- Indi-refresh
-- Teaches Indi-refresh
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(770)
end

function onItemUse(target)
    target:addSpell(770)
end