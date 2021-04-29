#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
sig_atomic_t alrm = 1;
int main() {
	pid_t pid = fork();
	if (pid == 0) { 
		
	} else {
		nice(5);
	}
	int l = 0;
	// alarm(1);
	while(alrm) {
		l++;
	}
	if (pid == 0) {
		printf("c*: %d\n", l);
	} else {
		printf("p*: %d\n", l);
	}
	
}
