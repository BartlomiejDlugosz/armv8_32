# How to run the simulation
First use 

```make simulation``` for making the simulation

or 


```make simulation_rpi``` for linking the pigpio module

then execute with

`./simulation <filename> ` or `./simulation_rpi <filename>`

Training takes quite a while, and can be 5 minutes or more even on a powerful machine.

The simulation will generate the necessary text files to create the graphs,
which can be made using 

```make graphs```

and will appear in the graphing subdirectory.


After running the simulation, the (random) wait times will be available in `<filename>`, whilst the assigned state durations are output to stdout.