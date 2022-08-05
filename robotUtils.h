
#ifndef _ROBOT_UTILS_H_
#define _ROBOT_UTILS_H_

#include "stdint.h"

#define K_NB_MOTOR 5
#define K_PWM_0DEG 1000
#define K_PWM_90DEG_RANGE 1000

typedef struct
{
    struct
    {
        int powerOn;
        float cmdAngle; //[-1.0,1.0] => [-90°,90°]
        float speed; // [0-1.0] => [0°/s , 90°/s]
    }
    motor[K_NB_MOTOR];  
}T_robotCmdData;


typedef struct
{    
    struct
    {
        float angle; //[-1.0,1.0] => [-90°,90°]
        int pwm;    
    }
    motor[K_NB_MOTOR];  
}T_robotData;


typedef struct
{
    uint8_t magicNum;
    uint8_t cmdCtrl[5];
    uint8_t checksum;
}T_robotFrame;


extern void robotFrameEncode(T_robotCmdData *pRobotDataIn,T_robotFrame *pRobotFrameOut);
extern int robotFrameDecodeByByte(uint8_t pBufferByte,T_robotCmdData *pRobotDataOut);
extern void robotUpdateData(T_robotCmdData *pRobotCmdData,T_robotData *pRobotData,float pDeltaTimeInUs);
extern void robotAutoAnim(T_robotCmdData *pRobotCmdData,int pDeltaTimeInUs);

#endif