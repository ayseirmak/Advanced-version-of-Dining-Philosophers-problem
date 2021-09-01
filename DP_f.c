#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <semaphore.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define NUMBER      8
#define TABLE_FOOD  2000
#define PRICE_RICE  25.8 //(FOR 2 KG )
struct table {
     int priority[8];
    int eating[8];
    int table_no;
    double bill;
    int tazelenme;
    int table_food;
    enum {THINKING, HUNGRY, EATING} state[8];
    int thread_id[8];
    pthread_cond_t      cond_vars[8];
    pthread_mutex_t     mutex_lock;
};
struct send{
                struct table* tbl_send;
                int thread_of;
            };

sem_t empty_table;
void *full_table(void *param);

void eating(int eating_time, struct send* rcv)
{
    printf("eating for %d seconds\n",eating_time);
    rcv->tbl_send->eating[rcv->thread_of]++;
    if(rcv->tbl_send->table_food==0){
        rcv->tbl_send->table_food=TABLE_FOOD;
        rcv->tbl_send->tazelenme ++;
    }
    rcv->tbl_send->table_food=rcv->tbl_send->table_food-100;
    sleep(eating_time);
};
int left_neighbor(int number)
{
    if (number == 0)
        return NUMBER - 1;
    else
        return number - 1;
}
 

int right_neighbor(int number)
{
    if (number==NUMBER - 1)
        return 0;
    else
        return number + 1;
}
void test(struct send* rcv)
{   int i = rcv->thread_of;
    printf( "you ara %d.phil.You are in test \n",i);
    if ( (rcv->tbl_send->state[left_neighbor(i)] == THINKING||(rcv->tbl_send->state[left_neighbor(i)]==HUNGRY && rcv->tbl_send->priority[left_neighbor(i)]<=rcv->tbl_send->priority[i])) && (rcv->tbl_send->state[i] == HUNGRY) && (rcv->tbl_send->state[right_neighbor(i)] == THINKING||(rcv->tbl_send->state[right_neighbor(i)]==HUNGRY && rcv->tbl_send->priority[right_neighbor(i)]<=rcv->tbl_send->priority[i]))) {
        rcv->tbl_send->state[i] = EATING;
        printf("in_test %d. phil state turn HUNGRY to EATING\n",i);
        pthread_cond_signal(&rcv->tbl_send->cond_vars[i]);
        }
    else{
        if((rcv->tbl_send->state[left_neighbor(i)]==HUNGRY && rcv->tbl_send->priority[left_neighbor(i)]>rcv->tbl_send->priority[i])&& (rcv->tbl_send->state[right_neighbor(i)]==THINKING||(rcv->tbl_send->state[right_neighbor(i)]==HUNGRY && rcv->tbl_send->priority[left_neighbor(i)]>rcv->tbl_send->priority[right_neighbor(i)]))){
            printf("in_test %d. phil left neighbour turn HUNGRY to EATING %d\n",i,left_neighbor(i));
            pthread_cond_signal(&rcv->tbl_send->cond_vars[left_neighbor(i)]);
        }
        if((rcv->tbl_send->state[right_neighbor(i)]==HUNGRY && rcv->tbl_send->priority[right_neighbor(i)]>rcv->tbl_send->priority[i])&& (rcv->tbl_send->state[left_neighbor(i)]==THINKING||(rcv->tbl_send->state[left_neighbor(i)]==HUNGRY && rcv->tbl_send->priority[right_neighbor(i)]>rcv->tbl_send->priority[left_neighbor(i)])))
        printf("in_test %d. phil right neighbour turn HUNGRY to EATING %d\n",i,right_neighbor(i));;
        pthread_cond_signal(&rcv->tbl_send->cond_vars[right_neighbor(i)]);
        }
        
    
}


