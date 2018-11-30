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

#include "Source/global.h"

class ThreadCalcSubregionVolume : public QThread
{
	Q_OBJECT

public:
	ThreadCalcSubregionVolume();
	ThreadCalcSubregionVolume(QObject *parent);
	~ThreadCalcSubregionVolume();
	void addJob(std::string id);
	


private:
	void run();
	long long int getVolume(int level);
	void save(long long int volume);

	bool Pass = false;
	int SubregionIndex = 0;
	std::string SubregionName;
	std::string SubregionPath;
	std::string SubregionID;


public slots:
	void kill_thread();




};
