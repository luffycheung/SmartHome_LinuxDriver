#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#define READ_TIME _IO('Z', 0)
#define SET_TIME _IO('Z', 1)

struct tm {
	int sec;
	int min;
	int hour;
	int date;
	int mon;
	int year;
};
int arry[5];
struct tm time;
int main()
{
	int fd, i;
	fd = open("/dev/myrtc", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	/*
	while(1)
	{
		sleep(1);
	ioctl(fd, READ_TIME, &time);
	arry[0] = time.sec;
	arry[1] = time.min;
	arry[2] = time.hour;
	arry[3] = time.date;
	arry[4] = time.mon;
	arry[5] = time.year;
	printf("%02d/%02d/%02d %02d:%02d:%02d\n", time.year, time.mon, time.date, time.hour, time.min, time.sec);
	}

	for (i = 0; i < 6; i++)
	{
		printf("%02x", arry[i]);
	}
	printf("\n");
	sleep(3);
	*/
	struct tm time = {
		0x00,
		0x19,
		0x11,
		0x19, 
		0x09,
		0x11,
	};
	ioctl(fd, SET_TIME, &time);
	ioctl(fd, READ_TIME, &time);
	printf("%02x:%02x:%02x %02x:%02x:%02x\n", time.year, time.mon, time.date, time.hour, time.min, time.sec);
	close(fd);
	return 0;
}
