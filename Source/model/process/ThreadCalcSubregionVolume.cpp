#include "ThreadCalcSubregionVolume.h"

ThreadCalcSubregionVolume::ThreadCalcSubregionVolume() {

}
ThreadCalcSubregionVolume::ThreadCalcSubregionVolume(QObject *parent)
	: QThread(parent)
{
	connect(parent, SIGNAL(thread_kill()), this, SLOT(kill_thread()));
}

ThreadCalcSubregionVolume::~ThreadCalcSubregionVolume()
{
}

void ThreadCalcSubregionVolume::kill_thread() {
	this->terminate();
	qDebug() << "in thread kill_thread";
}

void ThreadCalcSubregionVolume::addJob(std::string id) {
	SubregionID = id;
	std::vector<LayerSubregion>::iterator iter;
	for (iter = mGlobals.CurrentProject->mSubregion.begin(); iter != mGlobals.CurrentProject->mSubregion.end(); ++iter) {
		if (SubregionID.compare(iter->SubregionID) == 0) {
			if (iter->SubregionVolumeStatus == false) {
				Pass = true;
			}
			else {
				Pass = false;
			}
			SubregionName = iter->SubregionName;
			SubregionPath= iter->SubregionPath + "XY/";
			break;
		}
	}
}


void ThreadCalcSubregionVolume::save(long long int volume) {
	std::vector<LayerSubregion>::iterator iter;
	for (iter = mGlobals.CurrentProject->mSubregion.begin(); iter != mGlobals.CurrentProject->mSubregion.end(); ++iter) {
		if (SubregionID.compare(iter->SubregionID) == 0) {
			iter->SubregionVolumeStatus = true;
			iter->SubregionVolume = volume;
			break;
		}
	}
}


long long int ThreadCalcSubregionVolume::getVolume(int level) {
	
	QDir directory(QString::fromStdString(SubregionPath));

	QStringList target; 
	target << "*_" + QString::fromStdString(to_string(level)) + ".block";
	QStringList blocks = directory.entryList( target, QDir::Files);
	long long int volume = 0;
	long long int lvalue = 1;
	long long int size = lvalue * mGlobals.CurrentProject->DataBlockSize * mGlobals.CurrentProject->DataBlockSize * mGlobals.CurrentProject->DataBlockSize;
	unsigned char *subregion_block = new unsigned char[size];
	foreach(QString filename, blocks) {
		std::string path = SubregionPath + filename.toStdString();
		FILE *subregion_blck_rfp = fopen(path.c_str(), "rb");
		fread(subregion_block, size, 1, subregion_blck_rfp);
		for (long long int i = 0; i < size; ++i) {
			if (subregion_block[i] > 0) {
				volume += 1;
			}
		}
	}
	return volume;
}

void ThreadCalcSubregionVolume::run()
{
	if (Pass) {
		for (int i = mGlobals.CurrentProject->DataLevel; i > 0; i--) {
			std::string str = "Calculating " + SubregionID + "- level" + std::to_string(i) + " ";
			//mGlobals.mainWindow->statusBar()->showMessage(QString::fromStdString(str));
			long long int volume = getVolume(i);
			volume = volume * pow(2, i) * pow(2, i);
			qDebug() << QString::fromStdString(str) << volume;
			save(volume);
		}
	}
	
}