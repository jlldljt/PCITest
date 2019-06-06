#ifndef __DATAPATH_RGBDEFS_INCLUDED_
#define __DATAPATH_RGBDEFS_INCLUDED_

/*有效参数范围标识*/
#define RGBCAPTURE_FLAG_CURRENT                    0x00000000    //表示当前的采集参数
#define RGBCAPTURE_FLAG_REAL_WORLD                 0x04000000    //用于将标志的注册值转化为实际值
//not used or useless
//////////////////////////////////////////////////////////////////////////
#define RGBCAPTURE_FLAG_DETECT                     0x08000000    //表示检测的采集参数
#define RGBCAPTURE_FLAG_MINIMUM                    0x10000000    //表示最小的采集参数  
#define RGBCAPTURE_FLAG_DEFAULT                    0x20000000    //表示默认的采集参数
#define RGBCAPTURE_FLAG_MAXIMUM                    0x40000000    //表示最大的采集参数
#define RGBCAPTURE_FLAG_TEST                       0x80000000    //表示测试的采集参数
//////////////////////////////////////////////////////////////////////////




/*采集参数标志*/
#define RGBCAPTURE_PARM_VDIF_HTIMINGS              0x00000001    //对应采集参数：HorOffset(水平位置)
#define RGBCAPTURE_PARM_VDIF_VTIMINGS              0x00000002    //对应采集参数：HorOffset(垂直位置)
#define RGBCAPTURE_PARM_VDIF_DESCRIPTION           0x00000004    //对应采集参数：VideoTimings.Description(图象描述)
#define RGBCAPTURE_PARM_PHASE                      0x00000008    //对应采集参数：Phase(相位)
#define RGBCAPTURE_PARM_BRIGHTNESS                 0x00000010    //对应采集参数：Brightness(亮度)
#define RGBCAPTURE_PARM_BRIGHTNESS_R               0x00000020    //对应采集参数：Brightness(亮度)
#define RGBCAPTURE_PARM_BRIGHTNESS_G               0x00000040    //对应采集参数：Brightness(亮度)
#define RGBCAPTURE_PARM_BRIGHTNESS_B               0x00000080    //对应采集参数：Brightness(亮度)
#define RGBCAPTURE_PARM_CONTRAST                   0x00000100    //对应采集参数：Contrast(对比度)
#define RGBCAPTURE_PARM_CONTRAST_R                 0x00000200    //对应采集参数：Contrast(对比度)
#define RGBCAPTURE_PARM_CONTRAST_G                 0x00000400    //对应采集参数：Contrast(对比度)
#define RGBCAPTURE_PARM_CONTRAST_B                 0x00000800    //对应采集参数：Contrast(对比度)

#define RGBCAPTURE_PARM_HWND                       0x00001000    //对应采集参数：HWnd(接收消息窗口句柄)
#define RGBCAPTURE_PARM_DATAMODE                   0x00002000    //对应采集参数：datamode(数据模式：rgb 或 yuv)
#define RGBCAPTURE_PARM_NOTIFY_ARG                 0x00004000    //对应采集参数：NotifyArg(采集事件指针)
#define RGBCAPTURE_PARM_NOTIFY_FN                  0x00008000    //对应采集参数：NotifyFn(回调函数指针)
#define RGBCAPTURE_PARM_NOTIFY_FLAGS               0x00010000    //对应采集参数：NotifyFlags(通知消息)
#define RGBCAPTURE_PARM_PLL_DIVIDER                0x02000000    //对应采集参数：NotifyFlags(通知消息)
#define RGBCAPTURE_PARM_DETECT_FLAG                0x04000000    //对应采集参数：NotifyFlags(通知消息)
#define RGBCAPTURE_PARM_GRAYTEST                   0x08000000    //对应采集参数：graytest
//useless
//////////////////////////////////////////////////////////////////////////
#define RGBCAPTURE_PARM_INPUT                      0x00020000    //对应采集参数：Input(输入端口)
//#define RGBCAPTURE_PARM_FORMAT                     0x00000002    //对应采集参数：Format(输入信号格式)
//#define RGBCAPTURE_PARM_VDIF_CLOCKS                0x00000004    //对应采集参数：VideoTimings.PixelClock(像素时钟)
#define RGBCAPTURE_PARM_VDIF_FLAGS                 0x00040000    //对应采集参数：VideoTimings.Flags(图象标志)
//#define RGBCAPTURE_PARM_SYNCEDGE                   0x00004000    //对应采集参数：SyncEdge(同步沿)
//#define RGBCAPTURE_PARM_BLACKLEVEL                 0x00000400    //对应采集参数：BlackLevel(黑色度)     
//#define RGBCAPTURE_PARM_BLOCKING                   0x00100000    //对应采集参数：Flags(阻塞模式)
#define RGBCAPTURE_PARM_SCALED_SIZE                0x00080000    //对应采集参数：HScaled、VScaled(缩放比)
#define RGBCAPTURE_PARM_SAMPLERATE                 0x00100000    //对应采集参数：SampleRate(采样速度)
#define RGBCAPTURE_PARM_PIXELFORMAT                0x00200000    //对应采集参数：PixelFormat(像素显示模式)
#define RGBCAPTURE_PARM_VERSIONNUM                 0x00400000    //对应采集参数：versionnum(版本号)
#define RGBCAPTURE_PARM_BUFFERSNUM                 0x00800000    //对应采集参数：buffersnum(缓存个数)
#define RGBCAPTURE_PARM_FRAMESPSEC                 0x01000000    //对应采集参数：FramespSec(每秒帧数)
//////////////////////////////////////////////////////////////////////////

