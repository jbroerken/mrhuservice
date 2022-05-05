/**
 *  PackageService.h
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

#ifndef PackageService_h
#define PackageService_h

// C / C++

// External
#include <MRH_Event.h>

// Project
#include "./PackageConfiguration.h"
#include "../Event/EventContainer.h"


class PackageService : public PackageConfiguration
{
public:

    //*************************************************************************************
    // Event Container
    //*************************************************************************************
    
    class ServiceEventContainer : public EventContainer
    {
        friend class PackageService;
        
    public:
        
    private:
        
        //*************************************************************************************
        // Constructor / Destructor
        //*************************************************************************************
        
        /**
         *  Default constructor.
         *
         *  \param us_ReserveStep The amount of extra space to reserve on reallocation.
         */

        ServiceEventContainer(size_t us_ReserveStep) noexcept;

        /**
         *  Copy constructor. Disabled for this class.
         *
         *  \param c_ServiceEventContainer ServiceEventContainer class source.
         */

        ServiceEventContainer(ServiceEventContainer const& c_ServiceEventContainer) = delete;

        /**
         *  Default destructor.
         */

        ~ServiceEventContainer() noexcept;
        
        //*************************************************************************************
        // Data
        //*************************************************************************************
        
    protected:
        
    };
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param p_PackagePath The full path to the application package.
     *  \param p_EventLimit The max amount of event to be sent in a update.
     */

    PackageService(const char* p_PackagePath,
                   const char* p_EventLimit);

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_PackageService PackageService class source.
     */

    PackageService(PackageService const& c_PackageService) = delete;

    /**
     *  Default destructor.
     */

    ~PackageService() noexcept;
    
    //*************************************************************************************
    // Load
    //*************************************************************************************
    
    /**
     *  Load the service shared object for the package.
     */
    
    void LoadSharedObject();
    
    //*************************************************************************************
    // Init
    //*************************************************************************************
    
    /**
     *  Initialize shared object application service.
     */
    
    void Init();

    //*************************************************************************************
    // Update
    //*************************************************************************************

    /**
     *  Update thte running application service.
     *
     *  \return true on success, false on failure.
     */
    
    bool Update() noexcept;
    
    /**
     *  Recieve events from the running application service.
     *
     *  \return The app event container.
     */

    ServiceEventContainer* RecieveEvents() noexcept;
    
    //*************************************************************************************
    // Exit
    //*************************************************************************************
    
    /**
     *  Exit the running shared object application service.
     */
    
    void Exit() noexcept;

private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    // Shared object
    std::string s_SharedObjectPath;
    void* p_SharedObjectHandle;

    // Shared object function locations
    void* p_FunctionInitLocation;
    void* p_FunctionUpdateLocation;
    void* p_FunctionSendEventLocation;
    void* p_FunctionExitLocation;
    
    // Event container
    ServiceEventContainer* p_ServiceEventContainer;
    
    // Event send limit
    MRH_Uint32 u32_EventLimit;
    
protected:

};

#endif /* PackageService_h */
