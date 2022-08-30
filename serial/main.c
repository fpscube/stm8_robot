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

   static T_robotCmd stc_robotCmdBuffer[]=
        {
            {K_CMD_SPEED,200},

            {K_CMD_ANGLE_MIDLE,117},
            {K_CMD_ANGLE_FRONT_LEFT,144},
            {K_CMD_ANGLE_FRONT_RIGHT,36},
            {K_CMD_ANGLE_BACK_RIGHT,144},
            {K_CMD_ANGLE_BACK_LEFT,36},
            {K_CMD_WAIT_100MS,20},

            {K_CMD_ANGLE_MIDLE,63},
            {K_CMD_ANGLE_FRONT_LEFT,144},
            {K_CMD_ANGLE_FRONT_RIGHT,36},
            {K_CMD_ANGLE_BACK_RIGHT,144},
            {K_CMD_ANGLE_BACK_LEFT,36},
            {K_CMD_WAIT_100MS,20},

            {K_CMD_ANGLE_MIDLE,63},
            {K_CMD_ANGLE_FRONT_LEFT,36},
            {K_CMD_ANGLE_FRONT_RIGHT,144},
            {K_CMD_ANGLE_BACK_RIGHT,36},
            {K_CMD_ANGLE_BACK_LEFT,144},
            {K_CMD_WAIT_100MS,20},

            {K_CMD_ANGLE_MIDLE,117},
            {K_CMD_ANGLE_FRONT_LEFT,36},
            {K_CMD_ANGLE_FRONT_RIGHT,144},
            {K_CMD_ANGLE_BACK_RIGHT,36},
            {K_CMD_ANGLE_BACK_LEFT,144},
            {K_CMD_WAIT_100MS,20}
        };
    T_robotFrameCmd lRobotFrameOut[1000];

    robotEncodeCmdFrame(stc_robotCmdBuffer,sizeof(stc_robotCmdBuffer)/sizeof(T_robotCmd),&lRobotFrameOut);
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long prevtimeinus = spec.tv_nsec/1000;

    while(1)
    {

        clock_gettime(CLOCK_REALTIME, &spec);
        long deltaTime = spec.tv_nsec/1000 - prevtimeinus;
        prevtimeinus =  spec.tv_nsec/1000;
        int sizeToSend= sizeof(stc_robotCmdBuffer)/sizeof(T_robotCmd) * sizeof(T_robotFrameCmd);
        comWrite(portId,(const char * )&lRobotFrameOut,sizeToSend);
        printf("%ld New Frame %d octets m0:%f \n" ,spec.tv_nsec/1000,sizeToSend);
        usleep(1000000);
      
    }
}