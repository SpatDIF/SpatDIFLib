
What is SpatDIF:

In computer music, many spatialization algorithms use a self-contained syntax and storage-format, wherein control messages (e.g. trajectories to move a sound virtually) programmed for one application are incompatible with any other implementation.

This lack of a standardization complicates the portability of compositions and requires manual synchronization and conversion of control data - a time consuming affair. Incompatible data formats also prevent collaboration between researchers and institutions.
SpatDIF is a format that describes spatial sound information in a structured way, in order to support real-time and non-real-time applications. The format serves to describe, store and share spatial audio scenes across audio applications and concert venues.
You can learn more about the current state of the SpatDIF development in the wiki. If you are interested, please join the mailing list.

How to compile:

execute the following commands from your terminal.

> cd src
> cd make

The makefile detects the OS you are using, compile and generate a shared library named 
spatdiflib.dynlib on MacOS or spatdiflib.so on Linux. then compile the sources in the example folder.


