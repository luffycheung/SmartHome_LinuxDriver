#include <stdio.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
	int fd = open("/dev/key", O_RDWR);
	if (fd < 0)
	{
		perror("open /dev/key error");
		return -1;
	}

	int nread = 0;
	int adc_val = 0;
	while (1)
	{
		nread = read(fd, (void *)&adc_val, sizeof(adc_val));
		if (nread < 0)
		{
			perror("read adc");
			return -1;
		}
		printf("%d\n", adc_val);	
		sleep(1);
	}

	close(fd);

	return 0;
}
