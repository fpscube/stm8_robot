#include <SDL2/SDL.h>  
#include <stdio.h>
#include <math.h>
#include "robotUtils.h"

int nbTestKO=0;
int nbTestOK=0;
#define ABS(a) ((a>=0)?a:-a)
#define K_FLOAT_EPSILON   (float)1.0E-03
#define FLOAT_EQUALS(pX,pY) ((pX <  pY + K_FLOAT_EPSILON ) && (pX > pY - K_FLOAT_EPSILON ))

// void printMotoCmdInfo(T_robotCmdData *pRobotCmdData,int pMotor)
// {
//     printf("motor %d  angle:%f speed %d\n",pMotor,pRobotCmdData->motor[pMotor].cmdAngle,pRobotCmdData->motor[pMotor].speed);
// }

// void printMotoInfo(T_robotData *pRobotData,int pMotor)
// {
//     printf("motor %d  angle:%f pwm %d\n",pMotor,pRobotData->motor[pMotor].angle,pRobotData->motor[pMotor].pwm);
// }





int main(int argc, char* argv[]) {

    SDL_Window *lSdlWindow;                    // Declare a pointer
    SDL_Renderer *lSdlRenderer;                    // Declare a pointer
    SDL_Event lSdlEvent;
    

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (SDL_CreateWindowAndRenderer(800, 800, SDL_WINDOW_RESIZABLE, &lSdlWindow, &lSdlRenderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    while(1)
    {

        SDL_SetRenderDrawColor(lSdlRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(lSdlRenderer);
        
        while (SDL_PollEvent(&lSdlEvent))
        {
            switch (lSdlEvent.type) {
                 case SDL_QUIT:    
                    SDL_DestroyRenderer(lSdlRenderer);
                    SDL_DestroyWindow(lSdlWindow);
                    SDL_Quit();
                    exit(0);
                 break;
            }
            
        }

//     {
//         // center        back right   front right   back left   front left
//         {{{0.3,1},{0.6,1},{-0.6,1},{-0.6,1},{0.6,1}}},
//         {{{-0.3,1},{0.6,1},{-0.6,1},{-0.6,1},{0.6,1}}},
//         {{{-0.3,1},{-0.6,1},{0.6,1},{0.6,1},{-0.6,1}}},
//         {{{0.3,1},{-0.6,1},{0.6,1},{0.6,1},{-0.6,1}}}
//     };
        static T_robotCmd stc_robotCmdBuffer[]=
        {
            {K_CMD_SPEED,50},

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
        static T_robotFrameCmd stc_robotFrameCmd[256];
        static T_robotState stc_robotState;

        int lNbCmd = sizeof(stc_robotCmdBuffer)/sizeof(T_robotCmd);


        robotEncodeCmdFrame(stc_robotCmdBuffer,lNbCmd,stc_robotFrameCmd);
           
        /* bit 0  */
        /* bit 1-2  2 bits => res = 4 */
        /* bit 3-7  6 bits => res = 32 */
        for(int i=0;i<(lNbCmd * sizeof(T_robotFrameCmd));i++)
        {
            uint8_t  lByte = ((uint8_t*)&stc_robotFrameCmd)[i];
            robotDecodeAndSaveCmd(lByte,&stc_robotState);
            
        }
        SDL_Delay(10);
        robotUpdateState(&stc_robotState,10000);


        SDL_SetRenderDrawColor(lSdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        for(int mi=0;mi<K_NB_MOTOR;mi++)
        {

        // center        back right   front right   back left   front right
            static const int moteurPosX[K_NB_MOTOR] = {400, 600, 200, 600, 200 };
            static const int moteurPosY[K_NB_MOTOR] = {400, 200, 200, 600, 600 };
            int posX = moteurPosX[mi];
            int posY = moteurPosY[mi];
            SDL_RenderDrawLine(lSdlRenderer,posX, posY, sin(stc_robotState.motor[mi].angle*3.14/180)*100+posX, cos(stc_robotState.motor[mi].angle*3.14/180)*100+posY);

        }

        for (int mi=0;mi<5;mi++)
        { 
            //printf("S%d:%d ",mi,lRobotCmdDataOut.motor[mi].speed);
            printf("A%d:%d/%0.4f ",mi,stc_robotState.motor[mi].pwm,stc_robotState.motor[mi].angle);
        }
        printf("\n");

        SDL_RenderPresent(lSdlRenderer);
    }
}