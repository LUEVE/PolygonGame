#pragma once

#include "graphwidget.h"
#include "ui_QtGuiApplication1.h"
#include <QMenu>
#include <QDialog>
#include <QGridLayout>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

class Gameplay : public QMainWindow
{
	Q_OBJECT

public:
	Gameplay(QWidget *parent = Q_NULLPTR);
private:
	GraphWidget *widget;

	QMenuBar * mBar;
	QMenu * gameMenu;
	QMenu * helpMenu;
	QAction * newAction;
	QAction * undoAction;
	QAction * ansAction;
	QAction * exitAction;
	QAction * aboutAction;

	QDialog * inputWindow;
	QGridLayout * gridLayout;
	QLabel * label_random;
	QButtonGroup * rButtonGroup;
	QRadioButton * button_yes;
	QRadioButton * button_no;
	QLabel * label_number;
	QLineEdit * lineedit_number;
	QLabel * label_node;
	QLineEdit * lineedit_node;
	QLabel * label_operation;
	QLineEdit * lineedit_operation;
	QPushButton * button_start;



signals:
	void canStart(std::vector<NodeData>);

public slots :
	void clickNewGame();
	void choseIsRandom();
	void prepare();
	void startGame(std::vector<NodeData> nodesData);
	void exitGame();
	void about();
};
