# How to run the simulation
First use 

```make simulation``` for making the simulation

or 


```make simulation_rpi``` for linking the gpio module

then execute with

`./simulation` or `./simulation_rpi`


This will generate the necessary text files to create the graphs,
which can be made using 

```make graphs```

and will appear in the graphing subdirectory.


After running the simulation, the (random) wait times will be available in `output.txt`, whilst the assigned state durations are output to stout.


