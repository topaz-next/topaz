-----------------------------------------
-- Teaches Aerora II
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(833)
end

function onItemUse(target)
    target:addSpell(833)
end