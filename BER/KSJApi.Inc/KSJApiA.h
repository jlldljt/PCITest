#ifndef __KSJ_API_A_H__
#define __KSJ_API_A_H__


#ifdef KSJAPI_EXPORTS
    #define KSJ_API __declspec(dllexport)
#else
    #define KSJ_API __declspec(dllimport)
#endif

/*-----------------------------------------------------------------------------------------------------
	Supper Command for Internal only. 
  ====================================================================================================*/
#ifdef __cplusplus
extern "C"{
#endif

KSJ_API  int __stdcall KSJ_WrFx2Reg( int nChannel, unsigned short uRegAdress, unsigned char btValue  );
KSJ_API  int __stdcall KSJ_RdFx2Reg( int nChannel, unsigned short uRegAdress, unsigned char *pbtValue  );

KSJ_API  int __stdcall KSJ_WrFx2Sfr( int nChannel, unsigned short uRegAdress, unsigned char btValue  );
KSJ_API  int __stdcall KSJ_RdFx2Sfr( int nChannel, unsigned short uRegAdress, unsigned char *pbtValue  );

KSJ_API  int __stdcall KSJ_ResetFx2( int nChannel, bool bReset );

// Use this command， User can Write/Read to device rom. 
// Note: we use KSJ_WrEEPROM( m_nDeviceCurSel, 0x1fff, m_btIndex  ) to write device Index to rom address 0x1fff, this will get by KSJ_DeviceGetInformation filled to third parameter. So this address's data is your camera's index .
// Note: Beause these is firmware in rom's lower address about 6K bytes and total rom size is 8K bytes, user call KSJ_WrEEPROM and uRomAdress should not less than 0x1800!!!!!!! 
// To ensure safety, uRomAdress range is 0x1E00 - 0x1ffe, 511 Bytes can be used by user zone.
 
// 对于24LC64,寻址空间为为0x0000-0x1FFF（8K字节），对于24LC256, 寻址空间为0x0000-0X7FFF(32K字节）
KSJ_API  int __stdcall KSJ_WrEEPROM( int nChannel, unsigned short uRomAdress, unsigned char btValue  );
KSJ_API  int __stdcall KSJ_RdEEPROM( int nChannel, unsigned short uRomAdress, unsigned char *pbtValue  );

// 对于U3C相机，使用24C1024, 寻址空间为0x00000-0x1FFFF（128K字节）, 需要4个字节的地址值
KSJ_API  int __stdcall KSJ_WrEEPROMEx( int nChannel, unsigned long ulRomAdress, unsigned char btValue  );
KSJ_API  int __stdcall KSJ_RdEEPROMEx( int nChannel, unsigned long ulRomAdress, unsigned char *pbtValue  );

KSJ_API  int __stdcall KSJ_WrSensorReg( int nChannel, unsigned char btRegAdress, unsigned short wValue  );
KSJ_API  int __stdcall KSJ_RdSensorReg( int nChannel, unsigned char btRegAdress, unsigned short *pwValue  );

KSJ_API  int __stdcall KSJ_WrSensorRegX( int nChannel, unsigned short wRegAdress, unsigned short wValue  );
KSJ_API  int __stdcall KSJ_RdSensorRegX( int nChannel, unsigned short wRegAdress, unsigned short *pwValue  );

KSJ_API  int __stdcall KSJ_WrAD9923A( int nChannel, unsigned short wRegAddress, unsigned long dwValue );


//
// 发送控制命令，各字节符合USB协议
//
// MSB( IoBlockEx.uOffset ) = SETUPDAT[3], LSB( IoBlockEx.uOffset ) =  SETUPDAT[2] 
// MSB( IoBlockEx.uIndex )  = SETUPDAT[5], LSB( IoBlockEx.uIndex )  =  SETUPDAT[4] 
// MSB( IoBlockEx.uLength ) = SETUPDAT[7], LSB( IoBlockEx.uLength ) =  SETUPDAT[6] 
// wValue  = uOffset
// wIndex  = uIndex
// wLength = uLength
#pragma pack ( 1 )
typedef struct _tagUSBCmd
{
	unsigned char  bmRequest;
	unsigned char  bRequest;
	unsigned short wValue;
	unsigned short wIndex;
	unsigned short wLength;					// 表示发送或接受数据的长度
}USBCMD, *PUSBCMD;

#pragma pack ( 8 )

KSJ_API  int __stdcall KSJ_VendorCmd( int nChannel, PUSBCMD pUsbCmd, unsigned char *pBuf );

#ifdef __cplusplus
}
#endif

#endif