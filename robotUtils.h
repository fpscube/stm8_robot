
#ifndef _ROBOT_UTILS_H_
#define _ROBOT_UTILS_H_

#include "stdint.h"

#define K_NB_MOTOR 5
#define K_PWM_START 1000
#define K_PWM_RANGE_180 1000

typedef enum
{
 K_ROBOT_CMD_REPEAT =0,
 K_ROBOT_CMD_SELECT_MOTOR,
 K_ROBOT_CMD_SET_SPEED,
 K_ROBOT_CMD_SET_ANGLE,
 K_ROBOT_CMD_WAIT_100MS,
 K_ROBOT_CMD_HALT,
 K_ROBOT_CMD_SET_ALL_SPEED,
 K_ROBOT_CMD_SET_ALL_ANGLE
}T_robotCmdId;

typedef enum
{
 K_MOTOR_CENTER =0,
 K_MOTOR_BACK_RIGHT,
 K_MOTOR_FRONT_RIGHT,
 K_MOTOR_BACK_LEFT,
 K_MOTOR_FRONT_LEFT
}T_robotMotoId;

typedef struct
{
    uint8_t cmdId;
    uint8_t val;  
}T_robotCmd;


typedef struct
{
    uint8_t magicNum;
    uint8_t seqId;
    uint8_t cmdId;
    uint8_t val;  
    uint8_t checksum;
}T_robotFrameCmd;


typedef struct
{ 
    int animationCounter;
    T_robotCmd animationCmdList[255];
    int selectedMotor;
    int halt;
    float watingCounter;
    int wating;
    struct
    {
        float orderAngle; //[0,180°]
        int speedLimit; // [0°/s-255°/s] 
        float angle; //[0°,180°]
        int pwm;    
    }
    motor[K_NB_MOTOR];  
}T_robotState;


extern void robotEncodeCmdFrame(T_robotCmd *pRobotCmd,uint8_t pNbCmd,T_robotFrameCmd *pRobotFrameCmd);
extern void robotDecodeAndSaveCmd(uint8_t pBufferByte,T_robotState *pRobotState);
extern void robotUpdateState(T_robotState *pRobotState,float pDeltaTimeInUs);

#endif