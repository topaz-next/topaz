﻿/*
===========================================================================

Copyright (c) 2010-2015 Darkstar Dev Teams

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/

===========================================================================
*/

#ifndef _LUAINSTANCE_H
#define _LUAINSTANCE_H

#include "../../common/cbasetypes.h"
#include "luautils.h"

class CLuaBaseEntity;
class CInstance;
class CLuaInstance
{
    CInstance* m_PLuaInstance;

public:
    CLuaInstance(CInstance*);

    CInstance* GetInstance() const
    {
        return m_PLuaInstance;
    }

    uint8  getID();
    auto   getName() -> std::string;
    auto   getAllies() -> sol::table;
    auto   getChars() -> sol::table;
    auto   getMobs() -> sol::table;
    auto   getNpcs() -> sol::table;
    auto   getPets() -> sol::table;
    uint32 getTimeLimit();
    auto   getEntryPos() -> sol::table;
    uint32 getLastTimeUpdate();
    uint32 getProgress();
    uint32 getWipeTime();
    auto   getEntity(uint16 targid, sol::object const& filterObj) -> std::optional<CLuaBaseEntity>;
    uint32 getStage();

    void setLevelCap(uint8 cap);
    void setLastTimeUpdate(uint32 ms);
    void setProgress(uint32 progress);
    void setWipeTime(uint32 ms);
    void setStage(uint32 stage);

    void fail();
    bool failed();
    void complete();
    bool completed();

    auto insertAlly(uint32 groupid) -> std::optional<CLuaBaseEntity>;

    static void Register();
};

#endif
