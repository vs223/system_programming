# System Programming hw2
컴퓨터 공학부 
2012-11262
이승준

## Abstraction
 10MB 크기의 파일을 아래와 같은 방식으로 복사하는 프로그램을 작성하고 수행시간을 측정한다. 
1. 1 character씩 read/write
2. 4KB 씩 read/write
3. mmapcopy 사용
4. sendfile() 사용
5. LINUX cp 사용

## Environment
- ubuntu 16.04 LTS
- 인텔 브로드웰 i3u cpu

## Files
- Makefile : 각 c 코드들을 컴파일하는 옵션과, 실행파일을 실행시킨 결과를 저장 및 올바르게 카피되었는 지 확인하기 위한 diff하는 옵션을 두었다.
- hw2-1.c : 1 character씩 read/write 방식
- hw2-2.c : 4KB 씩 read/write
- hw2-3.c : mmapcopy 사용
- hw2-4.c : sendfile() 사용
- hw2-5.c : exec을 이용하여 LINUX cp 사용
- dummy_maker.py : 10mb 파일을 생성하는 파이선 코드

## Measurement data
 | | 1 byte	 | 4 byte	 | mmap	 | sendfile	 | fork and cp | 
 | - | - | - | - | - | - | 
 | | 21028448499	 | 17415249	 | 12747543	 | 13196718	 | 16074807 | 
 | | 21204281750	 | 21259027	 | 9370809	 | 12192927	 | 11639863 | 
 | | 21134083746	 | 20762787	 | 13710640	 | 7066010	 | 16342585 | 
 | | 	 | 19512920	 | 15114309	 | 11613385	 | 10013984 | 
 | | 	 | 17669570	 | 15452088	 | 12536439	 | 15983403 | 
 | | 	 | 12478966	 | 13303108	 | 6633151	 | 13194524 | 
 | | 	 | 16890420	 | 8878244	 | 13628956	 | 15903406 | 
 | | 	 | 16348012	 | 16450962	 | 7130175	 | 15216425 | 
 | | 	 | 20745854	 | 16391198	 | 7039588	 | 9972849 | 
 | | 	 | 17454328	 | 8810747	 | 13865601	 | 12224558 | 
| avg(ns)	 | 21,122,271,331.6667	 | 18,053,713.3	 | 13,022,964.8	 | 10,490,295	 | 13,656,640.4 | 
| stdev	 | 88509789.7895708	 | 2650890.06289565	 | 3023197.08677974	 | 3104059.59775897	 | 2563149.17699971 | 

## Code
- hw2-1.c
```
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
```
- hw2-2.c
```
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
	char *buf = (char*)calloc(4100, 1);

	int read_fd , write_fd;

	read_fd = open( "./src.dummy", O_RDONLY);
	if(read_fd == 0 )
		return 0;
	write_fd = open( "./dest2.dummy", O_WRONLY | O_CREAT | O_EXCL, 0644);
	if(write_fd == 0)
		return 0;
	
	timespec_get(&ts, TIME_UTC);
	start_time = ts.tv_sec*1000000000 + ts.tv_nsec;
	while( (read_res = read( read_fd, buf, 4096)) > 0){		
		write( write_fd, buf, read_res);
	}
	timespec_get(&ts, TIME_UTC);
	end_time = ts.tv_sec*1000000000 + ts.tv_nsec;

	printf("%ld\n", end_time - start_time);

	close(read_fd);
	close(write_fd);
	

	return 0;
}
```
- hw2-3.c
```
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
```
- hw2-4.c
```
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
```
-hw2-5.c
```
#include <unistd.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>

int main(){
	struct timespec ts;
	time_t start_time;
	time_t end_time;
	pid_t pid;
	int status;

	timespec_get(&ts, TIME_UTC);
	start_time = ts.tv_sec*1000000000 + ts.tv_nsec;

	if(pid = fork()){
		waitpid(pid, &status, 0);
	}
	else{
		//child
		execlp( "cp", "cp", "src.dummy", "dest5.dummy", NULL);
	}	
	
	timespec_get(&ts, TIME_UTC);
	end_time = ts.tv_sec*1000000000 + ts.tv_nsec;

	printf("%ld\n", end_time - start_time);

	return 0;
}
```


- Makefile
```
test1: hw2-1.c
	gcc hw2-1.c -o test1

test2: hw2-2.c
	gcc hw2-2.c -o test2

test3: hw2-3.c
	gcc hw2-3.c -o test3

test4: hw2-4.c
	gcc hw2-4.c -o test4   

test5: hw2-5.c
	gcc hw2-5.c -o test5

run1: test1
	./test1 >> output.txt
	diff src.dummy dest1.dummy
	rm -rf dest1.dummy

run2: test2
	./test2 >> output.txt
	diff src.dummy dest2.dummy
	rm -rf dest2.dummy

run3: test3
	./test3 >> output.txt
	diff src.dummy dest3.dummy
	rm -rf dest3.dummy

run4: test4
	./test4 >> output.txt
	diff src.dummy dest4.dummy
	rm -rf dest4.dummy

run5: test5
	./test5 >> output.txt
	diff src.dummy dest5.dummy
	rm -rf dest5.dummy
```
- dummy_maker.py
```
for i in range(1000*1000):
	for  j in range(10):
		print(j, end="")
```
