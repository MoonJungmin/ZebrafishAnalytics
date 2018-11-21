#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <list>

#include <stdio.h>
#include <stdlib.h>

#include <QString>
#include <QColor>
#include <QDebug>
#include <QTime>

#include "Source/Utils.h"

class LayerSubregion
{
public:
	LayerSubregion();
	~LayerSubregion();

	void Initialize(std::string name, std::string path);

	std::string SubregionName;
	
	std::string SubregionPath;
	std::string SubregionHeaderPath;
	std::string SubregionXYPath;
	std::string SubregionYZPath;
	std::string SubregionZXPath;

	std::list<unsigned int> IncludeIndex;

};

