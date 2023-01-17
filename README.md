# OSA3
Concurrency, IPC, and a Kernel Module

Introduction
Includes experiments with the Dining Philosophers' problem, some basic Interprocess Communication, and a kernel module which prints the process information to the kernel information log.

Question 1
I'm utilising Djikstra's solution to prevent deadlocks for all variations of the first question.
All variations contain the following attributes:
  • int philosopher[5]:
    o An integer array of philosophers who can be thinking or eating [0 or 1].
    o 1a1 also [0, 1, 2] as thinking, hungry and eating
  • sem_t semaphores[5]:
      A semaphore array of forks [1a1 doesn’t have this].
  • int l3[3]:
    o An integer array of last three philosophers who ate. L3 = Last three.
    o Part of the starvation reduction system.
  • Int forks[5]:
    o Exclusive to 1a1.
    o Contains index of philosopher last using the fork.
  • sem_t bowls[2]:
    o Exclusive to 1b
    o Represents the soup bowls.
Apart from 1a1, all variations contain the following functions:
  • pickFork(int i):
    o locks fork i with sem_wait.
    o For 1a1 [int pickFork(int i, int philNum)], if fork is unused or philosopher last using the fork has returned to thinking, or if the philosopher last using the fork was the current one, marks phil[philNum] as hungry and labels fork[i] to be in use by philNum.
  • putFork(int i):
    o unlocks fork I with sem_post.
    o Doesn’t do anything for 1a1. The less the possibility of an overwrite, the better.
  • eat(int i):
    o marks philosopher[i] to be eating.
    o Adds i to next index of l3.
  • int l3Check(int j):
    o checks if more than 1 of the last two eatings were of j
  • grabSoup(int i):
    o if bowl0 was unused at time of sem_getvalue, will attempt to lock bowl0
    o if bowl0 was in use but b1 unused, will attempt to lock bowl1
    o if both used, even philosophers will lock 0, and odd will lock 1
    o returns currently occupied bowl
  • void returnSoup(int bowlOccupied):
    o Releases the bowl occupied.
  • void *philosphise(void *_i):
    o if l3Check(i)==-1, sleeps the thread for philosopher i
    o otherwise, picks lower indexed fork first, followed by larger numbered fork [1a1 calls continue here if pickfork(i) returns -1]
    o [1b exclusive] grabs soup():
    o Then, it calls eat(), and sleeps for a second
    o Finally, [releases soup for 1b and then] puts down the forks in the order they were picked.
    o Returns the philosopher to thinking.
    o Runs till user interrupt.

Question 2
All implementations have a constructor function, which generates random strings of 4 characters
and stores them into a character pointer array (strings).
P1 for all implementations is divided in two parts:
  • IPC generation: Happens before the while loop. Generates the socket/FIFO/SHM file, and gets a filedescriptor/pointer for the same.
  • The loop: This is further divided in two parts:
    1. String generation and sending: Generates the string to be sent and writes it into the datastream.
    2. Response reception: Waits for a response from P2 and prints it.
P2 for all implementations is also divided in two parts:
  • IPC connection: Happens before the loop. Gets a filedescriptor/pointer for the IPC.
  • The loop: Reads the datastream, processes it, and writes back a response with the highest index.

Question 3
Takes pid of process as an parameter, and prints its PID, User ID, Group ID and Command path in
the kernel information log. Uses find_get_pid(pidInt) to convert integer pid into the
corresponding pid structure, and pid_task(pidStruct, PIDTYPE_PID) to get the corresponding task
structure. This task structure is used to get all relevant information.
