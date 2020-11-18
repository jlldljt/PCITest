#include <stdio.h> 
#include <string.h> 
#include <iostream>
#include "conio.h"
#include "lusb0_usb.h"
#pragma comment(lib, "libusb.lib")

#define m_dev_VID	0x0483        /* Vendor ID of the m_dev         */ 
#define m_dev_PID	0x5740        /* Product ID of the m_dev        */ 
#define EP1_OUT_SIZE	64	      //可根据设备修改大小
#define EP1_IN_SIZE	64

int main(int argc, char* argv[])
{
	struct usb_device* m_dev = NULL;
	struct usb_dev_handle* m_dev_handle;
	char str[64];
	memset(str, 0, sizeof(str));
	//调用 void usb_init(void); 进行初始化
	usb_init();
	//调用usb_find_busses、usb_find_devices和usb_get_busses这三个函数，获得已找到的USB总线序列；然后通过链表遍历所有的USB设备，根据已知的要打开USB设备的ID（VID/PID），找到相应的USB设备
	usb_find_busses();
	usb_find_devices();

	struct usb_bus* bus;
	for (bus = usb_get_busses(); bus; bus = bus->next)
	{
		struct usb_device* dev;
		for (dev = bus->devices; dev; dev = dev->next)
		{
			if (dev->descriptor.idVendor == m_dev_VID && dev->descriptor.idProduct == m_dev_PID)
				m_dev = dev;
		}
	}
	if (!m_dev)
	{
		printf("m_dev not found\n");
		return 1;
	}
	//调用usb_open函数打开该USB设备
	m_dev_handle = usb_open(m_dev);
	if (!m_dev_handle)
	{
		printf("Could not open m_dev\n");
		return 1;
	}
	printf("设备打开成功!\n");
	//设置配置 
	if (usb_set_configuration(m_dev_handle, 1) < 0)
	{
		printf("Could not set configuration\n");
		usb_close(m_dev_handle);
		return 1;
	}
	// 注册与操作系统通信的接口，这个函数必须被调用，因为只有注册接口，才能做相应的操作
	if (usb_claim_interface(m_dev_handle, 0) < 0) //claim_interface 0 注册与操作系统通信的接口 0
	{
		printf("Could not claim interface\n");
		usb_close(m_dev_handle);
		return 1;
	}


	//-----获取端点的地址-----------------------------
	struct usb_endpoint_descriptor* endpoint = m_dev->config->interface->altsetting->endpoint;
	//int ep = m_dev->config->interface->altsetting->endpoint->bEndpointAddress;
	int ep = endpoint[2].bEndpointAddress;
	int EP_IN = 0x81;
	int EP_OUT = 0x02;
	//if (ep > 0x0f)
	//{
	//	EP_IN = ep;
	//	EP_OUT = ep - 0x80;
	//}
	//else
	//{
	//	EP_OUT = ep;
	//	EP_IN = ep + 0x80;
	//}

	printf("EP_IN: 0x%02x , EP_OUT: 0x%02x \n", EP_IN, EP_OUT);
	//------------------------------------------------------------

	char WriteTestData[2048] = { 0 };
	char ReadTestData[2048] = { 0 };
	//for (int i = 0; i < 2048; i++)
	//{
	//	WriteTestData[i] = i;
	//}
	sprintf(WriteTestData, "am host A B C D E F G A B C D E F G A B C D E F G A B C D E F G A B C D E F G A B C D E F G A B C D E F G A B C D E F G\r\n");
	//端点1写入数据
	int ret;
	int wr_size = 299;
	// usblib的bulk int可互换
	ret = usb_interrupt_write(m_dev_handle, EP_OUT, WriteTestData, wr_size/*EP1_OUT_SIZE*/, 500);// 
	if (ret != wr_size/*EP1_OUT_SIZE*/)
	{
		printf("端点1写入数据失败! %d\n", ret);
		return 1;
	}
	else
	{
		printf("端点1写入数据成功!\n");
	}

	//端点1读取数据 // 只能读取下面prepare大小相同的长度，或者超时返回错误，垃圾函数
	memset(ReadTestData, 0, 2048);
	//ret = usb_interrupt_read(m_dev_handle, 0X82, ReadTestData, 64, 500);
	ret = usb_bulk_read/*usb_bulk_read*/(m_dev_handle, EP_IN, ReadTestData, wr_size/*EP1_IN_SIZE*/, 500);
	//if (ret  != EP1_IN_SIZE)
	if (ret <0)
	{
		printf("端点1读取数据失败! %d\n", ret);
		return 1;
	}
	else
	{
		printf("端点1读取数据成功! %d\n", ret);
			printf("%s ", ReadTestData);
		//for (int i = 0; i < EP1_IN_SIZE; i++)
		//{
		//	//printf("%02X ", ReadTestData[i]);
		//	//if (((i + 1) % 16) == 0)
		//	//{
		//	//	printf("\n");
		//	//}
		//}
		printf("\n");
	}


	/**************************************************************************/
	//注销被usb_claim_interface函数调用后的接口，释放资源，和usb_claim_interface对应使用。
	usb_release_interface(m_dev_handle, 0);		//注销接口，释放资源，和usb_claim_interface搭配使用。
	//与usb_open相对应，关闭设备，是必须调用的, 返回0成功，<0 失败
	usb_close(m_dev_handle);
	printf("\n设备关闭\n");
	system("pause");
	return 0;
}