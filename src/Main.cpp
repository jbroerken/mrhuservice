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
#include <csignal>
#include <cstring>
#include <thread>

// External

// Project
#include "./Package/PackageService.h"
#include "./Package/PackagePaths.h"
#include "./Event/EventHandler.h"
#include "./Environment.h"
#include "./Logger.h"
#include "./Timer.h"
#include "./Revision.h"


//*************************************************************************************
// Data
//*************************************************************************************

namespace
{
    // Parameters
    typedef enum
    {
#ifdef __MRH_MRHCKM_SUPPORTED__
        MRH_PARAM_BIN = 0,
        MRH_PARAM_PACKAGE_PATH = 1,
        MRH_PARAM_EV_OUTPUT_FD = 2,
        MRH_PARAM_EV_OUTPUT_KEY = 3,
        MRH_PARAM_EV_EVENT_LIMIT = 4,

        MRH_PARAM_MAX = 4,

        MRH_PARAM_COUNT = 5
#else
        MRH_PARAM_BIN = 0,
        MRH_PARAM_PACKAGE_PATH = 1,
        MRH_PARAM_EV_OUTPUT_FD = 2,
        MRH_PARAM_EV_EVENT_LIMIT = 3,

        MRH_PARAM_MAX = 3,

        MRH_PARAM_COUNT = 4
#endif
    }MRH_Parameters;

    // Last signal
    int i_LastSignal = -1;
}

//*************************************************************************************
// Signal Handler
//*************************************************************************************

// Prevent name wrangling
extern "C"
{
    void SignalHandler(int i_Signal)
    {
        switch (i_Signal)
        {
            case SIGILL:
            case SIGTRAP:
            case SIGFPE:
            case SIGABRT:
            case SIGSEGV:
                Logger::Singleton().Backtrace(25, "Caught Signal: " + std::to_string(i_Signal));
                exit(EXIT_FAILURE);
                break;
                
            case SIGTERM:
                i_LastSignal = i_Signal;
                break;
                
            default:
                i_LastSignal = -1;
                break;
        }
    }
}

//*************************************************************************************
// Package Name
//*************************************************************************************

static std::string GetPackageName(std::string const& s_PackagePath) noexcept
{
    size_t us_ExtPos = s_PackagePath.find_last_of(PACKAGE_EXTENSION);
    
    if (us_ExtPos == std::string::npos)
    {
        return "unknown";
    }
    
    us_ExtPos += std::strlen(PACKAGE_EXTENSION);
    size_t us_SlashPos = s_PackagePath.find_last_of("/", us_ExtPos);
    
    if (us_SlashPos == std::string::npos)
    {
        us_SlashPos = 0;
    }
    
    return s_PackagePath.substr(us_SlashPos, us_ExtPos);
}

//*************************************************************************************
// Main
//*************************************************************************************

