/*
 * gpioint.c
 *
 *  Created on: Apr 7, 2019
 *      Author: alexandru.bogdan
 */

#include "ft4222.h"

#include <stdio.h>

static HANDLE hRxEvent;
static HANDLE hInterruptThread;
static HANDLE hStopEvent;
static DWORD WINAPI interruptThread(LPVOID lpParameter);

bool gpioint_init(void)
{
	FT_STATUS ftStatus;

	if(!ftHandleGpio)
	{
		printf("FT4222 B not initialized!\n");
		return false;
	}

	hRxEvent = CreateEvent(
		NULL,
		false,
		false,
		NULL
	);

	ftStatus = FT_SetEventNotification(ftHandleGpio, FT_EVENT_RXCHAR, hRxEvent);

	if(ftStatus != FT_OK)
	{
		printf("FT_SetEventNotification failed\n");
		return false;
	}

	GPIO_Dir gpioDir[4];
	gpioDir[0] = GPIO_OUTPUT;
	gpioDir[1] = GPIO_OUTPUT;
	gpioDir[2] = GPIO_OUTPUT;
	gpioDir[3] = GPIO_INPUT;

	if(FT4222_GPIO_Init(ftHandleGpio, gpioDir) != FT4222_OK)
	{
		return false;
	}

	if(FT4222_SetWakeUpInterrupt(ftHandleGpio, true) != FT4222_OK)
	{
		return false;
	}

	if(FT4222_SetInterruptTrigger(ftHandleGpio, GPIO_TRIGGER_FALLING) != FT4222_OK)
	{
		return false;
	}

	hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(!hStopEvent)
	{
		return false;
	}

	hInterruptThread = CreateThread(NULL, 0, interruptThread, NULL, 0, NULL);
	if(!hInterruptThread)
	{
		return false;
	}

	return true;
}

void gpioint_uninit(void)
{
	if(hInterruptThread)
	{
		SetEvent(hStopEvent);
		WaitForSingleObject(hInterruptThread, INFINITE);
		CloseHandle(hInterruptThread);
		CloseHandle(hRxEvent);
		hInterruptThread = NULL;
		hRxEvent = NULL;
	}
}

static DWORD WINAPI interruptThread(LPVOID lpParameter)
{
	bool keepGoing = true;
	HANDLE hEvents[2];
	hEvents[0] = hRxEvent;
	hEvents[1] = hStopEvent;

	while(keepGoing)
	{
		BOOL value;
		DWORD dw = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
		switch(dw)
		{
		case WAIT_OBJECT_0:
			if(FT4222_GPIO_Read(ftHandleGpio, (GPIO_Port)GPIO_PORT3, &value) == FT4222_OK)
			{
				printf("GPIO INT: %d\n", value);
			}
			break;
		case WAIT_OBJECT_0 + 1:
			keepGoing = false;
			break;
		default:
			break;
		}
	}

	return 0;
}

