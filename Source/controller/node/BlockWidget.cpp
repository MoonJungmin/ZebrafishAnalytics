#include "BlockWidget.h"

BlockWidget::BlockWidget(QObject *parent)
	: QObject(parent)
{
}

BlockWidget::~BlockWidget()
{
}

void BlockWidget::initialize(std::string name, int aflags, int awidth, int aheight, QColor acolor, QNEBlock *parent) {
	BlockName = name;
	BackgroundColor = acolor;
	BlockFlag = aflags;
	generate_ToolBox(aflags);



	switch (aflags) {
		case OriginBlock:
			CellIndexListOutput = CellIndexListInput;
			generate_DataHeatmap(awidth, aheight);
			generate_DataInputOutput(awidth, aheight);
			break;
		case DataBlock:
			CellIndexListOutput = CellIndexListInput;
			generate_DataHeatmap(awidth, aheight);
			generate_DataInputOutput(awidth, aheight);
			break;
		case FeatureBlock:
			CellIndexListOutput = CellIndexListInput;
			generate_FeatureHistogram(awidth, aheight);
			generate_FeatureDropdown(awidth, aheight);
			generate_DataInputOutput(awidth, aheight);
			break;
		case SubregionBlock:
			CellIndexListOutput = CellIndexListInput;
			generate_SubregionDropdown(awidth, aheight);
			generate_DataInputOutput(awidth, aheight);
			break;

		case SimilarityBlock:

			break;
		case SetBlock:
			CellIndexListOutput = CellIndexListInput;
			generate_SetDropdown(awidth, aheight);
			generate_DataInputOutput(awidth, aheight);
			break;
	}
	



	m_block = parent;
}

void BlockWidget::generate_ToolBox(int aflag) {
	
	ToolBox = new QWidget;
	QString backgroundcolor_style = "background-color:" + BackgroundColor.name();
	ToolBox->setStyleSheet(backgroundcolor_style);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->setContentsMargins(0, 5, 2, 5);

	if (aflag == OriginBlock || aflag == DataBlock) {
		BucketColorBtn = new QPushButton;
		BucketColorBtn->setFixedSize(QSize(15, 15));
		BucketColorBtn->setFlat(true);
		BucketColorBtn->setAutoFillBackground(true);
		DataColor = QColor(255, 255, 123);
		updateColorBox(BucketColorBtn, DataColor);
		layout->addWidget(BucketColorBtn);
		connect(BucketColorBtn, SIGNAL(released()), this, SLOT(handleBlockColorBtn()));

	}

	AnnotationBtn = new QPushButton;
	AnnotationBtn->setIcon(QIcon("Resources/icon_annotation.png"));
	AnnotationBtn->setIconSize(QSize(15, 15));
	AnnotationBtn->setFixedSize(QSize(18, 18));
	connect(AnnotationBtn, SIGNAL(released()), this, SLOT(handleAnnotationBtn()));
	layout->addWidget(AnnotationBtn);

	StatisticsBtn = new QPushButton;
	StatisticsBtn->setIcon(QIcon("Resources/icon_statistics.png"));
	StatisticsBtn->setIconSize(QSize(15, 15));
	StatisticsBtn->setFixedSize(QSize(18, 18));
	connect(StatisticsBtn, SIGNAL(released()), this, SLOT(handleStatisticsBtn()));
	layout->addWidget(StatisticsBtn);

	if (aflag != OriginBlock) {
		CloseBtn = new QPushButton;
		CloseBtn->setIcon(QIcon("Resources/icon_blockclose.png"));
		CloseBtn->setIconSize(QSize(15, 15));
		CloseBtn->setFixedSize(QSize(18, 18));
		connect(CloseBtn, SIGNAL(released()), this, SLOT(handleCloseBtn()));
		layout->addWidget(CloseBtn);
	}
	

	ToolBox->setLayout(layout);
}

