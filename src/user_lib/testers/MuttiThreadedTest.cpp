// https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html
#include <pthread.h>
#include "Arduino.h"

// #ifndef LED_BUILTIN
#define LED_BUILTIN 27//4
// #endif

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_cond  = PTHREAD_COND_INITIALIZER;
// pthread_cond_wait( &condition_cond, &condition_mutex );
int  counter = 0;

void *printThreadId(void *threadid) {
    Serial.print("start! ");
    Serial.println((int)threadid);
    pthread_mutex_lock( &mutex1 );
    counter++;
    Serial.print((int)counter);
    Serial.print(" threadid: ");
    Serial.println((int)threadid);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(3000);
    pthread_mutex_unlock( &mutex1 );
    Serial.print("End! ");
    Serial.println((int)threadid);
    return 0;
}
 
void mt_setup() {
 
   Serial.begin(115200);
   pinMode(LED_BUILTIN, OUTPUT);
}
 
void mt_loop() {
   int rc1, rc2;
   pthread_t thread1, thread2;

   /* Create independent threads each of which will execute functionC */

   if( (rc1=pthread_create( &thread1, NULL, printThreadId, (void *)1)) )
   {
      Serial.print("Thread creation failed: ");
      Serial.println((int)rc1);
   }

   if( (rc2=pthread_create( &thread2, NULL, printThreadId, (void *)2)) )
   {
      Serial.print("Thread creation failed: ");
      Serial.println((int)rc2);
   }

   /* Wait till threads are complete before main continues. Unless we  */
   /* wait we run the risk of executing an exit which will terminate   */
   /* the process and all threads before the threads have completed.   */

   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL); 
}