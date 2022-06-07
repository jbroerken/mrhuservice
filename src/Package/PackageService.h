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
