#include <unistd.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
	struct timespec ts;
	time_t start_time;
	time_t end_time;
	size_t read_res;
	char buf[2];

	int read_fd , write_fd;

	read_fd = open( "./src.dummy", O_RDONLY);
	if(read_fd == 0 )
		return 0;
	write_fd = open( "./dest1.dummy", O_WRONLY | O_CREAT | O_EXCL, 0644);
	if(write_fd == 0)
		return 0;
	timespec_get(&ts, TIME_UTC);
	start_time = ts.tv_sec*1000000000 + ts.tv_nsec;
	while( (read_res = read( read_fd, buf, 1)) > 0){
		write( write_fd, buf, read_res);
	}
	timespec_get(&ts, TIME_UTC);
	end_time = ts.tv_sec*1000000000 + ts.tv_nsec;

	printf("%ld\n", end_time - start_time);

	close(read_fd);
	close(write_fd);

	return 0;
}
