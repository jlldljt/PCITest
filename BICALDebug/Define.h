#ifndef _DEFINE_H_
#define _DEFINE_H_

//#define MAXFILENAMELEN		256

enum{
	SHOW_WINDOWS= 1,
	RESEND_COM,
};

//刷新缓存结构
typedef struct RefreshTmp
{
	bool bComUpdate;							//	标记是否需要更新Com内容
}RefreshTmp;


#endif

