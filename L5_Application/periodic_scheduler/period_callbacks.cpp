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
static int Acc_X_t_1 = 0;
static int Acc_Y_t_1 = 0;
static int Acc_Z_t_1 = 0;
uint8_t pointer2[6];
static int Acc_X_t_2 = 0;
static int Acc_Y_t_2 = 0;
static int Acc_Z_t_2 = 0;


/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{
	static I2C1 &i2c1 = I2C1::getInstance();
	i2c1.readRegisters(0x3A, 0xA8,pointer1, 6);

	static I2C2 &i2c2 = I2C2::getInstance();
	i2c2.readRegisters(0x3A, 0xA8,pointer2, 6);

	for(int i = 0; i< 10; i++)
	{
		pointer1[0] =0;
		pointer1[1] =0;
		pointer1[2] =0;
		pointer1[3] =0;
		pointer1[4] =0;
		pointer1[5] =0;
		/*if(pointer[1] && 0x80)
			pointer[1] = ~pointer[1] + 1;*/

		i2c1.readRegisters(0x3A, 0xA8,pointer1, 6);
	Acc_X_t_1 +=short( pointer1[0] | (pointer1[1] << 8));
	Acc_Y_t_1 +=short( pointer1[2] | (pointer1[3] << 8));
	Acc_Z_t_1 +=short( pointer1[4] | (pointer1[5] << 8));

	}

	Acc_X_t_1 = Acc_X_t_1/ 10;
	Acc_Y_t_1 = Acc_Y_t_1/ 10;
	Acc_Z_t_1 = Acc_Z_t_1/ 10;
	printf("%d  %d  %d \n",Acc_X_t_1, Acc_Y_t_1, Acc_Z_t_1);

	for(int i = 0; i< 10; i++)
		{
			pointer2[0] =0;
			pointer2[1] =0;
			pointer2[2] =0;
			pointer2[3] =0;
			pointer2[4] =0;
			pointer2[5] =0;
			/*if(pointer[1] && 0x80)
				pointer[1] = ~pointer[1] + 1;*/

			i2c2.readRegisters(0x3A, 0xA8,pointer2, 6);
		Acc_X_t_2 +=short( pointer2[0] | (pointer2[1] << 8));
		Acc_Y_t_2 +=short( pointer2[2] | (pointer2[3] << 8));
		Acc_Z_t_2 +=short( pointer2[4] | (pointer2[5] << 8));

		}

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
    //LE.toggle(1);

}

void period_10Hz(void)
{

	int Acc_X1, Acc_Y1, Acc_Z1;
				int Acc_X2, Acc_Y2, Acc_Z2;
					static I2C1 &i2c1 = I2C1::getInstance();
						i2c1.readRegisters(0x3A, 0xA8,pointer1, 6);

						Acc_X1 =short( pointer1[0] | (pointer1[1] << 8));
						Acc_Y1 =short( pointer1[2] | (pointer1[3] << 8));
						Acc_Z1 =short( pointer1[4] | (pointer1[5] << 8));
						Acc_X1 = Acc_X1 - Acc_X_t_1;
						Acc_Y1 = Acc_Y1 - Acc_Y_t_1;
						Acc_Z1 = Acc_Z1 - Acc_Z_t_1;
					//printf("X 1 %d %d %d \n", Acc_X1,Acc_Y1,Acc_Z1);

					static I2C2 &i2c2 = I2C2::getInstance();
						i2c2.readRegisters(0x3A, 0xA8,pointer2, 6);

						Acc_X2 =short( pointer2[0] | (pointer2[1] << 8));
						Acc_Y2 =short( pointer2[2] | (pointer2[3] << 8));
						Acc_Z2 =short( pointer2[4] | (pointer2[5] << 8));
						Acc_X2 = Acc_X2 - Acc_X_t_2;
						Acc_Y2 = Acc_Y2 - Acc_Y_t_2;
						Acc_Z2 = Acc_Z2 - Acc_Z_t_2;
					//printf("%d %d %d\n", Acc_X2, Acc_Y2, Acc_Z2);

				LOG_RAW_MSG("X1 \t %d X2 \t %d \t Y1 \t %d Y2 %d \t Z1 \t %d Z2 \t %d ", Acc_X1, Acc_X2 ,Acc_Y1, Acc_Y2, Acc_Z1, Acc_Z2);

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
