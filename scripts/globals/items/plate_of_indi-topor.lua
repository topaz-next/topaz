-----------------------------------------
-- ID: 4743
-- Indi-Topor
-- Teaches Indi-Topor
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(792)
end

function onItemUse(target)
    target:addSpell(792)
end