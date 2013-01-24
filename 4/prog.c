#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


volatile int rez = 0;
volatile int mutex = 0;
volatile int numofthread = 2;

void* functionInc(void* argument)
{
    __asm("pusha");
    for (int i = 0; i <= 500; ++i)
    {
	__asm volatile ("pusha");
    __asm volatile ("xorl %ecx, %ecx");
    __asm volatile ("incl %ecx");
    __asm volatile ("wait:");
    __asm volatile ("mov %0, %%ebx" : : "r" (&mutex));
    __asm volatile ("xorl %eax,%eax");
    __asm volatile ("lock; cmpxchgl %ecx, (%ebx)");
    __asm volatile ("jnz wait");
	__asm volatile ("popa");
    rez = rez + 1;
	__asm volatile ("pusha");
    __asm volatile ("movl $0, (%0)" : : "r" (&mutex));
	__asm volatile ("popa");
    }
    __asm("popa");
}

void* functionDec(void* argument)
{
    __asm("pusha");
    for (int i = 0; i <= 500; ++i)
    {
	__asm volatile ("pusha");
    __asm volatile ("xorl %ecx, %ecx");
    __asm volatile ("incl %ecx");
    __asm volatile ("wait:");
    __asm volatile ("mov %0, %%ebx" : : "r" (&mutex));
    __asm volatile ("xorl %eax,%eax");
    __asm volatile ("lock; cmpxchgl %ecx, (%ebx)");
    __asm volatile ("jnz wait");
	__asm volatile ("popa");
    rez = rez - 1;
	__asm volatile ("pusha");
    __asm volatile ("movl $0, (%0)" : : "r" (&mutex));
    __asm volatile ("popa");
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
    
	if (rez != 0){ printf("Something wrong\n"); exit(-1);}
	printf("Threads Synchronized!\n");
}