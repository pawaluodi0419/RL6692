#include "platform.h"

u8 _by_Pattern0_Setup_dut3()
{
	u8 i;
	u8 read_icstatus_data[20];
	u8 Buff_dut3_XGPIO_0[8];

	switch(dut3.g_pattern_step)
	{
	//normal mode：VMON非满偏(XGPIO13 -> L, XGPIO14 -> H), GPIO10为low(XGPIO23 -> L)
	//power rail: S0_5V_SW(XGPIO8 -> L)
	//DP reverse/U2 mux B: XGPIO22 -> H
	case 0x0000:
	{
		if(dut3.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut3_XGPIO_0[0] = 0x00;                          	//REG0005 ouput value[7:0]
			Buff_dut3_XGPIO_0[1] = 0xFF;							//REG0006
			Buff_dut3_XGPIO_0[2] = 0x40;							//REG0007 output value[15:8]
			Buff_dut3_XGPIO_0[3] = 0x9E;							//REG0008
			Buff_dut3_XGPIO_0[4] = 0x40|(dut3.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut3_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut3_XGPIO_0[6] = 0x02;							//REG000b output value[31:24]
			Buff_dut3_XGPIO_0[7] = 0xFC;							//REG000c

			XGpio_dut3_Relay_WriteByte(XPAR_AXI_GPIO_dut3_1_BASEADDR,Buff_dut3_XGPIO_0);
			dut3.g_dut_pattern_status_buf[7] = 0x01;
			//xil_printf("dut3.pattern0_gpio_control_completed!\r\n");
			msdelay(10);
		}
		else if(dut3.g_dut_pattern_status_buf[7] == 0x01)
		{
			if(dut3.g_relay_control_timer == 0 )
			{
				dut3.g_dut_pattern_status_buf[7] = 0x00;
				dut3.g_pattern_step++;

				//XGpio_2_WriteBit(0,dut3_FT2_PWR_CTRL_OFFSET,0x00);	//S0_PWR_EN# output low, power on DUT
				i2c_mcp23008_output(AD7994_DEV3_ADDR, MCP23008_ADDR, 0x80);
				xil_printf("dut3 power on!\r\n\r\n");
				msdelay(200);
			}
		}
		//output fail result
		else
		{
			dut3.g_result_fail = 0x01;
			dut3.g_result_fail_tmrcount = 0xffff;
		}
		break;
	}

	case 0x0001:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_vdcmdenable;
		dut3.g_pattern_smbus_control_buf[2] = 0xda;
		dut3.g_pattern_smbus_control_buf[3] = 0x0b;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	case 0x0002:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut3.g_pattern_smbus_control_buf[2] = 0x07;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut3 read 0xdf07 =%x\r\n", dut3.g_pattern_smbus_control_buf[10]);

			if(dut3.g_pattern_smbus_control_buf[10] == 0x00)
			{
				xil_printf("dut3 state： ROM code!\r\n\r\n");
				dut3.g_pattern_step++;
			}
			else
			{
				dut3.g_result_fail = 0x01;
				dut3.g_result_fail_tmrcount = 0xffff;
			}

			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//GPIO4
	//rh_smbuswritemem 0xcc 0xdfcc 1 0x1c
	case 0x0003:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0xcc;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x1c;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	case 0x0004:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut3.g_pattern_smbus_control_buf[2] = 0xcc;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			if(((dut3.g_pattern_smbus_control_buf[10] >> 3) & 0x03) == 0x03)
			{
				dut3.g_gpio_reread_timer = 3;
				dut3.g_pattern_step++;
			}
			else
			{
				if(dut3.g_gpio_reread_timer > 0)
				{
					dut3.g_pattern_step = 0x03;
					dut3.g_gpio_reread_timer--;
				}
				else
				{
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
			}

			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//GPIO5
	//rh_smbuswritemem 0xcc 0xdfcd 1 0x1c
	case 0x0005:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0xcd;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x1c;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	case 0x0006:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut3.g_pattern_smbus_control_buf[2] = 0xcd;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			if(((dut3.g_pattern_smbus_control_buf[10] >> 3) & 0x03) == 0x03)
			{
				dut3.g_gpio_reread_timer = 3;
				dut3.g_pattern_step++;
			}
			else
			{
				if(dut3.g_gpio_reread_timer > 0)
				{
					dut3.g_pattern_step = 0x05;
					dut3.g_gpio_reread_timer--;
				}
				else
				{
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
			}

			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//GPIO7
	//rh_smbuswritemem 0xcc 0xdfcf 1 0x1c
	case 0x0007:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0xcf;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x1c;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	case 0x0008:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut3.g_pattern_smbus_control_buf[2] = 0xcf;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			if(((dut3.g_pattern_smbus_control_buf[10] >> 3) & 0x03) == 0x03)
			{
				dut3.g_gpio_reread_timer = 3;
				dut3.g_pattern_step++;
			}
			else
			{
				if(dut3.g_gpio_reread_timer > 0)
				{
					dut3.g_pattern_step = 0x07;
					dut3.g_gpio_reread_timer--;
				}
				else
				{
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
			}

			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//GPIO8
	//rh_smbuswritemem 0xcc 0xdfd0 1 0x1c
	case 0x0009:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0xd0;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x1c;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	case 0x000a:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut3.g_pattern_smbus_control_buf[2] = 0xd0;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			if(((dut3.g_pattern_smbus_control_buf[10] >> 3) & 0x03) == 0x03)
			{
				dut3.g_gpio_reread_timer = 3;
				dut3.g_pattern_step++;
			}
			else
			{
				if(dut3.g_gpio_reread_timer > 0)
				{
					dut3.g_pattern_step = 0x09;
					dut3.g_gpio_reread_timer--;
				}
				else
				{
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
			}

			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//GPIO9
	//rh_smbuswritemem 0xcc 0xdfd1 1 0x18
	case 0x000b:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0xd1;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x18;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	case 0x000c:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut3.g_pattern_smbus_control_buf[2] = 0xd1;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			if(((dut3.g_pattern_smbus_control_buf[10] >> 3) & 0x03) == 0x03)
			{
				dut3.g_gpio_reread_timer = 3;
				dut3.g_pattern_step++;
			}
			else
			{
				if(dut3.g_gpio_reread_timer > 0)
				{
					dut3.g_pattern_step = 0x0b;
					dut3.g_gpio_reread_timer--;
				}
				else
				{
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
			}

			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//GPIO12
	//rh_smbuswritemem 0xcc 0xdfd4 1 0x18
	case 0x000d:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0xd4;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x18;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	case 0x000e:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut3.g_pattern_smbus_control_buf[2] = 0xd4;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			if(((dut3.g_pattern_smbus_control_buf[10] >> 3) & 0x03) == 0x03)
			{
				xil_printf("dut3 GPIO config success!\r\n\r\n");
				dut3.g_gpio_reread_timer = 3;
				dut3.g_pattern_step++;
			}
			else
			{
				if(dut3.g_gpio_reread_timer > 0)
				{
					dut3.g_pattern_step = 0x0d;
					dut3.g_gpio_reread_timer--;
				}
				else
				{
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
			}

			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//FT mode：VMON满偏(XGPIO13 -> H, XGPIO14 -> L), GPIO10为low(XGPIO23 -> L)
	case 0x000f:
	{
		if(dut3.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut3_XGPIO_0[0] = 0x00;                          	//REG0005 ouput value[7:0]
			Buff_dut3_XGPIO_0[1] = 0xFF;							//REG0006
			Buff_dut3_XGPIO_0[2] = 0x20;							//REG0007 output value[15:8]
			Buff_dut3_XGPIO_0[3] = 0x9E;							//REG0008
			Buff_dut3_XGPIO_0[4] = 0x40|(dut3.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut3_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut3_XGPIO_0[6] = 0x02;							//REG000b output value[31:24]
			Buff_dut3_XGPIO_0[7] = 0xFC;							//REG000c

			XGpio_dut3_Relay_WriteByte(XPAR_AXI_GPIO_dut3_1_BASEADDR,Buff_dut3_XGPIO_0);
			dut3.g_dut_pattern_status_buf[7] = 0x01;
			xil_printf("dut3.pattern0_ft_mode_control_completed!\r\n");
			msdelay(10);
		}
		else if(dut3.g_dut_pattern_status_buf[7] == 0x01)
		{
			if(dut3.g_relay_control_timer == 0 )
			{
				dut3.g_dut_pattern_status_buf[7] = 0x00;
				dut3.g_pattern_step++;
			}
		}
		//output fail result
		else
		{
			dut3.g_result_fail = 0x01;
			dut3.g_result_fail_tmrcount = 0xffff;
		}
		break;
	}

	//MCU reboot
	case 0x0010:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0x08;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0xff;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	case 0x0011:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0x08;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x00;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	case 0x0012:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0x07;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x80;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
			msdelay(200);
		}
		break;
	}

	case 0x0013:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_vdcmdenable;
		dut3.g_pattern_smbus_control_buf[2] = 0xda;
		dut3.g_pattern_smbus_control_buf[3] = 0x0b;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	//check FT mode
	case 0x0014:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut3.g_pattern_smbus_control_buf[2] = 0x07;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut3 read 0xdf07 =%x\r\n", dut3.g_pattern_smbus_control_buf[10]);

			if(dut3.g_pattern_smbus_control_buf[10] == 0x20)
			{
				xil_printf("dut3 state： FT mode!\r\n\r\n");
				dut3.g_pattern_step++;
				dut3.g_pattern_step++;
			}
			else
			{
				dut3.g_result_fail = 0x01;
				dut3.g_result_fail_tmrcount = 0xffff;
			}

			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

//	//disconnect aux mux switch
//	case 0x0015:
//	{
//		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
//		dut3.g_pattern_smbus_control_buf[2] = 0x15;
//		dut3.g_pattern_smbus_control_buf[3] = 0xc4;
//		dut3.g_pattern_smbus_control_buf[4] = 0x01;
//		dut3.g_pattern_smbus_control_buf[5] = 0x00;
//
//		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
//		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=1; i<60; i++)
//			{
//				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//
//			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut3.g_pattern_step++;
//		}
//		break;
//	}

	case 0x0016:
	{
		dut3.g_start_test_flag1 = 0x01;

		i2c_get_result_dut3(AD7994_DEV0_ADDR, USB_HOST_ADDR);
		msdelay(5);

		for(i=0;i<5;i++)
		{
			xil_printf("dut3.g_i2cRecBuf[%02d]= %02x\r\n", i, dut3.g_i2cRecBuf[i]);
		}
		xil_printf("\r\n");

		dut3.g_pattern_step++;

		break;
	}

	case 0x0017:
	{
		if((dut3.g_i2cRecBuf[4] & 0x07) == 0x07)
		{
			xil_printf("dut3.pattern_u2_host_test1_pass!\r\n\r\n");

			dut3.g_result_polling_tmrcount = 30;
			dut3.g_pattern_step++;
		}
		else
		{
			if(dut3.g_result_polling_tmrcount > 0)
			{
				i2c_get_result_dut3(AD7994_DEV0_ADDR, USB_HOST_ADDR);
				msdelay(5);

//				for(i=0;i<5;i++)
//				{
//					dut3.g_i2cRecBuf[i] = get_result_databuf[i];
//				}

				for(i=0;i<5;i++)
				{
					xil_printf("dut3.g_i2cRecBuf[%02d]= %02x\r\n", i, dut3.g_i2cRecBuf[i]);
				}

				dut3.g_result_polling_tmrcount--;
				xil_printf("dut3.g_result_polling_tmrcount = %d\r\n", dut3.g_result_polling_tmrcount);
				msdelay(200);	//每200ms polling一次结果
			}
			else
			{
				xil_printf("dut3.pattern_u2_host_test1_fail!\r\n\r\n");

				dut3.g_result_fail = 0x01;
				dut3.g_result_fail_tmrcount = 0xffff;
			}
		}
		break;
	}

	//check MCM flash ID
	case 0x0018:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_vdcmdenable;
		dut3.g_pattern_smbus_control_buf[2] = 0x04;
		dut3.g_pattern_smbus_control_buf[3] = 0x00;
		dut3.g_pattern_smbus_control_buf[4] = 0x02;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut3.get_mcmflash_id1 =0x%x\r\n", dut3.g_pattern_smbus_control_buf[10]);
			xil_printf("dut3.get_mcmflash_id2 =0x%x\r\n", dut3.g_pattern_smbus_control_buf[11]);

			if((dut3.g_pattern_smbus_control_buf[10] == 0xef) && (dut3.g_pattern_smbus_control_buf[11] == 0x11))
			{
			    xil_printf("dut3 check MCM flash ID pass!\r\n\r\n");

				for(i=1; i<60; i++)
				{
					dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
				}

				dut3.g_pattern_timer = 0xfff;
				dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut3.g_pattern_step = 0x00;
				//dut3.g_dut_pattern_status_buf[2]++;
				dut3.g_uartPatternEnable = 0x00;
				dut3.g_uartPatternNum++;
				result_output_for_v50(XPAR_AXI_GPIO_dut3_1_BASEADDR,dut3.g_uartPatternNum);
				xil_printf("dut3_pattern0_setup_pass!\r\n\r\n");
			}
			else
			{
				xil_printf("dut3 check MCM flash ID fail!\r\n\r\n");

				dut3.g_result_fail = 0x01;
				dut3.g_result_fail_tmrcount = 0xffff;
			}
		}
		break;
	}
	}

	if(dut3.g_result_fail == 0x01)
	{
		dut3.g_gpio_reread_timer = 3;
		xil_printf("dut3_pattern0_setup_fail! fail_step = %x\r\n",dut3.g_pattern_step);
		pattern_safe_state_dut3();
	}

	return 0;
}


