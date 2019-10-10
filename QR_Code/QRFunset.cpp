#include "QRFunset.h"


QRFunset::QRFunset()
{
	m_index = 0;
	m_errType = 0;
}


QRFunset::~QRFunset()
{
}

QString QRFunset::GetLastErr()
{
	switch (m_errType)
	{
	case QRCODE_IDENTIFY_ERR_TYPE::QRCODE_IDENTIFY_SRCIMGEMPTY:
		return "源图像为空";
		break;
	case QRCODE_IDENTIFY_ERR_TYPE::QRCODE_IDENTIFY_DETECTORNULL:
		return "斑点检测器未加载";
		break;
	case QRCODE_IDENTIFY_ERR_TYPE::QRCODE_IDENTIFY_POINTCROSS:
		return "检测POINT点数越界";
		break;
	default:
		break;
	}
}

int QRFunset::QRcode(cv::Mat img, DEAL_IMG_RESULT& result)
{
	cv::Mat srcimg = img.clone();
	cv::Mat relImg;
	int type = cutExecute(img, relImg, result);
	if (type < 0)
	{
		return type;
	}
	return QRCODE_IDENTIFY_ERR_TYPE::QRCODE_IDENTIFY_OK;
}
int QRFunset::cutExecute(cv::Mat img, cv::Mat& dstImg, DEAL_IMG_RESULT& result)
{
	if (img.empty())
	{
		m_errType= QRCODE_IDENTIFY_ERR_TYPE::QRCODE_IDENTIFY_SRCIMGEMPTY;
		return QRCODE_IDENTIFY_ERR_TYPE::QRCODE_IDENTIFY_SRCIMGEMPTY;
	}
	m_keypoints.clear();
	cv::Mat rgbimg = img.clone();
	cv::Mat srcimg = img.clone();
	cv::Mat cutimg = img.clone();
	cv::Size sz;
	pyrDown(srcimg, srcimg, sz);
	pyrDown(srcimg, srcimg, sz);
	pyrDown(srcimg, srcimg, sz);
	if (m_blobDetector == NULL)
	{
		m_errType= QRCODE_IDENTIFY_ERR_TYPE::QRCODE_IDENTIFY_DETECTORNULL;
		return QRCODE_IDENTIFY_ERR_TYPE::QRCODE_IDENTIFY_DETECTORNULL;
	}
	m_blobDetector->detect(srcimg, m_keypoints);

	cv::Point point[128];
	int PointSize = m_keypoints.size();
	if (PointSize == 0 || PointSize >= 128)
	{
		m_errType= QRCODE_IDENTIFY_ERR_TYPE::QRCODE_IDENTIFY_POINTCROSS;
		return QRCODE_IDENTIFY_ERR_TYPE::QRCODE_IDENTIFY_POINTCROSS;
	}
	for (int i = 0; i < PointSize; i++)
	{
		cv::KeyPoint pt = m_keypoints.at(i);
		point[i].x = pt.pt.x;
		point[i].y = pt.pt.y;
	}
//	Cmpare(point, PointSize);
//	Cmpare2(point, PointSize);
	for (int i = 0; i < PointSize; i++)
	{
		point[i].x *= m_fScaleDownSample;
		point[i].y *= m_fScaleDownSample;
	}
	cv::Range rowrange, colrange;
	PlatParam m_platparam;
	m_platparam.x_start_offset = 60;
	m_platparam.x_range = 130;
	m_platparam.y_start_offset = 60;
	m_platparam.y_range = 130;

	int index = 0;
	int totalNum = 0;
	int errnum = 0;
	for (int i = 0; i < PointSize; i++)
	{
		cv::Point ele = point[i];
		rowrange.start = int(ele.y - m_platparam.y_start_offset + 0.5f);
		rowrange.end = int(ele.y - m_platparam.y_start_offset + m_platparam.y_range + 0.5f);
		colrange.start = int(ele.x - m_platparam.x_start_offset + 0.5f);
		colrange.end = int(ele.x - m_platparam.x_start_offset + m_platparam.x_range + 0.5f);

		if (rowrange.start < 0)
		{
			rowrange.start = 0;
		}
		if (rowrange.end > cutimg.rows)
		{
			rowrange.end = cutimg.rows - 1;
		}
		if (colrange.start < 0)
		{
			colrange.start = 0;
		}
		if (colrange.end > cutimg.cols)
		{
			colrange.end = cutimg.cols - 1;
		}
		cv::Mat Patch = cutimg(rowrange, colrange).clone();
		cv::Rect rect(colrange.start, rowrange.start, colrange.end - colrange.start, rowrange.end - rowrange.start);
		cv::rectangle(rgbimg, rect, cv::Scalar(200, 10, 10), 2, 8, 0);

		QString str = QString("D:/QTTest/%1.bmp").arg(index);
		index++;
		imwrite(str.toStdString(), Patch);

		std::string code = "";
		int ret = test_QRCode_decode(Patch, code);
		if (ret != 0)
		{
			cv::rectangle(rgbimg, rect, cv::Scalar(10, 10, 200), 5, 8, 0);
			errnum++;
			continue;
		}
		if (code.length() != QRCODE_LEN)
		{
			cv::rectangle(rgbimg, rect, cv::Scalar(10, 10, 200), 5, 8, 0);
			errnum++;
			continue;
		}
		result.onetieInfo[i - errnum].serialNum = i - errnum;
		result.onetieInfo[i - errnum].qrInfo = code;
		std::string coupons = code.substr(code.length() - 3, code.length());
		result.onetieInfo[i - errnum].coupons = atoi(coupons.c_str());
		const char *cstr;
		std::string strNode= std::to_string(totalNum)+"["+ coupons+"圆"+"]";
		cstr = strNode.c_str();;
		putTextZH(rgbimg, cstr, cv::Point(colrange.start+150, rowrange.start), cv::Scalar(10, 200, 10), 50, "Arial", false, true);
		if (code.length() == QRCODE_LEN)
		{
			totalNum++;
		}
	}
	result.totalNum = totalNum;
	result.dstImg = rgbimg;
	QString qstr = QString("D:/QTTest/keypoints%1.bmp").arg(m_index);
	m_index++;
	imwrite(qstr.toStdString(), rgbimg);

	m_errType = QRCODE_IDENTIFY_ERR_TYPE::QRCODE_IDENTIFY_OK;
	return QRCODE_IDENTIFY_ERR_TYPE::QRCODE_IDENTIFY_OK;
	// 		//高斯模糊
	// 		GaussianBlur(Patch, Patch, Size(5, 5), 1.0, 1.0);
	// 		//拉普拉斯求梯度
	// 		int scale = 1;
	// 		int delta = 0;
	// 		int ddepth = CV_8U;
	// 		int kernel_size = 5;
	// 		Laplacian(Patch, Patch, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);
	// 		convertScaleAbs(Patch, Patch);
	// 		//二值化
	// 		threshold(Patch, Patch, 150, 255, CV_THRESH_BINARY/*THRESH_TOZERO*/);
	// 		imwrite("D:/QTTest/000.bmp", Patch);
}



