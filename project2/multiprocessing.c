// Eve Collier
// project 2: multiprocessing
// csc 328 fall 2023

// well here we go 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>

int x; // our global variable named "x"
// int processNum; // another global to keep track of the numof processes to create


// childProcess will add 50 to x and incrment num and nump. Essentially it performs the functions of the childProcess
void childProcess(int *x, int *num, int *nump){
  *x = *x + 50;
  *num = *num + 1;
  *nump = *nump + 1;


} // END of childProcess()

// printSeqProcess prints the information indicating the user that the sequential processes are occuring
void printSeqProcess(bool flag, FILE *fp, int processNum, int x, int num, int *nump){
    fprintf(fp, "The program is creating sequential processes...\n");
    fprintf(fp, "%s %d %s\n", "The program will create", processNum, "processes.");
    fprintf(fp, "%s %d\n", "The current value of X is:", x);
    fprintf(fp, "%s %d\n", "The current value of num is:", num);
    fprintf(fp, "%s %d\n", "The current value of nump is:", *nump);
    fprintf(fp, "\n");
    fflush(fp);
} // END of printSeqProcess()

// printConProcess prints the information indicating the user that the concurrent processes are occuring
void printConProcess(bool flag, FILE *fp, int processNum, int x, int num, int *nump){
    fprintf(fp, "The program is now creating concurrent processes...\n");
    fprintf(fp, "%s %d %s\n", "The program will create", processNum, "processes.");
    fprintf(fp, "%s %d\n", "The current value of X is:", x);
    fprintf(fp, "%s %d\n", "The current value of num is:", num);
    fprintf(fp, "%s %d\n", "The current value of nump is:", *nump);
    fprintf(fp, "\n");
    fflush(fp);
} // END of printConProcess()


// sequentialProcess performs the operations that the sequential process needs to do
void sequentialProcess(int processNum, bool flag, FILE *fp, int x, int num, int *nump){
  int status;
  int childCounter = 0;
  pid_t p;
 

  for (int i = 1; i <= processNum; i++){
    p = fork();
    switch(p){
      case -1:
         // error stuff
         perror("fork");
         exit(-1);

      case 0:
        // child stuff
        fprintf(fp, "%s %d %s %d%s %s %d %s %d %s %d\n", "BEFORE increment: child #", i, "with PID of", getpid(),":", "value of X =", x, "value of num =", num, "value of nump =", *nump);
        childProcess(&x, &num, nump);
        fprintf(fp, "%s %d %s %d%s %s %d %s %d %s %d\n", "AFTER increment: child #", i, "with PID of", getpid(),":", "value of X =", x, "value of num =", num, "value of nump =", *nump);
        exit(-1);
        break;


      default:
        // parent stuff
        // int status;
        wait(&status);
        if (WIFEXITED(status)){
          // printf("%d\n", WEXITSTATUS(status));
        }
        fprintf(fp,"The child process has been completed.\n");
        fflush(fp);
    }

   }


} // END of sequentialProcess

// concurrentProcess performs the operations that the concurrent process needs to do
void concurrentProcess(int processNum, bool flag, FILE *fp, int x, int num, int *nump){
  // int status;
  int childCounter = 0;

  pid_t p;


  for (int j = 1; j <= processNum; j++){
    p = fork();
    switch(p){
	case -1:
         // error stuff
         perror("fork");
         exit(-1);

	case 0:
        // child stuff
        fprintf(fp, "%s %d %s %d%s %s %d %s %d %s %d\n", "BEFORE increment: child #", childCounter, "with PID of", getpid(),":", "value of X =", x, "value of num =", num, "value of nump =", *nump);
        childProcess(&x, &num, nump);
        fprintf(fp, "%s %d %s %d%s %s %d %s %d %s %d\n", "AFTER increment: child #", childCounter, "with PID of", getpid(),":", "value of X =", x, "value of num =", num, "value of nump =", *nump);
        exit(j); 
        break;  


	default: // parent stuff
        childCounter++;
  
    }


  }
  // parent stuff
  for (int k = 0; k < childCounter; k++){
    int status;
    wait(&status);
    if (WIFEXITED(status)){
      // printf("%d\n", WEXITSTATUS(status));
    }
    fprintf(fp,"The child process has been completed.\n");
    // fflush(fp);
  }

} // END of concurrentProcess

// main utilizes all of the helper functions needed in order to execute the program
int main(int argc, char** argv){
  FILE *fp;
  int processNum;
  int num; // our int variable named "num"
  int pointing; // so that we can assign values to nump
  int *nump = &pointing; // annnd the integer pointer variable 
  bool flag = false; // will help determine whether or not user provided a file to write to 
  int childCounter = 0;  
  int status;
  
  if (5  > argc){
    printf("At least four command line arguments are required!\n");
    exit(-1);
  }

  if (argc > 6){
    printf("Five is the maximum number of command line arguments you're allowed to provide. Calm down.\n");
    exit(-1);
  }
  

  processNum = atoi(argv[1]);
  // printf("%d\n", processNum);

  // processNum is only allowed to be between 1-10, with that said...
  if (processNum < 1){
    printf("The number of processes to create must be between 1 and 10 (inclusive)!\n");
    exit(-1);
  }
  if (processNum > 10){
    printf("The number of processes to create must be between 1 and 10 (inclusive)!\n");
    exit(-1);
  }

  x = atoi(argv[2]);

  num = atoi(argv[3]);

  *nump = atoi(argv[4]);
  // printf("%d\n", *nump);

  if (argc == 6){
    // user provided filename to write to
    flag = true;
    fp = fopen(argv[5], "w");
    if (fp == NULL){
      perror("fopen");
      exit(-1);
    }
 
  }
  else{ // user did NOT provide a filename, so...
    fp = stdout;
  }

 
  printSeqProcess(flag, fp, processNum, x, num, nump);
  sequentialProcess(processNum, flag, fp, x, num, nump);

  // that's one process. Hooray.  Reset all values to their original value 
  x = atoi(argv[2]);
  num = atoi(argv[3]);
  *nump = atoi(argv[4]);
  childCounter = 0;

  // and now the fun begins again, this time, concurrently
  printConProcess(flag, fp, processNum, x, num, nump); 
  concurrentProcess(processNum, flag, fp, x, num, nump);
  
 
  if (flag == true){
    fclose(fp);
    // check fclose 
    if (fclose == NULL){
      perror("There was an issue closing the file.");
      exit(-1);
    }
  }

} // END of main()
