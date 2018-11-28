#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <list>

#include <stdio.h>
#include <stdlib.h>

#include <QString>
#include <QColor>
#include <QDebug>
#include <QTime>

#include "Source/Utils.h"

class DataCellAnalytics
{
public:
	DataCellAnalytics();
	~DataCellAnalytics();
	void Init(std::string path);
	
	std::vector<cell> mCellList;
	long long MaxCellIndex;
	long long MaxCellCount;
	std::string AnalyticsResultPath;
	bool CellColorGPU_On = false;


private:

	std::ifstream mifs;
	void LoadCellInformation();
	void LoadFeature();
	
};

