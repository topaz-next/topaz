-----------------------------------------
-- Indi-Regen
-- Teaches Indi-Regen
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(768)
end

function onItemUse(target)
    target:addSpell(768)
end