void pickup_forks(struct send* rcv)
{   int number=rcv->thread_of;
    pthread_mutex_lock(&rcv->tbl_send->mutex_lock);
    printf("mutex_locked for %d. phil\n",rcv->thread_of);
    rcv->tbl_send->state[number] = HUNGRY;
    test(rcv);
 
    while (rcv->tbl_send->state[number] != EATING) {
        pthread_cond_wait(&rcv->tbl_send->cond_vars[number], &rcv->tbl_send->mutex_lock);
    }
 
    pthread_mutex_unlock(&rcv->tbl_send->mutex_lock);
}
 
 
void return_forks(struct send* rcv)
{
    int number=rcv->thread_of;
    pthread_mutex_lock(&rcv->tbl_send->mutex_lock);
    rcv->tbl_send->state[number] = THINKING;
    rcv->thread_of=left_neighbor(number);
    test(rcv);
    rcv->thread_of=right_neighbor(number);
    test(rcv);
    pthread_mutex_unlock(&rcv->tbl_send->mutex_lock);
}
void *philosopher(void *param)
{
    struct send *rcv;   
    rcv = (struct send*) param;
    int number= rcv->thread_of;
    printf("GroupNo: %d\n",rcv->tbl_send->table_no);
    printf("PhilNo: %d\n",number);
    int eating_time;
    int thinking_time;
    srand((unsigned int)time(NULL));
    thinking_time =((random() % 5) + 1);

    printf("thinking for %d seconds\n", thinking_time);
    sleep(thinking_time);
    printf("pick_up_fork %d.phil\n",rcv->thread_of);
    pickup_forks(rcv);

    printf("Philosopher %d is eating\n",number);

    eating_time = ((random() % 5) + 1);
    eating(eating_time,rcv);

    printf("Philosopher %d is thinking\n",number);
    return_forks(rcv);
        
}

int main(){
    int group_number;
    sem_init(&empty_table,0,10);
    printf("Enter the number of groups:\n");
	scanf("%d",&group_number);
    int zeros[8]={0};
    struct table groups[group_number];
    for(int i=0;i<group_number;i++){
        memcpy(groups[i].eating,zeros,sizeof(groups[i].eating));
        memcpy(groups[i].priority,zeros,sizeof(groups[i].priority));
        groups[i].bill=0;
        groups[i].table_food=TABLE_FOOD;
        groups[i].tazelenme=0;
        groups[i].table_no=i;
    } 
    double bill_f[group_number];
    pthread_t tid[group_number];
    for(int a=0;a<group_number;a++){
        pthread_create(&tid[a],NULL,&full_table,&groups[a]);}
    for(int k=0;k<group_number;k++){
        pthread_join(tid[k],NULL);
        bill_f[k]=groups[k].bill;
        printf("%d. grup is comming\n",k);}
    double final_bill=0;
    for(int k=0;k<group_number;k++){
        final_bill=final_bill+bill_f[k];
    }
    printf("Final Bill : %lf\n",final_bill);

}

void *full_table(void * param){
    
    sem_wait(&empty_table);
    struct table *tbl;   
    tbl = (struct table*) param;
    printf("%d. table is preparing\n",tbl->table_no);
        tbl->bill=99.90;
        pthread_t tid[8];
        for (int i = 0; i < 8; i++) {
            tbl->state[i] = THINKING;
            tbl->thread_id[i]= i;
            pthread_cond_init(&(tbl->cond_vars[i]),NULL);
        }
        pthread_mutex_init(&tbl->mutex_lock, NULL);
        struct send sd[8]; 
        for(int s=0;s<8;s++){
            sd[s].tbl_send=tbl;
            sd[s].thread_of=s;
        }        
        struct sched_param pr;
        srand((unsigned int)time(NULL));
        pthread_attr_t attr; 
        for (int a = 0; a < 8; a++) {
            printf("%d. philosopher sit\n",a);
            pthread_attr_init(&attr);
            pthread_attr_getschedparam(&attr, &pr);
            int prio=(rand() % 5) + 1;
            pr.sched_priority = prio;
            tbl->priority[a]=prio;
            pthread_attr_setschedparam (&attr, &pr);
            printf("Priority of %d. philosopher in %d. group is: %d \n",a,tbl->table_no,tbl->priority[a]);
            pthread_create(&tid[a], &attr, &philosopher, &sd[a] );
        }
        for (int k = 0; k < 8; k++){
            pthread_join(tid[k],NULL);
            printf("%d. phil return\n",k);
            }
        printf("Tazelenme of group%d is :%d\n",tbl->table_no,tbl->tazelenme);
        tbl->bill=tbl->bill+(tbl->tazelenme*19.90+(tbl->tazelenme+1)*PRICE_RICE);
        //printf("everybody come");
    sem_post(&empty_table);
   
}