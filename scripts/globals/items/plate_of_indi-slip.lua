-----------------------------------------
-- ID: 4743
-- Indi-Slip
-- Teaches Indi-Slip
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(791)
end

function onItemUse(target)
    target:addSpell(791)
end