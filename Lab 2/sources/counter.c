#include <xparameters.h>
#include <xgpio.h>
#include <xstatus.h>
#include <xil_printf.h>

/* Definitions */
#define GPIO_DEVICE_ID_LED		XPAR_LED_DEVICE_ID		/* GPIO device that LEDs are connected to */
#define GPIO_DEVICE_ID_SWITCH	XPAR_INPUTS_DEVICE_ID	/* GPIO device that INPUTS are connected to */
#define WAIT_VAL 10000000

int delay(void);

int main()
{
	int count;
	int count_masked;
	XGpio leds;
	XGpio inputs;
	int led_status, input_status;
	int read_state, led_state;
	
	led_status = XGpio_Initialize(&leds, GPIO_DEVICE_ID_LED);
	XGpio_SetDataDirection(&leds, 1, 0x00);	// Initialize GPIO block as output
	if(led_status != XST_SUCCESS)
	{
		xil_printf("Initialization failed");
	}
	
	input_status = XGpio_Initialize(&inputs, GPIO_DEVICE_ID_SWITCH);
	XGpio_SetDataDirection(&inputs, 1, 0xFF);	// Initialize GPIO block as input
	if(input_status != XST_SUCCESS)
	{
		xil_printf("Initialization failed");
	}

	// Data format:	<Push Button state - 4 bits> <DIP Switch state - 4 bits>
	
	count = 0;
	while(1)
	{
		read_state = XGpio_DiscreteRead(&inputs, 1);		// Read values from inputs

		// Push button 0 is pressed - Increment counter
		if( read_state & 0x10 )
		{
			count++;
			count_masked = count & 0x0F;
			xil_printf("Push button 0 pressed, Value of counter = 0x%x \n\r", count_masked);
		}
		
		// Push button 1 is pressed - Decrement counter
		else if( read_state & 0x20 )
		{
			count--;
			count_masked = count & 0x0F;
			xil_printf("Push Button 1 pressed, Value of counter = 0x%x \n\r", count_masked);
		}
		
		// Push button 2 is pressed - Display status of DIP switches
		else if( read_state & 0x40 )
		{
			int switch_state = XGpio_DiscreteRead(&inputs, 1);
			led_state = switch_state & 0x0F;		// Grab lower 4 bits - values of DIP switches
			
			// Write switch state to LEDs
			XGpio_DiscreteWrite( &leds, 1, led_state );
			xil_printf("Push button 2 pressed, Value of switches = 0x%x \n\r",led_state);
		}
		
		// Push button 4 is pressed - Display counter value on LEDs
		else if( read_state & 0x80 )
		{
			count_masked = count & 0x0F;
			
			// Write counter value to LEDs
			XGpio_DiscreteWrite( &leds, 1, count_masked );
			xil_printf("Push button 4 pressed, Value of LEDs = 0x%x \n\r", count_masked);
		}
		
		delay();
	}
	
	return(0);
}

int delay(void)
{
	volatile int delay_count = 0;
	while(delay_count < WAIT_VAL)
	{
		delay_count++;
	}
	
	return(0);
}
