#ifndef QR_CODE_H
#define QR_CODE_H

#include <QtWidgets/QMainWindow>
#include "ui_qr_code.h"
#include "QRCodeDiscern.h"
class QR_Code : public QMainWindow
{
	Q_OBJECT

public:
	QR_Code(QWidget *parent = 0);
	~QR_Code();
	private slots:
	void on_pushButton_clicked();
private:
	Ui::QR_CodeClass ui;
	QRCodeDiscern* m_QRcode;
};

#endif // QR_CODE_H
