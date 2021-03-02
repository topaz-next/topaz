-----------------------------------
-- func: givels
-- desc: Makes a linkpearl for the given linkshell (pearlsack for gm)
-----------------------------------

cmdprops =
{
    permission = 1,
    parameters = "s"
}

function error(player, msg)
    player:PrintToPlayer(msg)
    player:PrintToPlayer("!givels <linkshell name> {target}")
end

function onTrigger(player, lsname, target)
    -- validate target
    if not lsname then
        error(player, "You must enter a linkshell name.")
        return
    end
    local targ
    if target == nil then
        targ = player
    else
        targ = GetPlayerByName(target)
        if targ == nil then
            error(player, string.format("Player named '%s' not found!", target))
            return
        end
    end
    if targ:addLinkpearl(lsname, false) then
        player:PrintToPlayer("Linkpearl created for \""..lsname.."\"!")
    else
        error(player, string.format("Unable to create linkpearl for \"%s\"!", lsname))
    end
end
