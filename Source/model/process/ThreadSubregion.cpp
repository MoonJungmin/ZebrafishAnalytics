#include "ThreadSubregion.h"

void printArray(const float* a, const unsigned int n) {
	QString s = "(";
	unsigned int ii;
	for (ii = 0; ii < n - 1; ++ii)
		s.append(QString::number(a[ii])).append(", ");
	s.append(QString::number(a[ii])).append(")");

	qDebug() << s;
}


ThreadSubregion::ThreadSubregion() {

}
ThreadSubregion::ThreadSubregion(QObject *parent)
	: QThread(parent)
{
	connect(parent, SIGNAL(thread_kill()), this, SLOT(kill_thread()));
}

ThreadSubregion::~ThreadSubregion()
{
}

void ThreadSubregion::kill_thread() {
	this->terminate();
	qDebug() << "in thread kill_thread";
}

void ThreadSubregion::addJob(std::string id, BlockWidget *parent) {
	SubregionID = id;
	mParent = parent;

	std::vector<LayerSubregion>::iterator iter;
	for (iter = mGlobals.CurrentProject->mSubregion.begin(); iter != mGlobals.CurrentProject->mSubregion.end(); ++iter) {
		if (SubregionID.compare(iter->SubregionID) == 0) {
			SubregionName = iter->SubregionName;
			SubregionPath = iter->SubregionPath + "XY/";
			break;
		}
	}

	pos_x = parent->plane_pos_x;
	pos_y = parent->plane_pos_y;
	pos_z = parent->plane_pos_z;

	up_x = parent->plane_up_x;
	up_y = parent->plane_up_y;
	up_z = parent->plane_up_z;


	include_map.clear();
	intersect_map.clear();
	newJobFlag = true;
	waitFlag = false;
}


void ThreadSubregion::save(long long int volume, int level) {
	mParent->SubregionVolume = volume;

	mParent->TouchIndex.clear();
	mParent->IntersectIndex.clear();
	mParent->CompleteIndex.clear();

	mParent->SubregionPrecision = mGlobals.CurrentProject->DataLevel - level + 1;
	std::map<unsigned int, bool>::iterator mapmParent = include_map.begin();
	for (mapmParent = include_map.begin(); mapmParent != include_map.end(); ++mapmParent) {
		mParent->TouchIndex[mapmParent->first] = true;
		mParent->CompleteIndex[mapmParent->first] = true;
	}
	for (mapmParent = intersect_map.begin(); mapmParent != intersect_map.end(); ++mapmParent) {
		mParent->IntersectIndex[mapmParent->first] = true;
		mParent->CompleteIndex.erase(mapmParent->first);
	}
}


long long int ThreadSubregion::getVolume(int level) {
	Utils mUtil;
	QDir directory(QString::fromStdString(SubregionPath));
	QStringList target; 
	target << "*_" + QString::fromStdString(to_string(level)) + ".block";
	QStringList blocks = directory.entryList( target, QDir::Files);
	long long int volume = 0;
	long long int lvalue = 1;
	long long int size = lvalue * mGlobals.CurrentProject->DataBlockSize * mGlobals.CurrentProject->DataBlockSize * mGlobals.CurrentProject->DataBlockSize;
	unsigned char *subregion_block = new unsigned char[size];
	unsigned int *cell_block = new unsigned int[size];

	float information[12];
	information[0] = mGlobals.CurrentProject->DataBlockSize;
	information[1] = mGlobals.CurrentProject->mLayerCell->MaxCellCount + 1;
	information[2] = pos_x / pow(2, level);
	information[3] = pos_y / pow(2, level);
	information[4] = pos_z;
	information[5] = up_x;
	information[6] = up_y;
	information[7] = up_z;
	information[11] = level;




	long long int readtime = 0;
	long long int processtime = 0;
	QTime t;

	int loopsize = blocks.size();
	float progress_value = 0;
	emit progress_update(0);
	foreach(QString filename, blocks) {
		progress_value += 1.0 / (float)loopsize * 100;
		emit progress_update(progress_value);
		if (newJobFlag) { break; }
		std::string path = SubregionPath + filename.toStdString();
		std::string cell_path = mGlobals.CurrentProject->mLayerCell->DataPathXY + filename.toStdString();
		vector<std::string> str_vec = mUtil.Split(filename.toStdString().c_str(), "_");

		information[8] = (float)stoi(str_vec[2]) * (float)mGlobals.CurrentProject->DataBlockSize;
		information[9] = (float)stoi(str_vec[1]) * (float)mGlobals.CurrentProject->DataBlockSize;
		information[10] = (float)stoi(str_vec[0]) * (float)mGlobals.CurrentProject->DataBlockSize;
				
		FILE *subregion_blck_rfp = fopen(path.c_str(), "rb");
		fread(subregion_block, size, 1, subregion_blck_rfp);
		volume += cuda_parallel_sum(subregion_block, information);
		
		if (QFileInfo::exists(QString::fromStdString(cell_path)) && QFileInfo(QString::fromStdString(cell_path)).isFile()) {
			FILE *cell_blck_rfp = fopen(cell_path.c_str(), "rb");
			fread(cell_block, size * sizeof(unsigned int), 1, cell_blck_rfp);
			int *table = cuda_parallel_inclusion(subregion_block, cell_block, information);
			
			for (int i = 0; i <= mGlobals.CurrentProject->mLayerCell->MaxCellCount; ++i) {
				if (table[i] > 0) {
					include_map[i] = true;
				}
				
				if (table[i] == 2) {
					intersect_map[i] = true;
				}
			}
			fclose(cell_blck_rfp);
		}
		fclose(subregion_blck_rfp);
	}

	delete[] subregion_block;
	delete[] cell_block;

	qDebug() << "Readtime : " << readtime;
	qDebug() << "Proctime : " << processtime;
	return volume;
}

void ThreadSubregion::run()
{
	int deviceCount = 0;
	int cudaDevice = 0;
	char cudaDeviceName[100];
	cuInit(0);
	cuDeviceGetCount(&deviceCount);
	cuDeviceGet(&cudaDevice, 0);
	cuDeviceGetName(cudaDeviceName, 100, cudaDevice);
	qDebug() << "Number of devices: " << deviceCount;
	qDebug() << "Device name:" << cudaDeviceName;

	while (true) {
	
		while (waitFlag) {
			sleep(1);
		}
				
		for (int i = mGlobals.CurrentProject->DataLevel; i > 0; i--) {
			if (newJobFlag) {
				i = mGlobals.CurrentProject->DataLevel + 1;
				newJobFlag = false;
				continue;
			}

			int temp = mGlobals.CurrentProject->DataLevel - i;
			emit progress_log_update("Calculating precision level " + QString::fromStdString(std::to_string(temp)));
			std::string str = "Calculating " + SubregionID + "- level" + std::to_string(i+1) + " ";
			//mGlobals.mainWindow->statusBar()->showMessage(QString::fromStdString(str));
			qDebug() << QString::fromStdString(str);
			long long int volume = getVolume(i);
			volume = volume * pow(2, i) * pow(2, i);
			qDebug() << volume;

			if (newJobFlag) {
				i = mGlobals.CurrentProject->DataLevel + 1;
				newJobFlag = false;
				continue;
			}
			else {
				save(volume, i);
				emit subregion_information_update(QString::fromStdString(SubregionID));
			}
		}
		emit progress_log_update("Finish");
		waitFlag = true;
	}
}
