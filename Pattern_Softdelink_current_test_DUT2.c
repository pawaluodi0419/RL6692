#include "platform.h"

u8 _by_Pattern_Softdelink_current_test_dut2()
{
	u8 i;
	u32 j;
	u8 temp;
	static u16 data_sum = 0;
	u8 Buff_dut2_XGPIO_0[8];
	u8 Loop_run_enable = 0;

	switch(dut2.g_pattern_step)
	{
	//GPIO10接AD7994_I2C_EN, S0_FPGA_Control_K10(XGPIO23)设定输出high
	case 0x0000:
	{
		i2c_mcp23008_output(AD7994_DEV2_ADDR, MCP23008_ADDR, 0x81);
		msdelay(10);

		if(dut2.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut2_XGPIO_0[0] = 0x31;                            //REG0005 ouput value[7:0]
			Buff_dut2_XGPIO_0[1] = 0xC2;							//REG0006
			Buff_dut2_XGPIO_0[2] = 0x60;							//REG0007 output value[15:8]
			Buff_dut2_XGPIO_0[3] = 0x9E;							//REG0008
			Buff_dut2_XGPIO_0[4] = 0x80|(dut2.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut2_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut2_XGPIO_0[6] = 0x01;							//REG000b output value[31:24]
			Buff_dut2_XGPIO_0[7] = 0xFE;							//REG000c

			XGpio_dut2_Relay_WriteByte(XPAR_AXI_GPIO_dut2_1_BASEADDR,Buff_dut2_XGPIO_0);
			dut2.g_dut_pattern_status_buf[7] = 0x01;
			//xil_printf("PatternNum = %x\r\n",dut2.g_uartPatternNum);
			xil_printf("dut2.pattern_softdelink_current_test_gpio_control_completed!\r\n");
			msdelay(10);
		}
		else if(dut2.g_dut_pattern_status_buf[7] == 0x01)
		{
			if(dut2.g_relay_control_timer == 0 )
			{
				dut2.g_dut_pattern_status_buf[7] = 0x00;
				dut2.g_pattern_step++;
				//msdelay(550);
			}
		}
		//output fail result
		else
		{
			dut2.g_result_fail = 0x01;
			dut2.g_result_fail_tmrcount = 0xffff;
		}
		break;
	}

	//write pattern index
	case 0x0001:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0x33;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x15;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut2 write pattern index =%x\r\n", dut2.g_pattern_smbus_control_buf[5]);
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	//polling ack bit
	case 0x0002:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut2.g_pattern_smbus_control_buf[2] = 0x33;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut2 polling pattern ack bit =%x\r\n", dut2.g_pattern_smbus_control_buf[10]);

			if(dut2.g_pattern_smbus_control_buf[10] == 0x95)
			{
				for(i=1; i<60; i++)
				{
					dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				}

				dut2.g_ack_bit_reread_timer = 20;
				dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut2.g_pattern_step++;
				//xil_printf("dut2.pattern_softdelink_current_test_pass!\r\n");
			}
			else
			{
				if(dut2.g_ack_bit_reread_timer > 0)
				{
					for(i=1; i<60; i++)
					{
						dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
					}
					dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

					//xil_printf("dut2.g_ack_bit_reread_timer =%x\r\n", dut2.g_ack_bit_reread_timer);
					dut2.g_ack_bit_reread_timer--;
					msdelay(50);	//每50ms polling一次结果，timeout时间1s
				}
				else
				{
					dut2.g_result_fail = 0x01;
					dut2.g_result_fail_tmrcount = 0xffff;
				}
			}
		}
		break;
	}

	case 0x0003:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
		dut2.g_pattern_smbus_control_buf[2] = 0x25;
		dut2.g_pattern_smbus_control_buf[3] = 0x00;
		dut2.g_pattern_smbus_control_buf[4] = 0x02;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut2 softdelink_current_adc_code =%d\r\n\r\n", dut2.g_pattern_smbus_control_buf[10]);

			if(dut2.g_pass_count < 20)
			{
				j = dut2.g_pass_count;
				dut2.g_current_data_buf[j] = dut2.g_pattern_smbus_control_buf[10];

				for(i=0; i<20; i++)
				{
					xil_printf("dut2.g_current_data_buf[%02d] =%02d\r\n", i, dut2.g_current_data_buf[i]);
				}
				xil_printf("dut2.pattern_softdelink_current_test_pass!\r\n\r\n");
				//dut2.g_pattern_step++;
			}

			else if(dut2.g_pass_count == 20)
			{
				for(i=0; i<19; i++)
				{
					for(j=0; j<19-i; j++)
					{
						if(dut2.g_current_data_buf[j] > dut2.g_current_data_buf[j+1])
						{
							temp = dut2.g_current_data_buf[j+1];
							dut2.g_current_data_buf[j+1] = dut2.g_current_data_buf[j];
							dut2.g_current_data_buf[j] = temp;
						}
					}
				}

				for(i=0; i<20; i++)
				{
					xil_printf("dut2.g_current_data_buf[%02d] =%02d\r\n", i, dut2.g_current_data_buf[i]);
				}

				for(i=1; i<19; i++)
				{
					data_sum += dut2.g_current_data_buf[i];
					//xil_printf("dut2.g_current_data_sum =%02d\r\n", data_sum);
				}
				dut2.g_current_data_centre = data_sum / 18;
				xil_printf("dut2.g_current_data_centre =%02d\r\n", dut2.g_current_data_centre);

				if((dut2.g_pattern_smbus_control_buf[10] >= (dut2.g_current_data_centre - 5)) && (dut2.g_pattern_smbus_control_buf[10] <= (dut2.g_current_data_centre + 5)))
				{
					xil_printf("dut2.pattern_softdelink_current_test_pass!\r\n\r\n");
					//dut2.g_pattern_step++;
				}
				else
				{
					dut2.g_result_fail = 0x01;
					dut2.g_result_fail_tmrcount = 0xffff;
				}
			}

			else
			{
				xil_printf("dut2.g_current_data_centre =%02d\r\n", dut2.g_current_data_centre);

				if((dut2.g_pattern_smbus_control_buf[10] >= (dut2.g_current_data_centre - 5)) && (dut2.g_pattern_smbus_control_buf[10] <= (dut2.g_current_data_centre + 5)))
				{
					xil_printf("dut2.pattern_softdelink_current_test_pass!\r\n\r\n");
					//dut2.g_pattern_step++;
				}
				else
				{
					dut2.g_result_fail = 0x01;
					dut2.g_result_fail_tmrcount = 0xffff;
				}
			}

			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	//read efuse
	case 0x0004:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
		dut2.g_pattern_smbus_control_buf[2] = 0x00;
		dut2.g_pattern_smbus_control_buf[3] = 0x00;
		dut2.g_pattern_smbus_control_buf[4] = 0x28;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=0; i<40; i++)
			{
				xil_printf("dut2.efuse_data[%02d]= %02x\r\n", i, dut2.g_pattern_smbus_control_buf[i+10]);
			}

			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	case 0x0005:
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
		result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR,dut2.g_uartPatternNum);
		xil_printf("\r\ndut2.FT2_pass, dut2.g_pass_count = %d\r\n\r\n", dut2.g_pass_count);
		//xil_printf("dut2.g_ft2_test_done = %x\r\n",dut2.g_ft2_test_done);
		//xil_printf("dut2.g_dut_start_ready = %x\r\n\r\n",dut2.g_dut_start_ready);
		//xil_printf("dut2.g_result_fail = %x\r\n\r\n",dut2.g_result_fail);
		//xil_printf("dut2.g_pattern_step = %x\r\n\r\n",dut2.g_pattern_step);

		pattern_safe_state_dut2();

		break;
	}
	}

	if(dut2.g_result_fail == 0x01)
	{
		dut2.g_ack_bit_reread_timer = 20;
		xil_printf("dut2.pattern_softdelink_current_test_fail! fail_step = %x\r\n",dut2.g_pattern_step);
		pattern_safe_state_dut2();
	}

	return 0;
}
