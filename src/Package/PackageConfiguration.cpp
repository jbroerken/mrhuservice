/**
 *  PackageConfiguration.cpp
 *
 *  This file is part of the MRH project.
 *  See the AUTHORS file for Copyright information.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

// C / C++
#include <cerrno>
#include <cstring>

// External
#include <MRH_Event.h>
#include <libmrhbf.h>

// Project
#include "./PackageConfiguration.h"
#include "./PackagePaths.h"

// Pre-defined
namespace
{
    enum Identifier
    {
        // Block Name
        BLOCK_EVENT_VERSION = 0,
        BLOCK_RUN_AS = 1,
        BLOCK_APP_SERVICE = 2,

        // Event Version Key
        KEY_EVENT_VERSION_SERVICE = 3,

        // Run As Key
        KEY_RUN_AS_USER_ID = 4,
        KEY_RUN_AS_GROUP_ID = 5,
        
        // App Service Key
        KEY_APP_SERVICE_UPDATE_TIMER,

        // Bounds
        IDENTIFIER_MAX = KEY_APP_SERVICE_UPDATE_TIMER,

        IDENTIFIER_COUNT = IDENTIFIER_MAX + 1
    };

    const char* p_Identifier[IDENTIFIER_COUNT] =
    {
        // Block Name
        "EventVersion",
        "RunAs",
        "AppService",

        // Event Version Key
        "AppService",

        // Run As Key
        "UserID",
        "GroupID"
    
        // App Service Key
        "UpdateTimerS"
    };

    constexpr MRH_Uint32 u32_MinUpdateTimerS = 300; // 5 Min

    // Event version bounds
    constexpr int i_EventVerMin = 1;
    constexpr int i_EventVerMax = MRH_EVENT_VERSION;
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PackageConfiguration::PackageConfiguration(std::string s_PackagePath) : i_UserID(-1),
                                                                        i_GroupID(-1),
                                                                        u32_UpdateTimerS(u32_MinUpdateTimerS)
{
    // Get configuration values
    if (*(s_PackagePath.end() - 1) != '/')
    {
        s_PackagePath += "/";
    }
    
    int i_EventVer = -1;
    
    try
    {
        MRH_BlockFile c_File(s_PackagePath + std::string(PACKAGE_CONFIGURATION_PATH));
        
        for (auto& Block : c_File.l_Block)
        {
            std::string s_Name(Block.GetName());
            
            if (s_Name.compare(p_Identifier[BLOCK_EVENT_VERSION]) == 0)
            {
                i_EventVer = std::stoi(Block.GetValue(p_Identifier[KEY_EVENT_VERSION_SERVICE]));
            }
            else if (s_Name.compare(p_Identifier[BLOCK_RUN_AS]) == 0)
            {
                i_UserID = std::stoi(Block.GetValue(p_Identifier[KEY_RUN_AS_USER_ID]));
                i_GroupID = std::stoi(Block.GetValue(p_Identifier[KEY_RUN_AS_GROUP_ID]));
            }
            else if (s_Name.compare(p_Identifier[BLOCK_APP_SERVICE]) == 0)
            {
                u32_UpdateTimerS = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[KEY_APP_SERVICE_UPDATE_TIMER])));
                
                if (u32_UpdateTimerS < u32_MinUpdateTimerS)
                {
                    u32_UpdateTimerS = u32_MinUpdateTimerS;
                }
            }
        }
    }
    catch (std::exception& e) // + MRH_BFException
    {
        throw Exception("Could not read package configuration (" + std::string(e.what()) + ")!");
    }
    
    if (i_EventVer < i_EventVerMin || i_EventVer > i_EventVerMax)
    {
        throw Exception("Invalid app event version: Got " +
                        std::to_string(i_EventVer) +
                        ", but bounds are " +
                        std::to_string(i_EventVerMin) +
                        " (Min) to " +
                        std::to_string(i_EventVerMax) +
                        " (Max)!");
    }
}

PackageConfiguration::~PackageConfiguration() noexcept
{}

//*************************************************************************************
// Getters
//*************************************************************************************

int PackageConfiguration::GetUserID() const noexcept
{
    return i_UserID;
}

int PackageConfiguration::GetGroupID() const noexcept
{
    return i_GroupID;
}

MRH_Uint32 PackageConfiguration::GetUpdateTimerS() const noexcept
{
    return u32_UpdateTimerS;
}
