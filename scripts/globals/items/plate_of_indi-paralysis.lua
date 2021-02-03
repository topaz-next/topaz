-----------------------------------------
-- ID: 4743
-- Indi-paralysis
-- Teaches Indi-paralysis
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(796)
end

function onItemUse(target)
    target:addSpell(796)
end