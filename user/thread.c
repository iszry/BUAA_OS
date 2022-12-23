#include "lib.h"
/*
typedef struct tcb{
    int* tcb_pc;
    int tcb_pid;
    int tcb_status;
}tcb;
#define PIDNUM 2
#define THREAD_RUNNABLE 1
#define THREAD_UNRUNNABLE 2
#define THREAD_NULL 0
*/
static tcb tcbs[2][PIDNUM];
static int tcbPoint=0;
extern tcb* curTcb=NULL;
static tcb* tmpTcb=NULL;
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg){
    int i;
    for(i=1;i<PIDNUM;i++){
        if(tcbs[tcbPoint][i].tcb_status==THREAD_NULL&&tcbs[1-tcbPoint][i].tcb_status==THREAD_NULL){
            tcbs[tcbPoint][i].tcb_pid=i;
            tcbs[tcbPoint][i].tcb_pc=start_routine;
            tcbs[tcbPoint][i].tcb_status=THREAD_WAITING;
            tcbs[tcbPoint][i].tcb_paras=arg;
            tcbs[tcbPoint][i].tcb_join=0;
            tcbs[1-tcbPoint][i].tcb_pid=i;
            tcbs[1-tcbPoint][i].tcb_pc=start_routine;
            tcbs[1-tcbPoint][i].tcb_status=THREAD_UNRUNNABLE;
            tcbs[1-tcbPoint][i].tcb_paras=arg;
            tcbs[1-tcbPoint][i].tcb_join=0;
            writef("Thread with ID %d has been created\n", i);
            return i;
        }
    }
    return -1;
}
void pthread_exit(void *retval){
    u_int pid;
    if(retval==NULL){
        pid=curTcb->tcb_pid;
    }else{
        pid=*((u_int*)retval);
    }
    tcbs[tcbPoint][pid].tcb_status=THREAD_NULL;
    writef("Thread with ID %d exits\n", pid);
}
int pthread_join(pthread_t id, void **retval){
    curTcb->tcb_join=id;
    curTcb->tcb_status=THREAD_UNRUNNABLE;
    return 0;
}
int pthread_cancel(pthread_t id){
    if(tcbs[tcbPoint][id].tcb_status!=THREAD_NULL){
        tcbs[tcbPoint][id].tcb_status=THREAD_NULL;
        writef("Thread with ID %d has been cancel\n", id);
        return id;
    }
    writef("Thread with ID %d Not Found!\n", id);
    return -1;
}
void tcbCopy(u_int pid){
    tcbs[1-tcbPoint][pid].tcb_join=tcbs[tcbPoint][pid].tcb_join;
    tcbs[1-tcbPoint][pid].tcb_paras=tcbs[tcbPoint][pid].tcb_paras;
    tcbs[1-tcbPoint][pid].tcb_pc=tcbs[tcbPoint][pid].tcb_pc;
    tcbs[1-tcbPoint][pid].tcb_pid=tcbs[tcbPoint][pid].tcb_pid;
    tcbs[1-tcbPoint][pid].tcb_status=THREAD_WAITING;
    tcbs[tcbPoint][pid].tcb_status=THREAD_NULL;
}
void pthread_run(){
    curTcb->tcb_status=THREAD_RUNNABLE;
        //writef("in function pthread_run\n");
    (void *)(*(curTcb->tcb_pc))(curTcb->tcb_paras);
        //writef("in function pthread_run\n");
    curTcb->tcb_status=THREAD_WAITING;
    tcbCopy(curTcb->tcb_pid);
}
void sched_pthread(){
    int i=0;
    for(i=1;i<PIDNUM;i++){
        if(tcbs[tcbPoint][i].tcb_status==THREAD_WAITING||tcbs[tcbPoint][i].tcb_status==THREAD_RUNNABLE||
                        (tcbs[tcbPoint][i].tcb_join!=0&&tcbs[tcbPoint][i].tcb_status==THREAD_UNRUNNABLE&&tcbs[tcbPoint][tcbs[tcbPoint][i].tcb_join].tcb_status==THREAD_NULL)){
            tmpTcb=curTcb;       
            //tcbs[tcbPoint][i].tcb_status=THREAD_NULL;
            //tcbCopy(tmpTcb->tcb_pid);       
            curTcb=&(tcbs[tcbPoint][i]);
            pthread_run();
            curTcb=tmpTcb;
            return;
        }
    }
    tcbPoint=1-tcbPoint;
    for(i=1;i<PIDNUM;i++){
        if(tcbs[tcbPoint][i].tcb_status==THREAD_WAITING||tcbs[tcbPoint][i].tcb_status==THREAD_RUNNABLE||
                        (tcbs[tcbPoint][i].tcb_join!=0&&tcbs[tcbPoint][i].tcb_status==THREAD_UNRUNNABLE&&tcbs[tcbPoint][tcbs[tcbPoint][i].tcb_join].tcb_status==THREAD_NULL)){
            tmpTcb=curTcb;       
            //tcbs[tcbPoint][i].tcb_status=THREAD_NULL;
            //tcbCopy(tmpTcb->tcb_pid);       
            curTcb=&(tcbs[tcbPoint][i]);
            pthread_run();
            curTcb=tmpTcb;
            return;
        }
    }
}