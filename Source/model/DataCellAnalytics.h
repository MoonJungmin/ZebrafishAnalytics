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
	

	std::vector<std::vector<long long>> volumeHist;
	std::vector<std::vector<long long>> surfaceareaHist;
	std::vector<std::vector<long long>> sphericityHist;
	std::vector<std::vector<long long>> eccentricityHist;
	std::vector<std::vector<long long>> intensityHist;

	std::vector<cell> mCellList;
	long long MaxCellIndex;
	long long MaxCellCount;
	std::string AnalyticsResultPath;
	bool CellColorGPU_On = false;


private:
	int HistWidth = 300;
	int HistBarWidth = 1;

	long long volumeMax = -9999999;
	long long surfaceareaMax = -9999999;
	long double sphericityMax = -9999999;
	long double eccentricityMax = -9999999;
	long double intensityMax = -9999999;

	long long volumeMin = 9999999;
	long long surfaceareaMin = 9999999;
	long double sphericityMin = 9999999;
	long double eccentricityMin = 9999999;
	long double intensityMin = 9999999;


	std::ifstream mifs;
	void LoadCellInformation();
	void LoadFeature();
	
	void GenerateHistogram();
};

