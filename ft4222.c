/*
 * ft4222.c
 *
 *  Created on: Apr 7, 2019
 *      Author: alexandru.bogdan
 */

#include "ft4222.h"
#include <stdio.h>

FT_HANDLE ftHandleStream = NULL;
FT_HANDLE ftHandleGpio = NULL;

bool ft4222_init(void)
{
	FT_STATUS ftStatus;
	DWORD numOfDevices = 0;

	if(ftHandleStream || ftHandleGpio)
	{
		printf("FT4222 already initialized\n");
		return false;
	}

	ftStatus = FT_CreateDeviceInfoList(&numOfDevices);

	for(DWORD iDev = 0; iDev < numOfDevices; ++iDev)
	{
	    FT_DEVICE_LIST_INFO_NODE devInfo;
	    memset(&devInfo, 0, sizeof(devInfo));

	    ftStatus = FT_GetDeviceInfoDetail(iDev, &devInfo.Flags, &devInfo.Type,
	                                    &devInfo.ID, &devInfo.LocId,
	                                    devInfo.SerialNumber,
	                                    devInfo.Description,
	                                    &devInfo.ftHandle);
	    printf("Open Device\n");
	    printf("  Flags= 0x%lx\n", 		devInfo.Flags);
	    printf("  Type= 0x%lx\n",      	devInfo.Type);
	    printf("  ID= 0x%lx\n",         devInfo.ID);
	    printf("  LocId= 0x%lx\n",      devInfo.LocId);
	    printf("  SerialNumber= %s\n",  devInfo.SerialNumber);
	    printf("  Description= %s\n",   devInfo.Description);
	    printf("  ftHandle= 0x%x\n",    (unsigned int)devInfo.ftHandle);

	    if (FT_OK == ftStatus)
	    {
	        if(strcmp(devInfo.Description, "FT4222 A") == 0)
	        {
	            ftStatus = FT_OpenEx((PVOID)devInfo.LocId, FT_OPEN_BY_LOCATION, &ftHandleStream);
	            if (FT_OK != ftStatus)
	            {
		            printf("Open a FT4222 A device failed!\n");
	            	return false;
	            }
	        }
	    }

	    if (FT_OK == ftStatus)
	    {
	        if(strcmp(devInfo.Description, "FT4222 B") == 0)
	        {
	            ftStatus = FT_OpenEx((PVOID)devInfo.LocId, FT_OPEN_BY_LOCATION, &ftHandleGpio);
	            if (FT_OK != ftStatus)
	            {
		            printf("Open a FT4222 B device failed!\n");
	            	return false;
	            }
	        }
	    }
	}

	if(numOfDevices == 0)
	{
		printf("No FT4222 device is found!\n");
		return false;
	}

	return true;
}

void ft4222_uninit(void)
{
	if(ftHandleStream)
	{
		printf("UnInitialize FT4222 A\n");
		FT4222_UnInitialize(ftHandleStream);

		printf("Close FT device\n");
		FT_Close(ftHandleStream);

		ftHandleStream = NULL;
	}

	if(ftHandleGpio)
	{
		printf("UnInitialize FT4222 B\n");
		FT4222_UnInitialize(ftHandleGpio);

		printf("Close FT device\n");
		FT_Close(ftHandleGpio);

		ftHandleGpio = NULL;
	}
}

