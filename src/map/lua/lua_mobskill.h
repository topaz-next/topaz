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

#ifndef _LUAMOBSKILL_H
#define _LUAMOBSKILL_H

#include "../../common/cbasetypes.h"
#include "luautils.h"

class CMobSkill;

class CLuaMobSkill
{
    CMobSkill* m_PLuaMobSkill;

public:
    CLuaMobSkill(CMobSkill*);

    CMobSkill* GetMobSkill() const
    {
        return m_PLuaMobSkill;
    }
    float  getTP();
    uint8  getMobHPP(lua_State*);
    uint16 getID();
    int16  getParam();
    bool   isAoE();
    bool   isConal();
    bool   isSingle();
    bool   hasMissMsg();
    void   setMsg(uint16 message);
    uint16 getMsg();
    uint16 getTotalTargets();

    static void Register();
};

#endif
