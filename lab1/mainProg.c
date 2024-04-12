#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

#include "ComputeTask.h"
#include "IOTask.h"

typedef enum OP
{
	COMPUTE_TASK = 1,
	IO_TASK
} OP_t;

typedef struct task
{
	struct task *next;
	OP_t taskType;
} task_t;

int main(int argc, char *argv[])
{
	double computeTaskTimeElapse = 0.0;
	double IOTaskTimeElapse = 0.0;
	double totalTimeElapse = 0.0;

	struct timeval computeTaskStartTime, computeTaskEndTime, IOTaskStartTime, IOTaskEndTime;

	pid_t fpid;
	task_t computeTask, ioTask;
	task_t *curTask = &computeTask;

	computeTask.taskType = COMPUTE_TASK;
	computeTask.next = &ioTask;
	ioTask.taskType = IO_TASK;
	ioTask.next = NULL;

	int parentProcess = 1;
	int childProcessNum = 0;

	while (NULL != curTask)
	{
		if (curTask->taskType == IO_TASK)
			gettimeofday(&IOTaskStartTime, NULL);
		else
			gettimeofday(&computeTaskStartTime, NULL);
		fpid = fork();
		if (0 == fpid)
		{ // This is the child process
			parentProcess = 0;
			break;
		}
		else if (-1 == fpid)
		{
			printf("Generate child Process error!\n");
			exit(0);
		}

		wait(NULL); // wait the child process finish execution

		if (COMPUTE_TASK == curTask->taskType)
			gettimeofday(&computeTaskEndTime, NULL);
		else
			gettimeofday(&IOTaskEndTime, NULL);
		printf("Generate child process with pid:%d\n", fpid);

		++childProcessNum;
		curTask = curTask->next;
	}

	if (parentProcess == 0)
	{
		if (curTask->taskType == IO_TASK)
		{
			executeIOTask();
			printf("This is a IO task, pid:%d parent pid:%d\n", getpid(), getppid()); // Print process ID and parent process ID
		}
		if (curTask->taskType == COMPUTE_TASK)
		{
			executeComputeTask();
			printf("This is a compute task, pid:%d parent pid:%d\n", getpid(), getppid()); // Print process ID and parent process ID
		}
	}
	else
	{
		// Parent Process, we calculate the time for executing computing task and the time fo executing IO task
		computeTaskTimeElapse = (double)(computeTaskEndTime.tv_sec - computeTaskStartTime.tv_sec) * 1000.0 + (double)(computeTaskEndTime.tv_usec - computeTaskStartTime.tv_usec) / 1000.0;
		IOTaskTimeElapse = (double)(IOTaskEndTime.tv_sec - IOTaskStartTime.tv_sec) * 1000.0 + (double)(IOTaskEndTime.tv_usec - IOTaskStartTime.tv_usec) / 1000.0;
		totalTimeElapse = (double)(IOTaskEndTime.tv_sec - computeTaskStartTime.tv_sec) * 1000.0 + (double)(IOTaskEndTime.tv_usec - computeTaskStartTime.tv_usec) / 1000.0;
		printf("Compute Task Time Consume:%fms, IO Task Time Consume: %fms, Total Time Consume:%fms \n", computeTaskTimeElapse, IOTaskTimeElapse, totalTimeElapse);
	}
}