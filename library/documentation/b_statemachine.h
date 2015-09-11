/**

@page state_machine State machine

NDS provides an hierarchical state machine in a form of a specialized Node.

To add a state machine to a node, just add to it the nds::StateMachine node as a child.

The state machine declares its own children PVs that will be used by the control system to set and/or
 retrieve the state.\n
When declaring the state machine your code has to specify the delegate functions to execute
 when a state transition is requested: the state transition will be executed on a secondary thread
 if the bAsync flag is set in the state machine constructor.

The available states are described in the enumeration nds::state_t: note that not all the states
 can be set directly by your application or by the control system.\n
The states that can be set directly are:
- off
- on
- running

The state fault is set automatically when an exception is thrown, while the intermediate states
 (initializing, starting, stopping, switchingOff) are set by the state machine immediately before
 executing the delegate function that carry on the transition.




*/
