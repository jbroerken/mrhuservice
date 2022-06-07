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

#ifndef EventContainer_h
#define EventContainer_h

// C / C++
#include <vector>

// External
#include <MRH_Event.h>

// Project


class EventContainer
{
public:

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get event count.
     *
     *  \return The event count.
     */
    
    size_t GetEventCount() noexcept;
    
    /**
     *  Get the next event in the container. This removes the event from the container.
     *
     *  \return The event on success, NULL if the container is empty.
     */
    
    MRH_Event* GetEvent() noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

protected:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param us_ReserveStep The amount of extra space to reserve on reallocation.
     */

    EventContainer(size_t us_ReserveStep) noexcept;

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param s_EventContainer EventContainer class source.
     */

    EventContainer(EventContainer const& s_EventContainer) = delete;

    /**
     *  Default destructor.
     */

    ~EventContainer() noexcept;
    
    //*************************************************************************************
    // Add
    //*************************************************************************************
    
    /**
     *  Add an event to the container.
     *
     *  \param p_Event The event to add. This event is consumed.
     */
    
    void AddEvent(MRH_Event*& p_Event) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::vector<MRH_Event*> v_Event;
    size_t us_ReserveStep;
};

#endif /* EventContainer_h */
