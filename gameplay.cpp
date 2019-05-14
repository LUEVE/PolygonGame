#include "gameplay.h"
#include "graphwidget.h"
#include <QPaintEvent>
#include <QtGui>
#include <iostream>
#include "QDebug"

std::vector<NodeData> nodesData;
const double pi = 3.1415926;

#define RANGE_INT_T 10



void createData(int n, int len)
{
	const int range = RANGE_INT_T - 1;
	nodesData.clear();
	for (int i = 0; i < n; i++)
	{
		double x = len* cos(i * 2 * pi*1.0 / n);
		double y = len* sin(i * 2 * pi*1.0 / n);
		int v = rand() % range + 1;
		bool k = rand() % 2;
		nodesData.push_back({ x,y,v,k });
	}

}

Gameplay::Gameplay(QWidget* parent) : QMainWindow(parent)
{
//	ui.setupUi(this);

	createData(12, 200);
	widget = new GraphWidget(nodesData, this);

	this->setCentralWidget(widget);

	this->resize(600, 600);

	mBar = new QMenuBar;
	this->setMenuBar(mBar);
	gameMenu = new QMenu("Game(&G)", mBar);
	mBar->addMenu(gameMenu);
	helpMenu = new QMenu("Help(&H)", mBar);
	mBar->addMenu(helpMenu);

	newAction = new QAction("&New Game", this);
	newAction->setShortcut(QKeySequence("CTRL+N"));
	gameMenu->addAction(newAction);

	gameMenu->addSeparator();

	undoAction = new QAction("&Undo", this);
	undoAction->setShortcut(QKeySequence("CTRL+Z"));
	gameMenu->addAction(undoAction);

	ansAction = new QAction("&Answer", this);
	ansAction->setShortcut(QKeySequence("CTRL+A"));
	gameMenu->addAction(ansAction);

	gameMenu->addSeparator();

	exitAction = new QAction("&Exit", this);
	exitAction->setShortcut(QKeySequence("CTRL+Q"));
	gameMenu->addAction(exitAction);

	aboutAction = new QAction("&About", this);
	helpMenu->addAction(aboutAction);

	connect(newAction, SIGNAL(triggered()), this, SLOT(clickNewGame()));
	connect(undoAction, SIGNAL(triggered()), widget, SLOT(clickBeforeStepButton()));
	connect(ansAction, SIGNAL(triggered()), widget, SLOT(clickAnswerButton()));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(exitGame()));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void Gameplay::clickNewGame()
{
	inputWindow = new QDialog;

	gridLayout = new QGridLayout;

	label_random = new QLabel;
	label_random->setText("Random:");

	rButtonGroup = new QButtonGroup;
	rButtonGroup->setExclusive(true);

	button_yes = new QRadioButton;
	button_yes->setText("YES");
	button_no = new QRadioButton;
	button_no->setText("NO");

	rButtonGroup->addButton(button_yes);
	rButtonGroup->addButton(button_no);

	connect(button_yes, SIGNAL(clicked()), this, SLOT(choseIsRandom()));
	connect(button_no, SIGNAL(clicked()), this, SLOT(choseIsRandom()));

	label_number = new QLabel;
	label_number->setText("Node Number:");
	lineedit_number = new QLineEdit;

	label_node = new QLabel;
	label_node->setText("Node List:");
	lineedit_node = new QLineEdit;

	label_operation = new QLabel;
	label_operation->setText("Operation List:");
	lineedit_operation = new QLineEdit;

	QRegExp regx_num("[0-9]+$");
	QValidator *validator_num = new QRegExpValidator(regx_num);
	lineedit_number->setValidator(validator_num);

	QRegExp regx_node("((\\+|\\-)?[0-9]\\s)+$");
	QValidator *validator_node = new QRegExpValidator(regx_node);
	lineedit_node->setValidator(validator_node);

	QRegExp regx_op("((\\+|\\*)\\s)+(\\+|\\*)$");
	QValidator *validator_op = new QRegExpValidator(regx_op);
	lineedit_operation->setValidator(validator_op);

	button_start = new QPushButton;
	button_start->setText("Start Game");

	connect(button_start, SIGNAL(clicked()), this, SLOT(prepare()));
	connect(this, SIGNAL(canStart(std::vector<NodeData>)), this, SLOT(startGame(std::vector<NodeData>)));

	button_yes->setChecked(true);
	lineedit_node->setEnabled(false);
	lineedit_operation->setEnabled(false);

	gridLayout->addWidget(label_random, 0, 0, 1, 1);
	gridLayout->addWidget(button_yes, 0, 1, 1, 1);
	gridLayout->addWidget(button_no, 0, 2, 1, 1);
	gridLayout->addWidget(label_number, 1, 0, 1, 2);
	gridLayout->addWidget(lineedit_number, 1, 2, 1, 2);
	gridLayout->addWidget(label_node, 2, 0, 1, 2);
	gridLayout->addWidget(lineedit_node, 2, 2, 1, 2);
	gridLayout->addWidget(label_operation, 3, 0, 1, 2);
	gridLayout->addWidget(lineedit_operation, 3, 2, 1, 2);
	gridLayout->addWidget(button_start, 4, 1, 1, 2);

	gridLayout->setSpacing(10);
	gridLayout->setContentsMargins(10, 10, 10, 10);

	inputWindow->setLayout(gridLayout);

	inputWindow->resize(400, 200);
	inputWindow->setWindowTitle("New Game");
	inputWindow->setWindowFlags(inputWindow->windowFlags()&~Qt::WindowMaximizeButtonHint);
	inputWindow->setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
	inputWindow->setFixedSize(inputWindow->width(), inputWindow->height());
	inputWindow->exec();
}

