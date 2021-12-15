#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Process{
	int Process_ID;
	int Arrival_Time;
	int Burst_Duration;
	int Priority;
	int TurnAroundTime;
	int WaitTime;
};
//**********************************************************
struct Gantt
{
	int Process_ID;
	int Arrival_Time;
	int End_Time;
};

void File_Content(struct  Process Processes[], int number);
void FCFS(struct Process Processes[], int number);
void SJFP(struct Process Processes[], int number);
void PRIORITY(struct Process Processes[], int number);
void RR(struct Process Processes[], int number, int quantum);
void Display_Output(struct  Process Processes[],int number,struct Gantt Rows[],int count, char* filename);

int main(int argc, char **argv)
{
	
	int number_of_pross = 0;
	
	if (argc != 2)
	{
		printf("Incorrect number of arguments.\n");
		return -1;
	}
	
	FILE* file1 = fopen(argv[1], "r");
	
	while(!feof(file1))
	{
		char  ch = fgetc(file1);
		if(ch == '\n')
		{
			number_of_pross++;
		}
	}
	struct Process Processes[number_of_pross];
	
	fseek(file1, 0, SEEK_SET);
	
	
	number_of_pross=0;
	
	
	while(fscanf(file1, "%d,%d,%d,%d",&Processes[number_of_pross].Process_ID,&Processes[number_of_pross].Arrival_Time,&Processes[number_of_pross].Burst_Duration,&Processes[number_of_pross].Priority) != EOF	)
	{
		number_of_pross++;
	}
	fclose(file1);
	
	File_Content(Processes,number_of_pross);
	FCFS(Processes,number_of_pross);
	SJFP(Processes,number_of_pross);
	PRIORITY(Processes,number_of_pross);
	
	return 0;
}
//--------------------------------------------------------
void File_Content(struct Process Processes[], int number)
{
	int i;
	printf("PROCESS ID\tARRIVAL TIME\tBURST DURATION\tPRIORITY\n");
	for(i=0;i<number;i++)
	{
		
		printf("%d\t\t%d\t\t%d\t\t%d\n",Processes[i].Process_ID,Processes[i].Arrival_Time,Processes[i].Burst_Duration,Processes[i].Priority);
		
	}
}
//---------------------------------------------------------------
void Display_Output(struct  Process Processes[],int number,struct Gantt Rows[],int count, char *filename)
{
	FILE* file;
	int i;
	file=fopen(filename,"w");
	fprintf(file,"PROCESS ID\tARRIVAL TIME\tEND TIME\n");
	
	for(i=0;i<count;i++)
	{
		fprintf(file,"%10d%10d%10d\n",Rows[i].Process_ID,Rows[i].Arrival_Time,Rows[i].End_Time);
	}
	
	float avgWait=0;
	float avgTurnAround=0;
	for(i=0;i<number;i++)
	{
		avgWait+=Processes[i].WaitTime;
		avgTurnAround+=Processes[i].TurnAroundTime;
	}
	fprintf(file,"Average Wait Time=%f\n",avgWait/number);
	fprintf(file,"TurnAround Time=%f\n",avgTurnAround/number);
	// Assumption: the value of count recieved is greater than exact number of rows in the Gantt chart by 1. Dont forget to adjust the value if yours is not so.
	fprintf(file,"Throughput=%f\n",((float)number/Rows[count-1].End_Time));
	fclose(file);
	
}
//------------------------------------------------------------

/*
 * Note: Use function Display_Output to display the result. The Processes is the
 * updated Processes with their respective waittime, turnAround time.
 * Use the array of Gantt Struct to mimic the behaviour of Gantt Chart. Rows is the base name of the same array.
 * number is the number of processes which if not changed during processing, is taken care. count is the number of rows in the Gantt chart. 
 * 
 * Number of Processes will not exceed 1000. Create an array of Gantt chart which can accomodate at least 1000 process.
 */
