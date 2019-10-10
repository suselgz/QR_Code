#define KX_QRCODE_MAIN_VERSION 1
#define KX_QRCODE_SUB_VERSION 1
#define KX_QRCODE_VERSION ((KX_QRCODE_MAIN_VERSION << 16) + KX_QRCODE_SUB_VERSION)
#pragma once
#include <QObject>
#include "opencv2/opencv.hpp"
#define  ONE_BOX_KUNNUM 20 //每箱捆数
#define  QRCODE_LEN 20     //二维码位数

enum IMAGE_RESULT_TYPE
{
	RESULT_TYPE_IDENTIFYOK = 0,
	RESULT_TYPE_NUMERR = -1,
	RESULT_TYPE_MIXERR = -2,
	RESULT_TYPE_PRODUCTERR = -3,
	RESULT_TYPE_INSERTDATAERR = -4,
	RESULT_TYPE_INSERTIMAGEERR = -5,
	RESULT_TYPE_UPDATEINDOERR = -6,
	RESULT_TYPE_QRJUDGEERR = -7
};
typedef struct ONE_TIE_INFO_TAG
{
	int             serialNum;  //序号,从左往右，从上到下
	int             coupons;    //券别
	std::string     qrInfo;     //二维码信息
	int             qrJudge;    //二维码识别是否正确校验信息
}ONE_TIE_INFO;
typedef struct DEAL_IMG_RESILT_TAG
{
	int            totalNum;
	int            type;
	cv::Mat        dstImg;
	ONE_TIE_INFO   onetieInfo[ONE_BOX_KUNNUM];
}DEAL_IMG_RESULT;
#define CODE_LIB_SOLUTION
#ifdef CODE_LIB_SOLUTION
#define CODE_LIB_EXPORT __declspec(dllexport)
#else
#define CODE_LIB_EXPORT __declspec(dllimport)
#endif

class   QRCodeDiscern : public QObject
{
	Q_OBJECT
public:
	virtual QString GetLastErr() = 0;
	virtual void SetParam(cv::SimpleBlobDetector::Params params) = 0;
	virtual int QRcode(cv::Mat img, DEAL_IMG_RESULT& result)=0;
};
CODE_LIB_EXPORT QRCodeDiscern * _stdcall QRCodeDiscernCreateInstance(int version = KX_QRCODE_VERSION);