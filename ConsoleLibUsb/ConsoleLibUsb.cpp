// ConsoleLibUsb.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
// 注意本项目要x64

#include <iostream>
#include <stdio.h>
#include "usb.h"
#include "libusb.h"
/* 获取相同pidvid下的某个序列号 */
libusb_device_handle* libusb_open_device_with_serialnumber(
	libusb_context* ctx, const char* serial_number) {
	struct libusb_device** devs;
	struct libusb_device* dev;
	struct libusb_device_handle* handle = NULL;
	char string[128];
	char serialnumber[128] = "";
	uint8_t string_index[3];
	size_t i = 0;
	int r = 0;

	if (serial_number)
		strcpy_s(serialnumber, serial_number);

	if (libusb_get_device_list(ctx, &devs) < 0)
		return NULL;

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		r = libusb_get_device_descriptor(dev, &desc);

		if (r < 0)
			goto out;

		string_index[2] = desc.iSerialNumber;
		r = libusb_open(dev, &handle);

		if (r < 0) {
			handle = NULL;
			break;
		}

		if (libusb_get_string_descriptor_ascii(handle, string_index[2], (unsigned char*)string, 128) >= 0) {
			if (!strcmp(serialnumber, string))
				break;
		}

		libusb_close(handle);
		handle = NULL;
	}

out:
	libusb_free_device_list(devs, 1);
	return handle;
}

/* 打印usb设备 */
static void print_devs(libusb_device** devs)
{
	libusb_device* dev;
	int i = 0, j = 0;
	uint8_t path[8];

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}

		printf("%04x:%04x (bus %d, device %d)",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev), libusb_get_device_address(dev));

		r = libusb_get_port_numbers(dev, path, sizeof(path));
		if (r > 0) {
			printf(" path: %d", path[0]);
			for (j = 1; j < r; j++)
				printf(".%d", path[j]);
		}
		printf("\n");
	}
}

//int main1()
//{
//	libusb_device** devs;
//	int r;
//	ssize_t cnt;
//
//	r = libusb_init(NULL);
//	if (r < 0)
//		return r;
//
//	cnt = libusb_get_device_list(NULL, &devs);
//	if (cnt < 0)
//		return (int)cnt;
//
//	print_devs(devs);
//	libusb_free_device_list(devs, 1);
//
//	system("pause");
//	//std::cout << "Hello World!\n";
//	libusb_exit(NULL);
//	return 0;
//}


#define VID 0x0483
#define PID 0x5740

#define EP_IN 0x81
#define EP_OUT 0x02

