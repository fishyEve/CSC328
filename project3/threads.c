// Eve Collier
// project3: threads
// CSC 328 section 010 fall 2023
// threads.c - a c program that will run and modify multiple threads


// here we go again
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdarg.h>

int x; // our global variable named "x"

// we will create a struct called 'data'. 
// STRUCT DATA
// holds the information for data - the information that will be manipulated by the threads
// int *num - element for our num variable
// int *nump - elment for *nump
// int *threadNum - keeps track of which thread we are on
typedef struct data{
  int *num;
  int *nump;
  int threadNum; // thread num we are on
} data_t;

// the efprintf() helper function serves as an error checker for every call to fprintf made throughout the program
// paramaters: FILE *stream - a stream output will be written to - input
//            *char fmt - format string (printf docs provides more detail) - input
//  	      ... - variadic arg - input
// void return type 
 void efprintf(FILE *stream, const char* fmt, ...){
  va_list ap;
  va_start(ap, fmt);
  int rc = vfprintf(stream, fmt, ap);
  va_end(ap);

  // error check on rc...
  if (rc < 0){
    perror("fprintf");
    exit(-1);
  }
  rc = fflush(stream);
  if (rc < 0){
    perror("fflush");
    exit(-1);
  }

 } // END of efprintf()



// the routine() function essentially performs the functions of the thread(s)
// parameters: void* val - input, basically, the struct called data created earlier. 
// return type is void* because we are dealing with threads, its so that we can put anything we want into the thread
void* routine(void* val){
  data_t *d = (data_t*) val;
  efprintf(stdout, "%s %d %s %lu%s %d %s %d %s %d\n", "BEFORE increment: Thread number", d->threadNum, "with TID of", pthread_self(), ": X =", x, "num =", *d->num, "nump =", *d->nump);
  //efprintf(stdout, "\n");

 
  x = x + 50;
  *d->num = *d->num + 1;
  *d->nump = *d->nump + 1;

  efprintf(stdout, "%s %d %s %lu%s %d %s %d %s %d\n", "AFTER increment: Thread number", d->threadNum, "with TID of", pthread_self(), ": X =", x, "num =", *d->num, "nump =", *d->nump);
  efprintf(stdout, "\n");
  pthread_exit(NULL);  

} // END of routine()


// main()
// skeleton of threads.c, operates the program and makes all necessary function calls
// takes in argc and argv - command line arguments - both are inputs
int main(int argc, char** argv){

  if (4 >= argc){
    efprintf(stdout, "At least four command line arguments are required!\n");
    exit(-1);
  }

  if (argc > 5){ 
    efprintf(stdout, "Four is the maximum number of command line arguments you're allowed to provide. Calm down.\n");
    exit(-1);
  }


   int processNum; // variable holding the number of processes to occur

   processNum = atoi(argv[1]);
 
   // processNum can only be between 1 -10 (inclusive), with that said...
   if (processNum < 1){
     efprintf(stdout, "The number of processes to create must be between 1 and 10 (inclusive)!\n");
     exit(-1);
   }
   if (processNum > 10){
     efprintf(stdout, "The number of processes to create must be between 1 and 10 (inclusive)!\n");
     exit(-1);
   }
  // data_t dat;
  data_t dat [processNum];


  x = atoi(argv[2]);
  int num = atoi(argv[3]);

  int pointing;
  int *nump = &pointing;

  *nump = atoi(argv[4]);
 

  // and now we notify the user we are creating threads...
  efprintf(stdout,"%s %d %s\n", "The program is creating", processNum, "thread(s).");
  efprintf(stdout, "%s %d\n", "Current value of X:", x);
  efprintf(stdout, "%s %d\n", "Current value of num:", num);
  efprintf(stdout, "%s %d\n", "Current value of nump:", *nump);
  efprintf(stdout, "\n");

  pthread_t thr[processNum]; 
  int threadNum = 1;

  for (int i = 0; i < processNum; i++){

    dat[i].num = &num;
    dat[i].nump = nump;

    dat[i].threadNum = threadNum;
    if (pthread_create(&thr[i], NULL, &routine,&dat[i]) != 0){
      perror("pthread create");
    }
     threadNum++;
  }

  for (int i = 0; i < processNum; i++){
    if (pthread_join(thr[i], NULL) != 0){
      perror("pthread join");
    }
  }


  // we made it this far. Hooray. Notify the user the processes are over
  efprintf(stdout, "The program has completed processing.\n");
  //pthread_exit(&thr);

} // END of main()
