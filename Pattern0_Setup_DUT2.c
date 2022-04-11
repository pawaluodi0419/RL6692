#include "platform.h"

u8 _by_Pattern0_Setup_dut2()
{
	u8 i;
	u8 read_icstatus_data[20];
	u8 Buff_dut2_XGPIO_0[8];

	switch(dut2.g_pattern_step)
	{
	//normal mode£ºVMON·ÇÂúÆ«(XGPIO13 -> L, XGPIO14 -> H), GPIO10Îªlow(XGPIO23 -> L)
	//power rail: S0_5V_SW(XGPIO8 -> L)
	//DP reverse/U2 mux B: XGPIO22 -> H
	case 0x0000:
	{
		if(dut2.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut2_XGPIO_0[0] = 0x00;                          	//REG0005 ouput value[7:0]
			Buff_dut2_XGPIO_0[1] = 0xFF;							//REG0006
			Buff_dut2_XGPIO_0[2] = 0x40;							//REG0007 output value[15:8]
			Buff_dut2_XGPIO_0[3] = 0x9E;							//REG0008
			Buff_dut2_XGPIO_0[4] = 0x40|(dut2.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut2_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut2_XGPIO_0[6] = 0x02;							//REG000b output value[31:24]
			Buff_dut2_XGPIO_0[7] = 0xFC;							//REG000c

			XGpio_dut2_Relay_WriteByte(XPAR_AXI_GPIO_dut2_1_BASEADDR,Buff_dut2_XGPIO_0);
			dut2.g_dut_pattern_status_buf[7] = 0x01;
			//xil_printf("dut2.pattern0_gpio_control_completed!\r\n");
			msdelay(10);
		}
		else if(dut2.g_dut_pattern_status_buf[7] == 0x01)
		{
			if(dut2.g_relay_control_timer == 0 )
			{
				dut2.g_dut_pattern_status_buf[7] = 0x00;
				dut2.g_pattern_step++;

				//XGpio_2_WriteBit(0,dut2_FT2_PWR_CTRL_OFFSET,0x00);	//S0_PWR_EN# output low, power on DUT
				i2c_mcp23008_output(AD7994_DEV2_ADDR, MCP23008_ADDR, 0x80);
				xil_printf("dut2 power on!\r\n\r\n");
				msdelay(200);
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

	case 0x0001:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_vdcmdenable;
		dut2.g_pattern_smbus_control_buf[2] = 0xda;
		dut2.g_pattern_smbus_control_buf[3] = 0x0b;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	case 0x0002:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut2.g_pattern_smbus_control_buf[2] = 0x07;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut2 read 0xdf07 =%x\r\n", dut2.g_pattern_smbus_control_buf[10]);

			if(dut2.g_pattern_smbus_control_buf[10] == 0x00)
			{
				xil_printf("dut2 state£º ROM code!\r\n\r\n");
				dut2.g_pattern_step++;
			}
			else
			{
				dut2.g_result_fail = 0x01;
				dut2.g_result_fail_tmrcount = 0xffff;
			}

			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//GPIO4
	//rh_smbuswritemem 0xcc 0xdfcc 1 0x1c
	case 0x0003:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0xcc;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x1c;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	case 0x0004:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut2.g_pattern_smbus_control_buf[2] = 0xcc;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			if(((dut2.g_pattern_smbus_control_buf[10] >> 3) & 0x03) == 0x03)
			{
				dut2.g_gpio_reread_timer = 3;
				dut2.g_pattern_step++;
			}
			else
			{
				if(dut2.g_gpio_reread_timer > 0)
				{
					dut2.g_pattern_step = 0x03;
					dut2.g_gpio_reread_timer--;
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
		}
		break;
	}

	//GPIO5
	//rh_smbuswritemem 0xcc 0xdfcd 1 0x1c
	case 0x0005:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0xcd;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x1c;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	case 0x0006:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut2.g_pattern_smbus_control_buf[2] = 0xcd;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			if(((dut2.g_pattern_smbus_control_buf[10] >> 3) & 0x03) == 0x03)
			{
				dut2.g_gpio_reread_timer = 3;
				dut2.g_pattern_step++;
			}
			else
			{
				if(dut2.g_gpio_reread_timer > 0)
				{
					dut2.g_pattern_step = 0x05;
					dut2.g_gpio_reread_timer--;
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
		}
		break;
	}

	//GPIO7
	//rh_smbuswritemem 0xcc 0xdfcf 1 0x1c
	case 0x0007:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0xcf;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x1c;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	case 0x0008:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut2.g_pattern_smbus_control_buf[2] = 0xcf;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			if(((dut2.g_pattern_smbus_control_buf[10] >> 3) & 0x03) == 0x03)
			{
				dut2.g_gpio_reread_timer = 3;
				dut2.g_pattern_step++;
			}
			else
			{
				if(dut2.g_gpio_reread_timer > 0)
				{
					dut2.g_pattern_step = 0x07;
					dut2.g_gpio_reread_timer--;
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
		}
		break;
	}

	//GPIO8
	//rh_smbuswritemem 0xcc 0xdfd0 1 0x1c
	case 0x0009:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0xd0;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x1c;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	case 0x000a:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut2.g_pattern_smbus_control_buf[2] = 0xd0;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			if(((dut2.g_pattern_smbus_control_buf[10] >> 3) & 0x03) == 0x03)
			{
				dut2.g_gpio_reread_timer = 3;
				dut2.g_pattern_step++;
			}
			else
			{
				if(dut2.g_gpio_reread_timer > 0)
				{
					dut2.g_pattern_step = 0x09;
					dut2.g_gpio_reread_timer--;
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
		}
		break;
	}

	//GPIO9
	//rh_smbuswritemem 0xcc 0xdfd1 1 0x18
	case 0x000b:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0xd1;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x18;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	case 0x000c:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut2.g_pattern_smbus_control_buf[2] = 0xd1;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			if(((dut2.g_pattern_smbus_control_buf[10] >> 3) & 0x03) == 0x03)
			{
				dut2.g_gpio_reread_timer = 3;
				dut2.g_pattern_step++;
			}
			else
			{
				if(dut2.g_gpio_reread_timer > 0)
				{
					dut2.g_pattern_step = 0x0b;
					dut2.g_gpio_reread_timer--;
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
		}
		break;
	}

	//GPIO12
	//rh_smbuswritemem 0xcc 0xdfd4 1 0x18
	case 0x000d:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0xd4;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x18;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	case 0x000e:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut2.g_pattern_smbus_control_buf[2] = 0xd4;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			if(((dut2.g_pattern_smbus_control_buf[10] >> 3) & 0x03) == 0x03)
			{
				xil_printf("dut2 GPIO config success!\r\n\r\n");
				dut2.g_gpio_reread_timer = 3;
				dut2.g_pattern_step++;
			}
			else
			{
				if(dut2.g_gpio_reread_timer > 0)
				{
					dut2.g_pattern_step = 0x0d;
					dut2.g_gpio_reread_timer--;
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
		}
		break;
	}

	//FT mode£ºVMONÂúÆ«(XGPIO13 -> H, XGPIO14 -> L), GPIO10Îªlow(XGPIO23 -> L)
	case 0x000f:
	{
		if(dut2.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut2_XGPIO_0[0] = 0x00;                          	//REG0005 ouput value[7:0]
			Buff_dut2_XGPIO_0[1] = 0xFF;							//REG0006
			Buff_dut2_XGPIO_0[2] = 0x20;							//REG0007 output value[15:8]
			Buff_dut2_XGPIO_0[3] = 0x9E;							//REG0008
			Buff_dut2_XGPIO_0[4] = 0x40|(dut2.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut2_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut2_XGPIO_0[6] = 0x02;							//REG000b output value[31:24]
			Buff_dut2_XGPIO_0[7] = 0xFC;							//REG000c

			XGpio_dut2_Relay_WriteByte(XPAR_AXI_GPIO_dut2_1_BASEADDR,Buff_dut2_XGPIO_0);
			dut2.g_dut_pattern_status_buf[7] = 0x01;
			xil_printf("dut2.pattern0_ft_mode_control_completed!\r\n");
			msdelay(10);
		}
		else if(dut2.g_dut_pattern_status_buf[7] == 0x01)
		{
			if(dut2.g_relay_control_timer == 0 )
			{
				dut2.g_dut_pattern_status_buf[7] = 0x00;
				dut2.g_pattern_step++;
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

	//MCU reboot
	case 0x0010:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0x08;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0xff;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	case 0x0011:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0x08;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x00;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	case 0x0012:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0x07;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x80;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
			msdelay(200);
		}
		break;
	}

	case 0x0013:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_vdcmdenable;
		dut2.g_pattern_smbus_control_buf[2] = 0xda;
		dut2.g_pattern_smbus_control_buf[3] = 0x0b;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	//check FT mode
	case 0x0014:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut2.g_pattern_smbus_control_buf[2] = 0x07;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut2 read 0xdf07 =%x\r\n", dut2.g_pattern_smbus_control_buf[10]);

			if(dut2.g_pattern_smbus_control_buf[10] == 0x20)
			{
				xil_printf("dut2 state£º FT mode!\r\n\r\n");
				dut2.g_pattern_step++;
			}
			else
			{
				dut2.g_result_fail = 0x01;
				dut2.g_result_fail_tmrcount = 0xffff;
			}

			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//disconnect aux mux switch
	case 0x0015:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0x15;
		dut2.g_pattern_smbus_control_buf[3] = 0xc4;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x00;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
		}
		break;
	}

	//check IC information
	case 0x0016:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
		dut2.g_pattern_smbus_control_buf[2] = 0x00;
		dut2.g_pattern_smbus_control_buf[3] = 0x00;
		dut2.g_pattern_smbus_control_buf[4] = 0x14;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=0;i<21;i++)
			{
				read_icstatus_data[i] = dut2.g_pattern_smbus_control_buf[i+10];
			}
			xil_printf("dut2.read_icstatus_data[11] =0x%x\r\n", read_icstatus_data[11]);
			xil_printf("dut2.read_icstatus_data[12] =0x%x\r\n", read_icstatus_data[12]);

			if((read_icstatus_data[11] == 0x57) && (read_icstatus_data[12] == 0x54))
			{
			    xil_printf("dut2 check IC information pass!\r\n\r\n");

				for(i=1; i<60; i++)
				{
					dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				}

				dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut2.g_pattern_step++;
			}
			else
			{
				xil_printf("dut2 check IC information fail!\r\n\r\n");

				dut2.g_result_fail = 0x01;
				dut2.g_result_fail_tmrcount = 0xffff;
			}
		}
		break;
	}

	//check MCM flash ID
	case 0x0017:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_vdcmdenable;
		dut2.g_pattern_smbus_control_buf[2] = 0x04;
		dut2.g_pattern_smbus_control_buf[3] = 0x00;
		dut2.g_pattern_smbus_control_buf[4] = 0x02;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut2.get_mcmflash_id1 =0x%x\r\n", dut2.g_pattern_smbus_control_buf[10]);
			xil_printf("dut2.get_mcmflash_id2 =0x%x\r\n", dut2.g_pattern_smbus_control_buf[11]);

			if((dut2.g_pattern_smbus_control_buf[10] == 0xef) && (dut2.g_pattern_smbus_control_buf[11] == 0x11))
			{
			    xil_printf("dut2 check MCM flash ID pass!\r\n\r\n");

				for(i=1; i<60; i++)
				{
					dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				}

				dut2.g_pattern_timer = 0xfff;
				dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut2.g_pattern_step = 0x00;
				//dut2.g_dut_pattern_status_buf[2]++;
				dut2.g_uartPatternEnable = 0x00;
				dut2.g_uartPatternNum++;
				result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR,dut2.g_uartPatternNum);
				xil_printf("dut2_pattern0_setup_pass!\r\n\r\n");
			}
			else
			{
				xil_printf("dut2 check MCM flash ID fail!\r\n\r\n");

				dut2.g_result_fail = 0x01;
				dut2.g_result_fail_tmrcount = 0xffff;
			}
		}
		break;
	}
	}

	if(dut2.g_result_fail == 0x01)
	{
		dut2.g_gpio_reread_timer = 3;
		xil_printf("dut2_pattern0_setup_fail! fail_step = %x\r\n",dut2.g_pattern_step);
		pattern_safe_state_dut2();
	}

	return 0;
}


