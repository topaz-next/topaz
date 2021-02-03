-----------------------------------------
-- Teaches Indi-Dex
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(773)
end

function onItemUse(target)
    target:addSpell(773)
end