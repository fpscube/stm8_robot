#include <stdio.h>
#include "rs232.h"
#include "../robotUtils.h"
#include <unistd.h>

#include <time.h>


int main(void)
{
    comEnumerate();
    int portId = comFindPort("ttyUSB0");
    if(portId==-1)
    {
        printf("Error: ttyUSB0 not found \n");
        exit(0);

    }
    else
    {
        printf("ttyUSB0 found \n");
    }
    if(!comOpen(portId,1200))
    {
        printf("Error: cannot open ttyUSB0 1200 \n");
        exit(0);
    }

    T_robotCmdData lRobotData;
    T_robotFrame lRobotFrameOut;
     struct timespec spec;
     clock_gettime(CLOCK_REALTIME, &spec);
     long prevtimeinus = spec.tv_nsec/1000;

    while(1)
    {

        clock_gettime(CLOCK_REALTIME, &spec);
        long deltaTime = spec.tv_nsec/1000 - prevtimeinus;
        prevtimeinus =  spec.tv_nsec/1000;
        robotAutoAnim(&lRobotData,60);
        robotFrameEncode(&lRobotData,&lRobotFrameOut);
        comWrite(portId,(const char * )&lRobotFrameOut,sizeof(lRobotFrameOut));
        printf("%ld New Frame %d octets m0:%f \n" ,spec.tv_nsec/1000,(int)sizeof(lRobotFrameOut),lRobotData.motor[0].cmdAngle );
        usleep(100000);
      
    }
}