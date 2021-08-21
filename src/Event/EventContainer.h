/**
 *  EventContainer.h
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
