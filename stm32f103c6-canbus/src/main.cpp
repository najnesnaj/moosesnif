#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include <printf.h>

#include <can.hpp>
#include <uart.hpp>

void init();
void loop();

int main() {
	init();
	while (true) {
		loop();
	}
}

void init() {
	// Configure clocks
	//rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSI_48MHZ]);
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
	//rcc_clock_setup_in_hsi_out_48mhz();
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);

	// Initialize USART
	Uart::init();
	printf("\n\nUART Ready\n");

	// Initialize CAN peripheral
	CAN::init();
	printf("CAN Ready\n");

	// PA12 CAN_TX PA11 CAN_RX
	// ik vermoed dat onderstaand is voor ledjes
	// gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11 | GPIO12);
	// gpio_set(GPIOA, GPIO11 | GPIO12);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

}
		CAN::Frame frame, frame1, frame2;

void loop() {

	// Loop over any CAN frames pending in the CAN buffer, and print out
	// the ID of the message and all the data bytes.
	CAN::Frame framereceived;
	while (CAN::pop(framereceived)) {
		gpio_set(GPIOC, GPIO13);
		// Do something with the received frame
		printf("Rx ID: %u Data: ", framereceived.id);
		printf("Hexadecimal representation: 0x%X\n", framereceived.id);
		for (int i = 0; i < framereceived.len; i++) {
			printf("%02x:", framereceived.data[i]);
		}
                if (framereceived.data[1]==80){ //80 is 0x50 hex
                printf("ECM request\n");

//First byte in reply is a technical field, like rolling counter, start (0x80)/end(0x40 flag of the multipart message, etc. The
//P/N is: 08 68 85 13 == 8688513

for (int i = 0; i < 6000; i++) { /* Wait a bit. */
                                        __asm__("nop");
                                }

		frame.id = 0x01000003; //pretend this is the CEM module
		frame.extended_id = true; //volvo uses 29bit
		frame.rtr = false;
		frame.len = 8;
		// Populate the payload bytes with accelerator pedal 0x3F92
		frame.data[0] = 0x8F; //
		frame.data[1] = 0x50; //CEM module
		frame.data[2] = 0xF9; //
		frame.data[3] = 0xF0; //
		frame.data[4] = 0x00;
		frame.data[5] = 0x08; //
		frame.data[6] = 0x68; //
		frame.data[7] = 0x85;
		CAN::transmit(frame);

for (int i = 0; i < 6000; i++) { /* Wait a bit. */
                                        __asm__("nop");
                                }

		frame1.id = 0x01000003; //pretend this is the CEM module
		frame1.extended_id = true; //volvo uses 29bit
		frame1.rtr = false;
		frame1.len = 8;
		// Populate the payload bytes with accelerator pedal 0x3F92
		frame1.data[0] = 0x09; //
		frame1.data[1] = 0x13; //CEM module
		frame1.data[2] = 0x20; //
		frame1.data[3] = 0x20; //
		frame1.data[4] = 0x41;
		frame1.data[5] = 0x30; //
		frame1.data[6] = 0x65; //
		frame1.data[7] = 0x70;
		CAN::transmit(frame1);

for (int i = 0; i < 6000; i++) { /* Wait a bit. */
                                        __asm__("nop");
                                }

		frame2.id = 0x01000003; //pretend this is the CEM module
		frame2.extended_id = true; //volvo uses 29bit
		frame2.rtr = false;
		frame2.len = 8;
		// Populate the payload bytes with accelerator pedal 0x3F92
		frame2.data[0] = 0x4C; 
		frame2.data[1] = 0x24; 
		frame2.data[2] = 0x20; 
		frame2.data[3] = 0x20; 
		frame2.data[4] = 0x41;
		frame2.data[5] = 0x00; 
		frame2.data[6] = 0x00;
		frame2.data[7] = 0x00;
		CAN::transmit(frame2);
                }
		printf("\n");
		gpio_clear(GPIOC, GPIO13);
	}
}
