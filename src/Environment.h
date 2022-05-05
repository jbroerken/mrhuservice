/**
 *  Environment.h
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

#ifndef Environment_h
#define Environment_h

// C / C++
#include <unistd.h>
#include <string>

// External

// Project
#include "./Exception.h"


class Environment
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param s_PackagePath The full package path.
     */

    Environment(std::string const& s_PackagePath);

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_Environment Environment class source.
     */

    Environment(Environment const& c_Environment) = delete;

    /**
     *  Default destructor.
     */

    ~Environment() noexcept;
    
    //*************************************************************************************
    // Package Path
    //*************************************************************************************
    
    /**
     *  Update the current package path.
     *
     *  \param s_PackagePath The full package path.
     */
    
    void UpdatePackagePath(std::string s_PackagePath);
    
    //*************************************************************************************
    // User & Group
    //*************************************************************************************
    
    /**
     *  Update the current user and group id.
     *
     *  \param s32_UserID The user id to set.
     *  \param s32_GroupID The group id to set.
     */
    
    void UpdateUserGroupID(uid_t s32_UserID, gid_t s32_GroupID);
    
    //*************************************************************************************
    // Working Directory
    //*************************************************************************************
    
    /**
     *  Set the current working directory to the package FSRoot directory.
     */
    
    void UpdateCurrentDir();
    
    //*************************************************************************************
    // Locale
    //*************************************************************************************
    
    /**
     *  Load the system locale.
     */
    
    void LoadSystemLocale();
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the current package path.
     *
     *  \return The current full package path.
     */
    
    std::string GetPackagePath() const noexcept;
    
    /**
     *  Get the current user id.
     *
     *  \return The current used id.
     */
    
    uid_t GetUserID() const noexcept;
    
    /**
     *  Get the current group id.
     *
     *  \return The current group id.
     */
    
    gid_t GetGroupID() const noexcept;
    
    /**
     *  Get the current locale.
     *
     *  \return The string for the current locale.
     */
    
    std::string GetLocale() const noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    // Package
    std::string s_PackagePath;
    
    // User
    uid_t s32_UserID;
    gid_t s32_GroupID;
    
    // Locale
    std::string s_Locale;
    
protected:

};

#endif /* Environment_h */
