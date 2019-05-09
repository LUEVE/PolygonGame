#pragma once
#include "answerwidget.h"
#include "node.h"
#include "qdebug.h"
#include <QKeyEvent>
#include <cassert>
#include <solveProblem.h>
int AnswerWidget::clickNextFlagForFirst = 0;

AnswerWidget::AnswerWidget(QWidget* parent)
	:QGraphicsView(parent), timerId(0), changeTime(1), theFirstEdge(nullptr)
{
	paintInit();
	repaint(1);
}


AnswerWidget::AnswerWidget(std::vector<NodeData> node_datas, QWidget* parent)
	: QGraphicsView(parent), timerId(0), changeTime(1), theFirstEdge(nullptr)
{
	for (auto a : node_datas)
		nodesData.push_back(a);
	constNodesdata = nodesData;

	int _finalans = 0;
	std::vector<NodeData> temp =  SolveProblem::getAnswer(constNodesdata,_finalans);
	qDebug() << _finalans << endl;
	for (int i = temp.size() - 1; i >= 0; i--)
	{
		ansData.push(temp[i]);
	}

	beforeStepButton.setParent(this);
	beforeStepButton.setText("before step");
	beforeStepButton.setFont(QFont("", 20));
	beforeStepButton.show();
	connect(&beforeStepButton, SIGNAL(clicked()), this, SLOT(clickBeforeStepButton()));

	nextStepButton.setParent(this);
	nextStepButton.setText("next step");
	nextStepButton.setFont(QFont("", 20));
	nextStepButton.move(200, 0);
	nextStepButton.show();
	connect(&nextStepButton, SIGNAL(clicked()), this, SLOT(clickNextStepButton()));

	paintInit();
	repaint(1);

}



AnswerWidget::~AnswerWidget()
{
	delete theFirstEdge;
	theFirstEdge = nullptr;
}


void AnswerWidget::paintInit()
{
	// init
	QGraphicsScene *_scene = new QGraphicsScene(this);
	_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	_scene->setSceneRect(-200, -200, 400, 400);
	setScene(_scene);
	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(AnchorUnderMouse);
	scale(qreal(0.8), qreal(0.8));
	setMinimumSize(400, 400);
	setWindowTitle(tr("Elastic Nodes"));

	// init
}

bool AnswerWidget::getEdgeKind(Node* node)
{
	//NodeData nd;
	int flag = 0;
	int kind = -1;
	double tx = node->x();
	double ty = node->y();
	for (auto a : nodesData)
	{
		if (isSame(node->x(), a.x)
			&& isSame(node->y(), a.y))
		{
			kind = a.kind;
			flag = 1;
			break;

		}

	}

	if (flag)
		return kind;
	assert(-1);
	return false;
}


bool AnswerWidget::getEdgeKind(NodeData nodedata)
{
	return nodedata.kind;
}

void AnswerWidget::repaint(int ifconstructor)
{

	scene()->clear();

	QVector<Node*> nodes;
	QFont font;
	font.setPointSize(20);
	for (int i = 0; i < (int)nodesData.size(); i++)
	{

		Node *node = new Node(this);
		nodes.push_back(node);
		node->setPos({ nodesData[i].x, nodesData[i].y });
		node->setvalue(nodesData[i].v);
		scene()->addItem(node);

		scene()->addText(QString::number(nodesData[i].v), font)->setPos({ nodesData[i].x, nodesData[i].y });
	}
	for (int i = 1; i < (int)nodes.size(); i++)
	{
		Edge *edge = new Edge(nodes[i - 1], nodes[i]);


		bool kind = getEdgeKind(nodes[i - 1]);
		QString n = kind ? "*" : "+";
		scene()->addItem(edge);

		double x = (nodes[i - 1]->x() + nodes[i]->x()) / 2;
		double y = (nodes[i - 1]->y() + nodes[i]->y()) / 2;
		scene()->addText(n, font)->setPos(x, y);
	}

	if (nodes.size() > 1 && changeTime == 1)
	{
		Edge *edge = new Edge(nodes[nodes.size() - 1], nodes[0]);
		scene()->addItem(edge);
		int i = nodes.size();
		bool kind = getEdgeKind(nodes[i - 1]);
		QString n = kind ? "*" : "+";
		scene()->addItem(edge);

		double x = (nodes[i - 1]->x() + nodes[0]->x()) / 2;
		double y = (nodes[i - 1]->y() + nodes[0]->y()) / 2;
		scene()->addText(n, font)->setPos(x, y);
	}
}


