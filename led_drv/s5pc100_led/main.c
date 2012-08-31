/*
 * main.c : test demo driver
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#define LED_ON 0x4800
#define LED_OFF 0x4801
int main()
{
	int i = 0;
	int dev_fd;
	dev_fd = open("/dev/simple",O_RDWR | O_NONBLOCK);
	if ( dev_fd == -1 ) {
		printf("Cann't open file /dev/simple\n");
		exit(1);
	}
	while(1)
	{
	ioctl(dev_fd,LED_ON,0);
  //     printf("on\n");
	sleep(1);
	ioctl(dev_fd,LED_OFF,0);
//     printf("off\n");
	sleep(1);
	}
	//getchar();
	return 0;
}
