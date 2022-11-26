#include "robotUtils.h"

#define K_MAGIC_NUMBER 0x7A


void robotEncodeCmdFrame(T_robotCmd *pRobotCmd,uint8_t pNbCmd,T_robotFrameCmd *pRobotFrameCmd)
{

    for (int i = 0;i<pNbCmd;i++)
    {
        pRobotFrameCmd[i].magicNum = K_MAGIC_NUMBER;
        pRobotFrameCmd[i].cmdId = pRobotCmd[i].cmdId;
        pRobotFrameCmd[i].val = pRobotCmd[i].val;
        pRobotFrameCmd[i].seqId = i;
        pRobotFrameCmd[i].checksum = pRobotFrameCmd[i].magicNum + pRobotFrameCmd[i].seqId +pRobotFrameCmd[i].cmdId + pRobotFrameCmd[i].val;
    }

}




void robotDecodeAndSaveCmd(uint8_t pBufferByte,T_robotState *pRobotState)
{
    static int stc_counter=0;
    static T_robotFrameCmd stc_robotFrameCmd;

    ((uint8_t *)&stc_robotFrameCmd)[stc_counter]=pBufferByte;

    if(stc_counter==0)
    {
        if(pBufferByte == K_MAGIC_NUMBER) 
        {
            stc_counter=1;
        }
    }
    else 
    {
        stc_counter++;
        if(stc_counter>= sizeof(T_robotFrameCmd))
        {
            uint8_t lChecksum = stc_robotFrameCmd.magicNum + stc_robotFrameCmd.seqId +stc_robotFrameCmd.cmdId + stc_robotFrameCmd.val;
            if(lChecksum== stc_robotFrameCmd.checksum)
            {
                pRobotState->animationCmdList[stc_robotFrameCmd.seqId].cmdId = stc_robotFrameCmd.cmdId;
                pRobotState->animationCmdList[stc_robotFrameCmd.seqId].val = stc_robotFrameCmd.val;
            }
            stc_counter=0;
        }
    }
}


#define ABS(a) ((a>=0)?a:-a)

void robotUpdateState(T_robotState *pRobotState,float pDeltaTimeInMs)
{
    T_robotCmd lCurrRoboCmd;
    lCurrRoboCmd =  pRobotState->animationCmdList[pRobotState->animationCounter];
    switch(lCurrRoboCmd.cmdId)
    {
        case K_CMD_SPEED:
            pRobotState->motor[K_MOTOR_CENTER].speedLimit = lCurrRoboCmd.val;
            pRobotState->motor[K_MOTOR_FRONT_LEFT].speedLimit = lCurrRoboCmd.val;
            pRobotState->motor[K_MOTOR_FRONT_RIGHT].speedLimit = lCurrRoboCmd.val;
            pRobotState->motor[K_MOTOR_BACK_LEFT].speedLimit = lCurrRoboCmd.val;
            pRobotState->motor[K_MOTOR_BACK_RIGHT].speedLimit = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_CMD_SPEED_MIDLE:
            pRobotState->motor[K_MOTOR_CENTER].speedLimit = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_CMD_SPEED_FRONT_LEFT:
            pRobotState->motor[K_MOTOR_FRONT_LEFT].speedLimit = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_CMD_SPEED_FRONT_RIGHT:
            pRobotState->motor[K_MOTOR_FRONT_RIGHT].speedLimit = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_CMD_SPEED_BACK_LEFT:
            pRobotState->motor[K_MOTOR_BACK_LEFT].speedLimit = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_CMD_SPEED_BACK_RIGHT:
            pRobotState->motor[K_MOTOR_BACK_RIGHT].speedLimit = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_CMD_ANGLE:
            pRobotState->motor[K_MOTOR_CENTER].orderAngle = lCurrRoboCmd.val;
            pRobotState->motor[K_MOTOR_FRONT_LEFT].orderAngle = lCurrRoboCmd.val;
            pRobotState->motor[K_MOTOR_FRONT_RIGHT].orderAngle = lCurrRoboCmd.val;
            pRobotState->motor[K_MOTOR_BACK_LEFT].orderAngle = lCurrRoboCmd.val;
            pRobotState->motor[K_MOTOR_BACK_RIGHT].orderAngle = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_CMD_ANGLE_MIDLE:
            pRobotState->motor[K_MOTOR_CENTER].orderAngle = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_CMD_ANGLE_FRONT_LEFT:
            pRobotState->motor[K_MOTOR_FRONT_LEFT].orderAngle = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_CMD_ANGLE_FRONT_RIGHT:
            pRobotState->motor[K_MOTOR_FRONT_RIGHT].orderAngle = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_CMD_ANGLE_BACK_LEFT:
            pRobotState->motor[K_MOTOR_BACK_LEFT].orderAngle = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_CMD_ANGLE_BACK_RIGHT:
            pRobotState->motor[K_MOTOR_BACK_RIGHT].orderAngle = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_CMD_HALT:
            pRobotState->halt = lCurrRoboCmd.val;
            break;
        case K_CMD_WAIT_100MS:
            if(pRobotState->wating==0)
            {
                pRobotState->watingCounter = lCurrRoboCmd.val;
                pRobotState->wating=1;
            }
            else
            {
                pRobotState->watingCounter -= pDeltaTimeInMs/100;
                if(pRobotState->watingCounter<=0)
                {
                    pRobotState->wating=0;
                    pRobotState->animationCounter++;
                }
            }
            break;
        default: // REPEAT
            pRobotState->animationCounter=0;
            break;
    }

    for(int mi=0;mi<5;mi++)
    {        
        float lAngle = pRobotState->motor[mi].angle;
        float lOrderAngle = pRobotState->motor[mi].orderAngle;
        float lMaxDeltaAngle = pRobotState->motor[mi].speedLimit*pDeltaTimeInMs/1000;
        float lErrorAngle = lOrderAngle - lAngle;

        if(ABS(lErrorAngle) < lMaxDeltaAngle)
        {
            pRobotState->motor[mi].angle = lOrderAngle;
        }
        else
        {
            if(lErrorAngle>0)
                pRobotState->motor[mi].angle = lAngle+lMaxDeltaAngle;
            else
                pRobotState->motor[mi].angle = lAngle-lMaxDeltaAngle;
        }

    }

    for(int mi=0;mi<5;mi++)
    {  
        float lAngle = pRobotState->motor[mi].angle/180.0;
        // invert angle pwm moto 1&2
        if (mi<3)  lAngle = 1.0-lAngle;
        pRobotState->motor[mi].pwm =  K_PWM_START + lAngle*K_PWM_RANGE_180;
    }
}

