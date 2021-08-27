#include "platform.h"

u8 _by_Pattern2_dut1()
{
	u8 i;
	u8 Buff_dut1_XGPIO_0[8];

	switch(dut1.g_pattern_step)
	{
	//GPIO8��24M OSC, S1_FPGA_RELAY001(XGPIO5)�趨���high
	case 0x0000:
	{
		if(dut1.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut1_XGPIO_0[0] = 0x20;                            //REG0005 ouput value[7:0]
			Buff_dut1_XGPIO_0[1] = 0xDF;							//REG0006
			Buff_dut1_XGPIO_0[2] = 0x20;							//REG0007 output value[15:8]
			Buff_dut1_XGPIO_0[3] = 0xDE;							//REG0008
			Buff_dut1_XGPIO_0[4] = 0xC0|(dut1.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut1_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut1_XGPIO_0[6] = 0x00;							//REG000b output value[31:24]
			Buff_dut1_XGPIO_0[7] = 0xFF;							//REG000c

			XGpio_dut1_Relay_WriteByte(XPAR_AXI_GPIO_dut1_1_BASEADDR,Buff_dut1_XGPIO_0);
			dut1.g_dut_pattern_status_buf[7] = 0x01;
			//xil_printf("PatternNum = %x\r\n",dut1.g_uartPatternNum);
			xil_printf("dut1.pattern2_gpio_control_completed!\r\n");
			msdelay(10);
		}
		else if(dut1.g_dut_pattern_status_buf[7] == 0x01)
		{
			if(dut1.g_relay_control_timer == 0 )
			{
				dut1.g_dut_pattern_status_buf[7] = 0x00;
				dut1.g_pattern_step++;
			}
		}
		//output fail result
		else
		{
			dut1.g_result_fail = 0x01;
			dut1.g_result_fail_tmrcount = 0xffff;
		}
		break;
	}

	//write pattern index
	case 0x0001:
	{
		dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut1.g_pattern_smbus_control_buf[2] = 0x33;
		dut1.g_pattern_smbus_control_buf[3] = 0xdf;
		dut1.g_pattern_smbus_control_buf[4] = 0x01;
		dut1.g_pattern_smbus_control_buf[5] = 0x02;

		smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut1 write pattern index =%x\r\n", dut1.g_pattern_smbus_control_buf[5]);
			for(i=1; i<60; i++)
			{
				dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut1.g_pattern_step++;
			//msdelay(100);
		}
		break;
	}

	//polling ack bit
	case 0x0002:
	{
		dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut1.g_pattern_smbus_control_buf[2] = 0x33;
		dut1.g_pattern_smbus_control_buf[3] = 0xdf;
		dut1.g_pattern_smbus_control_buf[4] = 0x01;

		smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut1 polling pattern ack bit =%x\r\n", dut1.g_pattern_smbus_control_buf[10]);
			//msdelay(10);
			if(dut1.g_pattern_smbus_control_buf[10] == 0x82)
			{
				for(i=1; i<60; i++)
				{
					dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
				}

				//dut1.g_ack_bit_reread_timer = 10;
				dut1.g_pattern_timer = 0xfff;
				dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut1.g_pattern_step = 0x00;
				//dut1.g_dut_pattern_status_buf[2]++;
				dut1.g_uartPatternEnable = 0x00;
				dut1.g_uartPatternNum++;
				result_output_for_v50(XPAR_AXI_GPIO_dut1_1_BASEADDR,dut1.g_uartPatternNum);
				xil_printf("dut1.pattern2_pass!\r\n\r\n");
			}
			else
			{
				//if(dut1.g_ack_bit_reread_timer > 0)
				//{
					//dut1.g_ack_bit_reread_timer--;
				//}
				//else
				//{
					dut1.g_result_fail = 0x01;
					dut1.g_result_fail_tmrcount = 0xffff;
				//}
			}
		}
		break;
	}
	}

	if(dut1.g_result_fail == 0x01)
	{
		xil_printf("dut1.pattern2_fail! fail_step = %x\r\n",dut1.g_pattern_step);
		pattern_safe_state_dut1();
	}

	return 0;
}