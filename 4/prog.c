#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int rez = 0;
int mutex = 0;
int numofthread = 2;

void* functionInc(void* argument)
{
    __asm("pusha");
    for (int i = 0; i <= 500; ++i)
    {
	__asm("pusha");
    __asm("xorl %ecx, %ecx");
    __asm("incl %ecx");
    __asm("wait:");
    __asm("mov %0, %%ebx" : : "r" (&mutex));
    __asm("xorl %eax,%eax");
    __asm("lock; cmpxchgl %ecx, (%ebx)");
    __asm("jnz wait");
	__asm("popa");
    rez = rez + 1;
	__asm("pusha");
    __asm("movl $0, (%0)" : : "r" (&mutex));
    }
    __asm("popa");
}

void* functionDec(void* argument)
{
    __asm("pusha");
    for (int i = 0; i <= 500; ++i)
    {
	__asm("pusha");
    __asm("xorl %ecx, %ecx");
    __asm("incl %ecx");
    __asm("wait:");
    __asm("mov %0, %%ebx" : : "r" (&mutex));
    __asm("xorl %eax,%eax");
    __asm("lock; cmpxchgl %ecx, (%ebx)");
    __asm("jnz wait");
	__asm("popa");
    rez = rez - 1;
	__asm("pusha");
    __asm("movl $0, (%0)" : : "r" (&mutex));
    }
    __asm("popa");
}


int main()
{
    pthread_t thread[numofthread];
    
    pthread_create(&thread[0], NULL, functionInc, NULL);
    pthread_create(&thread[1], NULL, functionDec, NULL);
       
    pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);
    
	if (rez1 != 0){ printf("Something wrong\n"); exit(-1)}
	printf("Threads Synchronized!\n");
}