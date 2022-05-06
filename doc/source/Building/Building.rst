********
Building
********
The project files are already prepared and include everything except the 
required dependencies. This document lists both dependencies as well as the 
build process to create the library from source.

Supported Platforms
-------------------
The supported platforms for mrhuservice are UNIX-likes, primarily 
Debian-based GNU/Linux Distributions like the Rapsberry Pi OS.

Dependencies
------------
This project has the following dependencies:

* mrhshared: https://github.com/jbroerken/mrhshared/
* libmrhbf: https://github.com/jbroerken/libmrhbf/
* libmrhev: https://github.com/jbroerken/libmrhev/

Build Tools
-----------
This release includes a CMake script (CMakeLists.txt) for a simplified build 
process. The minimal required version for CMake is 3.1.
Also needed is the GNU C++ Compiler. Full C++14 support is required.

Changing Pre-defined Settings
-----------------------------
Listed in the CMakeLists.txt file are preprocessor macros used to specify 
default file paths and other information. Change these to fit the requirements 
of the target use case.

.. list-table::
    :header-rows: 1

    * - Macro
      - Description
    * - MRH_LOCALE_FILE_PATH
      - The full path to the MRH locale file to use.
    * - MRH_USERVICE_LOG_FILE_PATH
      - The full path to the log file to use.
    * - MRH_USERVICE_BACKTRACE_FILE_PATH
      - The full path to the backtrace file to use.
    * - MRH_LOGGER_PRINT_CLI
      - If logging should be printed on the cli.
      

Build Process
-------------
The build process should be relatively straightforward:

1. Aqquire dependencies.
2. Move into the project "build" folder.
3. Compile Makefiles with the included CMakeLists.txt.
4. Run "make" to compile the binary.
5. Install the compiled binary.

Shell Commands
--------------
The following shell commands will create makefiles with the 
provided CMakeLists.txt, compile the project with the created 
makefiles and install the binary:

.. code-block::

    cd <Project Root Folder>/build
    cmake ..
    make
    sudo make install
