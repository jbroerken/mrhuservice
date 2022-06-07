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
#include <stdlib.h>

// External

// Project
#include "./EventContainer.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

EventContainer::EventContainer(size_t us_ReserveStep) noexcept
{
    if ((this->us_ReserveStep = us_ReserveStep) == 0)
    {
        this->us_ReserveStep = 1;
    }
    
    v_Event.reserve(this->us_ReserveStep);
}

EventContainer::~EventContainer() noexcept
{
    for (auto& Event : v_Event)
    {
        if (Event->p_Data != NULL)
        {
            free(Event->p_Data);
        }
        
        free(Event);
    }
}

//*************************************************************************************
// Add
//*************************************************************************************

void EventContainer::AddEvent(MRH_Event*& p_Event) noexcept
{
    if (p_Event != NULL)
    {
        // Reserve more space
        if (v_Event.size() == v_Event.capacity())
        {
            v_Event.reserve(v_Event.capacity() + us_ReserveStep);
        }
        
        v_Event.emplace_back(p_Event);
        p_Event = NULL;
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

size_t EventContainer::GetEventCount() noexcept
{
    return v_Event.size();
}

MRH_Event* EventContainer::GetEvent() noexcept
{
    if (v_Event.size() == 0)
    {
        return NULL;
    }
    
    MRH_Event* p_Event = v_Event[0];
    v_Event.erase(v_Event.begin());
    
    return p_Event;
}
