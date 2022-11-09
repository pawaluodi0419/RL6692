#include "platform.h"

u8 _by_Pattern_clock_detect_test_dut3()
{
	u8 i;
	u8 j;
	u16 temp;
	u16 clock_data_sum = 0;
	u16 clock_data_average = 0;

	switch(dut3.g_pattern_step)
	{
//	//enable counter module
//	case 0x0001:
//	{
//		dut3.g_clock_detect_reg_data_buf[0] = XGpio_ReadReg(XPAR_CLOCK_FREQ_DETECT_DUT3_BASEADDR, 0);
//
//		if(dut3.g_clock_detect_reg_data_buf[0] == 0x55aaaa55)
//		{
//			XGpio_WriteReg(XPAR_CLOCK_FREQ_DETECT_DUT3_BASEADDR, 4, 0x00000001);
//			dut3.g_pattern_step++;
//		}
//		else
//		{
//			xil_printf("dut3 counter module reg0 data error!\r\n\r\n");
//			dut3.g_result_fail = 0x01;
//		}
//
//		break;
//	}

	//polling cnt_end bit
	case 0x0000:
	{
		dut3.g_clock_detect_reg_data_buf[2] = XGpio_ReadReg(XPAR_CLOCK_FREQ_DETECT_DUT3_BASEADDR, 8);
//			xil_printf("dut3.g_clock_detect_reg_data_buf[2] =%x\r\n", dut3.g_clock_detect_reg_data_buf[2]);
		dut3.g_clock_detect_reg_data_buf[3] = XGpio_ReadReg(XPAR_CLOCK_FREQ_DETECT_DUT3_BASEADDR, 12);
//			xil_printf("dut3.g_clock_detect_reg_data_buf[3] =%x\r\n", dut3.g_clock_detect_reg_data_buf[3]);
		dut3.g_clock_detect_reg_data_buf[4] = XGpio_ReadReg(XPAR_CLOCK_FREQ_DETECT_DUT3_BASEADDR, 16);
//			xil_printf("dut3.g_clock_detect_reg_data_buf[4] =%x\r\n", dut3.g_clock_detect_reg_data_buf[4]);
		dut3.g_clock_detect_reg_data_buf[5] = XGpio_ReadReg(XPAR_CLOCK_FREQ_DETECT_DUT3_BASEADDR, 20);
//			xil_printf("dut3.g_clock_detect_reg_data_buf[5] =%x\r\n", dut3.g_clock_detect_reg_data_buf[5]);
		dut3.g_clock_detect_reg_data_buf[6] = XGpio_ReadReg(XPAR_CLOCK_FREQ_DETECT_DUT3_BASEADDR, 24);
//			xil_printf("dut3.g_clock_detect_reg_data_buf[6] =%x\r\n", dut3.g_clock_detect_reg_data_buf[6]);

		for(i=2; i<6; i++)
		{
			for(j=2; j<8-i; j++)
			{
				if(dut3.g_clock_detect_reg_data_buf[j] > dut3.g_clock_detect_reg_data_buf[j+1])
				{
					temp = dut3.g_clock_detect_reg_data_buf[j+1];
					dut3.g_clock_detect_reg_data_buf[j+1] = dut3.g_clock_detect_reg_data_buf[j];
					dut3.g_clock_detect_reg_data_buf[j] = temp;
				}
			}
		}

		for(i=2; i<7; i++)
		{
			xil_printf("dut3.g_clock_detect_reg_data_buf[%d] =%d\r\n", i, dut3.g_clock_detect_reg_data_buf[i]);
		}

		for(i=3; i<6; i++)
		{
			clock_data_sum += dut3.g_clock_detect_reg_data_buf[i];
		}
		clock_data_average = clock_data_sum / 3;
		xil_printf("dut3 clock_period =%d\r\n\r\n", clock_data_average * 20);
		xil_printf("dut3 1M/100K: %d.%02d\r\n", (dut3.g_rc16m_data * clock_data_average / 800) / 100, (dut3.g_rc16m_data * clock_data_average / 800) % 100);

		XGpio_WriteReg(XPAR_CLOCK_FREQ_DETECT_DUT3_BASEADDR, 4, 0x00000000);
		g_clock_detect_status = 0;

		dut3.g_dut_start_ready = 0;
		dut3.g_uartPatternNum = 0x17;
		dut3.g_pass_count++;

		result_output_for_v50(XPAR_AXI_GPIO_dut3_1_BASEADDR, dut3.g_uartPatternNum);
		xil_printf("\r\ndut3.FT2_pass, dut3.g_pass_count = %d\r\n\r\n", dut3.g_pass_count);
		//xil_printf("dut3.g_ft2_test_done = %x\r\n", dut3.g_ft2_test_done);
		//xil_printf("dut3.g_dut_start_ready = %x\r\n\r\n", dut3.g_dut_start_ready);
		//xil_printf("dut3.g_result_fail = %x\r\n\r\n", dut3.g_result_fail);
		//xil_printf("dut3.g_pattern_step = %x\r\n\r\n", dut3.g_pattern_step);

		pattern_safe_state_dut3();

		break;
	}
	}

	if(dut3.g_result_fail == 0x01)
	{
		xil_printf("dut3.pattern_clock_detect_test_fail! fail_step = %x\r\n", dut3.g_pattern_step);
		pattern_safe_state_dut3();
	}

	return 0;
}
