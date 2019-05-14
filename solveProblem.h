#pragma once
#ifndef SOLVEPROBLEM_H
#define SOLVEPROBLEM_H
#include "pch.h"
#include "nodeData.h"

class SolveProblem
{
public:
	static std::vector<NodeData> getAnswer(std::vector<NodeData> node_datas, int& _ans);
private:
	struct node {
		int v;
		int kind;
		std::function<int(int, int)> op;
	};

};



#endif SOLVEPROBLEM_H

