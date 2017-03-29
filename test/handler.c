#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<signal.h>
#include<sys/wait.h>
int counter = 1;
void handler(int sig){
  printf("hello");
  sleep(1);
  counter+=counter;
  printf("bye");
  return;
}


int main(){
  int i;
  int check =0;
  signal(2, handler);

  printf("here i am\n");
  if(fork() ==0){
    for(i=0;i<5;i++){
      kill(getppid(), 2);
      printf("SEND\n");
    }
    exit(0);

  }
  check = 1+counter;
  wait(NULL);
  check *=10;
  check +=counter;
  printf("%d %d\n", counter,check);

  return 0;
}
