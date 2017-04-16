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
