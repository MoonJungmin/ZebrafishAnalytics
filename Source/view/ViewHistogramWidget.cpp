#include "ViewHistogramWidget.h"

ViewHistogramWidget::ViewHistogramWidget(QWidget *parent)
	: QWidget(parent)
{
	mWidget = parent;
	HistBoxLayout = new QVBoxLayout;
	HistBoxLayout->setMargin(0);
	mWidget->setLayout(HistBoxLayout);
}


ViewHistogramWidget::~ViewHistogramWidget()
{
}
void ViewHistogramWidget::setInterface(QLineEdit *s, QLineEdit *e) {
	start_edit = s;
	end_edit = e;
}
void ViewHistogramWidget::setRenderingSize(int width, int height) {
	mWidth = width;
	mHeight = height;
	HistogramData.resize(mWidth);
}
void ViewHistogramWidget::setData(int feature_index, std::list<unsigned int> *in_ptr, std::list<unsigned int> *out_ptr)
{
	mInput = in_ptr;
	mOutput = out_ptr;
	mDataMaximum = -999999999;
	mDataMinimum = 999999999;

	std::list<unsigned int>::iterator iter = mInput->begin();
	for (iter = mInput->begin(); iter != mInput->end(); ++iter) {
		if (mDataMaximum <= mGlobals.CurrentProject->mFeature[feature_index].FeatureMap[*iter]) {
			mDataMaximum = mGlobals.CurrentProject->mFeature[feature_index].FeatureMap[*iter];
		}
		if (mDataMinimum >= mGlobals.CurrentProject->mFeature[feature_index].FeatureMap[*iter]) {
			mDataMinimum = mGlobals.CurrentProject->mFeature[feature_index].FeatureMap[*iter];
		}
	}
	clearHistogramData();
	mDataStep = (mDataMaximum - mDataMinimum) / (mWidth-1);
	for (iter = mInput->begin(); iter != mInput->end(); ++iter) {
		double value = mGlobals.CurrentProject->mFeature[feature_index].FeatureMap[*iter] - mDataMinimum;
		int index = value / mDataStep;
		//qDebug() << index;
		HistogramData[index].push_back(*iter);
	}

	//qDebug() << "feature data setting finish";

	redraw();
}
void ViewHistogramWidget::clearHistogramData() {
	std::vector<std::list<unsigned int>>::iterator temp = HistogramData.begin();
	for (temp = HistogramData.begin(); temp != HistogramData.end(); ++temp) {
		temp->clear();
	}
}
void ViewHistogramWidget::redraw() {
	mBarsets.clear();
	QBarSeries *series = new QBarSeries();
	for (int i = 0; i < mWidth; i++) {
		QBarSet *set = new QBarSet("");
		*set << log(HistogramData[i].size() + 1);
		set->setPen(Qt::NoPen);
		set->setColor(Qt::white);
		series->append(set);
		mBarsets.append(set);
	}
	
	series->setBarWidth(1);
	series->setUseOpenGL(true);

	Histogram *chart = new Histogram();
	chart->addSeries(series);

	chart->setMargins(QMargins(0, 0, 0, 0));
	chart->layout()->setContentsMargins(0, 0, 0, 0);
	chart->setBackgroundRoundness(0);
	chart->setBackgroundBrush(QBrush(QColor("#363636")));

	chart->setAnimationOptions(QChart::SeriesAnimations);

	chart->legend()->hide();
	//chart->createDefaultAxes(); 
	HistogramViewer *chartView = new HistogramViewer(chart);
	connect(chartView, SIGNAL(update_move_pos(float, float)), this, SLOT(update_move_box(float, float)));
	connect(chartView, SIGNAL(update_release_pos(float, float)), this, SLOT(update_release_box(float, float)));
	connect(chartView, SIGNAL(clear_mouse()), this, SLOT(clear_box()));

	chartView->setRenderHint(QPainter::Antialiasing);
	chartView->setFixedHeight(mHeight);
	chartView->setFixedWidth(mWidth);

	QLayoutItem *item = HistBoxLayout->takeAt(0);
	delete item;

	HistBoxLayout->addWidget(chartView);
}


void ViewHistogramWidget::clear_box() {
	foreach(QBarSet *item, mBarsets) {
		item->setColor(QColor("white"));
	}
	//emit focusedHist(mIndex);
}

void ViewHistogramWidget::update_release_box(float start, float end) {
	qDebug() << "update_release_box";
	if (start < 0) start = 0;
	if (end > 1) end = 1.0;

	int start_idx = mBarsets.length() * start;
	int end_idx = mBarsets.length() * end;

	HistStartIdx = start_idx;
	HistEndIdx = end_idx;

	Utils mUtil;
	for (int i = 0; i < start_idx; ++i) {
		mBarsets.at(i)->setColor(QColor("white"));
	}
	mOutput->clear();
	for (int i = start_idx; i < end_idx; ++i) {
		mBarsets.at(i)->setColor(QColor("#3A79A0"));
		std::list<unsigned int>::iterator iter = HistogramData[i].begin();
		for (iter = HistogramData[i].begin(); iter != HistogramData[i].end(); ++iter) {
			mOutput->push_back(*iter);
		}
	}

	for (int i = end_idx; i < mBarsets.length(); ++i) {
		mBarsets.at(i)->setColor(QColor("white"));
	}

	//updateCellColor();
	start_edit->setText(QString::fromStdString(to_string((int)(start * 100))));
	end_edit->setText(QString::fromStdString(to_string((int)(end * 100))));


	emit OutputUpdated();

}

void ViewHistogramWidget::update_move_box(float start, float end) {
	qDebug() << "update_move_box";
	if (start < 0) start = 0;
	if (end > 1) end = 1.0;

	int start_idx = mBarsets.length() * start;
	int end_idx = mBarsets.length() * end;
	
	Utils mUtil;
	for (int i = start_idx; i < end_idx; ++i) {
		mBarsets.at(i)->setColor(QColor("#3A79A0"));
	}

}
//
