#include "Contents.h"

Contents::Contents(QWidget *parent)
	: QObject(parent)
{
	mWidget = parent;
}

Contents::~Contents()
{
}


void Contents::initialize(QLayout *parent_layout, TopInterface *aTopInterface) {
	mInterface = aTopInterface;
	connect(mGlobals.CurrentProject, SIGNAL(project_on()), this, SLOT(handleProjectOn()));
	connect(mGlobals.CurrentProject, SIGNAL(analytics_on()), this, SLOT(handleAnalyticsOn()));

	QVBoxLayout *contents_left_layout = new QVBoxLayout;
	contents_left_layout->setAlignment(Qt::AlignCenter);
	contents_left_layout->setMargin(1);
	QWidget *contents_left_widget = new QWidget;
	contents_left_tabwidget = new QTabWidget(contents_left_widget);
	contents_left_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	contents_left_widget->setFixedSize(300, 870);
	contents_left_widget->setStyleSheet("background-color:(35,35,35);");
	contents_left_layout->addWidget(contents_left_widget);

	QVBoxLayout *contents_center_layout = new QVBoxLayout;
	contents_center_layout->setAlignment(Qt::AlignCenter);
	contents_center_layout->setMargin(1);
	QWidget *contents_center_widget = new QWidget;
	contents_center_tabwidget = new QTabWidget(contents_center_widget);
	contents_center_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	contents_center_widget->setFixedSize(1300, 870 * 0.60);

	contents_center_widget->setStyleSheet("background-color:(35,35,35);");
	contents_center_layout->addWidget(contents_center_widget);


	QGraphicsScene* scene = new QGraphicsScene();
	QGraphicsView *view = new QGraphicsView();
	view->setScene(scene);
	view->setRenderHint(QPainter::Antialiasing, true);

	QNodesEditor *nodesEditor = new QNodesEditor(this);
	nodesEditor->install(scene);

	QNEBlock *b = new QNEBlock(0);
	scene->addItem(b);
	b->addPort("Name", 0, QNEPort::NamePort);
	b->addPort("Type", 0, QNEPort::TypePort);
	b->addInputPort("in1");

	b->addOutputPort("out1");

	b = b->clone();
	b->setPos(150, 0);
	b = b->clone();
	b->setPos(150, 150);

	contents_center_layout->addWidget(view);



	QVBoxLayout *contents_right_layout = new QVBoxLayout;
	contents_right_layout->setAlignment(Qt::AlignCenter);
	contents_right_layout->setMargin(1);
	QWidget *contents_right_widget = new QWidget;
	contents_right_tabwidget = new QTabWidget(contents_right_widget);
	contents_right_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	contents_right_widget->setFixedSize(300, 870);
	contents_right_widget->setStyleSheet("background-color:(35,35,35);");
	contents_right_layout->addWidget(contents_right_widget);


	LeftTabInit(contents_left_tabwidget, contents_left_widget->width(), contents_left_widget->height());
	CenterTabInit(contents_center_tabwidget, contents_center_widget->width(), contents_center_widget->height());
	
	//mGlobals.CurrentProject->setViewInfoXY(contents_center_widget->width(), contents_center_widget->height());
	RightTabInit(contents_right_tabwidget, contents_right_widget->width(), contents_right_widget->height());

	parent_layout->addItem(contents_left_layout);
	parent_layout->addItem(contents_center_layout);
	parent_layout->addItem(contents_right_layout);
}

void Contents::LeftTabInit(QTabWidget *target, int width, int height) {
	//target->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	target->setFixedSize(width, height);
	//target->setStyleSheet("background-color:red;");

	target->addTab(new QWidget(), "Project Information");
	target->addTab(new QWidget(), "TAB 2-l");
}
void Contents::CenterTabInit(QTabWidget *target, int width, int height) {
	//target->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	//target->setFixedWidth(300);
	target->setFixedSize(width, height);
	
	target->addTab(new QWidget(), "4-View");
	//QOpenGLWidget *glWidgets = new ViewAxisGLWidget;
	target->addTab(new QWidget(), "XY");
	target->addTab(new QWidget(), "YZ");
	target->addTab(new QWidget(), "ZX");
}

