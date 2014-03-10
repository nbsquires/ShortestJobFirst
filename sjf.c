/**
 * sjf.c
 *
 *  Created on: 2014-02-27
 *      Author: Natasha Squires <nsquires@upei.ca>
 */
#include <stdio.h>
#include <stdlib.h>
#include "sjf.h"

struct process pArray[100];
FILE *file;
int count1, count2, count3, count4, processCount,
    shortest, time, currentCPUbrst, currentIObrst,
    process, IOprocess, waiting, earlierArrival, waitingCount;

int main(int argc, char *argv[])
{

	//file reading stuff
	file = fopen(argv[1], "r");

	if(file == NULL)
	{
		printf("Error. No file chosen. Exiting.\n");
		exit(1);
	}

	int integer;
	count1=0, count2=0;

	int temp[50][100], count[50];

	while(fscanf(file, "%d", &integer)!=EOF)
	{
		if(integer == -99)
		{
			count1++;
			count[count1-1]=count2;
			count2=0;
		}
		else
		{
			temp[count1][count2] = integer;
			count2++;
		}
	}

	processCount = count1;

	int i, j;

	//initializing some information for the process array
	for(i=0;i<processCount;i++)
	{
		pArray[i].processID = temp[i][0];
		pArray[i].arrivalTime = temp[i][1];
		pArray[i].arrivalTime0 = temp[i][1];
		pArray[i].CPUtime = 0, pArray[i].IOtime=0, pArray[i].turnAround=0;
		count3=0,count4=0;

		for(j=2; j<count[i]; j++)
		{
			if(j%2==0)
			{
				pArray[i].CPUbursts[count3] = temp[i][j];
				count3++;
			}
			else
			{
				pArray[i].IObursts[count4] = temp[i][j];
				count4++;
			}
		}
		pArray[i].CPUcount = count3;
		pArray[i].IOcount = count4;
	}

	shortestJobFirst();

	exit(0);
}

void shortestJobFirst()
{
	time = 0, currentCPUbrst=0, currentIObrst=0, waitingCount=0;
	int i,j;
	shortest = pArray[time].CPUbursts[currentCPUbrst];

	//initializing states for the processes
	for(i=1; i<count1; i++)
		pArray[i].state = START;
	pArray[time].state = READY;

	//while there are still processes to run
	while(processCount > 0 /*&& time <= 42*/)
	{
		//printf("At time %d:\n", time);
		//statusReport();

		if(pArray[process].state != DONE)
		{
			getWaiting(); //get waiting process's information, if it exists
		}

		process = getShortest(); //find the process with shortest CPU burst at time i

		//shortest = MAX when a process is finished. If it still remains MAX after getShortest(),
		//that means there are no other CPU bursts at that time to run.
		if(shortest!=MAX)
	        {
	 		for(i=0; i<count1; i++) // ready states need their waiting time to be incremented
	 		{
	 			if(pArray[i].state == 1)
	 				pArray[i].waitingTime++;
	 		}

			//if there are no waiting processes and a running process
			if(pArray[IOprocess].state!=WAITING && pArray[process].state == RUNNING)
			{
				pArray[process].CPUbursts[currentCPUbrst]--; //decrement CPU burst
				pArray[process].CPUtime++; //increment process's CPU time
			}
			//else if there is a waiting process
			else if(pArray[IOprocess].state==WAITING)
			{
				if(pArray[process].state == RUNNING) //if a CPU burst is also running
				{
					pArray[process].CPUbursts[currentCPUbrst]--;//decrement CPU burst
					pArray[process].CPUtime++; //increment CPU time
				}

				//don't allow an IOburst to be decremented to a value < 0
				if(pArray[IOprocess].IObursts[currentIObrst] > 0)
				{
					pArray[IOprocess].IObursts[currentIObrst]--; //decrement IO burst
					pArray[IOprocess].IOtime++; //increment IOtime
				}
			}

			//if CPU burst is done, and there is another CPU burst in the CPU burst array
			if(pArray[process].CPUbursts[currentCPUbrst]==0 && pArray[process].CPUbursts[currentCPUbrst+1])
			{
				pArray[process].state = WAITING; //set it to waiting so IO burst can be decremented
				shortest = MAX; //reset shortest value
			}
			//else if the CPU burst is done, there is no remaining CPU bursts for that process,
			//and there are no remaning IO bursts
			else if(pArray[process].CPUbursts[currentCPUbrst]==0 && 0 == pArray[process].CPUbursts[currentCPUbrst+1])
			{
				int IOburstsum=0;
				for(i=0; i<pArray[process].IOcount; i++)
					IOburstsum+=pArray[process].IObursts[i];

				if(IOburstsum == 0) //if there are no more IO bursts for that process
				{
					pArray[process].state = DONE; //process is finished
					processCount--; //decrement amount of processes to run
					pArray[process].turnAround = time; //this time is process's end time
					shortest = MAX; //reset shortest value
				}
			}
		       //else if the process's IOburst is done, and there are remaining IO bursts for that			     // process
		       else if(pArray[IOprocess].IObursts[currentIObrst]==0 && pArray[IOprocess].IObursts[currentIObrst+1])
			{
				pArray[IOprocess].state = READY;//set it to ready for the CPU
			}
			//else if the process's IO burst is done, there are no more IO bursts for that process
			//but the process hasn't finished yet (i.e on last CPU burst for that process)
			else if(pArray[IOprocess].IObursts[currentIObrst] == 0 && 0 == pArray[IOprocess].IObursts[currentIObrst+1] /*&& pArray[IOprocess].state!=DONE*/)
			{
				int CPUburstsum=0;
				for(i=0; i<pArray[IOprocess].CPUcount; i++)
					CPUburstsum+=pArray[IOprocess].CPUbursts[i];

				if(CPUburstsum!=0)
					pArray[IOprocess].state = READY;//set that process ready for the CPU
			}

			time++; //update over all time for the system
			for(i=0; i < count1; i++) //update processes that are in states above START
			{
				if(pArray[i].incrementTime==1)
					pArray[i].arrivalTime++;
			}
		}
		else
		{
			int i, IOburstsum=0, CPUburstsum=0;

			for(i=0; i<pArray[process].IOcount; i++)
				IOburstsum+=pArray[process].IObursts[i];
			for(i=0; i<pArray[process].CPUcount; i++)
				CPUburstsum!=pArray[process].CPUbursts[i];

			if(IOburstsum==0 && CPUburstsum==0)
			{
				pArray[process].state = DONE; //set state to done
				getWaiting();
			}
		        else if(IOburstsum!=0) //If process still has IO bursts left
			{
				pArray[process].state = WAITING; //set that process to waiting
				getWaiting(); //get waiting process information

  				if(pArray[IOprocess].IObursts[currentIObrst]>0)
                	        {
	                              pArray[IOprocess].IObursts[currentIObrst]--; //decrement IO burst
        	                      pArray[IOprocess].IOtime++; //increment IO time
		                }

				//if IO burst is done, and the process has another IO burst
				if(pArray[IOprocess].IObursts[currentIObrst]==0 && pArray[IOprocess].IObursts[currentIObrst+1])
					pArray[IOprocess].state = READY;//set it ready for the CPU
				//else if the IO burst is done, and there are no other IO bursts for that
				//process, but it has not finished (i.e. at last CPU burst)
				else if(pArray[IOprocess].IObursts[currentIObrst]==0 && 0 == pArray[IOprocess].IObursts[currentIObrst+1] && pArray[IOprocess].state!=DONE)
					pArray[IOprocess].state = READY; //set to ready for the CPU
			}
			//else if there are no more IO bursts but there are CPU bursts
			else if(IOburstsum==0 && CPUburstsum!=0)
				pArray[process].state = READY; //set to ready for the CPU
		}

	}

//	statusReport();
	printf("\n");
	finalReport();
	printf("\n");
}