#define RGBCAPTURE_PARM_ALL                                             \
    (RGBCAPTURE_PARM_BRIGHTNESS_R            | RGBCAPTURE_PARM_BRIGHTNESS_G          \
   | RGBCAPTURE_PARM_BRIGHTNESS_B      | RGBCAPTURE_PARM_VDIF_FLAGS      \
   | RGBCAPTURE_PARM_VDIF_HTIMINGS    | RGBCAPTURE_PARM_VDIF_VTIMINGS   \
   | RGBCAPTURE_PARM_VDIF_DESCRIPTION | RGBCAPTURE_PARM_PHASE           \
   | RGBCAPTURE_PARM_BRIGHTNESS       | RGBCAPTURE_PARM_CONTRAST        \
   | RGBCAPTURE_PARM_CONTRAST_R       | RGBCAPTURE_PARM_SAMPLERATE      \
   | RGBCAPTURE_PARM_PIXELFORMAT      | RGBCAPTURE_PARM_HWND            \
   | RGBCAPTURE_PARM_CONTRAST_G         | RGBCAPTURE_PARM_SCALED_SIZE     \
   | RGBCAPTURE_PARM_NOTIFY_FN        | RGBCAPTURE_PARM_NOTIFY_FLAGS    \
   | RGBCAPTURE_PARM_CONTRAST_B		  | RGBCAPTURE_PARM_NOTIFY_ARG      \
   | RGBCAPTURE_PARM_VERSIONNUM		  | RGBCAPTURE_PARM_BUFFERSNUM		\
   | RGBCAPTURE_PARM_DATAMODE         |RGBCAPTURE_PARM_INPUT)

/* Notification flags returned by RGBCaptureGetCapabilities to indicate the
 * notification capabilities of the hardware.
 * The same flags are used in calls to RGBCaptureGetParameters and
 * RGBCaptureSetParameters to set and query the required notification messages.
 * Note: The RGBWM_FRAMECAPTURED message (or its FrameCapturedFn equivalent) is
 * always sent.
 */


//useless
//////////////////////////////////////////////////////////////////////////
#define RGBCAPTURE_USERDATA_MAX                   8

enum
{
   RGBFORMAT_3WIRE_SYNC_ON_GREEN,
   RGBFORMAT_4WIRE_COMPOSITE_SYNC,
   RGBFORMAT_5WIRE_SEPARATE_SYNCS,
};

enum
{
   RGBPIXELFORMAT_UNKNOWN=0,
   RGBPIXELFORMAT_555=1,
   RGBPIXELFORMAT_565=2,
   RGBPIXELFORMAT_888=3,
   RGBPIXELFORMAT_UYVY=4,
   RGBPIXELFORMAT_COUNT=5        /* Should always be the last in the list. */
};

enum
{
   RGBSYNCEDGE_LEADING,
   RGBSYNCEDGE_TRAILING,
};
//////////////////////////////////////////////////////////////////////////



