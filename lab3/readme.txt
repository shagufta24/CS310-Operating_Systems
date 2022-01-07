CS 307 Operating Systems Programming Assignment 1 (Lab 3)
Objective: Building a /proc parser

Name: Shagufta Anjum
Roll no: 19XJ1A0568
Date of submission: 22/09/21

FILES:
1. proc_parse.py:
Contains the /proc parser code.
2. helpers.py:
Contains a helper class to display coloured output on the terminal.

VERSION 1:

To run:
$ python3 proc_parse.py

Sample output:

Current Working Directory: /mnt/c/Users/shagu/MU_Code/os_lab/lab_3
Processor type: Intel(R) Core(TM) i7-8565U CPU @ 1.80GHz
Kernel Version: Linux version 4.19.128-microsoft-standard (oe-user@oe-host) (gcc version 8.2.0 (GCC)) #1 SMP Tue Jun 23 12:58:10 UTC 2020
Total Memory: 12899692 kB
Uptime: 60.87 seconds

VERSION 2:

Input arguments:
1. <read_rate> : time interval between two consecutive reads
2. <printout_rate> : time interval over which the average values are calculated

To run: 
$ python3 proc_parse.py <read_rate> <printout_rate>

Note: I have used WSL2 here. WSL2 runs a full linux kernel inside a virtual machine on windows OS. So, /proc does not contain process, memory and hardware information corresponding to the windows kernel. 
As a consequence, some values such as rate of disk read/write are 0

Sample output: (for parameters: 2 10)

Processor states: 0.1600% user 0.1600% system 99.680% idle
Free Memory: 12761124.8 kB (98.925%)
Rate of disk read: 0.0 sectors per sec
Rate of disk write: 0.0 sectors per sec
Rate of context switches: 76.0 switches per sec
Rate of process creation: 3.6 processes per sec (36.0 processes created in 10 seconds)


Processor states: 0.2000% user 0.0% system 99.800% idle
Free Memory: 12760696.0 kB (98.922%)
Rate of disk read: 0.0 sectors per sec
Rate of disk write: 0.0 sectors per sec
Rate of context switches: 63.1 switches per sec
Rate of process creation: 3.0 processes per sec (30.0 processes created in 10 seconds)


Processor states: 0.3200% user 0.0% system 99.680% idle
Free Memory: 12760780.0 kB (98.923%)
Rate of disk read: 0.0 sectors per sec
Rate of disk write: 0.0 sectors per sec
Rate of context switches: 73.4 switches per sec
Rate of process creation: 3.6 processes per sec (36.0 processes created in 10 seconds)






