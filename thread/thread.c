nclude <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREAD 10

int pin_value = 0;

typedef struct {
  int id,event,type;
} param;

void *poll(void *arg) {
 int i =0;
 param *p=(param *)arg;
 time_t initial_t, current_t;
 //struct tm *ltime;

 time(&initial_t);

 printf("Starting Poll Func\n");     

 while(1) {
   time(&current_t);

   if(pin_value) {
     printf("Time =%3d - Pin is now High\n",(current_t-initial_t));  
     i++;
     if(i>=12) {
       p->type  = 171;
       p->event = 1;
     }
   }
   else {
     printf("Time =%3d - Pin is now Low\n",(current_t-initial_t));     
   }

   sleep(1);

 }

 return (void*)(1);
}

void *drive(void *arg) {
 int i =0;
 param *p=(param *)arg;

 printf("Starting Drive Func\n");     

 sleep(1);
 printf(" Drive High\n");     
 pin_value = 1;

 sleep(10);
 printf(" Drive Low \n");     
 pin_value = 0;

 sleep(5);
 printf(" Drive High \n");     
 pin_value = 1;

 sleep(7);
 printf(" Drive Low \n");     
 pin_value = 0;

 sleep(2);
 return (void*)(1);
}

int main(int argc, char* argv[]) {
 int n,i;
 pthread_t *threads;
 param *p;

/*  if (argc != 2) { */
/*   printf ("Usage: %s n\n",argv[0]); */
/*                 printf ("\twhere n is no. of threads\n"); */
/*   exit(1); */
/*  } */

/*  n=atoi(argv[1]); */

 n = 2;

 if ((n < 1) || (n > MAX_THREAD)) {
  printf ("arg[1] should be 1 - %d.\n",MAX_THREAD);
  exit(1);
 }

 threads=(pthread_t *)malloc(n*sizeof(*threads));

 p=(param *)malloc(sizeof(param)*n);
 /* Assign args to a struct and start thread */
 p[0].id=0;
 p[0].event=0;
 p[0].type=0;
 pthread_create(&threads[0],NULL,poll,(void *)(p+i));

 sleep(1);

 p[1].id=1;
 pthread_create(&threads[1],NULL,drive,(void *)(p+i));

 sleep(1);

 while(1) {
   
   if(p[0].event){
     printf ("Event %d happened\n",p[0].type);
     break;
   }

   sleep(2);

 }


 /* Wait for drive thread */
 int *x = malloc(sizeof(int));
 pthread_join(threads[1],(void*)x);

 /* Kill Poll thread */
 pthread_kill(threads[0],0);

 /* Free up memory */
 free(p);
 exit(0);
}

