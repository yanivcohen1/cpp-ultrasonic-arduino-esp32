#pragma once
#include <stdlib.h>     /* qsort */
#include "Arduino.h"

namespace userlib{ namespace Ultrasinic
{
    void setup(int trigPin, int echoPin);
    int ultrasinicDistance();
}}