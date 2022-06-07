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
#include <execinfo.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>

// External

// Project
#include "./Logger.h"

// Pre-defined
#ifndef MRH_USERVICE_LOG_FILE_PATH_BASE
    #define MRH_USERVICE_LOG_FILE_PATH_BASE "/var/log/mrh/mrhuservice/mrhuservice_"
#endif
#ifndef MRH_USERVICE_BACKTRACE_FILE_PATH_BASE
    #define MRH_USERVICE_BACKTRACE_FILE_PATH_BASE "/var/log/mrh/mrhuservice/bt_mrhuservice_"
#endif
#ifndef MRH_LOGGER_PRINT_CLI
    #define MRH_LOGGER_PRINT_CLI 0
#endif


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Logger::Logger() noexcept
{}

Logger::~Logger() noexcept
{
    if (f_LogFile.is_open() == true)
    {
        f_LogFile.close();
    }
    
    if (f_BacktraceFile.is_open() == true)
    {
        f_BacktraceFile.close();
    }
}

//*************************************************************************************
// Singleton
//*************************************************************************************

Logger& Logger::Singleton() noexcept
{
    static Logger c_Logger;
    return c_Logger;
}

//*************************************************************************************
// File
//*************************************************************************************

void Logger::OpenFiles(std::string const& s_PackageName) noexcept
{
    if (f_LogFile.is_open() == true)
    {
        f_LogFile.close();
    }
    
    if (f_BacktraceFile.is_open() == true)
    {
        f_BacktraceFile.close();
    }
    
    std::string s_LogFilePath(MRH_USERVICE_LOG_FILE_PATH_BASE + s_PackageName + ".log");
    std::string s_BacktraceFilePath(MRH_USERVICE_BACKTRACE_FILE_PATH_BASE + s_PackageName + ".log");
    
    f_LogFile.open(s_LogFilePath, std::ios::out | std::ios::trunc);
    f_BacktraceFile.open(s_BacktraceFilePath, std::ios::out | std::ios::trunc);
    
    if (f_LogFile.is_open() == false)
    {
        Log(Logger::WARNING, "Failed to open user service log file: " + s_LogFilePath,
            "Logger.cpp", __LINE__);
    }
    
    if (f_BacktraceFile.is_open() == false)
    {
        Log(Logger::WARNING, "Failed to open user service backtrace file: " + s_BacktraceFilePath,
            "Logger.cpp", __LINE__);
    }
}

//*************************************************************************************
// Log
//*************************************************************************************

void Logger::Log(LogLevel e_Level, std::string s_Message, std::string s_File, size_t us_Line) noexcept
{
    if (f_LogFile.is_open() == true)
    {
        f_LogFile << "[" << s_File << "][" << std::to_string(us_Line) << "][" << GetLevelString(e_Level) << "]: " << s_Message << std::endl;
    }
    
    if (MRH_LOGGER_PRINT_CLI > 0)
    {
        std::cout << "[" << s_File << "][" << std::to_string(us_Line) << "][" << GetLevelString(e_Level) << "]: " << s_Message << std::endl;
    }
}

//*************************************************************************************
// Backtrace
//*************************************************************************************

void Logger::Backtrace(size_t us_TraceSize, std::string s_Message) noexcept
{
    if (f_BacktraceFile.is_open() == false)
    {
        return;
    }
    
    // Retrieve current stack addresses
    char** p_Traceback = NULL;
    void* p_AdressList[us_TraceSize];
    
    us_TraceSize = backtrace(p_AdressList, (int)(sizeof(p_AdressList) / sizeof(void*)));
    
    // Get names
    if (us_TraceSize > 0)
    {
        p_Traceback = backtrace_symbols(p_AdressList, (int)us_TraceSize);
    }
    
    // File head
    WriteBacktrace(f_BacktraceFile, "====================================");
    WriteBacktrace(f_BacktraceFile, "= " + s_Message);
    WriteBacktrace(f_BacktraceFile, "====================================");
    
    // Print traceback stack
    if (us_TraceSize == 0)
    {
        WriteBacktrace(f_BacktraceFile, "Failed to get traceback!");
    }
    else
    {
        for (int i = 0; i < us_TraceSize && p_Traceback[i] != NULL; ++i)
        {
            WriteBacktrace(f_BacktraceFile, std::string(p_Traceback[i]));
        }
    }
}

void Logger::WriteBacktrace(std::ofstream& f_File, std::string s_Message) noexcept
{
    if (f_File.is_open() == true)
    {
        f_File << s_Message << std::endl;
    }
    
    if (MRH_LOGGER_PRINT_CLI > 0)
    {
        std::cout << s_Message << std::endl;
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

const char* Logger::GetLevelString(LogLevel e_Level) noexcept
{
    switch (e_Level)
    {
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
            
        default:
            return "UNKNOWN";
    }
}