int main(int argc, const char* argv[])
{
    // Log Setup
    Logger& c_Logger = Logger::Singleton();
    c_Logger.OpenFiles(GetPackageName(argc > MRH_PARAM_PACKAGE_PATH ? argv[MRH_PARAM_PACKAGE_PATH] : ""));
    
    c_Logger.Log(Logger::INFO, "=============================================", "Main.cpp", __LINE__);
    c_Logger.Log(Logger::INFO, "= Started MRH User App Service Parent (" + std::string(VERSION_NUMBER) + ")", "Main.cpp", __LINE__);
    c_Logger.Log(Logger::INFO, "=============================================", "Main.cpp", __LINE__);
    
    // Check params
    if (argc < MRH_PARAM_COUNT)
    {
        c_Logger.Log(Logger::ERROR, "Missing app service parent parameters!",
                     "Main.cpp", __LINE__);
        return EXIT_FAILURE;
    }
    
    // Print mrhevlib info
    c_Logger.Log(Logger::INFO, "Using mrhevlib version " +
                               std::to_string(MRH_EV_LIB_VERSION_MAJOR) +
                               "." +
                               std::to_string(MRH_EV_LIB_VERSION_MINOR) +
                               "." +
                               std::to_string(MRH_EV_LIB_VERSION_PATCH) +
                               ".",
                 "Main.cpp", __LINE__);
    
#ifdef __MRH_MRHCKM_SUPPORTED__
    c_Logger.Log(Logger::INFO, "Event transmission method: MRHCKM.", "Main.cpp", __LINE__);
#else
    c_Logger.Log(Logger::INFO, "Event transmission method: Pipe.", "Main.cpp", __LINE__);
#endif
    
    // Install signal handlers
    std::signal(SIGTERM, SignalHandler);
    std::signal(SIGILL, SignalHandler);
    std::signal(SIGTRAP, SignalHandler);
    std::signal(SIGFPE, SignalHandler);
    std::signal(SIGABRT, SignalHandler);
    std::signal(SIGSEGV, SignalHandler);
    
    // Setup components
    c_Logger.Log(Logger::INFO, "Initializing " + std::string(argv[MRH_PARAM_PACKAGE_PATH]) + " ...", "Main.cpp", __LINE__);
    
    PackageService* p_Service;
    EventHandler* p_EventHandler;
    Environment* p_Environment;
    Timer s_Timer;
    
    try
    {
        // Allocation and constructor setup
        p_Service = new PackageService(argv[MRH_PARAM_PACKAGE_PATH],
                                       argv[MRH_PARAM_EV_EVENT_LIMIT]);
        p_Environment = new Environment(argv[MRH_PARAM_PACKAGE_PATH]);
#ifdef __MRH_MRHCKM_SUPPORTED__
        p_EventHandler = new EventHandler(argv[MRH_PARAM_EV_OUTPUT_FD],
                                          argv[MRH_PARAM_EV_OUTPUT_KEY],
                                          argv[MRH_PARAM_EV_EVENT_LIMIT]);
#else
        p_EventHandler = new EventHandler(argv[MRH_PARAM_EV_OUTPUT_FD],
                                          argv[MRH_PARAM_EV_EVENT_LIMIT]);
#endif
        
        // Set environment
        // @NOTE: This has to happen in this order before the user app functions are called!
        //        Some environment functions require mrhcore permissions.
        p_Environment->LoadSystemLocale();
        p_Environment->UpdateCurrentDir();
        p_Environment->UpdateUserGroupID(p_Service->GetUserID(), p_Service->GetGroupID());
        
        // Initialize app service
        p_Service->LoadSharedObject();
        p_Service->Init();
    }
    catch (Exception& e)
    {
        c_Logger.Log(Logger::ERROR, std::string("Failed to setup components: ") + e.what(), "Main.cpp", __LINE__);
        return EXIT_FAILURE;
    }
    
    c_Logger.Log(Logger::INFO, "Package Path: " + p_Environment->GetPackagePath(), "Main.cpp", __LINE__);
    c_Logger.Log(Logger::INFO, "Locale: " + p_Environment->GetLocale(), "Main.cpp", __LINE__);
    c_Logger.Log(Logger::INFO, "User ID: " + std::to_string(p_Environment->GetUserID()), "Main.cpp", __LINE__);
    c_Logger.Log(Logger::INFO, "Group ID: " + std::to_string(p_Environment->GetGroupID()), "Main.cpp", __LINE__);
    c_Logger.Log(Logger::INFO, "Update Timer (Seconds): " + std::to_string(p_Service->GetUpdateTimerS()), "Main.cpp", __LINE__);
    c_Logger.Log(Logger::INFO, "Application service initialized, now running...", "Main.cpp", __LINE__);
    
    // Send events until termination
    while (i_LastSignal != SIGTERM)
    {
        s_Timer.Reset();
        
        if (p_Service->Update() == false)
        {
            c_Logger.Log(Logger::INFO, "Application service failed to update!", "Main.cpp", __LINE__);
            break;
        }
        
        p_EventHandler->SendEvents(p_Service->RecieveEvents());
        
        if (s_Timer.GetTimePassedSeconds() < p_Service->GetUpdateTimerS())
        {
            std::this_thread::sleep_for(std::chrono::seconds(p_Service->GetUpdateTimerS() - static_cast<MRH_Uint32>(s_Timer.GetTimePassedSeconds())));
        }
    }
    
    // Exit application
    c_Logger.Log(Logger::INFO, "Calling application service exit...", "Main.cpp", __LINE__);
    p_Service->Exit();
    
    // Send stop an remaining events
    c_Logger.Log(Logger::INFO, "Sending remaining and parent stop events...", "Main.cpp", __LINE__);
    
    while (p_EventHandler->GetRemainingEvents() == true)
    {
        p_EventHandler->SendEvents();
    }
    
    // Done, clean up
    delete p_Service;
    delete p_EventHandler;
    delete p_Environment;
    
    c_Logger.Log(Logger::INFO, "User application service finished.", "Main.cpp", __LINE__);
    return EXIT_SUCCESS;
}
