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
#include "Source/model/LayerLabel.h"
#include "Source/model/DataCellAnalytics.h"
#include "Source/Utils.h"



class DataProject : public QObject
{
	Q_OBJECT
public:
	bool ProjectStatus = false;
	QString ProjectName = "";
	QString ProjectPath = "";
	
	bool LayerBackgroundStatus = false;
	QString LayerBackgroundPath = "";
	LayerBackground *mLayerBack;

	bool LayerLabelStatus = false;
	QString LayerLabelPath = "";
	LayerLabel *mLayerLabel;

	bool AnalyticsStatus = false;
	QString AnalyticsResultPath = "";
	DataCellAnalytics *mAnalytics;

	int DataSizeX = 0;
	int DataSizeY = 0;
	int DataSizeZ = 0;
	int DataBlockSize = 0;
	int DataLevel = 0;


	float ViewPos_X = 0;
	float ViewPos_Y = 0;
	float ViewPos_Z = 123;
	float ViewZoomLevel = 3.0;

	int VisualizeMethod_Index = 0;

	void ProjectMake(QString aProjName, QString aProjPath, QString aLayerBGPath, QString aLayerLBPath, QString aAnalyticsPath);
	void ProjectOpen(QString aProjPath);
	void ProjectSave(QString aProjPath);

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

