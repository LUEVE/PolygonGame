#include "gameplay.h"
#include <QtWidgets/QApplication>
#include "graphwidget.h"
#include "pch.h"

std::vector<NodeData> nodesData;
const double pi = 3.1415926;
void createData(int n,int len)
{
	for (int i = 0; i < n; i++)
	{
		double x = len* cos(i * 2*pi*1.0 / n);
		double y = len* sin(i * 2*pi*1.0 / n);
		int v = rand() % 100 + 1;
		nodesData.push_back({ x,y,v });
	}
	
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Gameplay w;

	createData(8,200);
	GraphWidget *widget = new GraphWidget(nodesData);
	


	w.setCentralWidget(widget);
	w.show();
	return a.exec();
}