void Contents::RightTabInit(QTabWidget *target, int width, int height) {
	//target->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	target->setFixedSize(width, height);
	qDebug() << " Right size " << width << height;
	target->addTab(new QWidget(), "Morphology Feature");
	target->addTab(new QWidget(), "2D Scatter plot");
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

void Contents::handleProjectOn() {
	qDebug() << "Project On";
	CenterContentsOn();
}
void Contents::handleAnalyticsOn() {
	qDebug() << "Analytics On";
	RightContentsOn();
	contents_right_tabwidget->setCurrentIndex(0);
}

void Contents::RightContentsOn() {
	QVBoxLayout *HistListLayout = new QVBoxLayout;
	QString nameList[5] = { "Volume", "Surfacearea", "Sphericity", "Eccentricity", "Intensity" };

	for (int k = 0; k < 5; ++k) {
		
		MorphoHistBox[k] = new ViewHistogramBox(mWidget);

		switch (k) {
			case 0:
				MorphoHistBox[k]->Init(nameList[k], k, &mGlobals.CurrentProject->mAnalytics->volumeHist);
				break;
			case 1:
				MorphoHistBox[k]->Init(nameList[k], k, &mGlobals.CurrentProject->mAnalytics->surfaceareaHist);
				break;
			case 2:
				MorphoHistBox[k]->Init(nameList[k], k, &mGlobals.CurrentProject->mAnalytics->sphericityHist);
				break;
			case 3:
				MorphoHistBox[k]->Init(nameList[k], k, &mGlobals.CurrentProject->mAnalytics->eccentricityHist);
				break;
			case 4:
				MorphoHistBox[k]->Init(nameList[k], k, &mGlobals.CurrentProject->mAnalytics->intensityHist);
				break;
		}
		
		HistListLayout->addWidget(MorphoHistBox[k]->HistBoxFrame);
		connect(MorphoHistBox[k], SIGNAL(focusedHist(int)), this, SLOT(focusmanage_histogram(int)));

	}

/*
	QWidget *HistListWidget = new QWidget();
	HistListWidget->setLayout(HistListLayout);*/
	QScrollArea* scroller = new QScrollArea;
	scroller->horizontalScrollBar()->setEnabled(false);
	scroller->setLayout(HistListLayout);

	replaceTab(contents_right_tabwidget, 0, scroller, "Morphology Feature");

}


void Contents::CenterContentsOn() {
	GL_XYAxis_Main = new ViewAxisGLWidget;
	GL_XYAxis_Main->setAxisIndex(1, 0);
	connect(GL_XYAxis_Main, SIGNAL(update_view_state(int, int, int, float)), mInterface, SLOT(update_value(int, int, int, float)));
	connect(mInterface, SIGNAL(update_view(bool)), GL_XYAxis_Main, SLOT(updateByInterface(bool)));
	connect(GL_XYAxis_Main, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	//replaceTab(contents_center_tabwidget, 1, GL_XYAxis_Main, "XY");
	GL_YZAxis_Main = new ViewAxisGLWidget;
	GL_YZAxis_Main->setAxisIndex(2, 1);
	connect(GL_YZAxis_Main, SIGNAL(update_view_state(int, int, int, float)), mInterface, SLOT(update_value(int, int, int, float)));
	connect(mInterface, SIGNAL(update_view(bool)), GL_YZAxis_Main, SLOT(updateByInterface(bool)));
	connect(GL_YZAxis_Main, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	GL_ZXAxis_Main = new ViewAxisGLWidget;
	GL_ZXAxis_Main->setAxisIndex(3, 2);
	connect(GL_ZXAxis_Main, SIGNAL(update_view_state(int, int, int, float)), mInterface, SLOT(update_value(int, int, int, float)));
	connect(mInterface, SIGNAL(update_view(bool)), GL_ZXAxis_Main, SLOT(updateByInterface(bool)));
	connect(GL_ZXAxis_Main, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	replaceTab(contents_center_tabwidget, 1, GL_XYAxis_Main, "XY");
	replaceTab(contents_center_tabwidget, 2, GL_YZAxis_Main, "YZ");
	replaceTab(contents_center_tabwidget, 3, GL_ZXAxis_Main, "ZX");


	GL_XYAxis_Sub = new ViewAxisGLWidget;
	GL_XYAxis_Sub->setAxisIndex(1, 3);
	connect(GL_XYAxis_Sub, SIGNAL(update_view_state(int, int, int, float)), mInterface, SLOT(update_value(int, int, int, float)));
	connect(mInterface, SIGNAL(update_view(bool)), GL_XYAxis_Sub, SLOT(updateByInterface(bool)));
	connect(GL_XYAxis_Sub, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	GL_YZAxis_Sub = new ViewAxisGLWidget;
	GL_YZAxis_Sub->setAxisIndex(2, 4);
	connect(GL_YZAxis_Sub, SIGNAL(update_view_state(int, int, int, float)), mInterface, SLOT(update_value(int, int, int, float)));
	connect(mInterface, SIGNAL(update_view(bool)), GL_YZAxis_Sub, SLOT(updateByInterface(bool)));
	connect(GL_YZAxis_Sub, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	GL_ZXAxis_Sub = new ViewAxisGLWidget;
	GL_ZXAxis_Sub->setAxisIndex(3, 5);
	connect(GL_ZXAxis_Sub, SIGNAL(update_view_state(int, int, int, float)), mInterface, SLOT(update_value(int, int, int, float)));
	connect(mInterface, SIGNAL(update_view(bool)), GL_ZXAxis_Sub, SLOT(updateByInterface(bool)));
	connect(GL_ZXAxis_Sub, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	QVBoxLayout *view4_layout = new QVBoxLayout;
	QHBoxLayout *first_lay = new QHBoxLayout;
	QHBoxLayout *second_lay = new QHBoxLayout;
	first_lay->addWidget(GL_XYAxis_Sub);
	first_lay->addWidget(GL_YZAxis_Sub);
	view4_layout->addLayout(first_lay);
	second_lay->addWidget(GL_ZXAxis_Sub);
	second_lay->addWidget(new QWidget());
	view4_layout->addLayout(second_lay);
	QWidget *view4_Container = new QWidget;
	view4_Container->setLayout(view4_layout);
	replaceTab(contents_center_tabwidget, 0, view4_Container, "4-View");

}


void Contents::updateColorBox(QWidget *target, QColor color) {
	QPalette mPalette;
	mPalette.setColor(QPalette::Background, color);
	target->setPalette(mPalette);
	target->repaint();

}


void Contents::focusmanage_histogram(int index) {
	for (int i = 0; i < 5; ++i) {
		if (i == index) {
			MorphoHistBox[i]->FocusedIn();
		}
		else {
			MorphoHistBox[i]->FocusedOut();
		}
	}
}