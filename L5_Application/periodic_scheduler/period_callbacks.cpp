/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * This contains the period callback functions for the periodic scheduler
 *
 * @warning
 * These callbacks should be used for hard real-time system, and the priority of these
 * tasks are above everything else in the system (above the PRIORITY_CRITICAL).
 * The period functions SHOULD NEVER block and SHOULD NEVER run over their time slot.
 * For example, the 1000Hz take slot runs periodically every 1ms, and whatever you
 * do must be completed within 1ms.  Running over the time slot will reset the system.
 */

#include <stdint.h>
#include "io.hpp"
#include "periodic_callback.h"
#include "i2c1.hpp"
#include "i2c2.hpp"
#include "stdio.h"
#include "file_logger.h"
uint8_t pointer1[6];
uint8_t pointer1g[6];
static int Acc_X_t_1 = 0;
static int Acc_Y_t_1 = 0;
static int Acc_Z_t_1 = 0;
uint8_t pointer2[6];
uint8_t pointer2g[6];
static int Acc_X_t_2 = 0;
static int Acc_Y_t_2 = 0;
static int Acc_Z_t_2 = 0;


/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/// Called once before the RTOS is started, this is a good place to initialize things once

void initPointer(uint8_t pointer[], int deviceNo)
{/*
	        pointer[0] =0;
			pointer[1] =0;
			pointer[2] =0;
			pointer[3] =0;
			pointer[4] =0;
			pointer[5] =0;*/
			/*if(pointer[1] && 0x80)
				pointer[1] = ~pointer[1] + 1;*/

			//i2c1.readRegisters(0x3A, 0xA8,pointer1, 6);
			if(deviceNo==1)
			{
			Acc_X_t_1 +=short( pointer[0] | (pointer[1] << 8));
			Acc_Y_t_1 +=short( pointer[2] | (pointer[3] << 8));
			Acc_Z_t_1 +=short( pointer[4] | (pointer[5] << 8));
			}
			else
			{
			Acc_X_t_2 +=short( pointer[0] | (pointer[1] << 8));
			Acc_Y_t_2 +=short( pointer[2] | (pointer[3] << 8));
			Acc_Z_t_2 +=short( pointer[4] | (pointer[5] << 8));
			}

}

void computeValues(int *x, int *y, int *z,uint8_t pointer[],int sensor,int deviceNo)
{

						*x =short( pointer[0] | (pointer[1] << 8));
						*y =short( pointer[2] | (pointer[3] << 8));
						*z =short( pointer[4] | (pointer[5] << 8));
						if(sensor==1)
						{
							if(deviceNo==1)
							{
								*x = *x - Acc_X_t_1;
								*y = *y - Acc_Y_t_1;
								*z = *z - Acc_Z_t_1;

							}
							else
							{
								*x = *x - Acc_X_t_2;
								*y = *y - Acc_Y_t_2;
								*z = *z - Acc_Z_t_2;

							}
						}
}
bool period_init(void)
{
	static I2C1 &i2c1 = I2C1::getInstance();
	i2c1.readRegisters(0x3A, 0xA8,pointer1, 6);

	static I2C2 &i2c2 = I2C2::getInstance();
	i2c2.readRegisters(0x3A, 0xA8,pointer2, 6);




	for(int i = 0; i< 10; i++)
	{

		i2c1.readRegisters(0x3A, 0xA8,pointer1, 6);
		initPointer(pointer1,1);
		i2c2.readRegisters(0x3A, 0xA8,pointer2, 6);
		initPointer(pointer2,2);

	}

	Acc_X_t_1 = Acc_X_t_1/ 10;
	Acc_Y_t_1 = Acc_Y_t_1/ 10;
	Acc_Z_t_1 = Acc_Z_t_1/ 10;
	printf("%d  %d  %d \n",Acc_X_t_1, Acc_Y_t_1, Acc_Z_t_1);

	Acc_X_t_2 = Acc_X_t_2/ 10;
	Acc_Y_t_2 = Acc_Y_t_2/ 10;
	Acc_Z_t_2 = Acc_Z_t_2/ 10;
		printf("%d  %d  %d \n",Acc_X_t_2, Acc_Y_t_2, Acc_Z_t_2);
	return true; // Must return true upon success
}

/// Register any telemetry variables
bool period_reg_tlm(void)
{
    // Make sure "SYS_CFG_ENABLE_TLM" is enabled at sys_config.h to use Telemetry
    return true; // Must return true upon success
}



void period_1Hz(void)
{
/*

	static I2C1 &i2c1 = I2C1::getInstance();
		i2c1.readRegisters(0x3A, 0xA8,pointer1, 6);
		Acc_X_t_2 =short( pointer2[0] | (pointer2[1] << 8));
		Acc_Y_t_2 =short( pointer2[2] | (pointer2[3] << 8));
		Acc_Z_t_2 =short( pointer2[4] | (pointer2[5] << 8));

		Acc_X_t_1 =short( pointer1[0] | (pointer1[1] << 8));
		Acc_Y_t_1 =short( pointer1[2] | (pointer1[3] << 8));
		Acc_Z_t_1 =short( pointer1[4] | (pointer1[5] << 8));



	static I2C2 &i2c2 = I2C2::getInstance();
		i2c2.readRegisters(0x3A, 0xA8,pointer2, 6);

			printf(" 1 is %d  %d  %d \n",Acc_X_t_1, Acc_Y_t_1, Acc_Z_t_1);
			printf("2 is %d  %d  %d \n",Acc_X_t_2, Acc_Y_t_2, Acc_Z_t_2);
	//LE.toggle(1);
*/

	}

void period_10Hz(void)
{

					int Acc_X1, Acc_Y1, Acc_Z1;
						int Acc_X2, Acc_Y2, Acc_Z2;
						int g_X1, g_Y1, g_Z1;
						int g_X2, g_Y2, g_Z2;
							static I2C1 &i2c1 = I2C1::getInstance();
								i2c1.readRegisters(0x3A, 0xA8,pointer1, 6);
								computeValues(&Acc_X1,&Acc_Y1,&Acc_Z1,pointer1,1,1);


							//printf("X 1 %d %d %d \n", Acc_X1,Acc_Y1,Acc_Z1);

								i2c1.readRegisters(0xD6, 0xA8, pointer1g, 6);
								computeValues(&g_X1,&g_Y1,&g_Z1,pointer1g,2,1);



							static I2C2 &i2c2 = I2C2::getInstance();
								i2c2.readRegisters(0x3A, 0xA8,pointer2, 6);
								computeValues(&Acc_X2,&Acc_Y2,&Acc_Z2,pointer2,1,2);

							//printf("%d %d %d\n", Acc_X2, Acc_Y2, Acc_Z2);

								i2c2.readRegisters(0xD6, 0xA8, pointer2g, 6);

								computeValues(&g_X2,&g_Y2,&g_Z2,pointer2g,2,1);

						LOG_RAW_MSG("X,%d,%d,Y,%d,%d,Z,%d,%d,gx,%d,%d,gy,%d,%d,gz,%d,%d ", Acc_X1, Acc_X2 ,Acc_Y1, Acc_Y2, Acc_Z1, Acc_Z2, g_X1,g_X2,g_Y1,g_Y2,g_Z1,g_Z2);



	//LE.toggle(2);


	}

void period_100Hz(void)
{
    /*LE.toggle(3);*/

}

void period_1000Hz(void)
{
   // LE.toggle(4);
}
