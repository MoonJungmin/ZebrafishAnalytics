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
	BackgroundColor_style = "background-color : rgba(0,0,0,0)";
	BlockFlag = aflags;
	generate_ToolBox(aflags);
	mThread = new ThreadSubregion;
	connect(mThread, SIGNAL(subregion_information_update(QString)), this, SLOT(handleSubregionInformationUpdate(QString)));
	mFont.setPointSize(8);
	mFont.setBold(true);

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
			mThread->start();
			CellIndexListOutput = CellIndexListInput;
			generate_SubregionInformation(awidth, aheight);
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
	ToolBox->setAttribute(Qt::WA_TranslucentBackground);
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
	width = width - 2;
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
	DataInputOutput->setAttribute(Qt::WA_TranslucentBackground);
	QVBoxLayout *layout = new QVBoxLayout;
	
	QHBoxLayout *layout_input = new QHBoxLayout;
	layout_input->setContentsMargins(10, 0, 0, 0);
	QLabel *title_input = new QLabel;
	
	title_input->setFont(mFont);
	if (BlockFlag == SetBlock) {
		title_input->setText("Input 1 :");
	}
	else {
		title_input->setText("Input :");
	}


	count_input1= new QLabel;
	mFont.setBold(false);
	count_input1->setFont(mFont);
	int size_input = CellIndexListInput.size();
	count_input1->setText(QString::fromStdString(std::to_string(size_input)) + " cells");
	layout_input->addWidget(title_input);
	layout_input->addWidget(count_input1);

	QHBoxLayout *layout_input2 = new QHBoxLayout;
	layout_input2->setContentsMargins(10, 0, 0, 0);
	QLabel *title_input2 = new QLabel;
	
	mFont.setPointSize(8);
	mFont.setBold(true);
	title_input2->setFont(mFont);
	title_input2->setText("Input 2 :");

	count_input2 = new QLabel;
	mFont.setBold(false);
	count_input2->setFont(mFont);
	int size_input2 = CellIndexListInput_Sub.size();
	count_input2->setText(QString::fromStdString(std::to_string(size_input2)) + " cells");
	layout_input2->addWidget(title_input2);
	layout_input2->addWidget(count_input2);

	QHBoxLayout *layout_output = new QHBoxLayout;
	layout_output->setContentsMargins(10, 0, 0, 0);
	QLabel *title_output = new QLabel;
	mFont.setPointSize(8);
	mFont.setBold(true);
	title_output->setFont(mFont);
	title_output->setText("Output :");

	count_output = new QLabel;
	mFont.setBold(false);
	count_output->setFont(mFont);
	int size_output = CellIndexListOutput.size();
	count_output->setText(QString::fromStdString(std::to_string(size_output)) + " cells");
	layout_output->addWidget(title_output);
	layout_output->addWidget(count_output);

	layout->addLayout(layout_input);
	if (BlockFlag == SetBlock) {
		layout->addLayout(layout_input2);
	}
	layout->addLayout(layout_output);

	
	DataInputOutput->setLayout(layout);

}
void BlockWidget::generate_SubregionInformation(int width, int height) {
	SubregionInformationMaster = new QWidget;
	SubregionInformationMaster->setFixedWidth(width - 20);
	SubregionInformationMaster->setAttribute(Qt::WA_TranslucentBackground);

	QVBoxLayout *layout = new QVBoxLayout;
	QHBoxLayout *precision_layout = new QHBoxLayout;
	precision_layout->setContentsMargins(10, 0, 0, 0);
	QLabel *label0 = new QLabel;
	label0->setText("Precision :");
	mFont.setPointSize(8);
	mFont.setBold(true);
	label0->setFont(mFont);
	precision_layout->addWidget(label0);
	subreigon_precision = new QLabel;
	subreigon_precision->setText("level 1");
	mFont.setPointSize(8);
	mFont.setBold(false);
	subreigon_precision->setFont(mFont);
	precision_layout->addWidget(subreigon_precision);

	QHBoxLayout *volume_layout = new QHBoxLayout;
	volume_layout->setContentsMargins(10, 0, 0, 0);
	QLabel *label1 = new QLabel;
	label1->setText("Volume :");
	mFont.setPointSize(8);
	mFont.setBold(true);
	label1->setFont(mFont);
	volume_layout->addWidget(label1);
	subreigon_volume = new QLineEdit;
	subreigon_volume->setText("0");
	mFont.setPointSize(8);
	mFont.setBold(false);
	subreigon_volume->setFont(mFont);
	volume_layout->addWidget(subreigon_volume);

	QLabel *label2 = new QLabel;
	label2->setText("voxel");
	mFont.setPointSize(8);
	mFont.setBold(true);
	label2->setFont(mFont);
	volume_layout->addWidget(label2);


	layout->addLayout(precision_layout);
	layout->addLayout(volume_layout);

	SubregionInformationMaster->setLayout(layout);

}



