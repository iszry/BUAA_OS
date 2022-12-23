#include "lib.h"
//make clean&&make&&/OSLAB/gxemul -E testmips -C R3000 -M 64 ./gxemul/vmlinux

void *consumer(void *arg){
    if(x>0){
        x--;
        writef("%s: I get an x, the number of x is %d now\n",arg, x);
    }else{
        writef("%s: the number of x is 0 now, I can't get any x.\n",arg);
        pthread_join(1, NULL);
    }
    return NULL;
}
int x=0;
void *producer(void *arg){
    // thread code block start
    x++;
    writef("%s starts.\n",arg, x);
    return NULL;
    // thread code block end
}
void umain(){
    sem_init(&x,0,0);
    pthread_t thread1,thread2,thread3;
    char* name1="producer";
    char* name2="consumerA";
    char* name3="consumerB";
    int ret=pthread_create(&thread1,NULL,producer,name1);
    if(ret<0){
        writef("^^^^^producer create failed^^^^^\n");
    }
    ret=pthread_create(&thread2,NULL,consumer,name2);
    if(ret<0){
        writef("^^^^^consumerA create failed^^^^^\n");
    }
    ret=pthread_create(&thread3,NULL,consumer,name3);
    if(ret<0){
        writef("^^^^^consumerB create failed^^^^^\n");
    }
    int cnt;
    for(cnt=0;cnt<7;cnt++){
        sched_pthread();
		if(cnt==3)pthread_cancel(thread2);
    }
	pthread_cancel(thread1);
	pthread_cancel(thread3);

}

sem_t mutex;
sem_t x;
void *producer(void *arg){
	int value;
    sem_wait(&mutex);
	sem_getvalue(&x,&value);
	sem_post(&x);
	writef("[Thread ID: %d] ",curTcb->tcb_pid);
    writef("%s: I make an x, the number of x"
    " is %d now.\n",arg, value + 1);
    sem_post(&mutex);
    return NULL;
}
void *consumer(void *arg){
    sem_wait(&mutex);
	sem_wait(&x);
	int value;
	sem_getvalue(&x,&value);
	writef("[Thread ID: %d] ",curTcb->tcb_pid);
    writef("%s: I get an x, the number of x"
    " is %d now.\n",arg, value);
    sem_post(&mutex);
    return NULL;
}

