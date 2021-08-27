/*
 * smbus_drv.c
 *
 *  Created on: 2018-3-6
 *      Author: Ashley_su
 */

#include "platform.h"
#include "smbus_drv.h"

/******************************************************************************/
/**
*
* Config the smbus
*
* @param    devAddr :
*
* @param    chNum :
*
* @param    avgPoints :
*
* @return	adc conversion data
*
* @note		None
*
*******************************************************************************/

u8 smbus_master_write_blockwrite(u32 smbus_fpga_baseaddr, u8 smbus_writecmd_buf[], u8 smbus_writecmd_count)
{
	u8 i;
	u8 smbus_writedata_count;
	u8 smbus_write_cmdcode;
	u8 smbus_memAddr;
	u8 smbus_writedata_buf[20];

	XIic_DynInit(smbus_fpga_baseaddr);
	usdelay(5);
	XIic_DynSend(smbus_fpga_baseaddr, smbus_dut_slaveaddr, smbus_writecmd_buf, smbus_writecmd_count, XIIC_STOP);
	usdelay(10);

	return 0;
}

u8 smbus_master_read_blockwrite(u32 smbus_fpga_baseaddr, u16 smbus_memAddr, u8 smbus_read_cmdcode, u8 smbus_readdata_buf[], u8 smbus_readdata_count)
{
	u8 smbus_readcmd01_buf[5];
	u8 smbus_readcmd_count;

	smbus_readcmd_count = (smbus_readcmd_count&0x00);
	smbus_readcmd01_buf[0] = smbus_read_cmdcode;
	smbus_readcmd01_buf[1] = 0x03;
	smbus_readcmd01_buf[2] = (smbus_memAddr&(0xFF));
	smbus_readcmd01_buf[3] = ((smbus_memAddr&(~(0xFF)))>>8);
	smbus_readcmd01_buf[4] = smbus_readdata_count;

	XIic_DynInit(smbus_fpga_baseaddr);
	usdelay(5);
	XIic_Send(smbus_fpga_baseaddr, smbus_dut_slaveaddr, smbus_readcmd01_buf, smbus_first_readcmd_count, XIIC_STOP);   //the first writen
	usdelay(10);

	return 0;
}

u8 smbus_master_get_pinstate_dut0(u32 smbus_fpga_baseaddr, u8 smbus_get_pinstatus_result)
{
	u8 i;
	u8 smbus_master_pinstatus_buf[1] = {0x00};
	u8 *smus_master_pinstatus;
	u8 smbus_get_pinstatus_ack = 0x00;

	XIic_DynInit(smbus_fpga_baseaddr);
	usdelay(5);
	XIic_Recv(smbus_fpga_baseaddr, smbus_dut_slaveaddr, smbus_master_pinstatus_buf, 1, XIIC_STOP);
	usdelay(10);

	return smbus_master_pinstatus_buf[0];
}

u8 smbus_master_get_pinstate_dut1(u32 smbus_fpga_baseaddr, u8 smbus_get_pinstatus_result)
{
	u8 i;
	u8 smbus_master_pinstatus_buf[1] = {0x00};
	u8 *smus_master_pinstatus;
	u8 smbus_get_pinstatus_ack = 0x00;

	XIic_DynInit(smbus_fpga_baseaddr);
	usdelay(5);
	XIic_Recv(smbus_fpga_baseaddr, smbus_dut_slaveaddr, smbus_master_pinstatus_buf, 1, XIIC_STOP);
	usdelay(10);

	return smbus_master_pinstatus_buf[0];
}

u8 smbus_master_get_pinstate_dut2(u32 smbus_fpga_baseaddr, u8 smbus_get_pinstatus_result)
{
	u8 i;
	u8 smbus_master_pinstatus_buf[1] = {0x00};
	u8 *smus_master_pinstatus;
	u8 smbus_get_pinstatus_ack = 0x00;

	XIic_DynInit(smbus_fpga_baseaddr);
	usdelay(5);
	XIic_Recv(smbus_fpga_baseaddr, smbus_dut_slaveaddr, smbus_master_pinstatus_buf, 1, XIIC_STOP);
	usdelay(10);

	return smbus_master_pinstatus_buf[0];
}

u8 smbus_master_get_pinstate_dut3(u32 smbus_fpga_baseaddr, u8 smbus_get_pinstatus_result)
{
	u8 i;
	u8 smbus_master_pinstatus_buf[1] = {0x00};
	u8 *smus_master_pinstatus;
	u8 smbus_get_pinstatus_ack = 0x00;

	XIic_DynInit(smbus_fpga_baseaddr);
	usdelay(5);
	XIic_Recv(smbus_fpga_baseaddr, smbus_dut_slaveaddr, smbus_master_pinstatus_buf, 1, XIIC_STOP);
	usdelay(10);

	return smbus_master_pinstatus_buf[0];
}

