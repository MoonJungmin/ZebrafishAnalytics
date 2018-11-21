#include "ViewHistogramBox.h"


ViewHistogramBox::ViewHistogramBox(QWidget *parent)
	: QWidget(parent)
{
	mWidget = parent;
}
void ViewHistogramBox::Init(QString name, int index, std::vector<std::vector<long long>> *data_ptr)
{
	mIndex = index;
	mName = name;
	mData = data_ptr;
	
	QFont name_font("Arial", 12, QFont::Bold);
	HistBoxLayout = new QVBoxLayout;
	HistInfoLayout = new QHBoxLayout;
	HistName = new QLabel(mWidget);
	HistName->setText(mName);
	HistName->setFont(name_font);
	HistName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	HistColorLayout = new QHBoxLayout;

	HistOutColorBtn = new QPushButton(mWidget);
	HistOutColorBtn->setFixedSize(QSize(20, 20));
	HistOutColorBtn->setFlat(true);
	HistOutColorBtn->setAutoFillBackground(true);
	HistOutColor = QColor(255, 123, 0);
	updateColorBox(HistOutColorBtn, HistOutColor);
	connect(HistOutColorBtn, SIGNAL(clicked()), this, SLOT(handleOutColor()));

	HistStartColorBtn = new QPushButton(mWidget);
	HistStartColorBtn->setFixedSize(QSize(20, 20));
	HistStartColorBtn->setFlat(true);
	HistStartColorBtn->setAutoFillBackground(true);
	HistStartColor = QColor(255, 255, 123);
	updateColorBox(HistStartColorBtn, HistStartColor);
	connect(HistStartColorBtn, SIGNAL(clicked()), this, SLOT(handleStartColor()));

	HistEndColorBtn = new QPushButton(mWidget);
	HistEndColorBtn->setFixedSize(QSize(20, 20));
	HistEndColorBtn->setFlat(true);
	HistEndColorBtn->setAutoFillBackground(true);
	HistEndColor = QColor(123, 255, 0);
	updateColorBox(HistEndColorBtn, HistEndColor);
	connect(HistEndColorBtn, SIGNAL(clicked()), this, SLOT(handleEndColor()));


	HistColorLayout->addWidget(HistOutColorBtn);
	HistColorLayout->addWidget(HistStartColorBtn);
	HistColorLayout->addWidget(HistEndColorBtn);

	HistInfoLayout->addWidget(HistName);

	HistInfoLayout->addLayout(HistColorLayout);
	QBarSeries *series = new QBarSeries();
	
		
	for (int i = 0; i < 300; i++) {
		QBarSet *set = new QBarSet(mName);
		*set << log(mData->at(i).size() + 1);	
		set->setPen(Qt::NoPen);
		set->setColor(Qt::white);
		series->append(set);
		mBarsets.append(set);
	}


	series->setBarWidth(1);
	series->setUseOpenGL(true);

	ViewHistogram *chart = new ViewHistogram();
	chart->addSeries(series);

	chart->setMargins(QMargins(0, 0, 0, 0));
	chart->layout()->setContentsMargins(0, 0, 0, 0);
	chart->setBackgroundRoundness(0);
	chart->setBackgroundBrush(QBrush(QColor("#595959")));
	
	chart->setAnimationOptions(QChart::SeriesAnimations);
	
	chart->legend()->hide();
	//chart->createDefaultAxes(); 
	ViewHistogramViewer *chartView = new ViewHistogramViewer(chart);
	connect(chartView, SIGNAL(update_move_pos(float, float)), this, SLOT(update_move_box(float, float)));
	connect(chartView, SIGNAL(update_release_pos(float, float)), this, SLOT(update_release_box(float, float)));
	connect(chartView, SIGNAL(clear_mouse()), this, SLOT(clear_box()));

	chartView->setRenderHint(QPainter::Antialiasing);
	chartView->setFixedHeight(100);
	chartView->setFixedWidth(300);
	

	HistBoxLayout->addLayout(HistInfoLayout);
	HistBoxLayout->addWidget(chartView);
	
	HistBoxFrame = new QFrame(mWidget);
	HistBoxFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
	HistBoxFrame->setObjectName("HistBox_frame");
	HistBoxFrame->setStyleSheet("#HistBox_frame { color : gray; }");
	HistBoxFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	HistBoxFrame->setLayout(HistBoxLayout);


}


ViewHistogramBox::~ViewHistogramBox()
{
}


void ViewHistogramBox::focusInEvent(QFocusEvent *event) {
	emit focusedHist(mIndex);
	qDebug() << "Focused In";
	
	QWidget::focusInEvent(event);
}

