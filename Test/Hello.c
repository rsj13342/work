# ifndef _WIN32
	#define _BSD_SOURCE
	#include <unistd.h>
	#define Sleep(ms) usleep(ms * 1000)
# endif

#define _WIN32_WINNT 0x0501

# include <stdio.h>
# include "win.h"
# include "libmcp2221.h"
# include "hidapi.h"

int main(void)
{
	puts("Starting!");

	mcp2221_init();
	
	printf("Looking for devices... ");
	int count = mcp2221_find(MCP2221_DEFAULT_VID, MCP2221_DEFAULT_PID, NULL, NULL, NULL);
	printf("found %d devices\n", count);
	
	printf("Opening device... ");
	mcp2221_t* myDev = mcp2221_open();

	if(!myDev)
	{
		mcp2221_exit();
		puts("No MCP2221s found");
		return 0;
	}
	puts("done");

	mcp2221_error res;
	
//	Sleep(500);

	uint8_t buff[5];
	mcp2221_i2c_state_t state = MCP2221_I2C_IDLE;
	
	mcp2221_i2cState(myDev, &state);
	if(state != MCP2221_I2C_IDLE) {
		mcp2221_i2cCancel(myDev);
	}

	puts("Reading...");
	res = mcp2221_i2cRead(myDev, 10, sizeof(buff), MCP2221_I2CRW_NORMAL);
	printf("Read complete: %d\n", res);
	
	puts("Getting...");
	mcp2221_i2cGet(myDev, buff, sizeof(buff));
	puts("Get complete");

	printf("  Temp: %02d %02d %02d %02d %02d\n", buff[0], buff[1], buff[2], buff[3], buff[4]);

	mcp2221_exit();
	
	puts("Done");

	return 0;
}