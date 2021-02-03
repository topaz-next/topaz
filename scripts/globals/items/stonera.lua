-----------------------------------------
-- Teaches Stonera
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(834)
end

function onItemUse(target)
    target:addSpell(834)
end