/**
 *  Environment.cpp
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
#include <sys/stat.h>
#include <clocale>
#include <cstring>
#include <fstream>

// External
#include <libmrhbf.h>

// Project
#include "./Environment.h"
#include "./Package/PackagePaths.h"
#include "./Logger.h"

// Pre-defined
#ifndef MRH_LOCALE_FILE_PATH
    #define MRH_LOCALE_FILE_PATH "/usr/local/etc/mrh/MRH_Locale.conf"
#endif

namespace
{
    // Locale File
    enum LocaleIdentifier
    {
        // Block Name
        LOCALE_BLOCK_LOCALE = 0,

        // Locale Key
        LOCALE_KEY_LOCALE_ACTIVE = 1,
    
        // Bounds
        LOCALE_IDENTIFIER_MAX = LOCALE_KEY_LOCALE_ACTIVE,

        LOCALE_IDENTIFIER_COUNT = LOCALE_IDENTIFIER_MAX + 1
    };

    const char* p_LocaleIdentifier[LOCALE_IDENTIFIER_COUNT] =
    {
        // Block Name
        "Locale",

        // Locale Key
        "Active",
    };

    // Default locale to use
    const char* p_DefaultLocale = "en_US.UTF-8";
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Environment::Environment(std::string const& s_PackagePath) : s_PackagePath(""),
                                                             s32_UserID(-1),
                                                             s32_GroupID(-1),
                                                             s_Locale(p_DefaultLocale)
{
    try
    {
        UpdatePackagePath(s_PackagePath);
    }
    catch (Exception& e)
    {
        throw;
    }
    
    if ((s32_UserID = getuid()) < 0 || (s32_GroupID = getgid()) < 0)
    {
        throw Exception("Failed to recieve initial user and group id!");
    }
}

Environment::~Environment() noexcept
{}

//*************************************************************************************
// Package Path
//*************************************************************************************

void Environment::UpdatePackagePath(std::string s_PackagePath)
{
    if (s_PackagePath.size() == 0)
    {
        throw Exception("Invalid package path string recieved!");
    }
    
    // Correct path
    if (s_PackagePath[s_PackagePath.size() - 1] != '/')
    {
        s_PackagePath += "/";
    }
    
    // Change working directory
    if (chdir(s_PackagePath.c_str()) < 0)
    {
        throw Exception("Invalid package path recieved!");
    }
    
    this->s_PackagePath = s_PackagePath;
}

//*************************************************************************************
// User & Group
//*************************************************************************************

void Environment::UpdateUserGroupID(uid_t s32_UserID, gid_t s32_GroupID)
{
    if (s32_UserID < 0 || s32_GroupID < 0)
    {
        throw Exception("Invalid user / group ids recieved!");
    }
    
    // Update user and group
    // NOTE: Group must be set before setting the user!
    setgid(s32_GroupID);
    setegid(s32_GroupID);
    setuid(s32_UserID);
    seteuid(s32_UserID);
    
    // Check user id change
    // NOTE: We want this and the group call to fail!
    if (setuid(0) >= 0)
    {
        throw Exception("Failed to change user id!");
    }
    
    this->s32_UserID = s32_UserID;
    
    // Check group id change
    if (setgid(0) >= 0)
    {
        throw Exception("Failed to change group id!");
    }
    
    this->s32_GroupID = s32_GroupID;
}

//*************************************************************************************
// Working Directory
//*************************************************************************************

void Environment::UpdateCurrentDir()
{
    std::string s_WorkingDir = s_PackagePath + PACKAGE_FS_ROOT_PATH;
    
    if (chdir(s_WorkingDir.c_str()) < 0)
    {
        throw Exception("Failed to change current directory: " + std::string(std::strerror(errno)) + " (" + std::to_string(errno) + ")!");
    }
    
    Logger::Singleton().Log(Logger::INFO, "Current working dir set to: " + s_WorkingDir,
                            "Environment.cpp", __LINE__);
}

//*************************************************************************************
// Locale
//*************************************************************************************

void Environment::LoadSystemLocale()
{
    Logger& c_Logger = Logger::Singleton();
    c_Logger.Log(Logger::INFO, "Reading locale file " +
                               std::string(MRH_LOCALE_FILE_PATH) +
                               "...",
                 "Environment.cpp", __LINE__);
    
    try
    {
        MRH_BlockFile c_File(MRH_LOCALE_FILE_PATH);
        
        for (auto& Block : c_File.l_Block)
        {
            if (Block.GetName().compare(p_LocaleIdentifier[LOCALE_BLOCK_LOCALE]) != 0)
            {
                continue;
            }
            
            s_Locale = Block.GetValue(p_LocaleIdentifier[LOCALE_KEY_LOCALE_ACTIVE]);
            std::setlocale(LC_ALL, s_Locale.c_str());
            
            if (s_Locale.compare(std::setlocale(LC_ALL, NULL)) != 0)
            {
                throw Exception("Failed to update locale!");
            }
            
            c_Logger.Log(Logger::INFO, "Locale set to " + s_Locale + ".",
                         "Environment.cpp", __LINE__);
            break;
        }
    }
    catch (std::exception& e) // + MRH_BFException
    {
        c_Logger.Log(Logger::WARNING, std::string(e.what()) + ", using default locale.",
                     "Environment.cpp", __LINE__);
        std::setlocale(LC_ALL, p_DefaultLocale);
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

std::string Environment::GetPackagePath() const noexcept
{
    return s_PackagePath;
}

uid_t Environment::GetUserID() const noexcept
{
    return s32_UserID;
}

gid_t Environment::GetGroupID() const noexcept
{
    return s32_GroupID;
}

std::string Environment::GetLocale() const noexcept
{
    return s_Locale;
}
