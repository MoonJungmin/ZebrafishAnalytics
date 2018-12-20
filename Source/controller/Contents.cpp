#include "Contents.h"

Contents::Contents(QWidget *parent)
	: QObject(parent)
{
	mWidget = parent;
}
Contents::Contents(QWidget *parent, QLayout *parent_layout)
	: QObject(parent)
{
	mWidget = parent;
	
	

	QSplitter * splitter = new QSplitter(mWidget);

	contents_left_tabwidget = new QTabWidget(mWidget);
	contents_center_tabwidget = new QTabWidget(mWidget);


	contents_left_tabwidget->addTab(new QWidget(), "Project Information");
	contents_left_tabwidget->addTab(new QWidget(), "Subregion & Feature");
	


	contents_center_tabwidget->addTab(new QWidget(), "4-View");
	contents_center_tabwidget->addTab(new QWidget(), "XY");
	contents_center_tabwidget->addTab(new QWidget(), "YZ");
	contents_center_tabwidget->addTab(new QWidget(), "ZX");
	contents_center_tabwidget->addTab(new QWidget(), "3D");
	contents_center_tabwidget->addTab(new QWidget(), "Graph");

	splitter->addWidget(contents_left_tabwidget);
	splitter->addWidget(contents_center_tabwidget);

	QList<int> sizes;
	sizes << 480 << 1440;
	splitter->setSizes(sizes);

	parent_layout->addWidget(splitter);


}


Contents::~Contents()
{
}

void Contents::LeftTabInit(QTabWidget *target, int width, int height) {
	//target->setFixedSize(width, height);

	target->addTab(new QWidget(), "Project Information");
	target->addTab(new QWidget(), "Subregion & Feature");
}
void Contents::CenterTabInit(QTabWidget *target, int width, int height) {
	//target->setFixedSize(width, height);
	
	target->addTab(new QWidget(), "4-View");
	target->addTab(new QWidget(), "XY");
	target->addTab(new QWidget(), "YZ");
	target->addTab(new QWidget(), "ZX");
	target->addTab(new QWidget(), "3D");
	target->addTab(new QWidget(), "Graph");


}

void Contents::replaceTab(QTabWidget * tabs, int index, QWidget * replacement, QString label)
{
	Q_ASSERT(tabs && tabs->count() > index);
	tabs->removeTab(index);
	if (replacement) tabs->insertTab(index, replacement, label);
}
void Contents::updateAllWidget(int index, bool scaleflag) {
	GL_XYAxis_Main->update();
	GL_YZAxis_Main->update();
	GL_ZXAxis_Main->update();
	GL_XYAxis_Sub->update();
	GL_YZAxis_Sub->update();
	GL_ZXAxis_Sub->update();
}


