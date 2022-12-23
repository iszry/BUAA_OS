#include "lib.h"

extern tcb* curTcb;
int sem_init(sem_t *sem, int pshared, unsigned int value){
    sem->use=SEM_USE;
    sem->val=value;
    writef("unnamed semaphore has been created successfully.\n");
    return 0;
}
int sem_destroy(sem_t * sem){
    sem->use=SEM_UNUSE;
    sem->val=0;
    writef("unnamed semaphore has been destroyed successfully.\n");
    return 0;
}

int sem_wait(sem_t * sem){
    if(sem->val==0){
        int i,j;
        for(i=0;i<sem->curPos;i++){
            if(sem->waitlist[i]==curTcb->tcb_pid){
                j=i;
                break;
            }
        }
        if(i==sem->curPos){
            sem->waitlist[sem->curPos]=curTcb->tcb_pid;
            j=sem->curPos;
            sem->curPos++;
        }
        curTcb->tcb_status=THREAD_UNRUNNABLE;
        while(sem->val==0){
            sched_pthread();
        }
        sem->val--;
        curTcb->tcb_status=THREAD_WAITING;
        for(i=j;i<sem->curPos-1;i++){
            sem->waitlist[i]=sem->waitlist[i+1];
        }
        sem->curPos--;
    }
    return 0;
}
int sem_trywait(sem_t * sem){
    if(sem->val>0){
        sem->val--;
    }
    return 0;
}
int sem_post(sem_t * sem){
    sem->val++;
    return 0;
}
int sem_getvalue(sem_t *sem, int *sval){
    *sval=sem->val;
    return 0;
}