**************
Service Update
**************
User application services are updated by calling a service provided update 
function in a set time interval defined by the service configuration. Events 
to send to platform services are then collected from the service after each 
update finishes.

The service updates happens within a single thread. The amount of events sent 
depends on the amount of available events and the event limit given to 
mrhuservice.

Update Run
----------
The user application service is updated by mrhuservice by calling the following 
function:

.. code-block:: c

    int MRH_Update(void);

mrhuservice will call this function in a set interval defined by the service itself. 
The timer for the next update call starts after the update function has finished 
and all events to send were received. Returning a negative value from the update function 
stops the application service. 

.. note::

    Events to send will not be collected if the update function returns a negative value.

Send Events
-----------
Sending events from the user application service to the platform services is handled 
by calling the following function:

.. code-block:: c

    MRH_Event* MRH_SendEvent(void);

All events recieved from the user application service are sent to the platform 
services by the application service parent. The user application service itself 
only has to provide the events to mrhuservice. mrhuservice will then add the events 
to the send queue.

The MRH_SendEvent function will be called repeatedly until the user application 
service returns NULL or until the event limit has been reached. Returning NULL 
indicates that all events to send have been retrieved by the application service 
parent.

.. note::

    mrhuservice stores events until they are sent. Events from the user application 
    service will be cleaned up by mrhuservice after they have been sent.
    
.. warning::

    There is no guarantee that events will be sent from the application service parent 
    in the way that the events where retrieved from the user application service!
