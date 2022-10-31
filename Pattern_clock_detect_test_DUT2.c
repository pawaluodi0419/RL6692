#include "platform.h"

u8 _by_Pattern_clock_detect_test_dut2()
{
	u8 i;
	u8 j;
	u16 temp;
	u16 clock_data_sum = 0;
	u16 clock_data_average = 0;
	u8 Loop_run_enable = 0;

	switch(dut2.g_pattern_step)
	{
	//write pattern index
	case 0x0000:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0x33;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x16;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("\r\ndut2 write pattern index =%x\r\n", dut2.g_pattern_smbus_control_buf[5]);
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	//enable counter module
	case 0x0001:
	{
		dut2.g_clock_detect_reg_data_buf[0] = XGpio_ReadReg(XPAR_CLOCK_FREQ_DETECT_DUT2_BASEADDR, 0);

		if(dut2.g_clock_detect_reg_data_buf[0] == 0x55aaaa55)
		{
			XGpio_WriteReg(XPAR_CLOCK_FREQ_DETECT_DUT2_BASEADDR, 4, 0x00000001);
			dut2.g_pattern_step++;
		}
		else
		{
			xil_printf("dut2 counter module reg0 data error!\r\n\r\n");
			dut2.g_result_fail = 0x01;
		}

		break;
	}

	//polling cnt_end bit
	case 0x0002:
	{
		dut2.g_clock_detect_reg_data_buf[1] = XGpio_ReadReg(XPAR_CLOCK_FREQ_DETECT_DUT2_BASEADDR, 4);
//		xil_printf("dut2.g_clock_detect_reg_data_buf[1] =%x\r\n", dut2.g_clock_detect_reg_data_buf[1]);

		if(dut2.g_clock_detect_reg_data_buf[1] >> 1)
		{
			dut2.g_clock_detect_reg_data_buf[2] = XGpio_ReadReg(XPAR_CLOCK_FREQ_DETECT_DUT2_BASEADDR, 8);
//			xil_printf("dut2.g_clock_detect_reg_data_buf[2] =%x\r\n", dut2.g_clock_detect_reg_data_buf[2]);
			dut2.g_clock_detect_reg_data_buf[3] = XGpio_ReadReg(XPAR_CLOCK_FREQ_DETECT_DUT2_BASEADDR, 12);
//			xil_printf("dut2.g_clock_detect_reg_data_buf[3] =%x\r\n", dut2.g_clock_detect_reg_data_buf[3]);
			dut2.g_clock_detect_reg_data_buf[4] = XGpio_ReadReg(XPAR_CLOCK_FREQ_DETECT_DUT2_BASEADDR, 16);
//			xil_printf("dut2.g_clock_detect_reg_data_buf[4] =%x\r\n", dut2.g_clock_detect_reg_data_buf[4]);
			dut2.g_clock_detect_reg_data_buf[5] = XGpio_ReadReg(XPAR_CLOCK_FREQ_DETECT_DUT2_BASEADDR, 20);
//			xil_printf("dut2.g_clock_detect_reg_data_buf[5] =%x\r\n", dut2.g_clock_detect_reg_data_buf[5]);
			dut2.g_clock_detect_reg_data_buf[6] = XGpio_ReadReg(XPAR_CLOCK_FREQ_DETECT_DUT2_BASEADDR, 24);
//			xil_printf("dut2.g_clock_detect_reg_data_buf[6] =%x\r\n", dut2.g_clock_detect_reg_data_buf[6]);

			for(i=2; i<6; i++)
			{
				for(j=2; j<8-i; j++)
				{
					if(dut2.g_clock_detect_reg_data_buf[j] > dut2.g_clock_detect_reg_data_buf[j+1])
					{
						temp = dut2.g_clock_detect_reg_data_buf[j+1];
						dut2.g_clock_detect_reg_data_buf[j+1] = dut2.g_clock_detect_reg_data_buf[j];
						dut2.g_clock_detect_reg_data_buf[j] = temp;
					}
				}
			}

			for(i=2; i<7; i++)
			{
				xil_printf("dut2.g_clock_detect_reg_data_buf[%d] =%d\r\n", i, dut2.g_clock_detect_reg_data_buf[i]);
			}

			for(i=3; i<6; i++)
			{
				clock_data_sum += dut2.g_clock_detect_reg_data_buf[i];
			}
			clock_data_average = clock_data_sum / 3;
			xil_printf("dut2 clock_freq =%d\r\n", clock_data_average * 20);

			XGpio_WriteReg(XPAR_CLOCK_FREQ_DETECT_DUT2_BASEADDR, 4, 0x00000000);
			dut2.g_clock_detect_polling_tmrcount = 50;
			dut2.g_pattern_step++;
		}
		else
		{
			xil_printf("dut2 wait clock in...\r\n");

			if(dut2.g_clock_detect_polling_tmrcount > 0)
			{
				dut2.g_clock_detect_polling_tmrcount--;
				msdelay(1);
			}
			else
			{
				xil_printf("dut2 clock detect timeout!\r\n\r\n");

				XGpio_WriteReg(XPAR_CLOCK_FREQ_DETECT_DUT2_BASEADDR, 4, 0x00000000);
				dut2.g_clock_detect_polling_tmrcount = 50;
				dut2.g_result_fail = 0x01;
			}
		}

		break;
	}

	case 0x0003:
	{
		dut2.g_dut_start_ready = 0;

		if(Loop_run_enable == 1)
		{
			dut2.g_dut_start_ready = 0x11;
			dut2.g_uartPatternNum = 0x00;
			dut2.g_test_count++;
			xil_printf("dut2.g_test_count =%x\r\n", dut2.g_test_count);
		}
		else
		{
			dut2.g_uartPatternNum = 0x17;
			dut2.g_pass_count++;
		}
		result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR, dut2.g_uartPatternNum);
		xil_printf("\r\ndut2.FT2_pass, dut2.g_pass_count = %d\r\n\r\n", dut2.g_pass_count);
		//xil_printf("dut2.g_ft2_test_done = %x\r\n", dut2.g_ft2_test_done);
		//xil_printf("dut2.g_dut_start_ready = %x\r\n\r\n", dut2.g_dut_start_ready);
		//xil_printf("dut2.g_result_fail = %x\r\n\r\n", dut2.g_result_fail);
		//xil_printf("dut2.g_pattern_step = %x\r\n\r\n", dut2.g_pattern_step);

		pattern_safe_state_dut2();

		break;
	}
	}

	if(dut2.g_result_fail == 0x01)
	{
		xil_printf("dut2.pattern_clock_detect_test_fail! fail_step = %x\r\n", dut2.g_pattern_step);
		pattern_safe_state_dut2();
	}

	return 0;
}
