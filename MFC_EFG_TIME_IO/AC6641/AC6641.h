
extern "C" __int32 __declspec(dllimport) __stdcall AC6641_DI(HANDLE hDevice,__int32 ionum);
extern "C" __int32 __declspec(dllimport) __stdcall AC6641_DO(HANDLE hDevice,__int32 ionum,__int32 iodata);

extern "C" HANDLE __declspec(dllimport) __stdcall AC6641_OpenDevice(__int32 DeviceNum);
extern "C" __int32 __declspec(dllimport) __stdcall AC6641_CloseDevice(HANDLE hDevice);

extern "C" __int32 __declspec(dllimport) __stdcall AC6641_SN(HANDLE hDevice);

extern "C" __int32 __declspec(dllimport) __stdcall ac6641_inb(HANDLE hDevice,__int32 adr);
extern "C" __int32 __declspec(dllimport) __stdcall ac6641_outb(HANDLE hDevice,__int32 adr,__int32 data);

extern "C" __int32 __declspec(dllimport) __stdcall AC6641_SetIoMode(HANDLE hDevice,__int32 iomode0,__int32 iomode1);
extern "C" __int32 __declspec(dllimport) __stdcall AC6641_GetIoMode(HANDLE hDevice);