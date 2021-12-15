This package includes:

Scheduler.c[The Scheduler reads a list of processes with start times, burst durations, and priorities. 
It produces 3 output files that each report the order in which process will execute based on three 
scheduling algorithms: First Come First Serve(FCFS)[Non-preemptive]; Shortest Job First (SJF) 
[Preemptive]; Priority Scheduling [Preemptive]. Each also reports average turnaround time, average waiting 
time, and throughput.

To compile:
	make
	
To run:
	./Scheduler <filename>
	
Example:
	./Scheduler FirstTest.txt
