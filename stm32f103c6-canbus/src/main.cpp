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
	rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSI_48MHZ]);
	//rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSE8_72MHZ]);
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

void loop() {
	// Loop over any characters pending in the UART Rx buffer,
	// and send each one over the CAN bus as a single message.
	char c;
	c='e';
	//do not wait for user input, send message at 500kbs to simulate
	while (Uart::get(&c)) {
		printf(" smile!\n");
		gpio_set(GPIOC, GPIO13);
		Uart::put(c);
				for (int i = 0; i < 6000; i++) { /* Wait a bit. */
					__asm__("nop");
				}

		CAN::Frame frame;
		frame.id = 0x00222222; //pretend this is the CEM module
		frame.extended_id = true; //volvo uses 29bit
		frame.rtr = false;
		frame.len = 8;
		// Populate the payload bytes with accelerator pedal 0x3F92
		frame.data[0] = 0xCE; //length = 5
		frame.data[1] = 0x50; //CEM module
		frame.data[2] = 0xE6; //reply to A6
		frame.data[3] = 0x3F; //accel pedal
		frame.data[4] = 0x92;
		frame.data[5] = 0x24; //value
		frame.data[6] = 0x01; //number of messages
		frame.data[7] = 0x00;
		CAN::transmit(frame);
		gpio_clear(GPIOC, GPIO13);
	}

	// Loop over any CAN frames pending in the CAN buffer, and print out
	// the ID of the message and all the data bytes.
	CAN::Frame frame;
	while (CAN::pop(frame)) {
		gpio_set(GPIOC, GPIO13);
		// Do something with the received frame
		printf("Rx ID: %u Data: ", frame.id);
		printf("Hexadecimal representation: 0x%X\n", frame.id);
		for (int i = 0; i < frame.len; i++) {
			printf("%02x:", frame.data[i]);
		}
		printf("\n");
		gpio_clear(GPIOC, GPIO13);
	}
}
