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