//not used
//////////////////////////////////////////////////////////////////////////
#define RGBCAPTURECAPS_FLAG_INTERLACED             0x00000001
#define RGBCAPTURECAPS_FLAG_MERGE_BUFFERS          0x00000002
#define RGBCAPTURECAPS_FLAG_AUTOMATIC_SYNC         0x00000004
#define RGBCAPTURECAPS_FLAG_IMAGE_SCALING          0x00000008
#define RGBCAPTURECAPS_FLAG_NOTIFICATION           0x00000010
#define RGBCAPTURECAPS_FLAG_DETECT_INTERLACED      0x00010000
#define RGBCAPTURECAPS_FLAG_DETECT_VSYNCPOLARITY   0x00020000
#define RGBCAPTURECAPS_FLAG_DETECT_HSYNCPOLARITY   0x00040000
#define RGBCAPTURECAPS_FLAG_DETECT_RGBFORMAT       0x00080000
#define RGBCAPTURECAPS_FLAG_DETECT_VERTOTAL        0x00100000
#define RGBCAPTURECAPS_FLAG_DETECT_VERFREQUENCY    0x00200000
#define RGBCAPTURECAPS_FLAG_IMAGE_MANIPULATION     0x00400000

#define RGBCAPTURE_IMG_FLIP_VERTICAL               0x00000001
#define RGBCAPTURE_IMG_FLIP_HORIZONTAL             0x00000002
#define RGBCAPTURE_IMG_FLIP_TRANSPOSE              0x00000004

#define RGBCAPTURE_MAX_BUFFERS                  16

#define RGBCAPTURE_BUFFER_HEADER                32

//////////////////////////////////////////////////////////////////////////



/*有效通知标志*/
#define RGBNOTIFY_NO_SIGNAL                        0x00000001
#define RGBNOTIFY_MODE_CHANGE                      0x00000002


/*采集消息*/
/******************************************************************************/
#ifndef KERNEL_MODE

#define RGBWM_BASE                              WM_USER

/*
 * Message: RGBWM_FRAMECAPTURED
 *          当缓存内捕获到一帧完整数据时发出该消息
 *
 * wParam:  Not used.
 *
 * lParam:  记录当前活动的采集卡的索引值，以便多卡共同使用时进行区分消息来源
 *
 * Return:  N/A.
 */
#define RGBWM_FRAMECAPTURED                     ( RGBWM_BASE + 0x0100 )

/*
 * Message: RGBWM_NOSIGNAL
 *          当采集卡采集不到一帧完整信息时发出该消息
 *
 * wParam:  Not used.
 *
 * lParam:  记录当前活动的采集卡的索引值，以便多卡共同使用时进行区分消息来源
 *
 * Return:  N/A.
 */

#define RGBWM_NOSIGNAL                          ( RGBWM_BASE + 0x0001 )
// 无信号时的具体原因标志RGBWM_NOSIGNAL : wParam
#define NOSIGNAL_NO_RESOLUTION     0
#define NOSIGNAL_DDRCHECK_ERROR    1
#define NOSIGNAL_H_OVERFLOW        2
#define NOSIGNAL_V_OVERFLOW        3

/*
 * Message: RGBWM_MODECHANGED
 *          采集卡捕获数据时发现分辨率改发出该消息
 *
 * wParam:  Not used
 *
 * lParam:  记录当前活动的采集卡的索引值，以便多卡共同使用时进行区分消息来源
 *
 * Return:  N/A.
 */

#define RGBWM_MODECHANGED                       ( RGBWM_BASE + 0x0002 )


//not used
//////////////////////////////////////////////////////////////////////////
#define WPARAM_LINE_COUNT_MASK         (0x0000FFFF)
#define LPARAM_VERTICAL_REFRESH_MASK   (0x0003FFFF)
#define RGBWM_INTERNALERROR                     ( RGBWM_BASE + 0x0003 )
#define RGBWM_CAPTURE_ERROR                     ( RGBWM_BASE + 0x0004 )
//////////////////////////////////////////////////////////////////////////

#endif // not KERNEL_MODE




