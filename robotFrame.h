
#ifndef _ROBOT_FRAME_H_
#define _ROBOT_FRAME_H_

#include "stdint.h"

typedef struct
{
    struct 
    {

        float angle; //[-1.0,1.0] => [-90°,90°]
        float speed; // [0-1.0] => [0°/s , 90°/s]
        int powerOn;
    }motor[5];
    
}T_robotData;

typedef struct
{
    uint8_t magicNum;
    uint8_t pwmCtrl[5];
    uint8_t checksum;
}T_robotframe;


extern void encodeRobotFrame(T_robotData *pRobotDataIn,T_robotframe *pRobotFrameOut);
extern int decodeRobotFrameByByte(uint8_t pBufferByte,T_robotData *pRobotDataOut);

#endif