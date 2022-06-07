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

// C / C++
#include <unistd.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <cstring>
#include <new>

// External

// Project
#include "./PackageService.h"
#include "./PackagePaths.h"
#include "../Logger.h"

namespace
{
    // Shared object function names
    const char* p_FunctionInitName = "MRH_Init";
    const char* p_FunctionUpdateName = "MRH_Update";
    const char* p_FunctionSendEventName = "MRH_SendEvent";
    const char* p_FunctionExitName = "MRH_Exit";
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PackageService::PackageService(const char* p_PackagePath,
                               const char* p_EventLimit) : PackageConfiguration(p_PackagePath)
{
    // Initial setup
    s_SharedObjectPath = "";
    p_SharedObjectHandle = NULL;
    p_FunctionInitLocation = NULL;
    p_FunctionUpdateLocation = NULL;
    p_FunctionSendEventLocation = NULL;
    p_FunctionExitLocation = NULL;
    p_ServiceEventContainer = NULL;
    u32_EventLimit = 1;
    
    // Get shared object path
    if (p_PackagePath == NULL || std::strlen(p_PackagePath) == 0)
    {
        throw Exception("Invalid package path recieved!");
    }
    
    s_SharedObjectPath = p_PackagePath;
    
    if (*(s_SharedObjectPath.end() - 1) != '/')
    {
        s_SharedObjectPath += "/";
    }
    
    s_SharedObjectPath += PACKAGE_SERVICE_BINARY_PATH;
    
    // Set event limit to prevent spamming from the user application
    try
    {
        // Ugly, but we want the exception
        u32_EventLimit = static_cast<MRH_Uint32>(std::stoul(std::string(p_EventLimit)));
    }
    catch (std::exception& e)
    {
        throw Exception(std::string("Failed to set event limit: ") + e.what());
    }
    
    // Create container last, used for event exchange
    try
    {
        p_ServiceEventContainer = new ServiceEventContainer(u32_EventLimit);
    }
    catch (std::exception& e)
    {
        throw Exception(std::string("Failed to construct event container: ") + e.what());
    }
}

PackageService::~PackageService() noexcept
{
    if (p_ServiceEventContainer != NULL)
    {
        delete p_ServiceEventContainer;
    }
    
    if (p_SharedObjectHandle != NULL)
    {
        dlclose(p_SharedObjectHandle);
    }
}

PackageService::ServiceEventContainer::ServiceEventContainer(size_t us_ReserveStep) noexcept : EventContainer(us_ReserveStep)
{}

PackageService::ServiceEventContainer::~ServiceEventContainer() noexcept
{}

//*************************************************************************************
// Load
//*************************************************************************************

void PackageService::LoadSharedObject()
{
    // Clear dlerror, might still contain unrelated info
    dlerror();
    
    if ((p_SharedObjectHandle = dlopen(s_SharedObjectPath.c_str(), RTLD_NOW)) == NULL)
    {
        throw Exception("Failed to load shared object " + s_SharedObjectPath + " (" + std::string(dlerror()) + ")!");
    }
    
    // Get service functions from shared object
    p_FunctionInitLocation = dlsym(p_SharedObjectHandle, p_FunctionInitName);
    p_FunctionUpdateLocation = dlsym(p_SharedObjectHandle, p_FunctionUpdateName);
    p_FunctionSendEventLocation = dlsym(p_SharedObjectHandle, p_FunctionSendEventName);
    p_FunctionExitLocation = dlsym(p_SharedObjectHandle, p_FunctionExitName);
    
    if (p_FunctionInitLocation == NULL ||
        p_FunctionUpdateLocation == NULL ||
        p_FunctionSendEventLocation == NULL ||
        p_FunctionExitLocation == NULL)
    {
        throw Exception("Failed to load functions from " + s_SharedObjectPath + " (" + std::string(dlerror()) + ")!");
    }
}

//*************************************************************************************
// Init
//*************************************************************************************

void PackageService::Init()
{
    int (*FunctionInit)(void);
    FunctionInit = reinterpret_cast<int(*)(void)>(p_FunctionInitLocation);
    
    if (FunctionInit() < 0)
    {
        throw Exception("Failed to run app service init function!");
    }
}

//*************************************************************************************
// Update
//*************************************************************************************

bool PackageService::Update() noexcept
{
    int (*FunctionUpdate)(void);
    FunctionUpdate = reinterpret_cast<int(*)(void)>(p_FunctionUpdateLocation);
    
    if (FunctionUpdate() < 0)
    {
        return false;
    }
    
    return true;
}

PackageService::ServiceEventContainer* PackageService::RecieveEvents() noexcept
{
    MRH_Event* (*FunctionSendEvent)(void);
    FunctionSendEvent = reinterpret_cast<MRH_Event*(*)(void)>(p_FunctionSendEventLocation);
    
    MRH_Event* p_Event;
    MRH_Uint32 u32_Recieved = 0; // User service spam protection
    
    while (u32_Recieved < u32_EventLimit && (p_Event = FunctionSendEvent()) != NULL)
    {
        p_ServiceEventContainer->AddEvent(p_Event);
        ++u32_Recieved;
    }
    
    return p_ServiceEventContainer;
}

//*************************************************************************************
// Exit
//*************************************************************************************

void PackageService::Exit() noexcept
{
    void (*FunctionExit)(void);
    FunctionExit = reinterpret_cast<void(*)(void)>(p_FunctionExitLocation);
    
    FunctionExit();
}
