#include <unistd.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <time.h>

int main(){
	struct timespec ts;
	time_t start_time;
	time_t end_time;

	int read_fd , write_fd;

	read_fd = open( "./src.dummy", O_RDONLY);
	if(read_fd == 0 )
		return 0;
	write_fd = open( "./dest4.dummy", O_WRONLY | O_CREAT | O_EXCL, 0644);
	if(write_fd == 0)
		return 0;

	timespec_get(&ts, TIME_UTC);
	start_time = ts.tv_sec*1000000000 + ts.tv_nsec;

	sendfile(write_fd, read_fd, NULL, 10*1000*1000);
	
	timespec_get(&ts, TIME_UTC);
	end_time = ts.tv_sec*1000000000 + ts.tv_nsec;

	printf("%ld\n", end_time - start_time);
	
	close(read_fd);
	close(write_fd);

	return 0;
}
