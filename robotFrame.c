#include "robotFrame.h"

#define K_MAGIC_NUMBER 0x7A


void robotFrameEncode(T_robotCmdData *pRobotDataIn,T_robotframe *pRobotFrameOut)
{
    pRobotFrameOut->magicNum = K_MAGIC_NUMBER;
    pRobotFrameOut->checksum = K_MAGIC_NUMBER;
    for(int i=0;i<5;i++)
    {
        /* bit 0  */
        pRobotFrameOut->cmdCtrl[i] |= (pRobotDataIn->motor[i].powerOn & 0x1);
        /* bit 1-2  2 bits => res = 4 */
        pRobotFrameOut->cmdCtrl[i] |= (((int)(pRobotDataIn->motor[i].speed * 3)) & 0x3)<<1;
        /* bit 3-7  6 bits => res = 32 */
        pRobotFrameOut->cmdCtrl[i] |= (((int)(((pRobotDataIn->motor[i].cmdAngle + 1.0)/2.0) * 31.0 )) & 0x1F) <<3;

        pRobotFrameOut->checksum  += pRobotFrameOut->cmdCtrl[i];
    }



}

static int stc_counter=0;
static uint8_t stc_checksum=0;
static T_robotCmdData stc_robotDataOut={0};
int robotFrameDecodeByByte(uint8_t pBufferByte,T_robotCmdData *pRobotDataOut)
{
    if(stc_counter==0)
    {
        if(pBufferByte == K_MAGIC_NUMBER) stc_counter=1;
        stc_checksum=K_MAGIC_NUMBER;
    }
    else if(stc_counter<6)
    {
        stc_robotDataOut.motor[stc_counter-1].powerOn = (pBufferByte & 0x1);
        stc_robotDataOut.motor[stc_counter-1].speed = ((float)((pBufferByte >>1) & 0x3))/3.0;
        stc_robotDataOut.motor[stc_counter-1].cmdAngle = ((float)((pBufferByte >>3) & 0x1F))/31.0 * 2.0 - 1.0;
        stc_checksum +=pBufferByte; 
        stc_counter++;
    }
    else
    {

        stc_counter=0;
        if(stc_checksum==pBufferByte)
        {
            *pRobotDataOut = stc_robotDataOut;
            return 1;
        }
        else
        {
            return -1;
        }
    }

    return 0;
}


#define ABS(a) ((a>=0)?a:-a)


void robotUpdateData(T_robotCmdData *pRobotCmdData,T_robotData *pRobotData,float pDeltaTimeInUs)
{
    for(int mi=0;mi<5;mi++)
    {        
        float lAngle = pRobotData->motor[mi].angle;
        float lCmdAngle = pRobotCmdData->motor[mi].cmdAngle;
        float lMaxDeltaAngle = pRobotCmdData->motor[mi].speed*pDeltaTimeInUs/1000;
        float lErrorAngle = lCmdAngle - lAngle;

        if(ABS(lErrorAngle) < lMaxDeltaAngle)
        {
            pRobotData->motor[mi].angle = lCmdAngle;
        }
        else
        {
            if(lErrorAngle>0)
                pRobotData->motor[mi].angle = lAngle+lMaxDeltaAngle;
            else
                pRobotData->motor[mi].angle = lAngle-lMaxDeltaAngle;
        }

    }

    for(int mi=0;mi<5;mi++)
    {  
        float lAngle = pRobotData->motor[mi].angle;
        pRobotData->motor[mi].pwm = K_PWM_0DEG + lAngle*K_PWM_90DEG_RANGE;
    }
}

