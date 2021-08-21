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
project(mrhuservice VERSION 1.0
                    DESCRIPTION "MRH user application service executable"
                    LANGUAGES CXX)

#########################################################################
#
#  PATHS
#
#########################################################################

###
#  Binary Paths
#  ------------
#  The paths for our created binary file(s).
###
set(BIN_DIR_PATH "${CMAKE_SOURCE_DIR}/bin/")
file(MAKE_DIRECTORY ${BIN_DIR_PATH})

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

###
#  Library Paths
#  -------------
#  The paths to the external libraries to use. 
#  Both the binary and include paths should be set here.
#  Add OS specific libraries in their own lists.
###
set(LIB_DIR_PATH "${CMAKE_SOURCE_DIR}/lib/")
set(INCLUDE_DIR_PATH "${CMAKE_SOURCE_DIR}/include/")

set(LIB_BIN_LIST "${LIB_DIR_PATH}/libmrhev.a"
                 "${LIB_DIR_PATH}/libmrhbf.a")
set(LIB_INCLUDE_LIST "${INCLUDE_DIR_PATH}/mrhshared"
					 "${INCLUDE_DIR_PATH}/libmrhev"
					 "${INCLUDE_DIR_PATH}/libmrhbf")

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
set_target_properties(mrhuservice
    		          PROPERTIES
    		          ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
    		          LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
    		          RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR_PATH})

###
#  Required Libraries
#  ------------------
#  Libraries required by this application.
###
target_link_libraries(mrhuservice PUBLIC ${LIB_BIN_LIST})
include_directories(${LIB_INCLUDE_LIST})

###
#  Source Definitions
#  ------------------
#  Preprocessor source definitions.
###
target_compile_definitions(mrhuservice PRIVATE MRH_LOCALE_FILE_PATH="/etc/MRH_Locale.conf")
target_compile_definitions(mrhuservice PRIVATE MRH_USERVICE_LOG_FILE_PATH_BASE="/var/log/mrh/mrhuservice/mrhuservice_")
target_compile_definitions(mrhuservice PRIVATE MRH_USERVICE_BACKTRACE_FILE_PATH_BASE="/var/log/mrh/mrhuservice/bt_mrhuservice_")
target_compile_definitions(mrhuservice PRIVATE MRH_LOGGER_PRINT_CLI=0)