u8 smbus_master_read_blockread_dut0(u32 smbus_fpga_baseaddr, u8 smbus_readdata_count)
{
	u8 i;
	u8 smbus_master_block_read_result;
	u8 smbus_readcmd_count;
	u8 smbus_master_block_read_cmd_buf[60] = {0};

	u8 smbus_second_readcmd_buf[1];

	smbus_second_readcmd_buf[0] = smbus_second_readcmd;

	XIic_DynInit(smbus_fpga_baseaddr);
	usdelay(5);
	XIic_Send(smbus_fpga_baseaddr, smbus_dut_slaveaddr, smbus_second_readcmd_buf, 1, XIIC_REPEATED_START);
	usdelay(10);

    XIic_DynInit(smbus_fpga_baseaddr);
    usdelay(5);
	XIic_Recv(smbus_fpga_baseaddr, smbus_dut_slaveaddr, smbus_master_block_read_cmd_buf, smbus_readdata_count+1, XIIC_STOP);

	if(smbus_master_block_read_cmd_buf[0] == smbus_readdata_count)
	{
		smbus_master_block_read_result = 0x00;
	}
	else
	{
		smbus_master_block_read_result = 0x01;
	}

	for(i=0; i<smbus_readdata_count+1; i++)
	{
		dut0.g_pattern_smbus_control_buf[i+10] = smbus_master_block_read_cmd_buf[i+1];
	}

    return 0;
}

u8 smbus_master_read_blockread_dut1(u32 smbus_fpga_baseaddr, u8 smbus_readdata_count)
{
	u8 i;
	u8 smbus_master_block_read_result;
	u8 smbus_readcmd_count;
	u8 smbus_master_block_read_cmd_buf[60] = {0};

	u8 smbus_second_readcmd_buf[1];

	smbus_second_readcmd_buf[0] = smbus_second_readcmd;

	XIic_DynInit(smbus_fpga_baseaddr);
	usdelay(5);
	XIic_Send(smbus_fpga_baseaddr, smbus_dut_slaveaddr, smbus_second_readcmd_buf, 1, XIIC_REPEATED_START);
	usdelay(10);

    XIic_DynInit(smbus_fpga_baseaddr);
    usdelay(5);
	XIic_Recv(smbus_fpga_baseaddr, smbus_dut_slaveaddr, smbus_master_block_read_cmd_buf, smbus_readdata_count+1, XIIC_STOP);

	if(smbus_master_block_read_cmd_buf[0] == smbus_readdata_count)
	{
		smbus_master_block_read_result = 0x00;
	}
	else
	{
		smbus_master_block_read_result = 0x01;
	}

	for(i=0;i<smbus_readdata_count+1;i++)
	{
		dut1.g_pattern_smbus_control_buf[i+10] = smbus_master_block_read_cmd_buf[i+1];
	}

    return 0;
}

u8 smbus_master_read_blockread_dut2(u32 smbus_fpga_baseaddr, u8 smbus_readdata_count)
{
	u8 i;
	u8 smbus_master_block_read_result;
	u8 smbus_readcmd_count;
	u8 smbus_master_block_read_cmd_buf[60] = {0};

	u8 smbus_second_readcmd_buf[1];

	smbus_second_readcmd_buf[0] = smbus_second_readcmd;

	XIic_DynInit(smbus_fpga_baseaddr);
	usdelay(5);
	XIic_Send(smbus_fpga_baseaddr, smbus_dut_slaveaddr, smbus_second_readcmd_buf, 1, XIIC_REPEATED_START);
	usdelay(10);

    XIic_DynInit(smbus_fpga_baseaddr);
    usdelay(5);
	XIic_Recv(smbus_fpga_baseaddr, smbus_dut_slaveaddr, smbus_master_block_read_cmd_buf, smbus_readdata_count+1, XIIC_STOP);

	if(smbus_master_block_read_cmd_buf[0] == smbus_readdata_count)
	{
		smbus_master_block_read_result = 0x00;
	}
	else
	{
		smbus_master_block_read_result = 0x01;
	}

	for(i=0;i<smbus_readdata_count+1;i++)
	{
		dut2.g_pattern_smbus_control_buf[i+10] = smbus_master_block_read_cmd_buf[i+1];
	}

    return 0;
}

u8 smbus_master_read_blockread_dut3(u32 smbus_fpga_baseaddr, u8 smbus_readdata_count)
{
	u8 i;
	u8 smbus_master_block_read_result;
	u8 smbus_readcmd_count;
	u8 smbus_master_block_read_cmd_buf[60] = {0};

	u8 smbus_second_readcmd_buf[1];

	smbus_second_readcmd_buf[0] = smbus_second_readcmd;

	XIic_DynInit(smbus_fpga_baseaddr);
	usdelay(5);
	XIic_Send(smbus_fpga_baseaddr, smbus_dut_slaveaddr, smbus_second_readcmd_buf, 1, XIIC_REPEATED_START);
	usdelay(10);

    XIic_DynInit(smbus_fpga_baseaddr);
    usdelay(5);
	XIic_Recv(smbus_fpga_baseaddr, smbus_dut_slaveaddr, smbus_master_block_read_cmd_buf, smbus_readdata_count+1, XIIC_STOP);

	if(smbus_master_block_read_cmd_buf[0] == smbus_readdata_count)
	{
		smbus_master_block_read_result = 0x00;
	}
	else
	{
		smbus_master_block_read_result = 0x01;
	}

	for(i=0;i<smbus_readdata_count+1;i++)
	{
		dut3.g_pattern_smbus_control_buf[i+10] = smbus_master_block_read_cmd_buf[i+1];
	}

    return 0;
}