void BlockWidget::generate_DataHeatmap(int width, int height) {
	
	DataHeatmap = new ViewHeatmapGLWidget;

	QImage temp(QString::fromStdString(mGlobals.CurrentProject->mLayerBack->BackgroundThumbnailPath));
	float ratio = (float)temp.height() / (float)temp.width();
	qDebug() << "ratio : " << ratio;

	DataHeatmap->setRenderingSize(width, width * ratio);
	DataHeatmap->setTexturePath(mGlobals.CurrentProject->mLayerBack->BackgroundThumbnailPath);
	DataHeatmap->setBackgroundColor(BackgroundColor);
	DataHeatmap->setFixedSize(QSize(width, width * ratio));
	DataHeatmap->setContentsMargins(0, 0, 0, 0);

	coord Datasize;
	Datasize.x = mGlobals.CurrentProject->DataSizeX;
	Datasize.y = mGlobals.CurrentProject->DataSizeY;
	Datasize.z = mGlobals.CurrentProject->DataSizeZ;
	DataHeatmap->setHeatmapData(&mGlobals.CurrentProject->mLayerCell->mCellList, &this->CellIndexListInput, Datasize);
	DataHeatmap->updateDataColor(DataColor);
	//DataHeatmap->update();

}
void BlockWidget::generate_DataInputOutput(int width, int height) {
	DataInputOutput = new QWidget;
	QString backgroundcolor_style = "background-color:" + BackgroundColor.name();
	DataInputOutput->setStyleSheet(backgroundcolor_style);
	QVBoxLayout *layout = new QVBoxLayout;
	
	QHBoxLayout *layout_input = new QHBoxLayout;
	layout_input->setContentsMargins(10, 0, 0, 0);
	QLabel *title_input = new QLabel;
	QFont font;
	font.setPointSize(8);
	font.setBold(true);
	title_input->setFont(font);
	if (BlockFlag == SetBlock) {
		title_input->setText("Input 1 :");
	}
	else {
		title_input->setText("Input :");
	}


	count_input1= new QLabel;
	font.setBold(false);
	count_input1->setFont(font);
	int size_input = CellIndexListInput.size();
	count_input1->setText(QString::fromStdString(std::to_string(size_input)));
	layout_input->addWidget(title_input);
	layout_input->addWidget(count_input1);

	QHBoxLayout *layout_input2 = new QHBoxLayout;
	layout_input2->setContentsMargins(10, 0, 0, 0);
	QLabel *title_input2 = new QLabel;
	
	font.setPointSize(8);
	font.setBold(true);
	title_input2->setFont(font);
	title_input2->setText("Input 2 :");

	count_input2 = new QLabel;
	font.setBold(false);
	count_input2->setFont(font);
	int size_input2 = CellIndexListInput_Sub.size();
	count_input2->setText(QString::fromStdString(std::to_string(size_input2)));
	layout_input2->addWidget(title_input2);
	layout_input2->addWidget(count_input2);

	QHBoxLayout *layout_output = new QHBoxLayout;
	layout_output->setContentsMargins(10, 0, 0, 0);
	QLabel *title_output = new QLabel;
	font.setPointSize(8);
	font.setBold(true);
	title_output->setFont(font);
	title_output->setText("Output :");

	count_output = new QLabel;
	font.setBold(false);
	count_output->setFont(font);
	int size_output = CellIndexListOutput.size();
	count_output->setText(QString::fromStdString(std::to_string(size_output)));
	layout_output->addWidget(title_output);
	layout_output->addWidget(count_output);

	layout->addLayout(layout_input);
	if (BlockFlag == SetBlock) {
		layout->addLayout(layout_input2);
	}
	layout->addLayout(layout_output);

	DataInputOutput->setLayout(layout);
}

