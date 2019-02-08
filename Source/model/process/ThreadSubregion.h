#pragma once

#include <QThread>
#include <QObject>
#include <QDialog>
#include <QtWidgets>
#include <fstream>
#include <string>
#include <iostream>
#include <string>
#include <cmath>
#include <omp.h>
#include <QFileInfo>
#include <queue>
#include <cuda.h>
#include <builtin_types.h>
#include <cuda_runtime.h>

#include "calculator_subregion.cuh"
#include "Source/global.h"
#include "Source/controller/node/BlockWidget.h"

class BlockWidget;

class ThreadSubregion : public QThread
{
	Q_OBJECT

public:
	ThreadSubregion();
	ThreadSubregion(QObject *parent);
	~ThreadSubregion();
	void addJob(std::string id, BlockWidget *parent);
	



private:
	void run();
	long long int getVolume(int level);
	void save(long long int volume, int level);

	bool newJobFlag = false;
	bool waitFlag = true;
	int SubregionIndex = 0;
	std::string SubregionName;
	std::string SubregionPath;
	std::string SubregionID;

	float pos_x;
	float pos_y;
	float pos_z;
	float up_x;
	float up_y;
	float up_z;

	std::map<unsigned int, bool> include_map;
	std::map<unsigned int, bool> intersect_map;

	BlockWidget *mParent;

public slots:
	void kill_thread();

signals:
	void progress_update(float);
	void progress_log_update(QString);
	void subregion_information_update(QString);
	


};
