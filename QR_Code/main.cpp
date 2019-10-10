#include "qr_code.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QR_Code w;
	w.show();
	return a.exec();
}
