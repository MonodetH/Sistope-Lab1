#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "handlers.h"
#include "processes.h"

int SIGUSR1_COUNTER;
pid_t *PID_HIJOS;		// Hijos del root
pid_t PID_NIETO; 		// Solo un nieto
int N_HIJOS;

void handlerUSR1(int sig){
	SIGUSR1_COUNTER++;
	printf("<pid: %i, y he recibido esta llamada %i veces>\n",getpid(),SIGUSR1_COUNTER);
	return;
}

void handlerUSR2(int sig){
	if(PID_NIETO == 0){ 	// si este proceso no tiene hijo
		PID_NIETO = fork();	// crearlo
		if(PID_NIETO == 0){
			if(signal(SIGINT,handlerINTn) == SIG_ERR)
				printf("No se pudo asignar handler\n");
		}
	}
	return;
}

void handlerTERM(int sig){
	int wStatus;

	// Si tiene hijo, matarlo
	if(PID_NIETO != 0){
		printf("Antes matar al nieto %i\n",PID_NIETO);
		if(kill(PID_NIETO,SIGTERM)==-1){
			printf("No se pudo mandar la senal\n");
		}
		// esperar al hijo
		waitpid(PID_NIETO,&wStatus,0);
	}
	printf("Me muero :(\n");
	exit(0);
}

void handlerINT(int sig){
	int i,wStatus;
	// mandar SIGINT a todos los hijos
	for (i = 0; i < N_HIJOS; i++){
		if(kill(PID_HIJOS[i],SIGINT)==-1){
			printf("No se pudo mandar la senal SIGINT a hijo %i\n",PID_HIJOS[i]);
		}else{
			// esperar al hijo
			waitpid(PID_NIETO, &wStatus, 0);
		}
	}
	// restaurar SIGINT del padre
	if(signal(SIGINT,SIG_DFL) == SIG_ERR)
		printf("No se pudo asignar handler\n");
	return;
}

void handlerINTh(int sig){
	printf("<Soy el hijo con pid: %i, y estoy vivo aun>\n",getpid());
	// restaurar SIGINT de los hijos
	if(signal(SIGINT,SIG_DFL) == SIG_ERR)
		printf("No se pudo asignar handler\n");
	return;
}

void handlerINTn(int sig){
	printf("Quesloquecho");
	// restaurar SIGINT de los hijos
	if(signal(SIGINT,SIG_DFL) == SIG_ERR)
		printf("No se pudo asignar handler\n");
	return;
}


