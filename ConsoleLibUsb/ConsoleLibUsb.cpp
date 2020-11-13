// ConsoleLibUsb.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>


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
		strcpy(serialnumber, serial_number);

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

int main1()
{
	libusb_device** devs;
	int r;
	ssize_t cnt;

	r = libusb_init(NULL);
	if (r < 0)
		return r;

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0)
		return (int)cnt;

	print_devs(devs);
	libusb_free_device_list(devs, 1);

	system("pause");
	//std::cout << "Hello World!\n";
	libusb_exit(NULL);
	return 0;
}


#define VID 0x0483
#define PID 0x5740

#define edp2in 0x82
#define edp2out 0x02

int main(void)
{
    libusb_device** devs, * dev;
    int ret, i;
    ssize_t cnt;
    //usb_pro_t usb_pro;
    struct libusb_device_handle* handle = NULL;
    libusb_context* ctx = NULL;
    struct libusb_device_descriptor desc;

    char data[64];
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

    for (int i = 0; i < 1000; i++) {
        memset(data, 0, sizeof(data));
        /*  receive data from device  */
        /*
        ret = libusb_bulk_transfer(handle, edp2in, data, 64, &actual_len, 0);

        if (actual_len = 0) {
            printf("received nothing\r\n");
        } else {
            printf("bulk transfer: %s\r\n", data);
        }

        usleep(200000);
        */

        //char* str = "am host";

        sprintf(data, "am host %d\r\n", i);

        ret = libusb_bulk_transfer(handle, edp2out, (unsigned char*)data, strlen(data), &actual_len, 0);

        if (actual_len != 0) {
            printf("send data: %s\r\n", data);
        }

        Sleep(200);
    }

    libusb_close(handle);

error:

    printf("free device list\r\n");
    libusb_free_device_list(devs, 1);

    libusb_exit(NULL);

    return 0;
}