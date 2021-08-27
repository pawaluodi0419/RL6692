#include "platform.h"

u8 i2c_send_vendorcmd(u32 i2c_fpga_baseaddr, u8 slave_addr, u8 write_cmd)
{
	u8 i2c_usb_host_slaveaddr;
	u8 write_databuf[5] = {0};

	write_databuf[0] = 0x55;
	write_databuf[1] = write_cmd;
	//xil_printf("write_databuf =%x,%x\r\n",write_databuf[0],write_databuf[1]);

	i2c_usb_host_slaveaddr = slave_addr;

	XIic_DynInit(i2c_fpga_baseaddr);
	usdelay(5);
	XIic_Send(i2c_fpga_baseaddr, i2c_usb_host_slaveaddr, write_databuf, 2, XIIC_STOP);
	usdelay(10);
	//xil_printf("write_databuf =%x,%x\r\n",write_databuf[0],write_databuf[1]);

	return 0;
}

u8 i2c_get_result_dut0(u32 i2c_fpga_baseaddr, u8 slave_addr)
{
	u8 i;
	u8 i2c_usb_host_slaveaddr;
	u8 get_result_databuf[5] = {0};

	i2c_usb_host_slaveaddr = slave_addr;

	XIic_DynInit(i2c_fpga_baseaddr);
	usdelay(5);
	XIic_Recv(i2c_fpga_baseaddr, i2c_usb_host_slaveaddr, get_result_databuf, 5, XIIC_STOP);

	for(i=0;i<5;i++)
	{
		dut0.g_i2cRecBuf[i] = get_result_databuf[i];
	}

    return 0;
}

u8 i2c_get_result_dut1(u32 i2c_fpga_baseaddr, u8 slave_addr)
{
	u8 i;
	u8 i2c_usb_host_slaveaddr;
	u8 get_result_databuf[5] = {0};

	i2c_usb_host_slaveaddr = slave_addr;

	XIic_DynInit(i2c_fpga_baseaddr);
	usdelay(5);
	XIic_Recv(i2c_fpga_baseaddr, i2c_usb_host_slaveaddr, get_result_databuf, 5, XIIC_STOP);

	for(i=0;i<5;i++)
	{
		dut1.g_i2cRecBuf[i] = get_result_databuf[i];
	}

    return 0;
}

u8 i2c_get_result_dut2(u32 i2c_fpga_baseaddr, u8 slave_addr)
{
	u8 i;
	u8 i2c_usb_host_slaveaddr;
	u8 get_result_databuf[5] = {0};

	i2c_usb_host_slaveaddr = slave_addr;

	XIic_DynInit(i2c_fpga_baseaddr);
	usdelay(5);
	XIic_Recv(i2c_fpga_baseaddr, i2c_usb_host_slaveaddr, get_result_databuf, 5, XIIC_STOP);

	for(i=0;i<5;i++)
	{
		dut2.g_i2cRecBuf[i] = get_result_databuf[i];
	}

    return 0;
}

u8 i2c_get_result_dut3(u32 i2c_fpga_baseaddr, u8 slave_addr)
{
	u8 i;
	u8 i2c_usb_host_slaveaddr;
	u8 get_result_databuf[5] = {0};

	i2c_usb_host_slaveaddr = slave_addr;

	XIic_DynInit(i2c_fpga_baseaddr);
	usdelay(5);
	XIic_Recv(i2c_fpga_baseaddr, i2c_usb_host_slaveaddr, get_result_databuf, 5, XIIC_STOP);

	for(i=0;i<5;i++)
	{
		dut3.g_i2cRecBuf[i] = get_result_databuf[i];
	}

    return 0;
}





