-----------------------------------------
-- ID: 4743
-- Indi-Fade
-- Teaches Indi-Fade
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(789)
end

function onItemUse(target)
    target:addSpell(789)
end