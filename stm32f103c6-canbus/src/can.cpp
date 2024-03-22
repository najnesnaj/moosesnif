#include <stdint.h>
#include <string.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include <can.hpp>
#include <queue.hpp>

#include <printf.h>


#ifndef CAN_PERIPH_SPEED
#define CAN_PERIPH_SPEED 48
#endif // CAN_PERIPH_SPEED



namespace CAN {

	namespace {
		Queue<Frame, 64> msg_queue;

		enum baudrates
		{
			Baud125, Baud250, Baud500, Baud800, Baud1000, BaudLast
		};
		struct CANSPEED
		{
			uint32_t ts1;
			uint32_t ts2;
			uint32_t prescaler;
		};
		static const CANSPEED canSpeed[BaudLast] =
#if CAN_PERIPH_SPEED == 32
		{
			{ CAN_BTR_TS1_13TQ, CAN_BTR_TS2_2TQ, 16}, //125kbps at 32 MHz
			{ CAN_BTR_TS1_13TQ, CAN_BTR_TS2_2TQ, 8 }, //250kbps at 32 MHz
			{ CAN_BTR_TS1_13TQ, CAN_BTR_TS2_2TQ, 4 }, //500kbps at 32 MHz
			{ CAN_BTR_TS1_6TQ, CAN_BTR_TS2_1TQ, 5 }, //800kbps at 36 MHz
			{ CAN_BTR_TS1_13TQ, CAN_BTR_TS2_2TQ, 2 }, //1000kbps at 36 MHz
		};
#elif CAN_PERIPH_SPEED == 36
		{
			{ CAN_BTR_TS1_9TQ, CAN_BTR_TS2_6TQ, 18}, //125kbps at 36 Mhz
				{ CAN_BTR_TS1_9TQ, CAN_BTR_TS2_6TQ, 9 }, //250kbps at 36 MHz
				{ CAN_BTR_TS1_4TQ, CAN_BTR_TS2_3TQ, 9 }, //500kbps at 36 MHz
				{ CAN_BTR_TS1_5TQ, CAN_BTR_TS2_3TQ, 5 }, //800kbps at 36 MHz
				{ CAN_BTR_TS1_6TQ, CAN_BTR_TS2_5TQ, 3 }, //1000kbps at 36 MHz
		};
#elif CAN_PERIPH_SPEED == 48
		{
			{ CAN_BTR_TS1_13TQ, CAN_BTR_TS2_2TQ, 24}, //125kbps at 48 MHz
				{ CAN_BTR_TS1_13TQ, CAN_BTR_TS2_2TQ, 12}, //250kbps at 48 MHz
				{ CAN_BTR_TS1_13TQ, CAN_BTR_TS2_2TQ, 6 }, //500kbps at 48 MHz
				{ CAN_BTR_TS1_12TQ, CAN_BTR_TS2_2TQ, 4 }, //800kbps at 48 MHz
				{ CAN_BTR_TS1_13TQ, CAN_BTR_TS2_2TQ, 3 }, //1000kbps at 48 MHz

		};
#else
#error Unhandled CAN peripheral speed, please define prescalers
#endif




	}
	void SetBaudrate(enum baudrates baudrate)
	{
		// CAN cell init.
		// Setting the bitrate to 250KBit. APB1 = 36MHz,
		// prescaler = 9 -> 4MHz time quanta frequency.
		// 1tq sync + 9tq bit segment1 (TS1) + 6tq bit segment2 (TS2) =
		// 16time quanto per bit period, therefor 4MHz/16 = 250kHz
		//
		can_init(CAN1,
				false,          // TTCM: Time triggered comm mode?
				true,           // ABOM: Automatic bus-off management?
				false,          // AWUM: Automatic wakeup mode?
				false,          // NART: No automatic retransmission?
				false,          // RFLM: Receive FIFO locked mode?
				false,          // TXFP: Transmit FIFO priority?
				CAN_BTR_SJW_1TQ,
				canSpeed[baudrate].ts1,
				canSpeed[baudrate].ts2,
				canSpeed[baudrate].prescaler,                              // BRP+1: Baud rate prescaler
				false,
				false);
	}

	void init() {


		/* Enable peripheral clocks. */
		rcc_periph_clock_enable(RCC_AFIO);
		rcc_periph_clock_enable(RCC_GPIOA);
		rcc_periph_clock_enable(RCC_GPIOB);
		rcc_periph_clock_enable(RCC_CAN);
		/* Configure CAN pin: RX (input pull-up). */
		//PB9 TD PB8 RD change from original port A cause of conflict usb
		gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
				GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_CAN_PB_RX);
		gpio_set(GPIOB, GPIO_CAN_PB_RX);

