/**
 *  Copyright (C) 2021 - 2022 The MRH Project Authors.
 * 
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
