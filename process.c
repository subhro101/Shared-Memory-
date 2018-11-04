/*******************************************************************************************************
Name :-Subhrajyoti Pradhan
Professor :- Dr. Korzhova 
UID :- U79333962
Project Name :- Sempahore
File Name :- process.c
*******************************************************************************************************/
/* Disclaimer - In this code, I have occassionally used code snippets from the examples given 
during class by Dr. Korzhova
I hva also taken excerpts from books
I have tried to mention as much of the snippts I have used wherevefr I can 
Other works are purely my onw
*/

/*
The only thing which I did differently in this program was change the if statement
inside the loops to run till 1100000
so that even though the processes run only till there assigned
iterations
they can run until the total iterations complete
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include  <sys/sem.h>
#define SEMKEY ((key_t) 400L)
//We will now define the number of semaphores
#define NSEMS 2
// We shall nowchange the key number 
#define SHMKEY ((key_t) 1497)

//We now use the code snippet provided by Dr. Korzhova in Lecture 4 
typedef struct
{
  int value;
} shared_mem;

shared_mem *total;

//We shall now declare some structs 
int SemID1, SemID2;

typedef union
{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
} semunion;

static struct sembuf Op1 = { 0, -1, 0 };
static struct sembuf Ov2 = { 0, 1, 0 };

struct sembuf *A = &Op1;
struct sembuf *B = &Ov2;

//defining function for Pop
int
poping ()
{
  int STAT;
  STAT = semop (SemID1, A, 1);
  return STAT;
}

int
voping ()
{
  int STAT;
  STAT = semop (SemID1, B, 1);
  return STAT;
}




//will increment the variable "total" by X1 for 100,000 times  

void
PRS1 ()
{
  int k = 0;

  while (k < 100000)
    {


      poping ();
      if (total->value < 1100000)
	{
	  total->value = total->value + 1;
	}

      voping ();
      k++;

    }


  printf ("From Pocess1 total = %d\n", total->value);

}


//will increment the variable "total" by X1 for 200,000 times  

void
PRS2 ()
{
  int k = 0;

  while (k < 200000)
    {


      poping ();
      if (total->value < 1100000)
	{
	  total->value = total->value + 1;
	}

      voping ();
      k++;
    }

  printf ("From Process2 total = %d\n", total->value);

}

//will increment the variable "total" by X1 for 300,000 times  
void
PRS3 ()
{
  int k = 0;

  while (k < 300000)
    {


      poping ();
      if (total->value < 1100000)
	{
	  total->value = total->value + 1;
	}

      voping ();
      k++;
    }

  printf ("From Process3 total = %d\n", total->value);

}


//will increment the variable "total" by X1 for 500,000 times  
void
PRS4 ()
{
  int k = 0;

  while (k < 500000)
    {


      poping ();
      if (total->value < 1100000)
	{
	  total->value = total->value + 1;
	}

      voping ();
      k++;
    }

  printf ("From Process4 total = %d\n", total->value);

}

//now we write the main
int
main ()
{
  //declaring variabels 
  int shmid;
  int pid1;
  int pid2;
  int pid3;
  int pid4;
  int ID;
  int STAT;


  char *shmadd;
  shmadd = (char *) 0;

  //declaring semaphores
  int semnum = 0;
  int value, value1;
  semunion semctl_arg;
  semctl_arg.val = 1;

  //we shall now create thensempahores 
  SemID1 = semget (SEMKEY, NSEMS, IPC_CREAT | 0666);
  if (SemID1 < 0)
    printf ("now creating semaphore");

  SemID2 = semget (SEMKEY, NSEMS, IPC_CREAT | 0666);
  if (SemID2 < 0)
    printf ("now creating semaphdore");

  // We shall initialize the semaphores 
  value1 = semctl (SemID1, semnum, SETVAL, semctl_arg);

  value = semctl (SemID1, semnum, GETVAL, semctl_arg);
  if (value < 1)
    printf ("Eror detected in SETVAL");




// We shall now create and connect to a shared memory segmentt*/

  if ((shmid = shmget (SHMKEY, sizeof (int), IPC_CREAT | 0666)) < 0)
    {
      perror ("shmget");
      exit (1);
    }


  if ((total = (shared_mem *) shmat (shmid, shmadd, 0)) == (shared_mem *) - 1)
    {
      perror ("shmat");
      exit (0);
    }


  total->value = 0;

//We now follow the prompt given in the slide which creates processes 

  if ((pid1 = fork ()) == 0)
    PRS1 ();

  if ((pid1 != 0) && (pid2 = fork ()) == 0)
    PRS2 ();

  if ((pid1 != 0) && (pid2 != 0) && (pid3 = fork ()) == 0)
    PRS3 ();

  if ((pid1 != 0) && (pid2 != 0) && (pid3 != 0) && (pid4 = fork ()) == 0)
    PRS4 ();


  waitpid (pid1, NULL, 0);
  waitpid (pid2, NULL, 0);
  waitpid (pid3, NULL, 0);
  waitpid (pid4, NULL, 0);

  if ((pid1 != 0) && (pid2 != 0) && (pid3 != 0) && (pid4 != 0))  // That is all the processses have ended according to the requirements of the code 
    {
      waitpid (pid1);		//only triggers after process 1 is complete
      printf ("Child with ID %d has just exited.\n", pid1);

      waitpid (pid2);		//only exits after process 2 is complete
      printf ("Child with ID %d has just exited.\n", pid2);

      waitpid (pid3);		//only exits after process 3 is complete
      printf ("Child with ID %d has just exited.\n", pid3);

      waitpid (pid4);		//only exits after process 4 is complete 
      printf ("Child with ID %d has just exited.\n", pid4);

      if ((shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)
	{
	  perror ("shmctl");
	  exit (-1);
	}

      printf ("\t\t  End of Program\n");

      //We will now be deloc the semphore
      semctl_arg.val = 0;
      STAT = semctl (SemID1, 0, IPC_RMID, semctl_arg);
      if (STAT < 0)
	printf ("We found an error.\n");
    }

}

/***************************************************************END OF PROGRAM***************************************************************/


