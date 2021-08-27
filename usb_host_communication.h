#ifndef USB_HOST_COMMUNICATION_H_
#define USB_HOST_COMMUNICATION_H_

#define 	USB_HOST_ADDR	0x30
#define 	FIRST_TEST		0x01
#define 	SECOND_TEST		0x02
#define 	FINISH_TEST		0x03

u8 i2c_send_vendorcmd(u32 i2c_fpga_baseaddr, u8 slave_addr, u8 write_cmd);
u8 i2c_get_result_dut0(u32 i2c_fpga_baseaddr, u8 slave_addr);
u8 i2c_get_result_dut1(u32 i2c_fpga_baseaddr, u8 slave_addr);
u8 i2c_get_result_dut2(u32 i2c_fpga_baseaddr, u8 slave_addr);
u8 i2c_get_result_dut3(u32 i2c_fpga_baseaddr, u8 slave_addr);

#endif
