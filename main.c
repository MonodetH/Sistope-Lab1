#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "handlers.h"
#include "processes.h"

extern pid_t *PID_HIJOS;
extern int N_HIJOS;
pid_t *PID_HIJOS;
int N_HIJOS;


int main(int argc, char *argv[])
{
	int mFlag=0;
	int opt,wStatus;

	int sigTo, sigId;

	N_HIJOS = 0;

	/*
	 * Obtener opciones 
	 */
	while((opt = getopt(argc,argv,"mh:")) != -1){
		switch(opt){
			case 'm':
				mFlag = 1;
				break;
			case 'h':
				if(!sscanf(optarg,"%i",&N_HIJOS)){
					fprintf (stderr, "ERROR: Argumento de -h no valido, se requiere un entero.\n");
					return 1;
				}

				break;
			case '?':
				if(optopt=='h'){
					fprintf (stderr, "ERROR: Opcion -%c requiere un argumento.\n", optopt);
					return 1;
				}else{
					fprintf (stderr, "ERROR: Argumentos no validos.\n");
					return 1;
				}
				break;
			default:
				return 1;
		}
	}

	// si existen argumentos no asociados a opcion
	if (optind < argc){
    	fprintf (stderr, "ERROR: Argumentos no validos.\n");
		return 1;
	}

	// si no se especifica numero de hijos
	if (N_HIJOS < 1){
    	fprintf (stderr, "ERROR: Numero de hijos no valido.\n");
		return 1;
	}

	/**
	 * Se crean los hijos 
	 */
	PID_HIJOS = crearHijos(N_HIJOS);

	if(mFlag){
		printChildInfo(PID_HIJOS,N_HIJOS);
	}
	
	/**
	 * Asignar controlador para ctrl + c
	 */
	if(signal(SIGINT,handlerINT) == SIG_ERR)
		printf("No se pudo asignar handler\n");

	while(1){
		// preguntar por accion
		printf("Ingresar numero de hijo y senal a enviar (X - Y):\n");
		scanf("%i - %i",&sigTo,&sigId);
		printf("La senal %i sera enviada al hijo %i de pid %i\n",sigId,sigTo,PID_HIJOS[sigTo-1]);

		// enviar señal
		if(sigTo > 0 && sigTo <= N_HIJOS && PID_HIJOS[sigTo-1] != 0){
			switch(sigId){
				case 2:
					if(kill(PID_HIJOS[sigTo-1],SIGINT)==-1){
						printf("No se pudo mandar la sena SIGINTl\n");
					}else{
						waitpid(PID_HIJOS[sigTo-1], &wStatus, 0);
					}

					break;
				case 15:
					if(kill(PID_HIJOS[sigTo-1],SIGTERM)==-1){
						printf("No se pudo mandar la senal SISTERM\n");
					}else{
						waitpid(PID_HIJOS[sigTo-1], &wStatus, 0);
						PID_HIJOS[sigTo-1] = 0; // eliminar referencia al hijo
					}
					break;
				case 16:
					if(kill(PID_HIJOS[sigTo-1],SIGUSR1)==-1)
						printf("No se pudo mandar la senal SIGUSR1\n");
					break;
				case 17:
					if(kill(PID_HIJOS[sigTo-1],SIGUSR2)==-1)
						printf("No se pudo mandar la senal SIGUSR2\n");
					break;
				default:
					printf("Senal no reconocida\n");
			}
		}else{
			printf("No existe el hijo %i\n",sigTo);
		}
		
	}

	return 0;
}