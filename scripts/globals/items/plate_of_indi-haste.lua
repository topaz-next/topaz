-----------------------------------------
-- ID: 4743
-- Indi-Haste
-- Teaches Indi-Haste
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(771)
end

function onItemUse(target)
    target:addSpell(771)
end