int main1(void)
{
	libusb_device** devs, * dev;
	int ret, i;
	ssize_t cnt;
	//usb_pro_t usb_pro;
	struct libusb_device_handle* handle = NULL;
	libusb_context* ctx = NULL;
	struct libusb_device_descriptor desc;
#define DATA_LEN 2048
	char data[DATA_LEN];
	int actual_len = 0;
	int didi = 1000;

	ret = libusb_init(&ctx);
	if (ret < 0)
		return -1;

	libusb_set_debug(ctx, 3);

	cnt = libusb_get_device_list(ctx, &devs);
	if (cnt < 0) {
		printf("no usb dev on bus\r\n");
		return  -1;
	}

	i = 0;
	while ((dev = devs[i++]) != NULL) {

		ret = libusb_get_device_descriptor(dev, &desc);
		if (ret < 0) {
			printf("failed to get device descriptor");
			goto error;
		}

		if ((desc.idVendor == VID) && (desc.idProduct == PID)) {
			printf("bLength: 0x%04x\r\n", desc.bLength);
			printf("bDescriptorType: 0x%04x\r\n", desc.bDescriptorType);
			printf("bcdUSB: 0x%04x\r\n", desc.bcdUSB);
			printf("bDeviceClass: 0x%04x\r\n", desc.bDeviceClass);
			printf("bDeviceSubClass: 0x%04x\r\n", desc.bDeviceSubClass);
			printf("bDeviceProtocol: 0x%04x\r\n", desc.bDeviceProtocol);
			printf("bMaxPacketSize0: 0x%04x\r\n", desc.bMaxPacketSize0);
			printf("vendor id: 0x%04x\r\n", desc.idVendor);
			printf("product id: 0x%04x\r\n", desc.idProduct);
			printf("bcdDevice: 0x%04x\r\n", desc.bcdDevice);
			printf("iManufacturer: 0x%04x\r\n", desc.iManufacturer);
			printf("iProduct: 0x%04x\r\n", desc.iProduct);
			printf("iSerialNumber: 0x%04x\r\n", desc.iSerialNumber);
			printf("bNumConfigurations: 0x%04x\r\n", desc.bNumConfigurations);

		}

	}

	handle = libusb_open_device_with_vid_pid(ctx, VID, PID);

	if (handle == NULL) {
		printf("cant't open device\r\n");
		goto error;
	}
	else {
		printf("open device\r\n");
	}

	libusb_free_device_list(devs, 1);

	printf("free device list\r\n");

	if (libusb_kernel_driver_active(handle, 0) == 1) {
		printf("kernel driver active, detach it \r\n");

		if (libusb_detach_kernel_driver(handle, 0) == 0) {
			printf("detached kernel driver\r\n");
		}
		else {
			goto error;
		}
	}
	//ret = libusb_set_configuration(handle, 1);
	//if (ret < 0) {
	//    printf("can't libusb_set_configuration\r\n");
	//    goto error;
	//}
	//else {
	//    printf("libusb_set_configuration\r\n");
	//}

	ret = libusb_claim_interface(handle, 0);
	if (ret < 0) {
		printf("can't claim interface\r\n");
		goto error;
	}
	else {
		printf("claimed interface\r\n");
	}

	// send
	memset(data, 0, sizeof(data));
	printf("输入发送字符串\r\n");
	gets_s(data, sizeof(data));
	//sprintf(data, "am host %daaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\r\n", i);

	ret = libusb_bulk_transfer(handle, EP_OUT, (unsigned char*)data, strlen(data), &actual_len, 100);

	if (actual_len != 0)
	{
		printf("send data: %s\r\n", data);
	}
	system("pause");
	// recv
	memset(data, 0, sizeof(data));

	int shift = 0;
	int timeout = 500;

	while (shift < DATA_LEN)
	{

		ret = libusb_bulk_transfer(handle, EP_IN, (unsigned char*)data + shift, DATA_LEN - shift, &actual_len, timeout);
		if (ret < 0)
		{
			break;
		}
		else
		{
			shift += actual_len;
			timeout = 10;
		}
	}

	if (shift)
	{
		printf("端点1读取数据成功! %d\n", shift);
		printf("%s ", data);
		printf("\n");
	}
	else
	{
		printf("端点1读取数据失败!\n");
	}

	//// unbelivable sometime it will cause stm32 stop at step tx2
	//ret = libusb_interrupt_transfer(handle, edp2in, (unsigned char*)data, DATA_LEN, &actual_len, 100);

	//if (actual_len = 0)
	//{
	//	printf("received nothing\r\n");
	//}
	//else
	//{
	//	printf("received%d: %s\r\n", ret, data);
	//}

	ret = libusb_release_interface(handle, 0); //release the claimed interface
	if (ret != 0) {
		printf("Cannot Release Interface");
		return 1;
	}
	printf("Released Interface");
error:

	libusb_close(handle);
	libusb_exit(ctx);
	system("pause");
	return 0;
}

#define DEFAULT_BUFLEN 1024

