#include "robotFrame.h"
#include "stdio.h"

void printMotoInfo(T_robotData *pRobotData,int pMotor)
{
    printf("motor %d  powerOn:%d  angle:%f speed %f\n",pMotor,pRobotData->motor[pMotor].powerOn,pRobotData->motor[pMotor].angle,pRobotData->motor[pMotor].speed);
}

int main()
{
    T_robotData lRobotDataIn;
    T_robotData lRobotDataOut={0};

    lRobotDataIn.motor[0].angle = -1.0;
    lRobotDataIn.motor[1].angle = 1.0;
    lRobotDataIn.motor[2].angle = 0.0;
    lRobotDataIn.motor[3].angle = 0.5;
    lRobotDataIn.motor[4].angle = -0.2;


    lRobotDataIn.motor[0].powerOn = 0;
    lRobotDataIn.motor[1].powerOn = 1;
    lRobotDataIn.motor[2].powerOn = 0;
    lRobotDataIn.motor[3].powerOn = 1;
    lRobotDataIn.motor[4].powerOn = 1;


    lRobotDataIn.motor[0].speed = 0.75;
    lRobotDataIn.motor[1].speed = 0.35;
    lRobotDataIn.motor[2].speed = 0.0;
    lRobotDataIn.motor[3].speed = 1.0;
    lRobotDataIn.motor[4].speed = 0.86;

    T_robotframe lRobotFrame={0};
    uint8_t lBuffer[1000];

    encodeRobotFrame(&lRobotDataIn,&lRobotFrame);

    for(int i=0;i<1000;i++)
    {
        lBuffer[i] = 0;
    }

    for(int i=0;i<sizeof(lRobotFrame);i++)
    {
        lBuffer[500+i]=((uint8_t *)(&lRobotFrame))[i];
    }


    for(int i=0;i<1000;i++)
    {
        int ret = decodeRobotFrameByByte(lBuffer[i],&lRobotDataOut);
        if(ret==1)
        {
            printMotoInfo(&lRobotDataIn,0);
            printMotoInfo(&lRobotDataOut,0);
            printMotoInfo(&lRobotDataIn,1);
            printMotoInfo(&lRobotDataOut,1);
            printMotoInfo(&lRobotDataIn,2);
            printMotoInfo(&lRobotDataOut,2);
            printMotoInfo(&lRobotDataIn,3);
            printMotoInfo(&lRobotDataOut,3);
            printMotoInfo(&lRobotDataIn,4);
            printMotoInfo(&lRobotDataOut,4);
            printf("Decode OK\n");
        }
        if(ret==-1)
        {
            printf("Decode KO %d\n",i);
        }

    }
}