-----------------------------------------
-- ID: 4743
-- Indi-Wilt
-- Teaches Indi-Wilt
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(787)
end

function onItemUse(target)
    target:addSpell(787)
end