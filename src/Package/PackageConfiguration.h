/**
 *  PackageConfiguration.h
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
