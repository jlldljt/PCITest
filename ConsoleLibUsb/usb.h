#pragma once
#include "libusb.h"
#include "../Common/common.h"
using namespace ns_common;

class usb
{
public:
	libusb_device** devs, * dev;
	struct libusb_device_handle* handle;
	libusb_context* ctx;
	struct libusb_device_descriptor desc;
public:
	usb();
	~usb();
	virtual int Open(UINT16 VID, UINT16 PID);
	virtual void Close(void);
	virtual int Write(char* buf, int buf_len, int timeout = 0, unsigned char ep = 0x02);
	virtual int Read(char* buf, int buf_len, int timeout = 100, unsigned char ep = 0x81);



};

