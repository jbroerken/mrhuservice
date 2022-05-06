*************
Event Handler
*************
mrhuservice handles the event sending to platform services over mrhcore 
for the user application service. The event handler included uses libmrhev 
to send created events.

Event Container
---------------
The event handler uses a event container for storing events to send. 
Events retrieved from the user application service are stored the way 
they are recieved.

Event Limitations
-----------------
User application services can only send a specific set of events. The 
events allowed are defined by the core and platform services and are 
not handled by mrhuservice itself. The application service parent will 
send all events recieved.

.. note::

    Application service events have no user group id set for the 
    events sent.
    
Source: Pipe
------------
The pipe source uses one pipe for sending events. The pipe file descriptor 
is given to mrhuservice on startup.

Pipe event sources use polling in libmrhev to wait for data to send and 
receive. The time to wait for exchanging data is given to mrhuservice on 
startup as well.

Source: MRHCKM
--------------
The MRHCKM source is currently not implemented.