-----------------------------------------
-- Teaches Indi-Slip
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(791)
end

function onItemUse(target)
    target:addSpell(791)
end