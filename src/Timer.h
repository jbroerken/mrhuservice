/**
 *  Timer.h
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

#ifndef Timer_h
#define Timer_h

// C / C++
#include <chrono>

// External

// Project


class Timer
{
public:

    //*************************************************************************************
    // Types
    //*************************************************************************************
    
    typedef std::chrono::high_resolution_clock::time_point TimePoint;
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     */
    
    Timer() noexcept;
    
    /**
     *  Copy constructor.
     *
     *  \param c_Timer Process class source.
     */
    
    Timer(Timer const& c_Timer) noexcept;
    
    /**
     *  Default destructor.
     */
    
    ~Timer() noexcept;
    
    //*************************************************************************************
    // Reset
    //*************************************************************************************
    
    /**
     *  Reset the timer.
     */
    
    void Reset() noexcept;
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Get the start time point.
     *
     *  \return The high resolution start time point.
     */
    
    TimePoint GetStartTimePoint() const noexcept;
    
    /**
     *  Get the time passed in seconds.
     *
     *  \return The time passed in seconds from the start time point to now.
     */
    
    double GetTimePassedSeconds() const noexcept;
    
    /**
     *  Get the time passed in milliseconds.
     *
     *  \return The time passed in milliseconds from the start time point to now.
     */
    
    double GetTimePassedMilliseconds() const noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    TimePoint c_StartTime;
    
protected:
    
};

#endif /* Timer_h */
