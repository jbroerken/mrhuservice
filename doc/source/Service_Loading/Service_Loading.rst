***************
Service Loading
***************
User application service loading and setup are completely handled by the 
mrhuservice application service parent. The parent will load each application 
service the same way, giving each service the same setup and requirements.

mrhuservice loads services by reading the configuration and binary provided 
by the package given on startup. The configuration and functions are required 
to exist for the user application service to be loaded successfully. Missing any 
function or configuration value will result in the service failing to launch.

The environment setup for the user application service is also handled by 
mrhuservice. The application service parent will perform the setup for the used 
locale, user, group and more based on the information given by the user package.

Package Configuration
---------------------
The application service package configuration (Configuration.conf) is 
loaded first. mrhuservice will load this file from the package 
directory root by the name set by the **PACKAGE_CONFIGURATION_PATH** 
define in the PackagePaths file. The configuration will used to set part 
of the environment the user application service runs in.

.. note::

    It is recommended to set the permissions for the package 
    configuration in a way that the user application service itself 
    cannot change any values inside the configuration file.
    
mrhuservice only loads the values from the configuration which are required 
for the application service to run. The following values will be loaded:

.. list-table::
    :header-rows: 1

    * - Block
      - Key
      - Description
    * - EventVersion
      - AppService
      - The event version used by the service.
    * - RunAs
      - UserID
      - The UNIX user to run this service with.
    * - RunAs
      - GroupID
      - The UNIX group to run this service with.
    * - AppService
      - UpdateTimerS
      - The time in seconds to wait between each update.
        
Environment Setup
-----------------
The next step performed is the environment setup. This starts with reading 
and setting the current platform locale for the user application service. 

.. note::

    The default locale will be used if the platform locale could not be 
    loaded.

The current working directory is set next. The current working directory will 
be set to the FSRoot folder found inside the package directory.
    
After the working directory change comes the user and group setup. The user application 
service parent is currently running with the user and group id of the parent process, 
which will now be changed to the user application service user and group id.

.. note::

    No temporary directory will be created. The temporary folder will only 
    exist as long as the user application included is running (if any).

Application Service Binary
--------------------------
The user application service binary is loaded next. mrhuservice loads this 
by using the relative path set by the **PACKAGE_SERVICE_BINARY_PATH** define.
mrhuservice will then continue by looking up the required user application 
service functions from the loaded binary. These are:

.. code-block:: c

    int MRH_Init(void);
    int MRH_Update(void);
    MRH_Event* MRH_SendEvent(void);
    void MRH_Exit(void);

.. note::

    The user application service binary is required to be provided as a 
    shared library in the **.so** format.
    
Service Init
------------
With the package information loaded and the environment setup mrhuservice will continue with 
the initialization of the user application service. This is done by the parent calling a 
specific function provided by the user application service, called MRH_Init:

.. code-block:: c

    int MRH_Init(void);
    
The return value of this function will then be checked to confirm that the user application 
service has initialized successfully. The application service parent will close if failure is 
returned by MRH_Init.

.. note::

    Every negative value starting from -1 is considered a failure. 0 and above are considered as 
    a success.
