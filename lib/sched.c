#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *
 *
 * Hints:
 *  1. The variable which is for counting should be defined as 'static'.
 *  2. Use variable 'env_sched_list', which is a pointer array.
 *  3. CANNOT use `return` statement!
 */
/*nexercise 3.15 ***/

extern struct Env_list env_sched_list[];
extern struct Env *curenv;
void sched_yield(void)
{
	//env_run(LIST_FIRST(env_sched_list)); 
    static int count = 0; // remaining time slices of current env
    static int point = 0; // current env_sched_list index

	struct Env* e=LIST_FIRST(&env_sched_list[point]);
	struct Env *cur = curenv;
	//printf("\nin sched.c\n");
    //printf("\nin sched.c\n");
    if (count == 0 || cur == NULL || cur->env_status != ENV_RUNNABLE){
        int changePoint = 1;
        while(!LIST_EMPTY(&env_sched_list[point])){
            cur = LIST_FIRST(&env_sched_list[point]);
            if(cur->env_status == ENV_RUNNABLE){
                changePoint = 0;
                break;
            }
            LIST_REMOVE(cur, env_sched_link);
            LIST_INSERT_TAIL(&env_sched_list[!point], cur, env_sched_link);
        }
        if(changePoint){
            point = !point;
            while(!LIST_EMPTY(&env_sched_list[point])){
                cur = LIST_FIRST(&env_sched_list[point]);
                if(cur->env_status == ENV_RUNNABLE){
                    break;
                }
                LIST_REMOVE(cur, env_sched_link);
                LIST_INSERT_TAIL(&env_sched_list[!point], cur, env_sched_link);
            }
        }
    
        count = cur->env_pri;
        LIST_REMOVE(cur, env_sched_link);
        LIST_INSERT_TAIL(&env_sched_list[!point], cur, env_sched_link);
        //struct Env *test=LIST_FIRST(&env_sched_list[point]);
        //printf("\ntest list first env_id:%d status:%d\n",cur->env_id,cur->env_status);
    }
    count--;
    //printf("\ncount now:%d, point now:%d\n",count,point);
    env_run(cur);

    //printf("env_id:%d,  cnt=%d\n",cur->env_id,count);
    //env_run(LIST_FIRST(env_sched_list));    
}
