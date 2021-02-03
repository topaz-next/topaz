-----------------------------------------
-- ID: 4743
-- Indi-Vex
-- Teaches Indi-Vex
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(793)
end

function onItemUse(target)
    target:addSpell(793)
end