//Finds shortest ready CPU burst at time i
int getShortest()
{
	int i,j;

	for(i=0; i<count1; i++)
	{
		//if there is a process at that time, and it has not yet finished
		if(pArray[i].arrivalTime == time && pArray[i].state!=DONE)
		{
			//make sure to set it so its elapsed time updates with the system
			pArray[i].incrementTime = 1;
			for(j=0; j<pArray[i].CPUcount; j++)
			{
				//look at first non-zero CPU burst
				if(pArray[i].CPUbursts[j]!=0 && pArray[i].state != WAITING)
				{
					if(pArray[i].CPUbursts[j] < shortest)
					{
						shortest = pArray[i].CPUbursts[j];
						process = i;
						currentCPUbrst = j;
						break;
					}
					else
						pArray[i].state = READY; //set larger CPU bursts to ready
					break;
				}
			}
		}
		else
			pArray[i].incrementTime = 0;
	}


	pArray[process].state = RUNNING; //set process with shortest CPU burst to running state

	return process;
}

//Finds information about waiting states. (with multiple waiting states at time i, first come first serve
//is the approach I took
void getWaiting()
{
	int i,j;

	for(i=0; i < count1; i++)
	{
		//If current unfinished process's state is waiting
		if(pArray[i].state == WAITING && pArray[i].state !=DONE)
		{
			for(j=0; j<pArray[i].IOcount; j++)
			{
				//look at first waiting state, and its first non-zero entry in its
				//IO burst array
				if(pArray[i].IObursts[j]!=0)
				{
					currentIObrst = j;
					IOprocess = i;
					break;
				}
			}

			break;
		}
	}
}

//Displays process CPU/IO bursts and their process IDs and states.
void statusReport()
{
	int i,j;
  	for(i=0; i<count1; i++)
  	{
  		printf("Process %d in state %d: ", pArray[i].processID, pArray[i].state);
  		for(j=0; j < pArray[i].CPUcount; j++)
  		{
  			printf(" %d ", pArray[i].CPUbursts[j]);
  		}

  		for(j=0; j < pArray[i].IOcount;j++)
  		{
  			printf(" %d ", pArray[i].IObursts[j]);
  		}
  		printf("\n");
  	}
}

//Displays the final report for the processes
void finalReport()
{
	int i,j,totalTurnAround=0, totalWaiting=0;
 	for(i=0; i<count1; i++)
 	{
 		printf("process %d finished with waiting time of %d and turnaround time of %d\n",pArray[i].processID, pArray[i].waitingTime, pArray[i].turnAround-pArray[i].arrivalTime0);

		totalTurnAround+=(pArray[i].turnAround-pArray[i].arrivalTime0);
		totalWaiting+=pArray[i].waitingTime;
 	}

	printf("Average turn around time: %G\nAverage waiting time: %G\nThroughput: %G\n", ((double)totalTurnAround/count1), ((double)totalWaiting/count1), ((double)count1/time));


}
