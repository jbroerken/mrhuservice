#########################################################################
#
#  CMAKE
#
#########################################################################

###
#  Minimum Version
#  ---------------
#  The CMake version required.
###
cmake_minimum_required(VERSION 3.1)

###
#  CMake Configuration
#  -------------------
#  Configuration settings for CMake.
#
#  NOTE:
#  These settings have to be applied before the project() setting!
###
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 14)

###
#  Project Info
#  ------------
#  General simple information about our project.
###
project(mrhuservice VERSION 1.0.0
                    DESCRIPTION "MRH user application service executable"
                    LANGUAGES CXX)

#########################################################################
#
#  PATHS
#
#########################################################################

###
#  Install Paths
#  -------------
#  The paths for our created binary file(s).
###
set(BIN_INSTALL_PATH "/usr/bin/")

###
#  Build Paths
#  -----------
#  The paths for the cmake build.
###
set(BUILD_DIR_PATH "${CMAKE_SOURCE_DIR}/build/")
file(MAKE_DIRECTORY ${BUILD_DIR_PATH})

###
#  Source Paths
#  ------------
#  The paths to the source files to use.
#  Add OS specific source files in their own list.
###
set(SRC_DIR_PATH "${CMAKE_SOURCE_DIR}/src/")

set(SRC_LIST_ALL "${SRC_DIR_PATH}/Package/PackageConfiguration.cpp"
                 "${SRC_DIR_PATH}/Package/PackageConfiguration.h"
                 "${SRC_DIR_PATH}/Package/PackageService.cpp"
                 "${SRC_DIR_PATH}/Package/PackageService.h"
                 "${SRC_DIR_PATH}/Package/PackagePaths.h"
                 "${SRC_DIR_PATH}/Event/EventHandler.cpp"
                 "${SRC_DIR_PATH}/Event/EventHandler.h"
                 "${SRC_DIR_PATH}/Event/EventContainer.cpp"
                 "${SRC_DIR_PATH}/Event/EventContainer.h"
                 "${SRC_DIR_PATH}/Environment.cpp"
                 "${SRC_DIR_PATH}/Environment.h"
                 "${SRC_DIR_PATH}/Logger.cpp"
                 "${SRC_DIR_PATH}/Logger.h"
                 "${SRC_DIR_PATH}/Timer.cpp"
                 "${SRC_DIR_PATH}/Timer.h"
                 "${SRC_DIR_PATH}/Exception.h"
                 "${SRC_DIR_PATH}/Revision.h"
                 "${SRC_DIR_PATH}/Main.cpp")

#########################################################################
#
#  TARGET
#
#########################################################################

###
#  Target
#  ------
#  The target(s) to build.
###
add_executable(mrhuservice ${SRC_LIST_ALL})

###
#  Required Libraries
#  ------------------
#  Libraries required by this application.
###
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

find_package(Threads REQUIRED)
find_library(libmrhbf NAMES mrhbf REQUIRED)
find_library(libmrhev NAMES mrhev REQUIRED)

target_link_libraries(mrhuservice PUBLIC Threads::Threads)
target_link_libraries(mrhuservice PUBLIC ${CMAKE_DL_LIBS})
target_link_libraries(mrhuservice PUBLIC mrhbf)
target_link_libraries(mrhuservice PUBLIC mrhev)

###
#  Source Definitions
#  ------------------
#  Preprocessor source definitions.
###
target_compile_definitions(mrhuservice PRIVATE MRH_LOCALE_FILE_PATH="/etc/MRH_Locale.conf")
target_compile_definitions(mrhuservice PRIVATE MRH_USERVICE_LOG_FILE_PATH_BASE="/var/log/mrh/mrhuservice/mrhuservice_")
target_compile_definitions(mrhuservice PRIVATE MRH_USERVICE_BACKTRACE_FILE_PATH_BASE="/var/log/mrh/mrhuservice/bt_mrhuservice_")
target_compile_definitions(mrhuservice PRIVATE MRH_LOGGER_PRINT_CLI=0)

###
#  Install
#  -------
#  Application installation.
###
install(TARGETS mrhuapp
        DESTINATION ${BIN_INSTALL_PATH})