#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "handlers.h"
#include "processes.h"

pid_t PID_NIETO;

pid_t* crearHijos(int nHijos){
	int isRoot = 1;		// identifica al proceso que hizo la llamada
	pid_t* pidHijos;	// lista de hijos
	pid_t pidAux;
	int i;	// contadores

	pidHijos = (pid_t*)malloc(nHijos*sizeof(pid_t));

	for(i=0;i<nHijos && isRoot;i++){
		pidAux=fork();

		/**
		 * Aqui se crea al hijo, se asignan los handlers y queda en un bucle infinito
		 */
		if(pidAux == 0){
			pidHijos = NULL;
			PID_NIETO = 0;
			isRoot = 0;

			// Asignar manejadores
			if(signal(SIGUSR1,handlerUSR1) == SIG_ERR)
				printf("No se pudo asignar handler\n");
	
			if(signal(SIGUSR2,handlerUSR2) == SIG_ERR)
				printf("No se pudo asignar handler\n");

			if(signal(SIGTERM,handlerTERM) == SIG_ERR)
				printf("No se pudo asignar handler\n");

			if(signal(SIGINT,handlerINTh) == SIG_ERR)
				printf("No se pudo asignar handler\n");

			while(1){
				sleep(1);
			}

		// si es el padre simplemente guarda la id en un arreglo
		}else{
			pidHijos[i] = pidAux;
		}
	}
	return pidHijos;
}

void printChildInfo(pid_t *hijos, int nHijos){
	int i;
	printf("Mostrando la informacion por pantalla:\n");
	for(i=0;i<nHijos;i++){
		printf("Numero: %i\t,pid:%i\n", i+1,hijos[i]);
	}
}