void BlockWidget::generate_SubregionDropdown(int width, int height) {
	SubregionDropdownMaster = new QWidget;
	SubregionDropdownMaster->setFixedWidth(width-20);
	SubregionDropdownMaster->setAttribute(Qt::WA_TranslucentBackground);
	
	QVBoxLayout *layout = new QVBoxLayout;
	
	SubregionProgressbarLog = new QLabel;
	SubregionProgressbarLog->setText("Progress");
	SubregionProgressbarLog->setFont(mFont);
	layout->addWidget(SubregionProgressbarLog);

	SubregionProgressbar = new QProgressBar;
	connect(mThread, SIGNAL(progress_update(float)), this, SLOT(subregion_progess_update(float)));
	connect(mThread, SIGNAL(progress_log_update(QString)), this, SLOT(subregion_progess_log_update(QString)));
	layout->addWidget(SubregionProgressbar);

	QLabel *label = new QLabel;
	label->setText("Current Subregion");
	label->setFont(mFont);
	layout->addWidget(label);
	
	SubregionDropdown = new QComboBox;
	for each (LayerSubregion subregion in mGlobals.CurrentProject->mSubregion) {
		SubregionDropdown->addItem(QString::fromStdString(subregion.SubregionName));
	}
	//connect(SubregionDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
	layout->addWidget(SubregionDropdown);


	subregion_pos_x = new QLineEdit;
	subregion_pos_y = new QLineEdit;
	subregion_pos_z = new QLineEdit;
	subregion_up_x = new QLineEdit;
	subregion_up_y = new QLineEdit;
	subregion_up_z = new QLineEdit;

	QHBoxLayout *interface_pos_layout = new QHBoxLayout;
	QLabel *label2 = new QLabel;
	label2->setText("Pos X");
	label2->setFont(mFont);
	interface_pos_layout->addWidget(label2);
	subregion_pos_x->setText(QString::fromStdString(std::to_string(plane_pos_x)));
	interface_pos_layout->addWidget(subregion_pos_x);
	
	QLabel *label3 = new QLabel;
	label3->setText("Pos Y");
	label3->setFont(mFont);
	interface_pos_layout->addWidget(label3);
	subregion_pos_y->setText(QString::fromStdString(std::to_string(plane_pos_y)));
	interface_pos_layout->addWidget(subregion_pos_y);

	QLabel *label4 = new QLabel;
	label4->setText("Pos Z");
	label4->setFont(mFont);
	interface_pos_layout->addWidget(label4);
	subregion_pos_z->setText(QString::fromStdString(std::to_string(plane_pos_z)));
	interface_pos_layout->addWidget(subregion_pos_z);

	QHBoxLayout *interface_up_layout = new QHBoxLayout;
	QLabel *label5 = new QLabel;
	label5->setText("Up X");
	label5->setFont(mFont);
	interface_up_layout->addWidget(label5);
	subregion_up_x->setText(QString::fromStdString(std::to_string(plane_up_x)));
	interface_up_layout->addWidget(subregion_up_x);

	QLabel *label6 = new QLabel;
	label6->setText("Up Y");
	label6->setFont(mFont);
	interface_up_layout->addWidget(label6);
	subregion_up_y->setText(QString::fromStdString(std::to_string(plane_up_y)));
	interface_up_layout->addWidget(subregion_up_y);

	QLabel *label7 = new QLabel;
	label7->setText("Up Z");
	label7->setFont(mFont);
	interface_up_layout->addWidget(label7);
	subregion_up_z->setText(QString::fromStdString(std::to_string(plane_up_z)));
	interface_up_layout->addWidget(subregion_up_z);


	QLabel *label8 = new QLabel;
	label8->setText("Select Option");
	label8->setFont(mFont);
	layout->addWidget(label8);

	SubregionSelectMethod = new QComboBox;
	QStringList optionlist = { "Complete Overlap", "Intersection", "Touch" };
	SubregionSelectMethod->addItems(optionlist);
	//connect(SubregionSelectMethod, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
	layout->addWidget(SubregionSelectMethod);



	QPushButton *setbtn = new QPushButton;

	setbtn->setText("Generate");
	connect(setbtn, SIGNAL(released()), this, SLOT(handleSubregionSetBtn()));
	layout->addLayout(interface_pos_layout);
	layout->addLayout(interface_up_layout);
	layout->addWidget(setbtn);
	
	SubregionDropdownMaster->setLayout(layout);
}
void BlockWidget::updatedSubregionList() {
	int current_index = SubregionDropdown->currentIndex();
	disconnect(SubregionDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
	SubregionDropdown->clear();
	for each (LayerSubregion subregion in mGlobals.CurrentProject->mSubregion) {
		SubregionDropdown->addItem(QString::fromStdString(subregion.SubregionName));
	}
	SubregionDropdown->setCurrentIndex(current_index);
	connect(SubregionDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
}

void BlockWidget::subregion_progess_update(float v) {
	if (v > 99) {
		SubregionProgressbar->setValue(100);
	}
	else {
		SubregionProgressbar->setValue((int)v);
	}
}

void BlockWidget::subregion_progess_log_update(QString str) {
	SubregionProgressbarLog->setText(str);
}

void BlockWidget::generate_FeatureHistogram(int width, int height) {
	FeatureHistogramMaster = new QWidget;
	FeatureHistogramMaster->setFixedWidth(width - 10);
	FeatureHistogramMaster->setAttribute(Qt::WA_TranslucentBackground);
	QVBoxLayout *layout = new QVBoxLayout;
	QLabel *label = new QLabel;
	label->setText("Feature Histogram");
	label->setFont(mFont);
	layout->addWidget(label);
	
	histogram_start = new QLineEdit;
	histogram_start->setText("0");
	histogram_end = new QLineEdit;
	histogram_end->setText("0");
	QWidget *FeatureHistBox = new QWidget;
	FeatureHistBox->setContentsMargins(1, 1, 1, 1);
	FeatureHistogram = new ViewHistogramWidget(FeatureHistBox);
	FeatureHistogram->setInterface(histogram_start, histogram_end);
	FeatureHistogram->setRenderingSize(width-11, 100);
	FeatureHistogram->setData(0, &CellIndexListInput, &CellIndexListOutput);
	connect(FeatureHistogram, SIGNAL(OutputUpdated()), this, SLOT(handleHistogramUpdate()));
	layout->addWidget(FeatureHistBox);

	QHBoxLayout *interface_layout = new QHBoxLayout;
	QLabel *label2 = new QLabel;
	label2->setText("Start(%)");
	label2->setFont(mFont);
	interface_layout->addWidget(label2);
	interface_layout->addWidget(histogram_start);
	QLabel *label3 = new QLabel;
	label3->setText("End(%)");
	label3->setFont(mFont);
	interface_layout->addWidget(label3);
	interface_layout->addWidget(histogram_end);
	QPushButton *setbtn = new QPushButton;
	
	setbtn->setText("Set");
	connect(setbtn, SIGNAL(released()), this, SLOT(handleHistogramSetBtn()));
	interface_layout->addWidget(setbtn);
	
	layout->addLayout(interface_layout);
	FeatureHistogramMaster->setLayout(layout);
}

void BlockWidget::generate_FeatureDropdown(int width, int height) {

	FeatureDropdownMaster = new QWidget;
	FeatureDropdownMaster->setFixedWidth(width - 20);
	FeatureDropdownMaster->setAttribute(Qt::WA_TranslucentBackground);

	QVBoxLayout *layout = new QVBoxLayout;
	QLabel *label = new QLabel;
	label->setText("Current Feature");
	label->setFont(mFont);
	layout->addWidget(label);
	
	FeatureDropdown = new QComboBox;
	for each (DataFeature feature in mGlobals.CurrentProject->mFeature)
	{
		FeatureDropdown->addItem(QString::fromStdString(feature.FeatureName));
	}
	connect(FeatureDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
	layout->addWidget(FeatureDropdown);
	FeatureDropdownMaster->setLayout(layout);
}

void BlockWidget::updatedFeatureList() {
	int current = FeatureDropdown->currentIndex();
	disconnect(FeatureDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
	FeatureDropdown->clear();
	for each (DataFeature feature in mGlobals.CurrentProject->mFeature) {
		FeatureDropdown->addItem(QString::fromStdString(feature.FeatureName));
	}
	FeatureDropdown->setCurrentIndex(current);
	connect(FeatureDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
}


void BlockWidget::generate_SetDropdown(int width, int height) {
	
	SetDropdownMaster = new QWidget;
	SetDropdownMaster->setFixedWidth(width - 20);
	SetDropdownMaster->setAttribute(Qt::WA_TranslucentBackground);

	QVBoxLayout *layout = new QVBoxLayout;
	QLabel *label = new QLabel;
	label->setText("Set operation");
	label->setFont(mFont);
	layout->addWidget(label);

	SetDropdown = new QComboBox;
	QStringList operation = { "Union", "Subtraction", "Intersection" };
	SetDropdown->addItems(operation);
	connect(SetDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(handleDropdownChange(int)));
	layout->addWidget(SetDropdown);

	SetDropdownMaster->setLayout(layout);
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
	count_output->setText(QString::fromStdString(std::to_string(size_output)) + " cells");
	checkNextBlock();
}
void BlockWidget::handleHistogramSetBtn() {
	FeatureHistogram->update_release_box((float)histogram_start->text().toInt() / 100.0, (float)histogram_end->text().toInt() / 100.0);
}
void BlockWidget::handleSubregionSetBtn() {
	//FeatureHistogram->update_release_box((float)histogram_start->text().toInt() / 100.0, (float)histogram_end->text().toInt() / 100.0);
	qDebug() << "handleSubregionSetBtn";
	plane_pos_x = stoi(subregion_pos_x->text().toStdString());
	plane_pos_y = stoi(subregion_pos_y->text().toStdString());
	plane_pos_z = stoi(subregion_pos_z->text().toStdString());
	plane_up_x = stoi(subregion_up_x->text().toStdString());
	plane_up_y = stoi(subregion_up_y->text().toStdString());
	plane_up_z = stoi(subregion_up_z->text().toStdString());

	int subregion_index = SubregionDropdown->currentIndex();
	mThread->addJob(mGlobals.CurrentProject->mSubregion[subregion_index].SubregionID, this);


	updatedInputList();
}

void BlockWidget::handleSubregionInformationUpdate(QString id) {
	int subregion_index = SubregionDropdown->currentIndex();
	subreigon_precision->setText("level " + QString::fromStdString(std::to_string(SubregionPrecision)));

	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << SubregionVolume;
	std::string mystring = ss.str();
	subreigon_volume->setText(QString::fromStdString(mystring));

	//subreigon_precision->setText(QString::fromStdString());
	

	CellIndexListOutput.clear();
	std::list<unsigned int>::iterator iter = CellIndexListInput.begin();
	for (iter = CellIndexListInput.begin(); iter != CellIndexListInput.end(); ++iter) {
		if (SubregionSelectMethod->currentIndex() == 0) {
			std::map<unsigned int, bool>::iterator target_iter = CompleteIndex.end();
			if (CompleteIndex.find(*iter) != target_iter) {
				CellIndexListOutput.push_back(*iter);
			}
		}
		else if (SubregionSelectMethod->currentIndex() == 1) {
			std::map<unsigned int, bool>::iterator target_iter = IntersectIndex.end();
			if (IntersectIndex.find(*iter) != target_iter) {
				CellIndexListOutput.push_back(*iter);
			}
		}
		else if (SubregionSelectMethod->currentIndex() == 2) {
			std::map<unsigned int, bool>::iterator target_iter = TouchIndex.end();
			if (TouchIndex.find(*iter) != target_iter) {
				CellIndexListOutput.push_back(*iter);
			}
		}
	}
	int size_output = CellIndexListOutput.size();
	count_output->setText(QString::fromStdString(std::to_string(size_output)) + " cells");
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
	
	mGlobals.CurrentProject->mLayerBack->HeatmapColor.r = DataColor.redF();
	mGlobals.CurrentProject->mLayerBack->HeatmapColor.g = DataColor.greenF();
	mGlobals.CurrentProject->mLayerBack->HeatmapColor.b = DataColor.blueF();
	mGlobals.CurrentProject->mLayerBack->HeatmapColor.a = 1.0;
	mGlobals.CurrentProject->mLayerBack->HeatmapUpdated = true;
}


void BlockWidget::updatedInputList() {
	
	int size_input = CellIndexListInput.size();
	count_input1->setText(QString::fromStdString(std::to_string(size_input)) + " cells");

	if (BlockFlag == DataBlock || BlockFlag == OriginBlock) {
		CellIndexListOutput = CellIndexListInput;
		int size_output = CellIndexListOutput.size();
		count_output->setText(QString::fromStdString(std::to_string(size_output)) + " cells");
		DataHeatmap->updateHeatmap();
	}
	else if (BlockFlag == SubregionBlock) {
		CellIndexListOutput.clear();
		int size_output = CellIndexListOutput.size();
		count_output->setText(QString::fromStdString(std::to_string(size_output)) + " cells");
	}
	else if (BlockFlag == FeatureBlock) {
		CellIndexListOutput.clear();
		FeatureHistogram->setData(FeatureDropdown->currentIndex(), &CellIndexListInput, &CellIndexListOutput);
		int size_output = CellIndexListOutput.size();
		count_output->setText(QString::fromStdString(std::to_string(size_output)) + " cells");
	}
	else if (BlockFlag == SimilarityBlock) {

	}
	else if (BlockFlag == SetBlock) {
		int size_input2 = CellIndexListInput_Sub.size();
		count_input2->setText(QString::fromStdString(std::to_string(size_input2)) + " cells");
		CellIndexListOutput.clear();
		set_operation(SetDropdown->currentIndex());
		int size_output = CellIndexListOutput.size();
		count_output->setText(QString::fromStdString(std::to_string(size_output)) + " cells");
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
