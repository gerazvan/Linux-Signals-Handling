#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<time.h>

int nr=0; 
pid_t pa; 
sigset_t ms;

void h1(int n){
  signal(n, h1);
}
void h2(int n) {
  printf("Am trimis: %d\n", nr);
  exit(0);
 }
  
int main(){
  srand(time(NULL));
  int i;
  signal(SIGUSR1, h1);
  signal(SIGINT, h2);
  sigemptyset(&ms);
  sigaddset(&ms, SIGUSR1);
  sigprocmask(SIG_SETMASK, &ms, NULL);
  printf("PID propriu: %d\n", getpid());
  printf("PID advers: ");
  scanf("%d", &pa);
  sigfillset(&ms);
  sigdelset(&ms, SIGUSR1);
  sigdelset(&ms, SIGINT);
  //libc won't let you register handlers for the
  //reserved signals 32 and 33 (these are used to implement multithreading).
  int randomSig = 0;
  for(i = 0; i < 20; ++i){
    randomSig = rand() % 65;
    while(randomSig == 0 || randomSig == 32 || randomSig == 33 || randomSig == SIGUSR1 || randomSig == SIGKILL || randomSig == SIGSTOP || randomSig == SIGCONT) {
      randomSig = rand() % 65;
    }
    printf("%d\n", randomSig);
    kill(pa, randomSig);
    ++nr;
    sigsuspend(&ms);
  }

  kill(pa, SIGUSR1);
  sigsuspend(&ms);
  kill(pa, 9);
  nr += 2;
  printf("Am trimis: %d\n",nr);
  return 0;
}
