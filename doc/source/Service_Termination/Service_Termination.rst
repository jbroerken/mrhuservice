*******************
Service Termination
*******************
User application servicess are terminated for multiple reasons. A application 
service will terminate normally either by user application service request during 
the service update or by mrhcore request. Other application terminations are for 
obvious reasons like crashes.

Termination by Service
----------------------
User application services can terminate themselves by signaling a exit request to 
mrhuservice by the return value of the update function. This will cause the application 
service parent to call the following function:

.. code-block:: c

    void MRH_Exit(void);
    
This allows the application service to finish and clean up. The user application service 
parent will take as much time as needed for MRH_Exit to complete. mrhuservice will then 
attempt to send any remaining events to be sent to the platform services before terminating 
completing.

Termination by SIGTERM
----------------------
The MRH platform can choose to quit the application service at any time by sending 
the SIGTERM signal to the mrhuservice process. 

The termination of the user application service might now be timed. Termination by SIGTERM 
still proceeds like when the termination is requested by the service, but might, after a 
short while, simply terminate the whole process during the user application service 
termination process.

Termination by Crashing
-----------------------
A crash by the user application service will directly terminate the user application service 
parent. A backtrace file will be written before directly terminating the process with the 
return value 1. 

No exit functions provided by the user application service will be called and no remaining 
events will be sent.