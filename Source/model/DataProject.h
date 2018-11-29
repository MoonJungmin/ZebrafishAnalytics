#pragma once
#include <Qstring>
#include <QObject>
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <cmath>

#include "Source/model/LayerBackground.h"
#include "Source/model/LayerCell.h"
#include "Source/model/LayerSubregion.h"

#include "Source/model/DataFeature.h"
#include "Source/Utils.h"


class DataProject : public QObject
{
	Q_OBJECT
public:
	bool ProjectStatus = false;
	QString ProjectName = "";
	QString ProjectPath = "";
	
	bool LayerBackgroundStatus = false;
	LayerBackground *mLayerBack;

	bool LayerCellStatus = false;
	LayerCell *mLayerCell;
	QColor SelectedColor;
	QColor UnSelectedColor;

	std::vector<DataFeature> mFeature;
	
	std::vector<LayerSubregion> mSubregion;
	//std::list<int> mActivatedSubregionIndex;

	int DataSizeX = 0;
	int DataSizeY = 0;
	int DataSizeZ = 0;
	int DataBlockSize = 0;
	int DataLevel = 0;


	float ViewPos_X = 0;
	float ViewPos_Y = 0;
	float ViewPos_Z = 123;
	float ViewZoomLevel = 2.0;

	int VisualizeMethod_Index = 0;

	void ProjectMake(QString aProjName, QString aProjPath, QString aLayerBGPath, QString aLayerLBPath, QString aAnalyticsPath);
	void ProjectOpen(QString aProjPath);
	void ProjectSave(QString aProjPath);

	void AddFeature(QString aPath);
	void removeFeature(int index);
	void AddSubregion(QString aPath);
	void removeSubregion(int index);


	DataProject();
	~DataProject();
private:
	int getSerialIndex(int x, int y, int z, int lv);
	void ProjectSizeLoad();

	std::ifstream mIFS;


signals:
	void project_on();
	void analytics_on();


};