//在线升级app版3.2.3.23开始
void update_with_usb(void)
{
	char sendbuf[] = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	usb usb;

test3connect:
	//char com[20];
	//printf("输入串口，比如“COM1”\n");
	//gets_s(com, 20);
	//int isOta = 0;

	// Initialize
	if (usb.Open(VID,PID))
		exit(0);

	printf("Successed connect to VID %x PID %x!\n", VID, PID);

	char function = 0;

	printf("输入功能码：\n");
	printf("0————下载脚本\n");
	printf("1————下载固件\n");
	printf("A————更新BT\n");
	printf("C————更新COP\n");
	printf("其他————退出\n");

	int ret = getchar();

	if (ret == '0')
		function = 0;
	else if (ret == '1')
		function = 0x20;
	else if (ret == 'A')
		function = 0xA0;
	else if (ret == 'C')
		function = 0xC0;
	else
		exit(0);

	ret = getchar();//接收回车

	//getchar();
	// 获取文件
	char path[MAX_PATH];
	printf("输入文件目录\n");
	gets_s(path, MAX_PATH);

	wchar_t* wpath = CharToWchar(path);


	/////////////////////////////////////////////////////////////////////
	HANDLE hFile = ::CreateFile(wpath,     //创建文件的名称。
		GENERIC_READ,          // 写和读文件。
		FILE_SHARE_READ,                      // 不共享读写。
		NULL,                   // 缺省安全属性。
		OPEN_EXISTING,          // 如果文件存在，也创建。
		FILE_ATTRIBUTE_NORMAL, // 一般的文件。       
		NULL);                 // 模板文件为空。
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(TEXT("CreateFile fail!\r\n"));
	}

	//////////
	//发送命令
	//////////


	LARGE_INTEGER size = { 0 };
	if (!::GetFileSizeEx(hFile, &size))
	{
		OutputDebugString(TEXT("获取文件失败\r\n"));;
	}

	printf("File(%s) Size: %ld\n", wpath, size);

	//size.QuadPart = size.QuadPart + 2;
	delete wpath;
	Sleep(500);//getchar();
	  //crc
	//UCHAR uchBuf[20] = { 0xF3, 0x06, 0x6C, 0x81, 0x03, 0x00 };

	UCHAR uchBuf[20] = { 0xF1, 0x10, 0x00, 0x00, 0x01, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uchBuf[7] = function;
	memcpy_s(uchBuf + 8, sizeof(uchBuf), &size, 4);
	uint16_t crc;
	crc = getCRC16(uchBuf, 16);
	memcpy_s(uchBuf + 16, sizeof(uchBuf), &crc, 2);

	// Send an initial buffer
	iResult = usb.Write((char*)uchBuf, 18);

	if (iResult != 18)
	{
		printf("send failed\n");
		return;
	}

	// 等待接收初始化完成
	iResult = usb.Read(recvbuf, recvbuflen, 2000);

	if (iResult > 0) {
		//recvbuf[iResult] = 0x00;
		for (int i = 0; i < iResult; i++) {
			printf("%02x ", (UINT8)recvbuf[i]);
		}
		printf("\n");
	}
	else if (iResult == 0)
		printf("recv nothing\n");
	else
		printf("recv failed\n");

	Sleep(300); //getchar();
	  //////////
	  //发送脚本
	  //////////
#define READ_LEN 0x100
	char chBuffer[READ_LEN];
	crc = 0xFFFF;
	DWORD dwReadSize = 0;
	LONG lDistance = 0;

	::ZeroMemory(chBuffer, READ_LEN);

	lDistance = 0;
	while (1)// 发送
	{
		BOOL bRet = ::ReadFile(hFile, chBuffer, READ_LEN, &dwReadSize, NULL);
		if (bRet && dwReadSize)
		{
			//OutputDebugString(TEXT("ReadFile 读文件成功\r\n"));
		}
		else
		{
			//获取出错码。
			DWORD dwError = GetLastError();
			//处理出错。            
			TCHAR chErrorBuf[1024];
			wsprintf(chErrorBuf, TEXT("GetLastError()=%d\r\n"), dwError);
			OutputDebugString(chErrorBuf);
			break;
		}
		for (int i = 0; i < dwReadSize; i++) {
			crc = oneCRC16(chBuffer[i], crc);
		}
		lDistance += dwReadSize;
		DWORD dwPtr = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
		if (dwPtr == INVALID_SET_FILE_POINTER &&
			GetLastError() != NO_ERROR) // Test for failure
		{
			// Obtain the error code. 
			DWORD dwError = GetLastError();

			// Deal with failure 
			// . . . 

		} // End of error handler 

		Sleep(10);
		if (bRet)
		{
			//OutputDebugString(TEXT("ReadFile 读文件成功\r\n"));
			// Send
			iResult = usb.Write(chBuffer, dwReadSize);

			if (iResult != dwReadSize)
			{
				printf("send failed\n");
				printf("Bytes Sent: %ld\n", lDistance);
				return;
			}
		}
	}
	//Sleep(10);
	// Send crc
	// Send
	iResult = usb.Write((char*)&crc, 2);

	if (iResult != 2)
	{
		printf("send crc failed\n");
		printf("Bytes Sent: %ld\n", lDistance);
		return;
	}

	lDistance += 2;

	printf("CRC: %x\n", (UINT16)crc);
	printf("Bytes Sent: %ld\n", lDistance);

	////////////////////////////////////////////////

	iResult = usb.Read(recvbuf, recvbuflen);

	if (iResult > 0) {
		//recvbuf[iResult] = 0x00;
		for (int i = 0; i < iResult; i++) {
			printf("%02x ", (UINT8)recvbuf[i]);
		}
		printf("\n");
	}
	else if (iResult == 0)
		printf("recv nothing\n");
	else
		printf("recv failed\n");

	CloseHandle(hFile);
	usb.Close();

	printf("OVER");
	getchar();

	printf("继续升级？ y/n \n");
	ret = getchar();
	if (ret == 'y') {
		getchar();
		goto test3connect;
	}
	return;
}

