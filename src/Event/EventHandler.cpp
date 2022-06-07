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
#include <cstring>
#include <new>

// External

// Project
#include "./EventHandler.h"
#include "../Logger.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

#ifdef __MRH_MRHCKM_SUPPORTED__
EventHandler::EventHandler(const char* p_OutputPath,
                           const char* p_OutputKey,
                           const char* p_EventLimit) : p_OutputEventQueue(NULL),
                                                       p_HandlerEventContainer(NULL)
{
    // Check args
    if (p_OutputPath == NULL || std::strlen(p_OutputPath) == 0 ||
        p_OutputKey == NULL || std::strlen(p_OutputKey) == 0 ||
        p_EventLimit == NULL || std::strlen(p_EventLimit) == 0)
    {
        throw Exception("Invalid device info recieved!");
    }
    
    // Get param info
    try
    {
        p_OutputEventQueue = MRH_OpenOutputQueueMRHCKM(p_OutputPath, std::stoi(p_OutputKey), std::stoi(p_EventLimit));
    }
    catch (std::exception& e)
    {
        throw Exception(std::string("Failed to open event queues: ") + e.what());
    }
    
    // Result
    if (p_InputEventQueue == NULL || p_OutputEventQueue == NULL)
    {
        Exit();
        
        if (MRH_GetEventQueueError() != MRH_EV_Error_Type::MRH_EV_ERROR_NONE)
        {
            throw Exception(std::string("libmrhev error while initializing: ") + std::string(MRH_GetEventQueueErrorString()) + "!");
        }
        else
        {
            throw Exception("Unknown libmrhev error while initializing!");
        }
    }
    
    // Create container
    try
    {
        size_t us_ReserveStep;
        
        if ((us_ReserveStep = std::stoi(p_EventLimit)) == 0)
        {
            us_ReserveStep = 1;
        }
        
        p_HandlerEventContainer = new HandlerEventContainer();
    }
    catch (std::exception& e)
    {
        Exit();
        
        throw Exception("Failed to create event container: " + std::string(e.what()));
    }
}
#endif

EventHandler::EventHandler(const char* p_OutputFD,
                           const char* p_EventLimit) : p_OutputEventQueue(NULL),
                                                       p_HandlerEventContainer(NULL)
{
    // Check args
    if (p_OutputFD == NULL || std::strlen(p_OutputFD) == 0 ||
        p_EventLimit == NULL || std::strlen(p_EventLimit) == 0)
    {
        throw Exception("Invalid file descriptors recieved!");
    }
    
    // Get param info
    try
    {
        p_OutputEventQueue = MRH_OpenOutputQueuePipe(std::stoi(p_OutputFD), std::stoi(p_EventLimit));
    }
    catch (std::exception& e)
    {
        throw Exception(std::string("Failed to open event queues: ") + e.what());
    }
    
    // Result
    if (p_OutputEventQueue == NULL)
    {
        CheckLibraryError();
        Exit();
        
        throw Exception(std::string("libmrhev event queue(s) failed to initialize!"));
    }
    
    // Create container
    try
    {
        size_t us_ReserveStep;
        
        if ((us_ReserveStep = std::stoi(p_EventLimit)) == 0)
        {
            us_ReserveStep = 1;
        }
        
        p_HandlerEventContainer = new HandlerEventContainer(us_ReserveStep);
    }
    catch (std::exception& e)
    {
        Exit();
        
        throw Exception("Failed to create event container: " + std::string(e.what()));
    }
}

EventHandler::~EventHandler() noexcept
{
    Exit();
}

EventHandler::HandlerEventContainer::HandlerEventContainer(size_t us_ReserveStep) noexcept : EventContainer(us_ReserveStep)
{}

EventHandler::HandlerEventContainer::~HandlerEventContainer() noexcept
{}

//*************************************************************************************
// Update
//*************************************************************************************

void EventHandler::SendEvent(MRH_Event*& p_Event) noexcept
{
    if (p_Event != NULL)
    {
        if (MRH_AddEvent(p_OutputEventQueue, &p_Event) == NULL)
        {
            p_Event = NULL;
        }
        else
        {
            CheckLibraryError();
        }
    }
    
    // We try to send events even on error, maybe some events aren't sent yet
    SendEvents();
}

void EventHandler::SendEvents(EventContainer* p_EventContainer) noexcept
{
    if (p_EventContainer != NULL)
    {
        MRH_Event* p_Event;
        
        while ((p_Event = p_EventContainer->GetEvent()) != NULL)
        {
            if (MRH_AddEvent(p_OutputEventQueue, &p_Event) != NULL)
            {
                CheckLibraryError();
                break;
            }
        }
    }
    
    // We try to send events even on error, maybe some events aren't sent yet
    SendEvents();
}

void EventHandler::SendEvents() noexcept
{
    // Check for work
    if (p_OutputEventQueue == NULL || MRH_CanSendEvents(p_OutputEventQueue) < 0)
    {
        return;
    }
    
    // Send events
    MRH_SendEvents(p_OutputEventQueue);
    CheckLibraryError();
}

//*************************************************************************************
// Exit
//*************************************************************************************

void EventHandler::Exit() noexcept
{
    // Remove queues
    p_OutputEventQueue = MRH_CloseEventQueue(p_OutputEventQueue);
    
    CheckLibraryError();
    
    // Remove event container
    if (p_HandlerEventContainer != NULL)
    {
        delete p_HandlerEventContainer;
        p_HandlerEventContainer = NULL;
    }
}

//*************************************************************************************
// Error
//*************************************************************************************

void EventHandler::CheckLibraryError() noexcept
{
    if (MRH_GetEventQueueError() != MRH_EV_Error_Type::MRH_EV_ERROR_NONE)
    {
        Logger::Singleton().Log(Logger::WARNING, "libmrhev error while processing events: " +
                                                 std::string(MRH_GetEventQueueErrorString()) +
                                                 " (" +
                                                 std::string(MRH_GetEventQueueErrorFile()) +
                                                 ": " +
                                                 std::to_string(MRH_GetEventQueueErrorFileLine()) +
                                                 ")!",
                                "EventHandler.cpp", __LINE__);
        MRH_ResetEventQueueError();
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool EventHandler::GetRemainingEvents() const noexcept
{
    return MRH_CanSendEvents(p_OutputEventQueue) < 0 ? false : true;
}
