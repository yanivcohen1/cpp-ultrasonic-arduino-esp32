#include <stdlib.h>     /* qsort */
#include "Arduino.h"

namespace Ultrasinic
{
  int calcDisctance = 0;

  int compare (const void * a, const void * b)
  {
    return ( *(int*)a - *(int*)b );
  }

  double median(int arr[], int size){
    qsort (arr, size, sizeof(int), compare);
    if (size % 2 != 0)
        return (double)arr[size/2];
    return (double)(arr[(size-1)/2] + arr[size/2])/2.0;
  }

  int ultrasinicDistance(int trigPin, int echoPin) {
      int arr[10] = {0};
      for (int i = 0; i < 10; i++) {
          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          // Sets the trigPin on HIGH state for 10 micro seconds
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
          // Reads the echoPin, returns the sound wave travel time in microseconds
          // calculate how mach time is on (time to go forword and back)
          arr[i] = pulseIn(echoPin, HIGH);
          // Calculating the distance(cm) = duration(µs) * 0.034cm/µs (speed of sound) / 2(distance is duble forword and backword)
          // distance += duration # *0.034/2
      }
      //Serial.println("dist Arry:");
      // for(int i = 0; i < 10; i++)
      // {
      //   Serial.println(arr[i]);
      // }
      int size = sizeof(arr)/sizeof(arr[0]);
      int _median = median(arr, size);
      int send = int(_median * 0.034 / 2);
      // print('sending: ', send)
      calcDisctance = send;
      return send; // distace in (cm)
  }
}