//测试收发
void test1(void)
{
	char sendbuf[] = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	usb usb;

	//UCHAR uchBuf[] = "CDV INF";
	UCHAR uchBuf[8] = { 0x01, 0x03 , 0x00 , 0x0F , 0x00 , 0x6F , 0x35 , 0xE5 };
	// Send an initial buffer
	DWORD t1, t2;
	//char com[20];
	//printf("输入串口，比如“COM1”\n");
	//gets_s(com, 20);
	//int isOta = 0;

test1_start:
	// Initialize
	if (usb.Open(VID, PID))
		exit(0);

	printf("Successed connect to VID %x PID %x!\n", VID, PID);

	//////////
	//发送命令
	//////////
	Sleep(500);//getchar();
	  //crc
	//UCHAR uchBuf[20] = { 0xF3, 0x06, 0x6C, 0x81, 0x03, 0x00 };

test1_next:
	t1 = GetTickCount();
	iResult = usb.Write((char*)uchBuf, sizeof(uchBuf));

	if (iResult != sizeof(uchBuf))
	{
		printf("send failed\n");
		return;
	}
	Sleep(50);
	memset(recvbuf, 0, recvbuflen);
	// 等待接收初始化完成
	iResult = usb.Read(recvbuf, recvbuflen, 50);

	if (iResult > 0) {
		//printf("%s ", recvbuf);
		for (int i = 0; i < iResult; i++) {
			printf("%02x ", (UINT8)recvbuf[i]);
		}
		printf("\n");
	}
	else if (iResult == 0)
		printf("recv nothing\n");
	else
	{
		printf("recv failed\n");
		usb.Close();
		goto test1_start;
		return;
	}
	t2 = GetTickCount();
	printf("Use Time:%d ms\n", (t2 - t1));
	//Sleep(300); //getchar();
	goto test1_next;
	usb.Close();
	return;
}
// 注意本项目要x64
int main()
{
	test1();
	//update_with_usb();
	printf("over");
	getchar();
	return 1;
}