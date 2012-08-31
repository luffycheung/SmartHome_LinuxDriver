#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main (void) 
{
	int fd;
    int k = 0;
    int old_count = 0;

    char buf[20];
	fd = open ("/dev/myadc",O_RDWR);
    printf("asda");
	if (fd < 0) {
	  printf ("fd open failed\n");
	  exit(0);
	}
	printf ("\n/dev/mykey  opened, fd=%d\n",fd);
    while(1)
    {
     printf("%d\n",(int*)read(fd,buf,sizeof(buf)-1));
    sleep(1);
     
    }
	close (fd);
	printf ("/dev/hello closed :)\n");
	return 0;
}
