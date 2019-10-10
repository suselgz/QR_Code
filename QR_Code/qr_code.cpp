#include "qr_code.h"

QR_Code::QR_Code(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_QRcode = QRCodeDiscernCreateInstance();
}

QR_Code::~QR_Code()
{
	delete m_QRcode;
}

void QR_Code::on_pushButton_clicked()
{
	cv::Mat img = cv::imread("D:/QRTest/123.png");
	DEAL_IMG_RESULT result;
	memset(&result, 0, sizeof(DEAL_IMG_RESULT));
	m_QRcode->QRcode(img, result);
}