void ViewHistogramBox::handleOutColor() {
	QColorDialogTester color_test;
	QString color_str = color_test.onColor();
	qDebug() << color_str;
	if (color_str.length() > 2) {
		HistOutColor = QColor(color_str);
		updateColorBox(HistOutColorBtn, QColor(color_str));
	}
	emit focusedHist(mIndex);
}
void ViewHistogramBox::handleStartColor() {
	QColorDialogTester color_test;
	QString color_str = color_test.onColor();
	qDebug() << color_str;
	if (color_str.length() > 2) {
		HistStartColor = QColor(color_str);
		updateColorBox(HistStartColorBtn, QColor(color_str));
	}
	emit focusedHist(mIndex);
}
void ViewHistogramBox::handleEndColor() {
	QColorDialogTester color_test;
	QString color_str = color_test.onColor();
	qDebug() << color_str;
	if (color_str.length() > 2) {
		HistEndColor = QColor(color_str);
		updateColorBox(HistEndColorBtn, QColor(color_str));
	}
	emit focusedHist(mIndex);
}

void ViewHistogramBox::clear_box() {
	foreach(QBarSet *item, mBarsets) {
		item->setColor(QColor("white"));
	}
	emit focusedHist(mIndex);
}

void ViewHistogramBox::update_release_box(float start, float end) {
	qDebug() << "update_release_box";
	if (start < 0) start = 0;
	if (end > 1) end = 1.0;

	int start_idx = mBarsets.length() * start;
	int end_idx = mBarsets.length() * end;

	HistStartIdx = start_idx;
	HistEndIdx = end_idx;

	Utils mUtil;
	for (int i = 0; i < start_idx; ++i) {
		mBarsets.at(i)->setColor(HistOutColor);
	}

	for (int i = start_idx; i < end_idx; ++i) {
		mBarsets.at(i)->setColor(mUtil.interpolate_color(HistStartColor, HistEndColor, (double)(i-start_idx) / (double)(end_idx - start_idx)));
	}

	for (int i = end_idx; i < mBarsets.length(); ++i) {
		mBarsets.at(i)->setColor(HistOutColor);
	}

	updateCellColor();
	
	emit focusedHist(mIndex);
}

void ViewHistogramBox::updateCellColor() {
	std::vector<std::vector<long long>> *Hist_ptr;

	//switch (mIndex) {
	//	case 0:
	//		Hist_ptr = &mGlobals.CurrentProject->mLayerCell->volumeHist;
	//		break;
	//	case 1:
	//		Hist_ptr = &mGlobals.CurrentProject->mLayerCell->surfaceareaHist;
	//		break;
	//	case 2:
	//		Hist_ptr = &mGlobals.CurrentProject->mLayerCell->sphericityHist;
	//		break;
	//	case 3:
	//		Hist_ptr = &mGlobals.CurrentProject->mLayerCell->eccentricityHist;
	//		break;
	//	case 4:
	//		Hist_ptr = &mGlobals.CurrentProject->mLayerCell->intensityHist;
	//		break;
	//}
	
	for (int i = 0; i < 300; i++) {
		for (int j = 0; j < Hist_ptr->at(i).size(); ++j) {
			int cell_index = Hist_ptr->at(i)[j];
			mGlobals.CurrentProject->mLayerCell->mCellList.at(cell_index).color = mBarsets.at(i)->color();
		}
	}

	mGlobals.CurrentProject->mLayerCell->CellColorGPU_On = false;
}


void ViewHistogramBox::update_move_box(float start, float end) {
	qDebug() << "update_move_box";
	if (start < 0) start = 0;
	if (end > 1) end = 1.0;

	int start_idx = mBarsets.length() * start;
	int end_idx = mBarsets.length() * end;
	
	Utils mUtil;
	for (int i = start_idx; i < end_idx; ++i) {
		mBarsets.at(i)->setColor(mUtil.interpolate_color(HistStartColor, HistEndColor, (double)(i - start_idx) / (double)(end_idx - start_idx)));
	}

	emit focusedHist(mIndex);
}

void ViewHistogramBox::updateColorBox(QPushButton *target, QColor color) {
	qDebug() << "updateColorBox" << " " << color.name();
	target->setStyleSheet("");
	QPalette mPalette = target->palette();
	mPalette.setColor(target->backgroundRole(), color);
	target->setPalette(mPalette);
	target->setStyleSheet(QString("background-color: %1;foreground-color: %1; border-style: none;").arg(color.name()));
	target->setAutoFillBackground(true);
	target->setFlat(true);
	target->repaint();

	emit focusedHist(mIndex);
}


void ViewHistogramBox::FocusedIn() {
	
	HistBoxFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
	HistBoxFrame->setObjectName("HistBox_frame");
	HistBoxFrame->setStyleSheet("#HistBox_frame { color : red; }");
	HistBoxFrame->update();
	
}

void ViewHistogramBox::FocusedOut() {
	
	HistBoxFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
	HistBoxFrame->setObjectName("HistBox_frame");
	HistBoxFrame->setStyleSheet("#HistBox_frame { color : gray; }");
	HistBoxFrame->update();
	
}

