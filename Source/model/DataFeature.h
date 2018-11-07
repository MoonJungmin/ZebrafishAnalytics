#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <list>

#include <stdio.h>
#include <stdlib.h>

#include <QString>
#include <QColor>
#include <QDebug>
#include <QTime>

#include "Source/Utils.h"

class DataFeature
{
public:
	DataFeature();
	~DataFeature();


	void Initialize(std::string name);
	void setFeatureValue(long long int index, double value);

	std::string FeatureName;
	std::map<long long int, double> FeatureValue;
		
};

