//
//  Assignment3.c
//  
//
//  Created by Ironman on 9/23/12.
//
//

#include <stdio.h>
#include <time.h>
#include <math.h>

int main ()

{
    double diftick;
    double mm, ss;
    float ms;
    float elapsed_time;
    
    int f;
    int t1, t2;
    int i;
    
    
        t1 = clock();
    for (i = 0; i<1000000; i++) {
        sleep(.01);
        t2 = clock();
        diftick = t2-t1;
        
        elapsed_time = ((float)diftick)/CLOCKS_PER_SEC; // in seconds
        mm = floor(elapsed_time/60);
        ss = floor(fmod(elapsed_time, 60));
        
        //account for rounding error
        if (ss >= 59.5 && ss <60.5) {
            ss = 0;
        }
        
        ms = fmod(elapsed_time, 1)*100;
        
        //account for rounding error
        if (ms >= 99.5 && ss <100.5) {
            ms = 0;
        }
        //printf("seconds: %f\n", ss);
        printf ("Time elapsed: %2.0f : %2.0f : %2.0f\n",mm, ss, ms);
        
    }
    return 0;
}

