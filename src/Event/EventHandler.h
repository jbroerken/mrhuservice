/**
 *  EventHandler.h
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

#ifndef EventHandler_h
#define EventHandler_h

// C / C++

// External
#include <libmrhev.h>

// Project
#include "./EventContainer.h"
#include "../Exception.h"


class EventHandler
{
public:

    //*************************************************************************************
    // Event Container
    //*************************************************************************************
    
    class HandlerEventContainer : public EventContainer
    {
        friend class EventHandler;
        
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

        HandlerEventContainer(size_t us_ReserveStep) noexcept;

        /**
         *  Copy constructor. Disabled for this class.
         *
         *  \param s_HandlerEventContainer HandlerEventContainer class source.
         */

        HandlerEventContainer(HandlerEventContainer const& s_HandlerEventContainer) = delete;

        /**
         *  Default destructor.
         */

        ~HandlerEventContainer() noexcept;
        
        //*************************************************************************************
        // Data
        //*************************************************************************************
        
    protected:
        
    };
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

#ifdef __MRH_MRHCKM_SUPPORTED__
    /**
     *  MRHCKM-based constructor.
     *
     *  \param p_OutputPath The full path to the output device file.
     *  \param p_OutputKey The output key.
     *  \param p_EventLimit The max amount of event to be sent / recieved in a update.
     */
    
    EventHandler(const char* p_OutputPath,
                 const char* p_OutputKey,
                 const char* p_EventLimit);
#endif
    
    /**
     *  Pipe-based constructor.
     *
     *  \param p_OutputFD The event queue output file descriptor.
     *  \param p_EventLimit The max amount of event to be sent / recieved in a update.  
     */

    EventHandler(const char* p_OutputFD,
                 const char* p_EventLimit);

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param s_EventHandler EventHandler class source.
     */

    EventHandler(EventHandler const& s_EventHandler) = delete;

    /**
     *  Default destructor.
     */

    ~EventHandler() noexcept;

    //*************************************************************************************
    // Send
    //*************************************************************************************

    /**
     *  Send a new and remaining events to our parent.
     *
     *  \param p_Event The event to send. This event is consumed.
     */
    
    void SendEvent(MRH_Event*& p_Event) noexcept;
    
    /**
     *  Send new and remaining events to our parent.
     *
     *  \param p_EventContainer The events to send.
     */

    void SendEvents(EventContainer* p_EventContainer) noexcept;
    
    /**
     *  Send remaining events to our parent.
     */
    
    void SendEvents() noexcept;
    
    //*************************************************************************************
    // Exit
    //*************************************************************************************
    
    /**
     *  Exit event queue.
     */
    
    void Exit() noexcept;
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get wether events remain to send or not.
     *
     *  \return true if events remain, false if not.
     */
    
    bool GetRemainingEvents() const noexcept;
    
private:
    
    //*************************************************************************************
    // Error
    //*************************************************************************************
    
    /**
     *  Check libmrhev for errors.
     */
    
    inline void CheckLibraryError() noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    // Event queue
    MRH_OutputEventQueue* p_OutputEventQueue;

    // Event storage
    HandlerEventContainer* p_HandlerEventContainer;
    
protected:

};

#endif /* EventHandler_h */
