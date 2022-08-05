#include <SDL2/SDL.h>  
#include <stdio.h>
#include <math.h>
#include "robotUtils.h"

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


    T_robotFrame lRobotFrame={0};
    T_robotData lRobotData={0};
    T_robotCmdData lRobotCmdDataOut={0};
    T_robotCmdData lRobotCmdDataIn={0};


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
        robotAutoAnim(&lRobotCmdDataIn,100);

        robotFrameEncode(&lRobotCmdDataIn,&lRobotFrame);
           
        /* bit 0  */
        /* bit 1-2  2 bits => res = 4 */
        /* bit 3-7  6 bits => res = 32 */
        for(int i=0;i<sizeof(T_robotFrame);i++)
        {
            uint8_t  lByte = ((uint8_t*)&lRobotFrame)[i];
            int ret = robotFrameDecodeByByte(lByte,&lRobotCmdDataOut);
        }
        SDL_Delay(100);
        robotUpdateData(&lRobotCmdDataOut,&lRobotData,100);


        SDL_SetRenderDrawColor(lSdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        for(int mi=0;mi<K_NB_MOTOR;mi++)
        {
            static const int moteurPosX[K_NB_MOTOR] = {200, 200, 600, 600, 400 };
            static const int moteurPosY[K_NB_MOTOR] = {200, 600, 200, 600, 400 };
            int posX = moteurPosX[mi];
            int posY = moteurPosY[mi];
            SDL_RenderDrawLine(lSdlRenderer,posX, posY, sin(lRobotData.motor[mi].angle*3.14/2.0)*100+posX, cos(lRobotData.motor[mi].angle*3.14/2.0)*100+posY);

        }

        for (int mi=0;mi<5;mi++)
        { 
            printf("S%d:%0.2f ",mi,lRobotCmdDataOut.motor[mi].speed);
            printf("A%d:%0.4f/%0.4f ",mi,lRobotData.motor[mi].angle,lRobotCmdDataOut.motor[mi].cmdAngle);
        }
        printf("\n");

        SDL_RenderPresent(lSdlRenderer);
    }
}