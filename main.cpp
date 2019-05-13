#include "gameplay.h"
#include <QtWidgets/QApplication>
#include "graphwidget.h"
#include "pch.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Gameplay w;

	w.show();
	return a.exec();
}