void AnswerWidget::changeNode(NodeData& sourceItem)
{

	int _v = INT_MAX;
	bool sourKind = false;
	NodeData destItem;
	for (auto i = nodesData.begin(); i != nodesData.end(); i++)
	{
		if (isSame(i->x, sourceItem.x) && isSame(i->y, sourceItem.y))
		{
			_v = i->v;
			sourKind = getEdgeKind(*i);
			destItem = *(i + 1);
			nodesData.erase(i);
			break;
		}
	}

	for (auto i = nodesData.begin(); i != nodesData.end(); i++)
	{
		if (isSame(i->x, destItem.x) && isSame(i->y, destItem.y))
		{
			if (_v == INT_MAX) assert(-1);

			if (sourKind)
			{
				(*i).v *= _v;
			}
			else
			{
				(*i).v += _v;
			}
			break;
		}
	}
}


void AnswerWidget::ChangeInFirstTime(NodeData &nodedata)
{
	NodeData node_data;
	for (int i = 0; i < (int)nodesData.size();i++)
	{
		if(isSame(nodedata.x, nodesData[i].x) && isSame(nodedata.y, nodesData[i].y))
		{
			node_data = nodesData[i + 1];
			break;
		}
	}

	while (!(isSame(node_data.x,nodesData.front().x) 
		&& isSame(node_data.y,nodesData.front().y)))
	{
		auto t = nodesData.front(); nodesData.erase(nodesData.begin());
		nodesData.push_back(t);
	}

	return;
}


void AnswerWidget::clickBeforeStepButton()
{
	if (changeTime == 1) return;
	if (changeTime == 2)
	{

		// ÖØÖÃ
		int _finalans = 0;
		while (!ansData.empty())
		{
			ansData.pop();
		}

		nodesData = constNodesdata;
		std::vector<NodeData> temp = SolveProblem::getAnswer(constNodesdata, _finalans);
		qDebug() << _finalans << endl;
		for (int i = temp.size() - 1; i >= 0; i--)
		{
			ansData.push(temp[i]);
		}
		clickNextFlagForFirst = 0;
		changeTime--;
		repaint();
		// ÖØÖÃ --
		return;

	}
	if (changeTime > 2)
	{

		auto t = backsData1.top();
		backsData1.pop();
		ansData.push(t.first);

		for (auto i = nodesData.begin(); i != nodesData.end(); i++)
		{



			if (isSame(i->x, t.second.x)
				&& isSame(i->y, t.second.y))
			{
				bool k = t.first.kind;
				int bv = t.first.v;
				if (k)
				{
					i->v /= bv;
				}
				else
				{
					i->v -= bv;
				}
				nodesData.insert(i, t.first);
				break;
			}
		}
		repaint();
	}

	changeTime--;

}

void AnswerWidget::clickNextStepButton()
{
	if (ansData.empty())
	{
		return;
	}
	
	if(clickNextFlagForFirst == 0)
	{
		NodeData l = ansData.top();
		ansData.pop();
		ChangeInFirstTime(l);
		changeTime++;
		repaint();
		clickNextFlagForFirst++;
	}
	else
	{
		NodeData l = ansData.top();
		ansData.pop();

		putStack1(l);
		changeNode(l);
		changeTime++;

		repaint();
	}
;
		//}
}


void AnswerWidget::putStack1(NodeData &item)
{
	NodeData n1, n2;

	for (int i = 0; i < (int)nodesData.size() - 1;i++)
	{
		if (isSame(item.x, nodesData[i].x)
			&& isSame(item.y, nodesData[i].y))
		{
			n1 = nodesData[i];
			n2 = nodesData[i + 1];
		}
	}


	backsData1.push({ n1,n2 });
}

bool AnswerWidget::isSame(double l, double r)
{
	const double eps = 1e-8;
	return abs(l - r) < eps;
}

