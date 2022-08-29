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
        else
        {
            printf("error1\n");
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
            else
            {
                printf("error checksum %d %d \n",lChecksum , stc_robotFrameCmd.checksum);
            }
            stc_counter=0;
        }
    }
}


#define ABS(a) ((a>=0)?a:-a)

void robotUpdateState(T_robotState *pRobotState,float pDeltaTimeInUs)
{
    T_robotCmd lCurrRoboCmd =  pRobotState->animationCmdList[pRobotState->animationCounter];
    printf("animid:%d,cmdid:%d\n",pRobotState->animationCounter,lCurrRoboCmd.cmdId);
    switch(lCurrRoboCmd.cmdId)
    {
        case K_ROBOT_CMD_SELECT_MOTOR:
            pRobotState->selectedMotor = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_ROBOT_CMD_SET_SPEED:
            pRobotState->motor[pRobotState->selectedMotor].speedLimit = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_ROBOT_CMD_SET_ANGLE:
            pRobotState->motor[pRobotState->selectedMotor].orderAngle = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_ROBOT_CMD_HALT:
            pRobotState->halt = lCurrRoboCmd.val;
            break;
        case K_ROBOT_CMD_REPEAT:
            pRobotState->animationCounter=0;
            break;
        case K_ROBOT_CMD_SET_ALL_SPEED:
            pRobotState->motor[0].speedLimit = lCurrRoboCmd.val;
            pRobotState->motor[1].speedLimit = lCurrRoboCmd.val;
            pRobotState->motor[2].speedLimit = lCurrRoboCmd.val;
            pRobotState->motor[3].speedLimit = lCurrRoboCmd.val;
            pRobotState->motor[4].speedLimit = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_ROBOT_CMD_SET_ALL_ANGLE:
            pRobotState->motor[0].orderAngle = lCurrRoboCmd.val;
            pRobotState->motor[1].orderAngle = lCurrRoboCmd.val;
            pRobotState->motor[2].orderAngle = lCurrRoboCmd.val;
            pRobotState->motor[3].orderAngle = lCurrRoboCmd.val;
            pRobotState->motor[4].orderAngle = lCurrRoboCmd.val;
            pRobotState->animationCounter++;
            break;
        case K_ROBOT_CMD_WAIT_100MS:
            if(pRobotState->wating==0)
            {
                pRobotState->watingCounter = lCurrRoboCmd.val;
                pRobotState->wating=1;
            }
            else
            {
                pRobotState->watingCounter -= pDeltaTimeInUs/100000;
                if(pRobotState->watingCounter<=0)
                {
                    pRobotState->wating=0;
                    pRobotState->animationCounter++;
                }
            }
            break;
        default:
            pRobotState->animationCounter=0;
            break;
    }

    for(int mi=0;mi<5;mi++)
    {        
        float lAngle = pRobotState->motor[mi].angle;
        float lOrderAngle = pRobotState->motor[mi].orderAngle;
        float lMaxDeltaAngle = pRobotState->motor[mi].speedLimit*pDeltaTimeInUs/1000000;
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

