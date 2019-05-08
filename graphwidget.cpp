#pragma once
#include "graphwidget.h"
#include "node.h"
#include "qdebug.h"
#include <QKeyEvent>
#include <cassert>


GraphWidget::GraphWidget(QWidget* parent)
	: QGraphicsView(parent), timerId(0),changeTime(1),theFirstEdge(nullptr)
{
	paintInit();
	repaint(1);
}

GraphWidget::GraphWidget(std::vector<NodeData> node_datas, QWidget* parent)
	: QGraphicsView(parent), timerId(0),changeTime(1),theFirstEdge(nullptr)
{
	for (auto a : node_datas)
		nodesData.push_back(a);


	constNodesdata = nodesData;

	beforeStepButton.setParent(this);
	beforeStepButton.setText("before step");
	beforeStepButton.setFont(QFont("",20));
	beforeStepButton.show();
	connect(&beforeStepButton, SIGNAL(clicked()), this, SLOT(clickBeforeStepButton()));

	answerButton.setParent(this);
	answerButton.setText("answer");
	answerButton.setFont(QFont("", 20));
	answerButton.move(200, 0);
	answerButton.show();
	connect(&answerButton, SIGNAL(clicked()), this, SLOT(clickAnswerButton()));
	
	paintInit();
	repaint(1);
	scene()->addText("wdnmd");


}

GraphWidget::~GraphWidget()
{
	delete theFirstEdge;
	theFirstEdge = nullptr;
}


void GraphWidget::paintInit()
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

bool GraphWidget::getEdgeKind(Node* node)
{
	//NodeData nd;
	int flag = 0;
	int kind = -1;
	double tx = node->x();
	double ty = node->y();
	for (auto a : nodesData)
	{
		if(isSame(node->x(),a.x) 
			&& isSame(node->y(),a.y))
		{
			kind = a.kind;
			flag = 1;
			break;

		}

	}

	if(flag)
		return kind;
	assert(-1);
	qDebug() << "diao-------" << '\n';
	return false;
}


bool GraphWidget::getEdgeKind(NodeData nodedata)
{
	
	
	return nodedata.kind;


}

void GraphWidget::repaint(int ifcontructor)
{

	scene()->clear();
	



	QVector<Node*> nodes;
	QFont font;
	font.setPointSize(20);
	for (int i = 0;i < (int)nodesData.size();i++)
	{
		
		Node *node = new Node(this);
		nodes.push_back(node);
		node->setPos({ nodesData[i].x, nodesData[i].y });
		node->setvalue(nodesData[i].v);
		scene()->addItem(node);

		scene()->addText(QString::number(nodesData[i].v),font)->setPos({ nodesData[i].x, nodesData[i].y });
	}
	//
	if (ifcontructor) {
		for (auto &a : nodesData) a.kind = rand()%2;
		qDebug() << "fuck" << endl;
	}//
	for (int i = 1;i < (int)nodes.size();i++)
	{
		Edge *edge = new Edge(nodes[i - 1], nodes[i]);


		bool kind = getEdgeKind(nodes[i - 1]);
		QString n = kind ? "*" : "+";
		qDebug() << n << " ";
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
		qDebug() << n << " ";
		scene()->addItem(edge);
		
		double x = (nodes[i - 1]->x() + nodes[0]->x()) / 2;
		double y = (nodes[i - 1]->y() + nodes[0]->y()) / 2;
		scene()->addText(n, font)->setPos(x, y);
	}
}



