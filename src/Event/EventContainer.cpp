/**
 *  EventContainer.cpp
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