bool QRFunset::Cmpare2(cv::Point* pt, int len)
{
	cv::Point pt1[5], pt2[5], pt3[5], pt4[5];
	for (int i = 0; i < len; i++)
	{
		if (i < 5)
		{
			pt1[i] = pt[i];
		}
		else if (i >= 5 && i < 10)
		{
			pt2[i - 5] = pt[i];
		}
		else if (i >= 10 && i < 15)
		{
			pt3[i - 10] = pt[i];
		}
		else if (i >= 15 && i < 20)
		{
			pt4[i - 15] = pt[i];
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == 0)
			{

				if (SetSortRule(pt1[j], pt1[j + 1]))
				{
					cv::Point tmp = pt1[j];
					pt1[j] = pt1[j + 1];
					pt1[j + 1] = tmp;
				}

			}
			if (i == 1)
			{
				if (SetSortRule(pt2[j], pt2[j + 1]))
				{
					cv::Point tmp = pt2[j];
					pt2[j] = pt2[j + 1];
					pt2[j + 1] = tmp;
				}
			}
			if (i == 2)
			{
				if (SetSortRule(pt3[j], pt3[j + 1]))
				{
					cv::Point tmp = pt3[j];
					pt3[j] = pt3[j + 1];
					pt3[j + 1] = tmp;
				}
			}
			if (i == 3)
			{
				if (SetSortRule(pt4[j], pt4[j + 1]))
				{
					cv::Point tmp = pt4[j];
					pt4[j] = pt4[j + 1];
					pt4[j + 1] = tmp;
				}
			}
		}
	}
	cv::Point Tpt[128];
	for (int i = 0; i < len; i++)
	{
		if (i < 5)
		{
			pt[i] = pt1[i];
		}
		else if (i >= 5 && i < 10)
		{
			pt[i] = pt2[i - 5];
		}
		else if (i >= 10 && i < 15)
		{
			pt[i] = pt3[i - 10];
		}
		else if (i >= 15 && i < 20)
		{
			pt[i] = pt4[i - 15];
		}

	}
	//	pt = Tpt;
	return true;
}
bool QRFunset::Cmpare(cv::Point* pt, int len)
{
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = 0; j < len - 1 - i; j++)
		{
			if (SetSortYRule(pt[j], pt[j + 1]))
			{
				cv::Point tmp = pt[j];
				pt[j] = pt[j + 1];
				pt[j + 1] = tmp;
			}
		}
	}
	return true;
}

