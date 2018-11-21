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


#include "Source/Utils.h"

class DataFeature
{
public:
	DataFeature();
	~DataFeature();


	void Initialize(std::string name, std::string path);
	void setFeatureValue(unsigned int index, double value);

	std::string FeatureName;
	std::map<unsigned int, double> FeatureMap;
		
};