void Gameplay::prepare()
{
	if (button_yes->isChecked())
	{
		int number = lineedit_number->text().toInt();
		if (number < 3 || number > 20)
		{
			QMessageBox::about(this, tr("Warning"), tr("The number must between 3 to 20!"));
		}
		else
		{
			createData(number, 200);
			emit canStart(nodesData);
			emit this->inputWindow->accept();
		}
	}
	else
	{
		QStringList nodes = lineedit_node->text().split(' ');
		QStringList opers = lineedit_operation->text().split(' ');
		int len_nodes = nodes[nodes.size() - 1].size() != 0 ? nodes.size() : nodes.size() - 1;
		int len_opers = opers[opers.size() - 1].size() != 0 ? opers.size() : opers.size() - 1;

		if (len_nodes < 3 || len_nodes > 20 || len_opers < 3 || len_opers > 20)
		{
			QMessageBox::about(this, tr("Warning"), tr("Please enter 3-20 pairs of data!"));
		}
		else
		{
			int count = std::min(len_nodes, len_opers);

			createData(count, 200);

			for (int i = 0; i < count; i++)
			{
				int value = nodes[i].toInt();
				nodesData[i].v = value;

				if (opers[i][0].toLatin1() == '+')
				{
					nodesData[i].kind = false;
				}
				else
				{
					nodesData[i].kind = true;
				}
			}
			emit canStart(nodesData);
			emit this->inputWindow->accept();
		}
	}
}

void Gameplay::startGame(std::vector<NodeData> data)
{

	widget = new GraphWidget(data, this);
	this->setCentralWidget(widget);
	connect(undoAction, SIGNAL(triggered()), widget, SLOT(clickBeforeStepButton()));
	connect(ansAction, SIGNAL(triggered()), widget, SLOT(clickAnswerButton()));
}

void Gameplay::choseIsRandom()
{
	if (button_yes->isChecked())
	{
		lineedit_number->setEnabled(true);
		lineedit_node->setEnabled(false);
		lineedit_operation->setEnabled(false);
	}
	else
	{
		lineedit_number->setEnabled(false);
		lineedit_node->setEnabled(true);
		lineedit_operation->setEnabled(true);
	}
}

void Gameplay::exitGame()
{
	this->close();
}

void Gameplay::about()
{
	QMessageBox msg(this);
	msg.setWindowTitle("About us");
	QTextCodec *codec = QTextCodec::codecForName("GBK");
	msg.setText(codec->toUnicode("    卢爷：我带你们打    "));
	msg.setIcon(QMessageBox::Information);
	msg.setStandardButtons(QMessageBox::Ok);
	msg.exec();
}