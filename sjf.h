/*
 * sjf.h
 *
 *  Created on: 2014-02-27
 *      Author: Natasha Squires <nsquires@upei.ca>
 */

#ifndef SJF_H_
#define SJF_H_

//states
#define START 0
#define READY 1
#define RUNNING 2
#define WAITING 3
#define DONE 4
#define MAX 1000

void shortestJobFirst();
int getShortest();
void statusReport();
void finalReport();
void getWaiting();

struct process{
        int processID;
        int arrivalTime0;
        int arrivalTime;
        int CPUtime;
        int IOtime;
        int waitingTime;
	int turnAround;
        int state;
        int CPUbursts[50];
        int CPUcount;
        int IObursts[50];
        int IOcount;
        int incrementTime;
        int IOarrivalTime;
};

#endif /* SJF_H_ */
