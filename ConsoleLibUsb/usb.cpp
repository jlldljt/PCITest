#include "usb.h"
#include <stdio.h>

usb::usb()
{
	handle = NULL;
	ctx = NULL;
}

usb::~usb()
{
	Close();
}

int usb::Open(UINT16 VID, UINT16 PID)
{
	int ret, i;
	ssize_t cnt;

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
	return 0;
error:

	libusb_close(handle);
	libusb_exit(ctx);
	return -1;
}

void usb::Close(void)
{
	int ret;
	if (handle != NULL)
	{
		ret = libusb_release_interface(handle, 0); //release the claimed interface
		if (ret != 0) {
			printf("Cannot Release Interface");
		}
		printf("Released Interface");
		libusb_close(handle);
		libusb_exit(ctx);
		handle = NULL;
	}
}

int usb::Write(char* buf, int buf_len, int timeout, unsigned char ep)
{
	int actual_len = 0;
	int ret = libusb_bulk_transfer(handle, ep, (unsigned char*)buf, buf_len, &actual_len, timeout);
	return actual_len;
}

int usb::Read(char* buf, int buf_len, int timeout, unsigned char ep)
{
	int ret;
	int actual_len = 0;
	int shift = 0;

	while (shift < buf_len)
	{

		ret = libusb_bulk_transfer(handle, ep, (unsigned char*)buf + shift, buf_len - shift, &actual_len, timeout);
		if (ret < 0)
		{
			break;
		}
		else
		{
			shift += actual_len;
			//timeout = 10;
		}
	}

	return shift;
}