bool QRFunset::SetSortYRule(cv::Point p1, cv::Point p2)
{

	if (p1.y > p2.y) {
		return true;
	}
	else if (p1.y == p2.y) {
		return (p1.x > p2.x);
	}
	else {
		return false;
	}
}

bool QRFunset::SetSortRule(const cv::Point p1, const cv::Point p2)
{
	if (p1.x > p2.x) {
		return true;
	}
	else {
		return false;
	}
}
bool QRFunset::Dislodge(cv::Mat srcimage)
{
	cvtColor(srcimage, srcimage, CV_BGR2GRAY);
	// 初始化自适应阈值参数
	int blockSize = (srcimage.rows / 3) * 2 + 1;
	int constValue = (srcimage.rows / 10) * 2 + 1;
	const int maxVal = 255;
	// 图像自适应阈值操作
	adaptiveThreshold(srcimage, srcimage, maxVal, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, blockSize, constValue);
	srcimage = ~srcimage;
	imwrite("D:/000.bmp", srcimage);
	//	cv::imwrite("D://1.bmp", srcimage);
	cv::Scalar loDiff = cvScalarAll(10);
	cv::Scalar upDiff = cvScalarAll(10);
	cv::Scalar newVal = cvScalarAll(0);
	cv::Point seed;
	cv::Rect ccomp;
	//查找轮廓
	std::vector<std::vector<cv::Point>> contoursVec;
	std::vector<cv::Vec4i> contoursStorageLogic;
	int nSliderValue = 1;
	cv::Mat element_erode = getStructuringElement(cv::MORPH_RECT, cv::Size(1 * nSliderValue + 1, 1 * nSliderValue + 1), cv::Point(nSliderValue, nSliderValue));
	//进行膨胀操作
	dilate(srcimage, srcimage, element_erode);
	int index_ = 0;
	int index = 0;
	cv::Mat BinaryImage = srcimage.clone();
	findContours(BinaryImage, contoursVec, contoursStorageLogic, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	cv::Mat showmat = cv::Mat::zeros(BinaryImage.rows, BinaryImage.cols, CV_8UC1);
	imwrite("D:/000.bmp", showmat);
	double  area[128];
	memset(area, 0, sizeof(area));

	for (int i = 0; i < contoursVec.size(); i++)
	{
		seed.x = contoursVec[i][0].x;
		seed.y = contoursVec[i][0].y;
		double perimeter = arcLength(contoursVec[i], true);//轮廓周长
		area[i] = fabs(contourArea(contoursVec[i]));//轮廓面积
		if (area[i] < 5000)
		{
			//				return false;
			//				floodFill(srcimage, seed, newVal, &ccomp, loDiff, upDiff);
		}
	}
	return true;
}
void QRFunset::putTextZH(cv::Mat &dst, const char* str, cv::Point org, cv::Scalar color, int fontSize, const char *fn /*= "Arial"*/, bool italic /*= false*/, bool underline /*= false*/)
{
	CV_Assert(dst.data != 0 && (dst.channels() == 1 || dst.channels() == 3));

	int x, y, r, b;
	if (org.x > dst.cols || org.y > dst.rows) return;
	x = org.x < 0 ? -org.x : 0;
	y = org.y < 0 ? -org.y : 0;

	LOGFONTA lf;
	lf.lfHeight = -fontSize;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 5;
	lf.lfItalic = italic;   //斜体
	lf.lfUnderline = underline; //下划线
	lf.lfStrikeOut = 0;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = PROOF_QUALITY;
	lf.lfPitchAndFamily = 0;
	strcpy_s(lf.lfFaceName, fn);

	HFONT hf = CreateFontIndirectA(&lf);
	HDC hDC = CreateCompatibleDC(0);
	HFONT hOldFont = (HFONT)SelectObject(hDC, hf);

	int strBaseW = 0, strBaseH = 0;
	int singleRow = 0;
	char buf[1 << 12];
	strcpy_s(buf, str);
	char *bufT[1 << 12];  // 这个用于分隔字符串后剩余的字符，可能会超出。
						  //处理多行
	{
		int nnh = 0;
		int cw = 0, ch = 0;

		const char* ln = strtok_s(buf, "\n", bufT);
		while (ln != 0)
		{
			SIZE size;
			GetTextExtentPoint32A(hDC, ln, strlen(ln), &size);
			cw = size.cx;
			ch = size.cy;

			strBaseW = max(strBaseW, cw);
			strBaseH = max(strBaseH, ch);

			ln = strtok_s(0, "\n", bufT);
			nnh++;
		}
		singleRow = strBaseH;
		strBaseH *= nnh;
	}

	if (org.x + strBaseW < 0 || org.y + strBaseH < 0)
	{
		SelectObject(hDC, hOldFont);
		DeleteObject(hf);
		DeleteObject(hDC);
		return;
	}

	r = org.x + strBaseW > dst.cols ? dst.cols - org.x - 1 : strBaseW - 1;
	b = org.y + strBaseH > dst.rows ? dst.rows - org.y - 1 : strBaseH - 1;
	org.x = org.x < 0 ? 0 : org.x;
	org.y = org.y < 0 ? 0 : org.y;

	BITMAPINFO bmp = { 0 };
	BITMAPINFOHEADER& bih = bmp.bmiHeader;
	int strDrawLineStep = strBaseW * 3 % 4 == 0 ? strBaseW * 3 : (strBaseW * 3 + 4 - ((strBaseW * 3) % 4));

	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = strBaseW;
	bih.biHeight = strBaseH;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = strBaseH * strDrawLineStep;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	void* pDibData = 0;
	HBITMAP hBmp = CreateDIBSection(hDC, &bmp, DIB_RGB_COLORS, &pDibData, 0, 0);

	CV_Assert(pDibData != 0);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hDC, hBmp);

	//color.val[2], color.val[1], color.val[0]
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkColor(hDC, 0);
	//SetStretchBltMode(hDC, COLORONCOLOR);

	strcpy_s(buf, str);
	const char* ln = strtok_s(buf, "\n", bufT);
	int outTextY = 0;
	while (ln != 0)
	{
		TextOutA(hDC, 0, outTextY, ln, strlen(ln));
		outTextY += singleRow;
		ln = strtok_s(0, "\n", bufT);
	}
	uchar* dstData = (uchar*)dst.data;
	int dstStep = dst.step / sizeof(dstData[0]);
	unsigned char* pImg = (unsigned char*)dst.data + org.x * dst.channels() + org.y * dstStep;
	unsigned char* pStr = (unsigned char*)pDibData + x * 3;
	for (int tty = y; tty <= b; ++tty)
	{
		unsigned char* subImg = pImg + (tty - y) * dstStep;
		unsigned char* subStr = pStr + (strBaseH - tty - 1) * strDrawLineStep;
		for (int ttx = x; ttx <= r; ++ttx)
		{
			for (int n = 0; n < dst.channels(); ++n) {
				double vtxt = subStr[n] / 255.0;
				int cvv = vtxt * color.val[n] + (1 - vtxt) * subImg[n];
				subImg[n] = cvv > 255 ? 255 : (cvv < 0 ? 0 : cvv);
			}

			subStr += 3;
			subImg += dst.channels();
		}
	}

	SelectObject(hDC, hOldBmp);
	SelectObject(hDC, hOldFont);
	DeleteObject(hf);
	DeleteObject(hBmp);
	DeleteDC(hDC);
}