void GraphWidget::changeNode(QGraphicsItem* sourceItem,QGraphicsItem *destItem)
{

	int _v = INT_MAX;
	bool sourKind = false;
	
	for (auto i = nodesData.begin();i != nodesData.end();i++)
	{
		if(isSame(i->x,sourceItem->x()) && isSame(i->y,sourceItem->y()))
		{
			//qDebug() << item->x() << ":" << item->y() << '\n';
			_v = i->v;
			sourKind = getEdgeKind(*i);
			nodesData.erase(i);
			break;
		}
	}

	for (auto i = nodesData.begin();i!= nodesData.end();i++)
	{
		if (isSame(i->x, destItem->x()) && isSame(i->y, destItem->y()))
		{
			//qDebug() << item->x() << ":" << item->y() << '\n';
			if (_v == INT_MAX) assert(-1);

			//bool kind = getEdgeKind(*i);
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

void GraphWidget::ChangeInFirstTime(QGraphicsItem *item)
{
	QVector<NodeData> temp;
	//if()
	qDebug() << item->x() << "|" << item->y() << '\n';
	while (!(isSame(nodesData.front().x,item->x()) && isSame(nodesData.front().y,item->y())))
	{
		auto t = nodesData.front(); nodesData.erase(nodesData.begin());
		nodesData.push_back(t);
	}

	return;
}

void GraphWidget::clickBeforeStepButton()
{
	if(changeTime == 1) return;
	if (changeTime == 2)
	{
		delete theFirstEdge;
		theFirstEdge = nullptr;
		changeTime--;
		repaint();
		return;
		//scene()->addItem(theFirstEdge);
		
	}
	if(changeTime > 2)
	{
		//Edge* t = backsData.top();
		//backsData.pop();
		auto t = backsData1.top();
		backsData1.pop();

		for (auto i = nodesData.begin(); i != nodesData.end() ; i++)
		{
			//double x1 = t->sourceNode()->x();
			//double y1 = t->sourceNode()->y();
			//double x2 = t->destNode()->x();
			//double y2 = t->destNode()->y();


			if(isSame(i->x,t.second.x) 
				&& isSame(i->y,t.second.y))
			{
				//i->v -= 10000;
				bool k = t.first.kind;
				int bv = t.first.v;
				if(k)
				{
					i->v /= bv;
				}
				else
				{
					i->v -= bv;
				}
				//nodesData.insert(i , { t->sourceNode()->x(),t->sourceNode()->y(),t->sourceNode()->getvalue() });
				nodesData.insert(i, t.first);
				break;
			}
		}
		repaint();
	}

	changeTime--;


	qDebug() << "before step" << '\n';
}

void GraphWidget::clickAnswerButton()
{

	AnswerWidget *widget = new AnswerWidget(constNodesdata);
	q.setCentralWidget(widget);
	q.show();


}

void GraphWidget::mousePressEvent(QMouseEvent* event)
{
	if (QGraphicsItem *item = itemAt(event->pos())) {
		qDebug() << "You clicked on item" << item;
		//this->setDisabled(true);
		if (Edge *edge = qgraphicsitem_cast<Edge *>(item))
		{
			scene()->removeItem(item);
			
			if(changeTime == 1)
			{
				ChangeInFirstTime(edge->destNode());
				delete theFirstEdge;
				theFirstEdge = new Edge(edge->sourceNode(), edge->destNode());
				
			}
			else
			{
				//qDebug() << edge->sourceNode()->x() << ":" << edge->sourceNode()->y() << "-------"<<'\n';
				

				
				//backsData.push(temp);
				putStack1(edge->sourceNode(), edge->destNode());
				changeNode(edge->sourceNode(), edge->destNode());


			}
			changeTime++;
			repaint();
		}

		
	}
	else {
		qDebug("You didn't click on an item.");
	}
}


//
//
//
//void GraphWidget::putStack(QGraphicsItem *item,int v1,QGraphicsItem*item2,int v2)
//{
//	// 1.0
//	qDebug() << item->x() << ":" << item->y() << "----"<<'\n';
//	double sx = item->x(), sy = item->y();
//	double ex = item2->x(), ey = item2->y();
//	Node* s = new Node(this); s->setPos(sx, sy); s->setvalue(v1);
//	Node* e = new Node(this); e->setPos(ex, ey); e->setvalue(v2);
//	qDebug() << sx << ":" << sy << "----" << '\n';
//	Edge* edge = new Edge(s, e);
//	// 1.0
//
//
//	//backsData.push(edge);
//}


void GraphWidget::putStack1(QGraphicsItem *item,QGraphicsItem* item2)
{
	NodeData n1, n2;
	for (auto a:nodesData)
	{
		if (isSame(item->x(), a.x)
			&& isSame(item->y(), a.y))
		{
			n1 = a;
			break;
		}
	}

	for (auto a : nodesData)
	{
		if (isSame(item2->x(), a.x)
			&& isSame(item2->y(), a.y))
		{
			n2 = a;
			break;
		}
	}

	backsData1.push({ n1,n2 });
}

bool isSame(double l,double r)
{
	const double eps = 1e-8;
	return abs(l - r) < eps;
}