void BlockWidget::generate_SubregionDropdown(int width, int height) {
	SubregionDropdown = new QComboBox;
	SubregionDropdown->setFixedWidth(width - 20);
	for each (LayerSubregion subregion in mGlobals.CurrentProject->mSubregion) {
		SubregionDropdown->addItem(QString::fromStdString(subregion.SubregionName));
	}
	connect(SubregionDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
}
void BlockWidget::updatedSubregionList() {
	disconnect(SubregionDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
	SubregionDropdown->clear();
	for each (LayerSubregion subregion in mGlobals.CurrentProject->mSubregion) {
		SubregionDropdown->addItem(QString::fromStdString(subregion.SubregionName));
	}
	connect(SubregionDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
}


void BlockWidget::generate_FeatureHistogram(int width, int height) {
	FeatureHistogramMaster = new QWidget;
	FeatureHistogramMaster->setFixedSize(QSize(width-10, 100));
	FeatureHistogramMaster->setContentsMargins(1, 1, 1, 1);
	FeatureHistogram = new ViewHistogramWidget(FeatureHistogramMaster);
	FeatureHistogram->setRenderingSize(width-10, 100);
	FeatureHistogram->setData(0, &CellIndexListInput, &CellIndexListOutput);
	connect(FeatureHistogram, SIGNAL(OutputUpdated()), this, SLOT(handleHistogramUpdate()));
}

void BlockWidget::generate_FeatureDropdown(int width, int height) {
	FeatureDropdown = new QComboBox;
	FeatureDropdown->setFixedWidth(width - 20);
	for each (DataFeature feature in mGlobals.CurrentProject->mFeature)
	{
		FeatureDropdown->addItem(QString::fromStdString(feature.FeatureName));
	}
	connect(FeatureDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
}

void BlockWidget::updatedFeatureList() {
	disconnect(FeatureDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
	FeatureDropdown->clear();
	for each (DataFeature feature in mGlobals.CurrentProject->mFeature) {
		FeatureDropdown->addItem(QString::fromStdString(feature.FeatureName));
	}
	connect(FeatureDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
}


void BlockWidget::generate_SetDropdown(int width, int height) {
	SetDropdown = new QComboBox;
	SetDropdown->setFixedWidth(width - 20);
	QStringList operation = { "Union", "Subtraction", "Intersection" };
	SetDropdown->addItems(operation);
	connect(SetDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
}


void BlockWidget::handleBlockColorBtn() {
	QColorDialogTester color_test;
	QString color_str = color_test.onColor();
	qDebug() << color_str;
	if (color_str.length() > 2) {
		DataColor = QColor(color_str);
		updateColorBox(BucketColorBtn, QColor(color_str));
	}

	DataHeatmap->updateDataColor(DataColor);
}

void BlockWidget::handleCloseBtn() {
	qDebug() << "CloseBtn";
	emit deleteNode(m_block);
}

void BlockWidget::handleAnnotationBtn() {
	qDebug() << "AnnotationBtn";
	mGlobals.mDialogManager->mDialogAnnotation->setData(this);
	mGlobals.mDialogManager->mDialogAnnotation->exec();
}
void BlockWidget::handleStatisticsBtn() {
	qDebug() << "StatisticsBtn";
	mGlobals.mDialogManager->mDialogStatistics->generateStatistics(QString::fromStdString(BlockName), &CellIndexListInput);
	mGlobals.mDialogManager->mDialogStatistics->exec();
}

void BlockWidget::updateColorBox(QPushButton *target, QColor color) {
	qDebug() << "updateColorBox" << " " << color.name();
	target->setStyleSheet("");
	QPalette mPalette = target->palette();
	mPalette.setColor(target->backgroundRole(), color);
	target->setPalette(mPalette);
	target->setStyleSheet(QString("background-color: %1;foreground-color: %1; border-style: none;").arg(color.name()));
	target->setAutoFillBackground(true);
	target->setFlat(true);
	target->repaint();
}
void BlockWidget::handleDropdownChange(int index) {
	updatedInputList();
}
void BlockWidget::handleHistogramUpdate() {
	int size_output = CellIndexListOutput.size();
	count_output->setText(QString::fromStdString(std::to_string(size_output)));
	checkNextBlock();
}


void BlockWidget::addAnnotation(QString cmt) {
	annotation temp;
	temp.comment = cmt;
	temp.time = QDateTime::currentDateTime();
	mAnnotation.push_back(temp);
}
void BlockWidget::removeAnnotation(int index) {
	int step = 0;
	std::list<annotation>::iterator iter;
	for (iter = this->mAnnotation.begin(); iter != this->mAnnotation.end(); ++iter) {
		if (step == index) {
			this->mAnnotation.erase(iter);
			break;
		}
		step++;
	}
}

void BlockWidget::updatedCellColor() {
	std::vector<cell>::iterator iter_global= mGlobals.CurrentProject->mLayerCell->mCellList.begin();
	for (iter_global = mGlobals.CurrentProject->mLayerCell->mCellList.begin(); iter_global != mGlobals.CurrentProject->mLayerCell->mCellList.end(); ++iter_global) {
		iter_global->status = false;
	}
	
	std::list<unsigned int>::iterator iter_selected = CellIndexListOutput.begin();
	for (iter_selected = CellIndexListOutput.begin(); iter_selected != CellIndexListOutput.end(); ++iter_selected) {
		mGlobals.CurrentProject->mLayerCell->mCellList.at(*iter_selected-1).status = true;
	}
	mGlobals.CurrentProject->SelectedColor = DataColor;
}


void BlockWidget::updatedInputList() {
	
	int size_input = CellIndexListInput.size();
	count_input1->setText(QString::fromStdString(std::to_string(size_input)));

	if (BlockFlag == DataBlock || BlockFlag == OriginBlock) {
		CellIndexListOutput = CellIndexListInput;
		int size_output = CellIndexListOutput.size();
		count_output->setText(QString::fromStdString(std::to_string(size_output)));
		DataHeatmap->updateHeatmap();
	}
	else if (BlockFlag == SubregionBlock) {
		CellIndexListOutput.clear();
		std::list<unsigned int>::iterator iter = CellIndexListInput.begin();
		for (iter = CellIndexListInput.begin(); iter != CellIndexListInput.end(); ++iter) {
			if (mGlobals.CurrentProject->mSubregion[SubregionDropdown->currentIndex()].IncludeIndex.find(*iter) != mGlobals.CurrentProject->mSubregion[SubregionDropdown->currentIndex()].IncludeIndex.end()) {
				CellIndexListOutput.push_back(*iter);
			}
		}
		int size_output = CellIndexListOutput.size();
		count_output->setText(QString::fromStdString(std::to_string(size_output)));
	}
	else if (BlockFlag == FeatureBlock) {
		CellIndexListOutput.clear();
		FeatureHistogram->setData(FeatureDropdown->currentIndex(), &CellIndexListInput, &CellIndexListOutput);
		int size_output = CellIndexListOutput.size();
		count_output->setText(QString::fromStdString(std::to_string(size_output)));
	}
	else if (BlockFlag == SimilarityBlock) {

	}
	else if (BlockFlag == SetBlock) {
		int size_input2 = CellIndexListInput_Sub.size();
		count_input2->setText(QString::fromStdString(std::to_string(size_input2)));
		CellIndexListOutput.clear();
		set_operation(SetDropdown->currentIndex());
		int size_output = CellIndexListOutput.size();
		count_output->setText(QString::fromStdString(std::to_string(size_output)));
	}

	checkNextBlock();
}

void BlockWidget::checkNextBlock() {
	foreach(QNEPort *port, m_block->ports()) {
		if (port->portAlign() == QNEPort::Output) {
			foreach(QNEConnection *conn, port->connections()) {
				QNEBlock *next = conn->port2()->m_block;
				if (conn->port2()->portFlags() == QNEPort::SetSubInputPort) {
					next->mBlock->CellIndexListInput_Sub.clear();
					std::copy(CellIndexListOutput.begin(), CellIndexListOutput.end(), std::back_inserter(next->mBlock->CellIndexListInput_Sub));
					next->mBlock->updatedInputList();
				}
				else {
					next->mBlock->CellIndexListInput.clear();
					std::copy(CellIndexListOutput.begin(), CellIndexListOutput.end(), std::back_inserter(next->mBlock->CellIndexListInput));
					next->mBlock->updatedInputList();
				}
				
			}

		}
	}
}


void BlockWidget::set_operation(int index) {
	std::map<unsigned int, bool> temp;
	if (index == 0) { // Union
		std::list<unsigned int>::iterator iter = CellIndexListInput.begin();
		for (iter = CellIndexListInput.begin(); iter != CellIndexListInput.end(); ++iter) {
			temp[*iter] = true;
		}
		for (iter = CellIndexListInput_Sub.begin(); iter != CellIndexListInput_Sub.end(); ++iter) {
			temp[*iter] = true;
		}
		std::map<unsigned int, bool>::iterator mapiter = temp.begin();
		for (mapiter = temp.begin(); mapiter != temp.end(); ++mapiter) {
			CellIndexListOutput.push_back(mapiter->first);
		}
	}
	else if (index == 1) { // Subtraction
		std::list<unsigned int>::iterator iter = CellIndexListInput.begin();
		for (iter = CellIndexListInput.begin(); iter != CellIndexListInput.end(); ++iter) {
			temp[*iter] = true;
		}
		for (iter = CellIndexListInput_Sub.begin(); iter != CellIndexListInput_Sub.end(); ++iter) {
			std::map<unsigned int, bool>::iterator mapiter = temp.find(*iter);
			if (mapiter != temp.end()) {
				temp.erase(mapiter);
			}
		}
		std::map<unsigned int, bool>::iterator mapiter = temp.begin();
		for (mapiter = temp.begin(); mapiter != temp.end(); ++mapiter) {
			CellIndexListOutput.push_back(mapiter->first);
		}
	}
	else if (index == 2) { //Intersection
		std::list<unsigned int>::iterator iter = CellIndexListInput.begin();
		for (iter = CellIndexListInput.begin(); iter != CellIndexListInput.end(); ++iter) {
			temp[*iter] = true;
		}
		for (iter = CellIndexListInput_Sub.begin(); iter != CellIndexListInput_Sub.end(); ++iter) {
			if (temp.find(*iter) != temp.end()) {
				CellIndexListOutput.push_back(*iter);
			}
		}
	}
}
