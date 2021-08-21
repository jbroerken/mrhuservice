/**
 *  Timer.cpp
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

// External

// Project
#include "./Timer.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Timer::Timer() noexcept
{
    Reset();
}

Timer::Timer(Timer const& c_Timer) noexcept
{
    this->c_StartTime = c_Timer.c_StartTime;
}

Timer::~Timer() noexcept
{}

//*************************************************************************************
// Reset
//*************************************************************************************

void Timer::Reset() noexcept
{
    c_StartTime = std::chrono::high_resolution_clock::now();
}

//*************************************************************************************
// Getters
//*************************************************************************************

Timer::TimePoint Timer::GetStartTimePoint() const noexcept
{
    return c_StartTime;
}

double Timer::GetTimePassedSeconds() const noexcept
{
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - c_StartTime).count();
}

double Timer::GetTimePassedMilliseconds() const noexcept
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - c_StartTime).count();
}
