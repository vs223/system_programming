#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAP_LOCATION 0x6100

int main () {
	struct timespec ts;
	time_t start_time;
	time_t end_time;
	char *src, *dst;

	int read_fd , write_fd;

	struct stat stat_buf;
	off_t file_size = 0;
	
	read_fd = open( "./src.dummy", O_RDONLY);
	if(read_fd == 0 )
		return 0;
	write_fd = open( "./dest3.dummy", O_RDWR | O_CREAT | O_TRUNC, 0600);
	if(write_fd == 0)
		return 0;

	fstat (read_fd,&stat_buf) ;
	file_size = stat_buf.st_size;


	if (lseek (write_fd, file_size - 1, SEEK_SET) == -1) {
		printf ("lseek error\n");
		return 0;
	}

	timespec_get(&ts, TIME_UTC);
	start_time = ts.tv_sec*1000000000 + ts.tv_nsec;

	write (write_fd, "", 1);

	src = mmap ((void*) MAP_LOCATION, file_size, 
		PROT_READ, MAP_SHARED, read_fd, 0);

	dst = mmap ((void*) MAP_LOCATION + file_size , file_size , 
		PROT_READ | PROT_WRITE, MAP_SHARED, write_fd, 0);

	memcpy (dst, src, file_size);

	timespec_get(&ts, TIME_UTC);
	end_time = ts.tv_sec*1000000000 + ts.tv_nsec;

	printf("%ld\n", end_time - start_time);
	
	return 0;
} 