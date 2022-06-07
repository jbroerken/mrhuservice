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

#ifndef PackageConfiguration_h
#define PackageConfiguration_h

// C / C++

// External
#include <MRH_Typedefs.h>

// Project
#include "../Exception.h"


class PackageConfiguration
{
public:
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the application service user id.
     *
     *  \return The application service user id.
     */
    
    int GetUserID() const noexcept;
    
    /**
     *  Get the application service group id.
     *
     *  \return The application service group id.
     */
    
    int GetGroupID() const noexcept;
    
    /**
     *  Get the application service update timer.
     *
     *  \return The application service update timer in seconds.
     */
    
    MRH_Uint32 GetUpdateTimerS() const noexcept;

private:

    //*************************************************************************************
    // Data
    //*************************************************************************************

    // User
    int i_UserID;
    int i_GroupID;
    
    // Timers
    MRH_Uint32 u32_UpdateTimerS;
    
protected:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param s_PackagePath The path to the package.
     */

    PackageConfiguration(std::string s_PackagePath);

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_PackageConfiguration PackageConfiguration class source.
     */

    PackageConfiguration(PackageConfiguration const& c_PackageConfiguration) = delete;
    
    /**
     *  Default destructor.
     */

    ~PackageConfiguration() noexcept;
};

#endif /* PackageConfiguration_h */
