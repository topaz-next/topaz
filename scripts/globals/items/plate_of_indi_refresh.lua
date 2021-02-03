-----------------------------------------
-- ID: 4743
-- Indi-Refresh
-- Teaches Indi-Refresh
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(770)
end

function onItemUse(target)
    target:addSpell(770)
end