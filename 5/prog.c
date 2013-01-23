#include <ucontext.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>


#define KBTHREAD_STACKSIZE 4096 
#define NUMCONTEXTS 5             
#define INTERVAL 100

typedef struct{
	ucontext_t context;
	int isTerminated;
	int priority;
	time_t goodMorning;
	int time_to_sleep;
} context_extension;

int kbthreads_live;
static sigset_t set;                       
static ucontext_t scheduler_context;
void *signal_stack;
context_extension contexts[NUMCONTEXTS];
ucontext_t * current_context;
struct itimerval it;
int isDebug = 0;
int context_index = -1;

void sheduler_sleep(int);

void scheduler()
{
sheduler_sleep(0);
	while(kbthreads_live != 0){
	int k;
	time_t my_time = time(NULL);
	for (k = NUMCONTEXTS/2; k<NUMCONTEXTS; k++){
		if (contexts[k].isTerminated == 1)	continue;
		if (my_time > contexts[k].goodMorning)
			{
				context_index = k;
				current_context = &contexts[k].context;	
				printf("Start thread %d\n",k);
				sheduler_sleep(1);
				setcontext(current_context);
			}
	}
	for (k = 0 ; k < NUMCONTEXTS/2;k++){
		if (contexts[k].isTerminated == 1) continue;
		if (my_time > contexts[k].goodMorning)
		{
			context_index = k;
			current_context = &contexts[k].context;
			printf("Start thread %d\n",k);
			sheduler_sleep(1);
			setcontext(current_context);
		}
	}
	poll(NULL,0,50);
	}
}

void kbsthread_sleep(int ms, int index){
	time_t wut = time(NULL);
	contexts[index].goodMorning = wut + ms;
}

void timer_interrupt(int j, siginfo_t *si, void *old_context)
{
	printf("pause thread %d for %d seconds \n",context_index,contexts[context_index].time_to_sleep);
	contexts[context_index].context = *( ucontext_t *) old_context;
	kbsthread_sleep(contexts[context_index].time_to_sleep,context_index); 
	swapcontext(current_context , &scheduler_context);
}

void init_context(ucontext_t * signal_cont){
	getcontext(signal_cont);
	signal_cont->uc_stack.ss_sp = signal_stack;
	signal_cont->uc_stack.ss_size = KBTHREAD_STACKSIZE;
	signal_cont->uc_stack.ss_flags = 0;
	sigemptyset(&signal_cont->uc_sigmask);
	makecontext(signal_cont , scheduler , 0);
}

void setup_signals(void)
{
    struct sigaction action;
    action.sa_sigaction = timer_interrupt;
    int res = sigemptyset(&action.sa_mask);
    if (res < 0){ printf("OOps! Something wrong with it!\n"); exit(1);}
	action.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&set);
	sigaddset(&set, SIGALRM);
	if(sigaction(SIGALRM, &action, NULL) != 0) { printf("OOps! Something wrong with it!\n"); exit(1); }
}

void kbthread_low()
{
	int j;
	for(j = 0; j < 15 ; j++){ printf(" %d ",j); poll(NULL,0,100);}
    contexts[context_index].isTerminated = 1;
	kbthreads_live--;
	printf("Thread %d stop\n",context_index);
}

void kbthread_high(){
	int j;
	for(j = 97 ; j < 122 ; j++ ){ printf(" %c ",j);	poll(NULL,0,100); }
	contexts[context_index].isTerminated = 1;
	kbthreads_live--;
	printf("Thread %d stop\n",context_index);
}

void kbthread_create(ucontext_t *uc,  void *function)
{
    void * stack;
    getcontext(uc);
    stack = malloc(KBTHREAD_STACKSIZE);
    if (stack == NULL) {
        printf("Can't allocate memory\n");
        exit(1);
    }
    uc->uc_stack.ss_sp = stack;
    uc->uc_stack.ss_size = KBTHREAD_STACKSIZE;
    uc->uc_stack.ss_flags = 0;
    uc->uc_link = &scheduler_context;
    if (sigemptyset(&uc->uc_sigmask) < 0){
      printf("Can't initialize the signal set\n");
      exit(1);
    }
    makecontext(uc, function, 0 );
}


void sheduler_sleep(int sec){
	it.it_interval.tv_sec = sec;
	it.it_value.tv_sec = sec;
	if (setitimer(ITIMER_REAL , &it , NULL) < 0){ printf("Can't set timer"); exit(1); }
}

int main(int argc , char* argv[])
{
	kbthreads_live = NUMCONTEXTS;
	
	int i;
    for( i =  0; i < NUMCONTEXTS/2; i++)
	{
       	kbthread_create(&contexts[i].context, kbthread_low);
		contexts[i].isTerminated = 0;
		contexts[i].priority = 1;
		contexts[i].goodMorning = time(NULL);
		contexts[i].time_to_sleep = 4; 
	}
	
	for (i = NUMCONTEXTS/2 ; i< NUMCONTEXTS ; i++)
	{
		kbthread_create(&contexts[i].context,kbthread_high);
		contexts[i].isTerminated = 0;
		contexts[i].priority = 2;
		contexts[i].goodMorning = time(NULL);
		contexts[i].time_to_sleep = 5;
	}
	setup_signals();
	init_context(&scheduler_context);
	current_context = &scheduler_context;
	setcontext(current_context);
	return 0;
}
