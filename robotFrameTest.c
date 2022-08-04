#include "robotFrame.h"
#include "stdio.h"

int nbTestKO=0;
int nbTestOK=0;
#define ABS(a) ((a>=0)?a:-a)
#define K_FLOAT_EPSILON   (float)1.0E-03
#define FLOAT_EQUALS(pX,pY) ((pX <  pY + K_FLOAT_EPSILON ) && (pX > pY - K_FLOAT_EPSILON ))

void printMotoCmdInfo(T_robotCmdData *pRobotCmdData,int pMotor)
{
    printf("motor %d  powerOn:%d  angle:%f speed %f\n",pMotor,pRobotCmdData->motor[pMotor].powerOn,pRobotCmdData->motor[pMotor].cmdAngle,pRobotCmdData->motor[pMotor].speed);
}

void printMotoInfo(T_robotData *pRobotData,int pMotor)
{
    printf("motor %d  angle:%f pwm %d\n",pMotor,pRobotData->motor[pMotor].angle,pRobotData->motor[pMotor].pwm);
}

int main()
{
    T_robotCmdData lRobotCmdDataIn;
    T_robotCmdData lRobotCmdDataOut={0};
    T_robotData lRobotData={0};

    lRobotCmdDataIn.motor[0].cmdAngle = -1.0;
    lRobotCmdDataIn.motor[1].cmdAngle = 1.0;
    lRobotCmdDataIn.motor[2].cmdAngle = 0.0;
    lRobotCmdDataIn.motor[3].cmdAngle = 0.5;
    lRobotCmdDataIn.motor[4].cmdAngle = -0.2;


    lRobotCmdDataIn.motor[0].powerOn = 0;
    lRobotCmdDataIn.motor[1].powerOn = 1;
    lRobotCmdDataIn.motor[2].powerOn = 0;
    lRobotCmdDataIn.motor[3].powerOn = 1;
    lRobotCmdDataIn.motor[4].powerOn = 1;


    lRobotCmdDataIn.motor[0].speed = 0.75;
    lRobotCmdDataIn.motor[1].speed = 0.35;
    lRobotCmdDataIn.motor[2].speed = 0.0;
    lRobotCmdDataIn.motor[3].speed = 1.0;
    lRobotCmdDataIn.motor[4].speed = 0.86;

    T_robotframe lRobotFrame={0};
    uint8_t lBuffer[1000];

    robotFrameEncode(&lRobotCmdDataIn,&lRobotFrame);

    for(int i=0;i<1000;i++)
    {
        lBuffer[i] = 0;
    }

    for(int i=0;i<sizeof(lRobotFrame);i++)
    {
        lBuffer[500+i]=((uint8_t *)(&lRobotFrame))[i];
    }

    
    /* bit 0  */
    /* bit 1-2  2 bits => res = 4 */
    /* bit 3-7  6 bits => res = 32 */
    for(int i=0;i<1000;i++)
    {
        int ret = robotFrameDecodeByByte(lBuffer[i],&lRobotCmdDataOut);
        if(ret==1)
        {
            for (int mi=0;mi<5;mi++)
            {
                printMotoCmdInfo(&lRobotCmdDataIn,mi);
                printMotoCmdInfo(&lRobotCmdDataOut,mi);
                if( (ABS(lRobotCmdDataIn.motor[mi].cmdAngle-lRobotCmdDataOut.motor[mi].cmdAngle) < 0.033) &&
                    (ABS(lRobotCmdDataIn.motor[mi].speed-lRobotCmdDataOut.motor[mi].speed) < 0.251) &&
                    lRobotCmdDataIn.motor[mi].powerOn==lRobotCmdDataOut.motor[mi].powerOn )
                {

                    printf("Decode Motor %d OK \n",mi);
                }
                else
                {

                    printf("Decode Motor %d KO \n",mi);
                    nbTestKO++;
                }
            }
        }
        if(ret==-1)
        {
            printf("Decode KO %d\n",i);
            nbTestKO++;
        }

    }


    for(int i=0;i<20;i++)
    {
        printf("T%0.3f ",i*0.1);
       robotUpdateData(&lRobotCmdDataOut,&lRobotData,100);
        for (int mi=0;mi<5;mi++)
        { 
            printf("S%d:%0.2f ",mi,lRobotCmdDataOut.motor[mi].speed);
            printf("A%d:%0.4f/%0.4f ",mi,lRobotData.motor[mi].angle,lRobotCmdDataOut.motor[mi].cmdAngle);
        }
        printf("\n");
    }

    if(!FLOAT_EQUALS(lRobotData.motor[0].angle,-1.0f)) nbTestKO++;
    if(!FLOAT_EQUALS(lRobotData.motor[1].angle,0.6666f)) nbTestKO++;
    if(!FLOAT_EQUALS(lRobotData.motor[2].angle,0.000f)) nbTestKO++;
    if(!FLOAT_EQUALS(lRobotData.motor[3].angle,0.4839f)) nbTestKO++;
    if(!FLOAT_EQUALS(lRobotData.motor[4].angle,-0.2258f)) nbTestKO++;


    printf("nbTestKO %d\n",nbTestKO);
}