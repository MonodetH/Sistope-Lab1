#ifndef PROCESSES_H
#define PROCESSES_H 

#include <unistd.h>
#include <sys/types.h>

extern pid_t PID_NIETO;

pid_t* crearHijos(int nHijos);

void printChildInfo(pid_t *hijos, int nHijos);



#endif