void QRFunset::SetParam(cv::SimpleBlobDetector::Params params)
{
	//阈值控制
	m_params.minThreshold = params.minThreshold;
	m_params.maxThreshold = params.maxThreshold;
	m_params.thresholdStep = params.thresholdStep;
	m_params.minDistBetweenBlobs = params.minDistBetweenBlobs;
	m_params.minRepeatability = params.minRepeatability;

	//像素面积大小控制
	m_params.filterByInertia = params.filterByInertia;
	m_params.minInertiaRatio = params.minInertiaRatio;

	//形状（凸）
	m_params.filterByCircularity = params.filterByCircularity;
	m_params.maxCircularity = params.maxCircularity;
	//形状（凹）
	m_params.filterByConvexity = params.filterByConvexity;

	//斑点面积限制
	m_params.filterByArea = params.filterByArea;
	m_params.minArea = params.minArea;
	m_params.maxArea = params.maxArea;

	//提取黑色斑点
	m_params.filterByColor = params.filterByColor;
	m_params.blobColor = params.blobColor;

	//因为图像金字塔，缩小了8倍
	m_fScaleDownSample = 8.0f;
	m_blobDetector = cv::SimpleBlobDetector::create(m_params);
}

QRCodeDiscern * _stdcall QRCodeDiscernCreateInstance(int version)
{
	if (version == KX_QRCODE_VERSION)
	{
		return new QRFunset;
	}
	else
	{
		return 0;
	}
}