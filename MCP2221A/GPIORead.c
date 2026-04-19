/*
 * GPIORead.c
 *
 *  Created on: 2026/04/16
 *      Author: ken
 */
#ifndef _WIN32
	#define _BSD_SOURCE
	#include <unistd.h>
	#define Sleep(ms) usleep(ms * 1000)
#endif

#define _WIN32_WINNT 0x0501

#include <stdio.h>
#include<unistd.h>

#include "win.h"
#include "libmcp2221.h"
#include "hidapi.h"

static int readGPIOs(mcp2221_t* myDev)
{

	mcp2221_gpio_value_t values[MCP2221_GPIO_COUNT];
	mcp2221_readGPIO(myDev, values);

//	FILE *fp;
//	fp = fopen("GPIO_READ_RESULT.dat", "w");
//	fprintf(fp, "GPIO0=%d\n",values[0]);
//	fprintf(fp, "GPIO1=%d\n",values[1]);
//	fprintf(fp, "GPIO2=%d\n",values[2]);
//	fprintf(fp, "GPIO3=%d\n",values[3]);
//	printf("GPIO0=%d\n",values[0]);
//	printf("GPIO1=%d\n",values[1]);
//	printf("GPIO2=%d\n",values[2]);
//	printf("GPIO3=%d\n",values[3]);
//	fclose(fp);

	if(values[0] == 0 && values[1] == 1) {
		return 1;
	}
	if(values[0] == 1 && values[1] == 0) {
		return 2;
	}
	return 0;
}

int main(void)
{

	int curGpio;
	int oldGpio;

	mcp2221_init();

	mcp2221_find(MCP2221_DEFAULT_VID, MCP2221_DEFAULT_PID, NULL, NULL, NULL);

	mcp2221_t* myDev = mcp2221_open();

//	readGPIOs(myDev);
	oldGpio = readGPIOs(myDev);
	for(;;) {
		curGpio = readGPIOs(myDev);
		printf("Sw: %d\n", curGpio);
		if (oldGpio != curGpio) {
			if (curGpio == 1) {
				printf("Volume Up!!\n");
				system("VUP.BAT");
//				system("C:/TrayVolume/TrayVolume.exe -Volume:+15%");
			}
			if (curGpio == 2) {
				printf("Volume Down!!\n");
				system("VDOWN.BAT");
//				system("C:/TrayVolume/TrayVolume.exe -Volume:-15%");
			}
			oldGpio = curGpio;
		}

		//0.1sec
//		usleep(100000);

		//0.05sec
		usleep(50000);
	}

	mcp2221_exit();

	return 0;
}