		/* Configure CAN pin: TX. */
		gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
				GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_CAN_PB_TX);


		gpio_primary_remap( // Map CAN1 to use PA11/PA12
				AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_OFF, // Optional
				AFIO_MAPR_CAN1_REMAP_PORTB);


		nvic_enable_irq(NVIC_CAN_RX1_IRQ); //CAN RX1
		nvic_enable_irq(NVIC_USB_LP_CAN_RX0_IRQ);
		nvic_set_priority(NVIC_USB_LP_CAN_RX0_IRQ, 1);

		/* Reset CAN. */
		can_reset(CAN1);
		SetBaudrate(Baud1000);//todo
		/* CAN cell init. */
		printf("setting baudrate\n");
		//		if (can_init(CAN1,
		//					false,           /* TTCM: Time triggered comm mode? */
		//					true,            /* ABOM: Automatic bus-off management? */
		//					false,           /* AWUM: Automatic wakeup mode? */
		//					false,           /* NART: No automatic retransmission? */
		//					false,           /* RFLM: Receive FIFO locked mode? */
		//					false,           /* TXFP: Transmit FIFO priority? */
		//					CAN_BTR_SJW_1TQ,
		//					CAN_BTR_TS1_3TQ,
		//					CAN_BTR_TS2_4TQ,
		//					12,
		//					false,
		//					false))             /* BRP+1: Baud rate prescaler */
		//		{
		//			printf("can_init failed\n");
		//			/* Die because we failed to initialize. */
		//	//		while (1)
		//	//			__asm__("nop");
		//		}
		// Define our two example filters
		// Match all messages with IDs less than 256
		const uint16_t id1 = 0; // We want to assert the high bits are zero
		const uint16_t mask1 = 0;
			//			(0b111 << 12); // The only bits we want to compare are STDID[10:8]

			// Match all messages with an ID of 342 and the RTR bit set
			const uint16_t id2 = 0; // STDID
						//	const uint16_t id2 = ((342 << 5) | // STDID
						//			(1 << 4)     // RTR
						//				);
		const uint16_t mask2 = 0; // Match all 11 bits of STDID
					  //	const uint16_t mask2 = ((0b11111111111 << 5) | // Match all 11 bits of STDID
					  //			(1 << 4)               // Match the RTR bit
					  //			);

					  // Create a filter mask that passes all critical broadcast & command
					  // CAN messages
		can_filter_id_mask_16bit_init(0,          // Filter number
				id1, mask1, // Our first filter
				id2, mask2, // Our second filter
				0,          // FIFO 0
				true);      // Enable -- filter niet actief

		/* Enable CAN RX interrupt. */
		//FIFO message pending
		can_enable_irq(CAN1, CAN_IER_FMPIE0|CAN_IER_FMPIE1);
		//	nvic_enable_irq(NVIC_USB_LP_CAN_RX0_IRQ); //CAN RX0
		//	nvic_enable_irq(NVIC_CAN_RX1_IRQ); //CAN RX1

	}

	int transmit(Frame &frame) {
		int resultaat;
		printf("transmit can frame\n");
		resultaat =  can_transmit(CAN1, frame.id, frame.extended_id, frame.rtr, frame.len, frame.data);
		printf("resultaat %d\n", resultaat);
		return (resultaat);
	}

	void receive(uint8_t fifo) {
		// Copy CAN message data into main ram
		Frame frame;
		//printf("received can frame\n");
		can_receive(CAN1,
				fifo, // FIFO id
				true, // Automatically release FIFO after rx
				&frame.id, &frame.extended_id, &frame.rtr, &frame.filter_id,
				&frame.len, frame.data, &frame.ts);
		msg_queue.push(frame);
	}

	void isr() {
		// Fifo 0 message pending?
		if (CAN_RF0R(CAN1) & CAN_RF0R_FMP0_MASK) {
			//printf("isr called 0\n");
			receive(0);
		}

		// Fifo 1 message pending?
		if (CAN_RF1R(CAN1) & CAN_RF1R_FMP1_MASK) {
			//printf("isr called 1\n");
			receive(1);
		}
	}

	bool has_pending_messages() { return msg_queue.available(); }

	bool pop(Frame &out) { return msg_queue.pop(out); }

} // namespace CAN

void can_rx0_isr(void) { CAN::isr(); }
void can_rx1_isr(void) { CAN::isr(); }
void usb_lp_can_rx0_isr(void) { CAN::isr(); }