// -------------FCFS----------------------------------------
int compare(const void *p, const void *q){
	struct Process *p1 = (struct Process *)p;
	struct Process *p2 = (struct Process *)q;
	return (p1->Arrival_Time > p2->Arrival_Time) - (p1->Arrival_Time < p2->Arrival_Time);
}
void FCFS(struct Process Processes[], int number){
	//---------Start Processing
	struct Gantt *Gantt_Chart;
	Gantt_Chart = (struct Gantt*) malloc((2*number-1)*sizeof(struct Gantt));
	qsort(Processes, number, sizeof(struct Process), compare);
	int timer = 0;
	for(int i = 0; i < number; i++){
		if(timer < Processes[i].Arrival_Time){
			timer = Processes[i].Arrival_Time;
		}
		Gantt_Chart[i].Process_ID = Processes[i].Process_ID;
		Gantt_Chart[i].Arrival_Time = timer;
		Gantt_Chart[i].End_Time = timer + Processes[i].Burst_Duration;
		Processes[i].WaitTime = timer - Processes[i].Arrival_Time;
		Processes[i].TurnAroundTime = Gantt_Chart[i].End_Time - Processes[i].Arrival_Time;
		timer += Processes[i].Burst_Duration;
	}
	
	Display_Output(Processes, number, Gantt_Chart, number, "FCFS");
	//---------End of Processing
	//Hint: Call function with arguments shown Display_Output(Processes,number,Gantt_Chart,count,"FCFS");
}
//---------------------SJFP---------------------------------------
void SJFP(struct Process Processes[],int number){
	//---------Start Processing
	//dynamically allocate gantt chart with size for worst case
	struct Gantt *Gantt_Chart;
	Gantt_Chart = (struct Gantt*) malloc((2*number-1)*sizeof(struct Gantt));
	//copy processes to keep track of different burst times 
	struct Process process_c[number];
	for(int i = 0; i < number; i++){
		process_c[i] = Processes[i];
	}
	//minimum process, shortest burst time, clock, count for while, gantt chart location of process
	int min_p, min_b = INT_MAX, clock = 0, count = 0, gc_loc = 0;
	while(count < number){
		for(int i = 0; i < number; i++){
			//if a process has no arrived, don't worry about it
			if(clock < Processes[i].Arrival_Time){
				break;
			}
			//otherwise, update the minimum process index and minimum burst time
			else if(process_c[i].Burst_Duration != 0 && process_c[i].Burst_Duration < min_b){
				min_p = i;
				min_b = process_c[i].Burst_Duration;
			}
		}
		//if minimum burst time has been updated 
		if(min_b != INT_MAX){
			//decrement to represent execution 
			process_c[min_p].Burst_Duration--;
			//not starting a new process, running an old process 
			if(gc_loc != 0 && Gantt_Chart[gc_loc-1].Process_ID == Processes[min_p].Process_ID){
				Gantt_Chart[gc_loc-1].End_Time++;
			}
			else{
				Gantt_Chart[gc_loc].Process_ID = Processes[min_p].Process_ID;
				Gantt_Chart[gc_loc].Arrival_Time = clock;
				Gantt_Chart[gc_loc].End_Time = clock + 1;
				gc_loc++;
			}			
			if(process_c[min_p].Burst_Duration == 0){
				count++;
				min_b = INT_MAX;
			}
			Processes[min_p].TurnAroundTime = (clock + 1) - process_c[min_p].Arrival_Time;
			Processes[min_p].WaitTime = Processes[min_p].TurnAroundTime - Processes[min_p].Burst_Duration; 
		}
		clock++;
	}
	Display_Output(Processes, number, Gantt_Chart, gc_loc, "SJFP");
}
	
	//---------End of Processing
	

//------------------PRIORITY-------------------------------------
void PRIORITY(struct Process Processes[], int number){
	//---------Start Processing
	//dynamically allocate gantt chart with size for worst case
	struct Gantt *Gantt_Chart;
	Gantt_Chart = (struct Gantt*) malloc((2*number-1)*sizeof(struct Gantt));
	//copy processes to keep track of different burst times 
	struct Process process_c[number];
	for(int i = 0; i < number; i++){
		process_c[i] = Processes[i];
	}
	//minimum process, shortest burst time, clock, count for while, gantt chart location of process
	int min_p, min_b = INT_MAX, clock = 0, count = 0, gc_loc = 0;
	while(count < number){
		for(int i = 0; i < number; i++){
			//if a process has no arrived, don't worry about it
			if(clock < Processes[i].Arrival_Time){
				break;
			}
			//otherwise, update the minimum process index and minimum burst time
			else if(process_c[i].Burst_Duration != 0 && process_c[i].Priority < min_b){
				min_p = i;
				min_b = process_c[i].Priority;
			}
		}
		//if minimum burst time has been updated 
		if(min_b != INT_MAX){
			//decrement to represent execution 
			process_c[min_p].Burst_Duration--;
			//not starting a new process, running an old process 
			if(gc_loc != 0 && Gantt_Chart[gc_loc-1].Process_ID == Processes[min_p].Process_ID){
				Gantt_Chart[gc_loc-1].End_Time++;
			}
			else{
				Gantt_Chart[gc_loc].Process_ID = Processes[min_p].Process_ID;
				Gantt_Chart[gc_loc].Arrival_Time = clock;
				Gantt_Chart[gc_loc].End_Time = clock + 1;
				gc_loc++;
			}			
			if(process_c[min_p].Burst_Duration == 0){
				count++;
				min_b = INT_MAX;
			}
			Processes[min_p].TurnAroundTime = (clock + 1) - process_c[min_p].Arrival_Time;
			Processes[min_p].WaitTime = Processes[min_p].TurnAroundTime - Processes[min_p].Burst_Duration; 
		}
		clock++;
	}
	Display_Output(Processes, number, Gantt_Chart, gc_loc, "Priority");
	//---------End of Processing
	
}
