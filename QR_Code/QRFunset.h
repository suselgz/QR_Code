#pragma once
#include "QRCodeDiscern.h"
#include "funset.hpp"
#include <string.h>
#include <math.h>
#pragma comment(lib,"winmm.lib")
#include <windows.h>
struct PlatParam
{
	int iMinDiamter, iMaxDiamter, iBlobThreshold, iMarkNum;
	int x_start_offset, x_range;
	int y_start_offset, y_range;
	int iDistThreshold;
};
enum QRCODE_IDENTIFY_ERR_TYPE
{
	QRCODE_IDENTIFY_OK = 0,              //无误
	QRCODE_IDENTIFY_SRCIMGEMPTY = -1,    //源图像为空
	QRCODE_IDENTIFY_DETECTORNULL = -2,   //斑点检测器未加载
	QRCODE_IDENTIFY_POINTCROSS = -3,     //检测POINT点数越界
};
class QRFunset :public QRCodeDiscern
{
public:
	QRFunset();
	~QRFunset();
	QString GetLastErr();
	int QRcode(cv::Mat img, DEAL_IMG_RESULT& result);
	void SetParam(cv::SimpleBlobDetector::Params params);
private:
	int m_index;
	int m_errType;
	std::vector<cv::KeyPoint> m_keypoints;
	cv::Ptr<cv::SimpleBlobDetector> m_blobDetector;
	cv::SimpleBlobDetector::Params m_params;
	float m_fScaleDownSample;
	bool Dislodge(cv::Mat srcimage);
	bool Cmpare2(cv::Point* pt, int len);
	bool Cmpare(cv::Point* pt,int len);
	bool SetSortYRule(cv::Point p1, cv::Point  p2);
	bool SetSortRule(const cv::Point p1, const cv::Point  p2);
	int cutExecute(cv::Mat img, cv::Mat& dstImg, DEAL_IMG_RESULT& result);
	void putTextZH(cv::Mat &dst, const char* str, cv::Point org, cv::Scalar color, int fontSize,
		const char *fn = "Arial", bool italic = false, bool underline = false);
};