void Contents::InitGLView() {
	GL_XYAxis_Main = new ViewAxisGLWidget;
	GL_XYAxis_Main->setAxisIndex(1, 0);
	connect(GL_XYAxis_Main, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	GL_YZAxis_Main = new ViewAxisGLWidget;
	GL_YZAxis_Main->setAxisIndex(2, 1);
	connect(GL_YZAxis_Main, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	GL_ZXAxis_Main = new ViewAxisGLWidget;
	GL_ZXAxis_Main->setAxisIndex(3, 2);
	connect(GL_ZXAxis_Main, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));


	//GL_Volume_Main->setBackgroundVolumeTF(mGlobals.CurrentProject->mLayerBack->transferFunc);


	GL_Volume_Main = new ViewVolumeGLWidget;
	GL_Volume_Sub = new ViewVolumeGLWidget;

	replaceTab(contents_center_tabwidget, 1, GL_XYAxis_Main, "XY");
	replaceTab(contents_center_tabwidget, 2, GL_YZAxis_Main, "YZ");
	replaceTab(contents_center_tabwidget, 3, GL_ZXAxis_Main, "ZX");
	replaceTab(contents_center_tabwidget, 4, GL_Volume_Main, "3D");

	//GL_Volume_Main->setBackgroundVolume(mGlobals.CurrentProject->mLayerBack->volumeRenderData, width, height, depth);
	//GL_Volume_Main->setBackgroundVolumeTF(mGlobals.CurrentProject->mLayerBack->transferFunc);


	GL_XYAxis_Sub = new ViewAxisGLWidget;
	GL_XYAxis_Sub->setAxisIndex(1, 3);
	connect(GL_XYAxis_Sub, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	GL_YZAxis_Sub = new ViewAxisGLWidget;
	GL_YZAxis_Sub->setAxisIndex(2, 4);
	connect(GL_YZAxis_Sub, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	GL_ZXAxis_Sub = new ViewAxisGLWidget;
	GL_ZXAxis_Sub->setAxisIndex(3, 5);
	connect(GL_ZXAxis_Sub, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	//GL_Volume_Sub = new ViewVolumeGLWidget;


	QSplitter *view4_layout = new QSplitter;
	view4_layout->setOrientation(Qt::Vertical);
	QSplitter *first_lay = new QSplitter;
	first_lay->setOrientation(Qt::Horizontal);
	QSplitter *second_lay = new QSplitter;
	second_lay->setOrientation(Qt::Horizontal);
	first_lay->addWidget(GL_XYAxis_Sub);
	first_lay->addWidget(GL_YZAxis_Sub);
	second_lay->addWidget(GL_ZXAxis_Sub);
	second_lay->addWidget(GL_Volume_Sub);



	view4_layout->addWidget(first_lay);
	view4_layout->addWidget(second_lay);
	
	
	replaceTab(contents_center_tabwidget, 0, view4_layout, "4-View");


	

}

void Contents::InitGraph() {
	QWidget *widget = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout;
	GraphEditorView = new ViewGraphEditor(mWidget);
	GraphEditorView->initialize(layout);
	GraphEditorView->addOrigin();
	widget->setLayout(layout);
	replaceTab(contents_center_tabwidget, 5, widget, "Graph");
}

void Contents::InitProjectInfo() {
	QWidget *ProjectInfo_Container = new QWidget;
	Contents_ProjectInfo *master = new Contents_ProjectInfo(ProjectInfo_Container);
	replaceTab(contents_left_tabwidget, 0, ProjectInfo_Container, "Project Information");
}
void Contents::InitSubregionFeatureList() {
	QVBoxLayout *layout = new QVBoxLayout;
	QWidget *subregion = new QWidget(mWidget);
	QWidget *feature = new QWidget(mWidget);
	QVBoxLayout *subregion_layout = new QVBoxLayout;
	QHBoxLayout *subregion_title_layout = new QHBoxLayout;
	QVBoxLayout *feature_layout = new QVBoxLayout;
	QHBoxLayout *feature_title_layout = new QHBoxLayout;

	subregionList = new QListWidget(mWidget);
	featureList = new QListWidget(mWidget);

	QFont title_font;
	title_font.setPointSize(11);
	title_font.setBold(true);

	QLabel *subregion_title = new QLabel(mWidget);
	subregion_title->setText("Subregion");
	subregion_title->setFont(title_font);
	QWidget *filler1 = new QWidget;
	filler1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	QPushButton *subregionAddBtn = new QPushButton(mWidget);
	subregionAddBtn->setText("Add");
	subregion_title_layout->addWidget(subregion_title);
	subregion_title_layout->addWidget(filler1);
	subregion_title_layout->addWidget(subregionAddBtn);
	connect(subregionAddBtn, SIGNAL(released()), this, SLOT(handleSubregionAddBtn()));

	QLabel *feature_title = new QLabel(mWidget);
	feature_title->setText("Feature");
	feature_title->setFont(title_font);
	QPushButton *featureAddBtn = new QPushButton(mWidget);
	featureAddBtn->setText("Add");
	QWidget *filler2 = new QWidget;
	filler2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	feature_title_layout->addWidget(feature_title);
	feature_title_layout->addWidget(filler2);
	feature_title_layout->addWidget(featureAddBtn);
	connect(featureAddBtn, SIGNAL(released()), this, SLOT(handleFeatureAddBtn()));



	subregion_layout->addLayout(subregion_title_layout);
	subregion_layout->addWidget(subregionList);
	feature_layout->addLayout(feature_title_layout);
	feature_layout->addWidget(featureList);

	subregion->setLayout(subregion_layout);
	feature->setLayout(feature_layout);

	layout->addWidget(subregion);
	layout->addWidget(feature);

	QWidget *SubregionFeatureList_Container = new QWidget;
	SubregionFeatureList_Container->setLayout(layout);
	replaceTab(contents_left_tabwidget, 1, SubregionFeatureList_Container, "Subregion & Feature");
	feature_updated();
}

void Contents::updateColorBox(QPushButton *target, QColor color) {
	target->setStyleSheet("");
	QPalette mPalette = target->palette();
	mPalette.setColor(target->backgroundRole(), color);
	target->setPalette(mPalette);
	target->setStyleSheet(QString("background-color: %1;foreground-color: %1; border-style: none;").arg(color.name()));
	target->setAutoFillBackground(true);
	target->setFlat(true);
	target->repaint();
}


void Contents::handleSubregionAddBtn() {
	mGlobals.mDialogManager->mDialogAddSubregion->exec();
}
void Contents::handleFeatureAddBtn() {
	mGlobals.mDialogManager->mDialogAddFeature->exec();
}
void Contents::feature_updated() {
	qDebug() << "feature_updated";
	featureList->clear();
	featureList->setContentsMargins(1, 1, 1, 1);
	int index = 0;
	std::vector<DataFeature>::iterator iter;
	for (iter = mGlobals.CurrentProject->mFeature.begin(); iter != mGlobals.CurrentProject->mFeature.end(); ++iter) {
		
		
		QWidget *widget = new QWidget(mWidget);
		QHBoxLayout *list_item_layout= new QHBoxLayout;
		QFont font;
		font.setPointSize(10);
		font.setBold(true);

		QLabel *name= new QLabel(mWidget);
		name->setFont(font);
		name->setText(QString::fromStdString(iter->FeatureName));
		
		QPushButton *detailBtn = new QPushButton(mWidget);
		detailBtn->setIcon(QIcon("Resources/icon_detail.png"));
		detailBtn->setObjectName(QString::fromStdString(std::to_string(index)));
		detailBtn->setIconSize(QSize(15, 15));
		detailBtn->setFixedSize(QSize(20, 20));
		connect(detailBtn, SIGNAL(clicked()), this, SLOT(handleDetailBtn_feature()));

		QPushButton *deleteBtn = new QPushButton(mWidget);
		deleteBtn->setIcon(QIcon("Resources/icon_trash.png"));
		deleteBtn->setObjectName(QString::fromStdString(std::to_string(index)));
		deleteBtn->setIconSize(QSize(15, 15));
		deleteBtn->setFixedSize(QSize(20, 20));
		connect(deleteBtn, SIGNAL(clicked()), this, SLOT(handleDeleteBtn_feature()));
		

		QWidget *filler2 = new QWidget;
		filler2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		list_item_layout->addWidget(name);
		list_item_layout->addWidget(filler2);
		list_item_layout->addWidget(detailBtn);
		list_item_layout->addWidget(deleteBtn);
		widget->setLayout(list_item_layout);
		QListWidgetItem *item = new QListWidgetItem();
		
		item->setSizeHint(widget->sizeHint());

		featureList->addItem(item);
		featureList->setItemWidget(item, widget);

		index++;
	}


}
void Contents::subregion_updated() {

	
	subregionList->clear();
	subregionList->setContentsMargins(1, 1, 1, 1);
	int index = 0;
	std::vector<LayerSubregion>::iterator iter;
	for (iter = mGlobals.CurrentProject->mSubregion.begin(); iter != mGlobals.CurrentProject->mSubregion.end(); ++iter) {
		
		QWidget *widget = new QWidget(mWidget);
		QHBoxLayout *list_item_layout = new QHBoxLayout;
		QFont font;
		font.setPointSize(10);
		font.setBold(true);
		
		iter->SubregionIndex = index;

		QLabel *name = new QLabel(mWidget);
		name->setFont(font);
		name->setText(QString::fromStdString(iter->SubregionName));

		//QPushButton *infoBtn = new QPushButton(mWidget);
		//infoBtn->setIcon(QIcon("Resources/icon_info.png"));
		//infoBtn->setObjectName(QString::fromStdString(std::to_string(index)));
		//infoBtn->setIconSize(QSize(15, 15));
		//infoBtn->setFixedSize(QSize(20, 20));
		//connect(infoBtn, SIGNAL(clicked()), this, SLOT(handleInfoBtn_subregion()));


		QPushButton *colorbtn = new QPushButton;
		colorbtn->setObjectName(QString::fromStdString(std::to_string(index)));
		colorbtn->setFixedSize(QSize(15, 15));
		colorbtn->setFlat(true);
		colorbtn->setAutoFillBackground(true);
		updateColorBox(colorbtn, iter->SubregionColor);
		connect(colorbtn, SIGNAL(released()), this, SLOT(handleColorBtn_subregion()));
		
		QPushButton *deleteBtn = new QPushButton(mWidget);
		deleteBtn->setIcon(QIcon("Resources/icon_trash.png"));
		deleteBtn->setObjectName(QString::fromStdString(std::to_string(index)));
		deleteBtn->setIconSize(QSize(15, 15));
		deleteBtn->setFixedSize(QSize(20, 20));
		connect(deleteBtn, SIGNAL(clicked()), this, SLOT(handleDeleteBtn_subregion()));
		
		QSlider *slider = new QSlider(Qt::Horizontal);
		slider->setObjectName(QString::fromStdString(std::to_string(index)));
		slider->setFixedWidth(100);
		slider->setFocusPolicy(Qt::StrongFocus);
		slider->setTickPosition(QSlider::TicksBothSides);
		slider->setTickInterval(10);
		slider->setSingleStep(1);
		slider->setValue(iter->SubregionOpacity*100);
		connect(slider, SIGNAL(valueChanged(int)), this, SLOT(handleOpacity(int)));


		QWidget *filler2 = new QWidget;
		filler2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		list_item_layout->addWidget(name);
		list_item_layout->addWidget(filler2);
		//list_item_layout->addWidget(infoBtn);
		list_item_layout->addWidget(slider);
		list_item_layout->addWidget(colorbtn);
		//list_item_layout->addWidget(CheckBox);
		list_item_layout->addWidget(deleteBtn);
		widget->setLayout(list_item_layout);
		QListWidgetItem *item = new QListWidgetItem();

		item->setSizeHint(widget->sizeHint());

		subregionList->addItem(item);
		subregionList->setItemWidget(item, widget);

		index++;
	}
	updateAllWidget(0, false);
}

void Contents::handleDetailBtn_feature() {
	QObject *senderObj = sender(); // This will give Sender object
	QString senderObjName = senderObj->objectName();
	qDebug() << "detail Button: " << senderObjName;
	int index = std::stoi(senderObjName.toStdString());
	mGlobals.mDialogManager->mDialogDetailFeature->setIndexAndDraw(index);
	mGlobals.mDialogManager->mDialogDetailFeature->exec();
}

void Contents::handleOpacity(int value) {
	QObject *senderObj = sender(); // This will give Sender object
	QString senderObjName = senderObj->objectName();
	int index = std::stoi(senderObjName.toStdString());
	mGlobals.CurrentProject->mSubregion[index].SubregionOpacity = (float)value / 100.0f;
	qDebug() << index << " " << mGlobals.CurrentProject->mSubregion[index].SubregionOpacity;
	updateAllWidget(0, false);
}


void Contents::handleDeleteBtn_feature() {
	QObject *senderObj = sender(); // This will give Sender object
	QString senderObjName = senderObj->objectName();
	int index = std::stoi(senderObjName.toStdString());
	qDebug() << "delete Button feature: " << senderObjName;

	mGlobals.CurrentProject->removeFeature(index);
	featureList->removeItemWidget(featureList->takeItem(index));
}

void Contents::handleDeleteBtn_subregion() {
	QObject *senderObj = sender(); // This will give Sender object
	QString senderObjName = senderObj->objectName();
	int index = std::stoi(senderObjName.toStdString());
	qDebug() << "delete Button: subregion" << senderObjName;

	mGlobals.CurrentProject->removeSubregion(index);
	subregion_updated();
}
void Contents::handleInfoBtn_subregion() {
	QObject *senderObj = sender(); // This will give Sender object
	QString senderObjName = senderObj->objectName();
	int index = std::stoi(senderObjName.toStdString());
	qDebug() << "info Button: subregion" << senderObjName;
	mGlobals.mDialogManager->mDialogInfoSubregion->setIndexAndDraw(index);
	mGlobals.mDialogManager->mDialogInfoSubregion->exec();
}


void Contents::handleColorBtn_subregion() {
	QObject *senderObj = sender(); // This will give Sender object
	QString senderObjName = senderObj->objectName();
	int index = std::stoi(senderObjName.toStdString());
	QColorDialogTester color_test;
	QString color_str = color_test.onColor();
	qDebug() << color_str;

	if (color_str.length() > 2) {
		mGlobals.CurrentProject->mSubregion[index].SubregionColor = QColor(color_str);
		subregion_updated();
	}

}