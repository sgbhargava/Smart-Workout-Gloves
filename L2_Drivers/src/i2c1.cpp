#include "i2c1.hpp"
#include "LPC17xx.h"

extern "C"
{
    void I2C1_IRQHandler()
    {
        I2C1::getInstance().handleInterrupt();
    }
}


bool I2C1::init(unsigned int speedInKhz)
{
	 const uint32_t i2c_pin_mask = ( (1) | (1<<1) );
	    const bool i2c_wires_are_pulled_high = (i2c_pin_mask == (LPC_GPIO0->FIOPIN & i2c_pin_mask) );

	    LPC_PINCON->PINMODE0 &= ~(0xF ); // Both pins with Pull-Up Enabled
	    LPC_PINCON->PINMODE0 |=  (0xA); // Disable both pull-up and pull-down

	    // Enable Open-drain for I2C2 on pins P0.10 and P0.11
	    LPC_PINCON->PINMODE_OD0 |= i2c_pin_mask;

	    LPC_PINCON->PINSEL0 |= (0xF);  // Clear


	    lpc_pclk(pclk_i2c1, clkdiv_8);
	    const uint32_t pclk = sys_get_cpu_clock() / 8;

	    /**
	     * I2C wires should be pulled high for normal operation, so if they are, initialize I2C
	     * otherwise disable operations on I2C since I2C has a likely hardware BUS fault such as:
	     *  - I2C SDA/SCL with no pull-up
	     *  - I2C SDA/SCL shorted to ground
	     */
	    if (i2c_wires_are_pulled_high) {
	        return I2C_Base::init(pclk, speedInKhz);
	    }
	    else {
	        disableOperation();
	        return false;
	    }


}

I2C1::I2C1() : I2C_Base((LPC_I2C_TypeDef*) LPC_I2C1_BASE)
{

}
