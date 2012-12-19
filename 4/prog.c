#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int rez1 = 0;
int rez2 = 0;
int mutex = 0;
int numofthread = 2;

void* function(void* argument)
{
    __asm("pusha");
    for (int i = 0; i <= 500; ++i)
    {
    __asm("xorl %ecx, %ecx");
    __asm("incl %ecx");
    __asm("wait:");
    __asm("mov %0, %%ebx" : : "r" (&mutex));
    __asm("xorl %eax,%eax");
    __asm("lock; cmpxchgl %ecx, (%ebx)");
    __asm("jnz wait");
    rez1 = rez1 + 1;
    __asm("movl $0, (%0)" : : "r" (&mutex));
    }
    __asm("popa");
}

int main()
{
    pthread_t thread[numofthread];
    
    rez1 = 0;
	rez2 = 0;
    pthread_create(&thread[0], NULL, function, NULL);
    pthread_create(&thread[1], NULL, function, NULL);
       
    pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);
    
	rez2 = 500 * numofthread;	
    if (rez1 < rez2){ printf("Something wrong\n");}
	else {printf("Threads Synchronized!\n");}
}