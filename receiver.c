#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>

pid_t pa; 
sigset_t ms;
int count[NSIG];

void h1(int n) {
  signal(n, h1);
  count[n] += 1;
}

void h2(int n) {
  count[n] += 1;
  int s = 0;
  for(int sig = 0; sig < NSIG; sig++) {
    printf("%d: %d\n", sig, count[sig]);
    s += count[sig];
  }
  printf("Am primit: %d\n",s);
  kill(pa, SIGUSR1);
  exit(0);
}

  int main(){
  sigfillset(&ms);
  for(int sig = 0; sig < NSIG; sig++) {
    if(sig != SIGKILL && sig != SIGSTOP && sig != SIGCONT) {
      signal(sig, h1);
    }
  }
  signal(SIGUSR1, h2);
  sigprocmask(SIG_SETMASK, &ms, NULL);
  printf("PID propriu: %d\n", getpid());
  printf("PID advers: "); scanf("%d", &pa);
  sigemptyset(&ms);
  while(1){
    sigsuspend(&ms);
    kill(pa, SIGUSR1);
  }
  return 0;
}