/*错误标志*/
#define RGBERROR_BASE 0x10000
typedef enum tagRGBERROR {
 /*无误*/
RGBERROR_NO_ERROR                        = 0,
//not used
//////////////////////////////////////////////////////////////////////////
/**/
RGBERROR_INVALID_VDIF_CLOCKS             =( RGBERROR_BASE + 0x000E ),
 /**/
RGBERROR_DRIVER_NOT_FOUND                =( RGBERROR_BASE + 0x0000 ),
/**/
RGBERROR_INVALID_SAMPLERATE              =( RGBERROR_BASE + 0x0016 ),
 /* While the specified parameters are valid for the device, some of the
 * resources required are already in use. */
RGBERROR_INSUFFICIENT_BUFFERS            =( RGBERROR_BASE + 0x001A ),
/**/
RGBERROR_INSUFFICIENT_MEMORY             =( RGBERROR_BASE + 0x001B ),
/**/
RGBERROR_SIGNAL_NOT_DETECTED             =( RGBERROR_BASE + 0x001C ),
/**/
RGBERROR_OLD_FIRMWARE                    =( RGBERROR_BASE + 0x001E ),
 /* Both a Window handle and a function pointer for frame capturing have
 * been specified.*/
RGBERROR_HWND_AND_FRAMECAPTUREDFN        =( RGBERROR_BASE + 0x001F ),
 /* Information error messages that the horizontal values have
 * been adjusted to conform to the hardware rules. */
RGBERROR_HSCALE_NOT_WORD_DIVISIBLE       =( RGBERROR_BASE + 0x0024 ),
/**/
RGBERROR_HSCALE_NOT_DWORD_DIVISIBLE      =( RGBERROR_BASE + 0x0025 ),
/**/
RGBERROR_HORADDRTIME_NOT_WORD_DIVISIBLE  =( RGBERROR_BASE + 0x0026 ),
/**/
RGBERROR_HORADDRTIME_NOT_DWORD_DIVISIBLE =( RGBERROR_BASE + 0x0027 ),
 /* Version error - indicates that the user DLL and driver
 * version are incompatible. */
RGBERROR_VERSION_MISMATCH                =( RGBERROR_BASE + 0x0028 ),
/**/
RGBERROR_BUFFER_NOT_VALID                =( RGBERROR_BASE + 0x002A ),
/**/
RGBERROR_BUFFERS_STILL_ALLOCATED         =( RGBERROR_BASE + 0x002B ),
/**/
RGBERROR_NO_NOTIFICATION_SET             =( RGBERROR_BASE + 0x002C ),
/**/
RGBERROR_CAPTURE_DISABLED                =( RGBERROR_BASE + 0x002D ),
/**/
RGBERROR_INVALID_PIXELFORMAT             =( RGBERROR_BASE + 0x002E ),
/**/
RGBERROR_ILLEGAL_CALL                    =( RGBERROR_BASE + 0x002F ),
 /*
 *  Error define for the mode database reader/writer.
 */
RGBERROR_MODE_NOT_FOUND                  =( RGBERROR_BASE + 0x0031 ),
/**/
RGBERROR_CANNOT_DETECT                   =( RGBERROR_BASE + 0x0032 ),
/**/
RGBERROR_NO_MODE_DATABASE                =( RGBERROR_BASE + 0x0033 ),
/**/
RGBERROR_CANT_DELETE_MODE                =( RGBERROR_BASE + 0x0034 ),
/*
*   Error codes that should have been in here from the start.
*/
RGBERROR_MUTEX_FAILURE                   =( RGBERROR_BASE + 0x0035 ),
/**/
RGBERROR_NO_COMPLETION                   =( RGBERROR_BASE + 0x0037 ),
/**/
RGBERROR_INSUFFICIENT_RESOURCES_HALLOC   =( RGBERROR_BASE + 0x0038 ),
/**/
RGBERROR_INSUFFICIENT_RESOURCES_RGBLIST  =( RGBERROR_BASE + 0x0039 ),
/**/
RGBERROR_INVALID_PITCH                   =( RGBERROR_BASE + 0x0040 ),

//////////////////////////////////////////////////////////////////////////
/*分配内存资源失败*/
RGBERROR_UNABLE_TO_LOAD_DRIVER           =( RGBERROR_BASE + 0x0001 ),
/*没有找到VGA视频采集卡*/
RGBERROR_HARDWARE_NOT_FOUND              =( RGBERROR_BASE + 0x0002 ),
/*无效的索引*/
RGBERROR_INVALID_INDEX                   =( RGBERROR_BASE + 0x0003 ),
/*设备已被占用*/
RGBERROR_DEVICE_IN_USE                   =( RGBERROR_BASE + 0x0004 ),
/*设备未被使用*/
RGBERROR_INVALID_HRGBCAPTURE             =( RGBERROR_BASE + 0x0005 ),
/*错误的指针*/
RGBERROR_INVALID_POINTER                 =( RGBERROR_BASE + 0x0006 ),
/*错误的配置参数大小*/
RGBERROR_INVALID_SIZE                    =( RGBERROR_BASE + 0x0007 ),
 /* 不支持的 ulFlag*/
RGBERROR_INVALID_FLAGS                   =( RGBERROR_BASE + 0x0008 ),
 /* 打开设备失败 */
RGBERROR_INVALID_DEVICE                  =( RGBERROR_BASE + 0x0009 ),
/*不支持的或设置失败 输入端口*/
RGBERROR_INVALID_INPUT                   =( RGBERROR_BASE + 0x000A ),
/*不支持的输入信号格式*/
RGBERROR_INVALID_FORMAT                  =( RGBERROR_BASE + 0x000D ),
/*不支持或设置失败相位*/
RGBERROR_INVALID_PHASE                   =( RGBERROR_BASE + 0x0011 ),
/*不支持或设置失败亮度*/
RGBERROR_INVALID_BRIGHTNESS              =( RGBERROR_BASE + 0x0013 ),
/*不支持或设置失败对比度*/
RGBERROR_INVALID_CONTRAST                =( RGBERROR_BASE + 0x0014 ),
/*不支持的黑色度*/
RGBERROR_INVALID_BLACKLEVEL              =( RGBERROR_BASE + 0x0015 ),
/*不支持的显示模式*/
RGBERROR_INVALID_PIXEL_FORMAT            =( RGBERROR_BASE + 0x0017 ),
/*不支持的消息接受窗口*/
RGBERROR_INVALID_HWND                    =( RGBERROR_BASE + 0x0018 ),
/**/
RGBERROR_INSUFFICIENT_RESOURCES          =( RGBERROR_BASE + 0x0019 ),
/*不支持的同步沿*/
RGBERROR_INVALID_SYNCEDGE                =( RGBERROR_BASE + 0x001D ),
/*不支持的水平缩放比例*/
RGBERROR_HSCALED_OUT_OF_RANGE            =( RGBERROR_BASE + 0x0020 ),
/*不支持的垂直缩放比例*/
RGBERROR_VSCALED_OUT_OF_RANGE            =( RGBERROR_BASE + 0x0021 ),
/*不支持或设置水平偏移失败*/
RGBERROR_SCALING_NOT_SUPPORTED           =( RGBERROR_BASE + 0x0022 ),
/*采集缓存太小*/
RGBERROR_BUFFER_TOO_SMALL                =( RGBERROR_BASE + 0x0023 ),
 /* This error is a return from the AccPerformDraw function,
 * which does the accelerated draw into the window.  If this
 * "error" is returned, then something has happened to the
 * display surface which has caused the horizon driver to
 * require a reallocation, which should be done before any more
 * calls to RGBCaptureFrameBufferReady are made.
 */
RGBERROR_ACC_REALLOCATE_BUFFERS          =( RGBERROR_BASE + 0x0029 ),
/*设备未被打开*/
RGBERROR_CAPTURE_OUTSTANDING             =( RGBERROR_BASE + 0x0030 ),
/*创建信号灯失败*/
RGBERROR_THREAD_FAILURE                  =( RGBERROR_BASE + 0x0036 ),
//fpga或9888参数写入失败
RGBERROR_FPGAor9888_WRITE_FAILED         =( RGBERROR_BASE + 0x0042 ),
//ddr设置参数失败
RGBERROR_DDR_ERROR					     =( RGBERROR_BASE + 0x0041 ),
//获取数据模式失败
RGBERROR_GETDATAMODE_ERROR               =( RGBERROR_BASE + 0x0042 ),
//设置钳位位置失败
RGBERROR_SETCLAMPPLACEMENT_ERROR         =( RGBERROR_BASE + 0x0043 ),
//设置钳位延迟失败
RGBERROR_SETCLAMPDURATION_ERROR          =( RGBERROR_BASE + 0x0044 )
} RGBERROR;




#endif


