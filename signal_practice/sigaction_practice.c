#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>


int sync_p = 0;

void usr1Handler(int sig){
	// block other signals.
	sigset_t new, old;
	sigfillset(&new);
	sigemptyset(&old);
	if(sigprocmask(SIG_BLOCK, &new, &old) < 0)
		perror("usr1 handler blocking error");

	// something process...
	printf("Hi usr1 handler\n");	

	// unblock other signals.
	if(sigprocmask(SIG_SETMASK, &old, 0) < 0)
		perror("usr1 handler setting error");
}

void usr2Handler(int sig){
	// block other signals.
	sigset_t new, old;
	sigfillset(&new);
	sigemptyset(&old);
	if(sigprocmask(SIG_BLOCK, &new, &old) < 0)
		perror("usr2 handler blocking error");

	// something process...
	printf("Good usr2 handler\n");	

	// unblock other signals.
	if(sigprocmask(SIG_SETMASK, &old, 0) < 0)
		perror("usr2 handler setting error");
}

void usr1HandlerP(int sig){
	sync_p = 1;	
}

void usr2HandlerP(int sig){
	sync_p = 0;	
}

void quitHandler(int sig){
	exit(0);
}



int main(void){
	pid_t pid;

	int status = 0;
	sigset_t newmask, oldmask;
	int i=0;
	
	struct sigaction usr1, usr2;
	usr1.sa_handler = usr1Handler;
	usr2.sa_handler = usr2Handler;

	sigemptyset(&usr1.sa_mask);
	sigemptyset(&usr2.sa_mask);

	if(sigaction(SIGUSR1, &usr1, NULL) == -1)
		perror("sigaction USR1");
	if(sigaction(SIGUSR2, &usr2, NULL) == -1)
		perror("sigaction USR2");

	pid = fork();

	switch(pid){
	case -1:
		perror("fork");
		break;
	case 0:
		//signal(SIGUSR1, usr1Handler);
		//signal(SIGUSR2, usr2Handler);
		signal(SIGQUIT, quitHandler);
		sigemptyset(&newmask);
		sigemptyset(&oldmask);
		
		sigaddset(&newmask, SIGUSR1);
		sigaddset(&newmask, SIGUSR2);

		if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) perror("SIG_BLOCK error");
		kill(getppid(), SIGUSR1);
		sleep(5);
		kill(getppid(), SIGUSR2);
	
		if(sigprocmask(SIG_SETMASK, &oldmask, 0) < 0)
			perror("SIG_SETMASK error");
		kill(getppid(), SIGUSR1);	

		while(1);

	default:
		signal(SIGUSR1, usr1HandlerP);
		signal(SIGUSR2, usr2HandlerP);
		for(i = 0; i < 10; i++){
			while(!sync_p);
			if(i % 2 == 0){
				kill(pid, SIGUSR1);
			}
			else{
				kill(pid, SIGUSR2);
			}
			sleep(1);
		}
		kill(pid, SIGQUIT);
		if(wait(&status) == pid){
			exit(0);
		}
	}	
	printf("Not good exit\n");
}
