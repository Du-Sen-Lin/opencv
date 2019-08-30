
#include "QtTask2.h"
#define _CRT_SECURE_NO_WARNINGS
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtTask2 w;
	w.show();
	return a.exec();
}
