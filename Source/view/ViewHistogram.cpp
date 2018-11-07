#include "ViewHistogram.h"


ViewHistogram::ViewHistogram(QGraphicsItem *parent, Qt::WindowFlags wFlags)
	: QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
	// Seems that QGraphicsView (QChartView) does not grab gestures.
	// They can only be grabbed here in the QGraphicsWidget (QChart).
	//grabGesture(Qt::PanGesture);
	//grabGesture(Qt::PinchGesture);
}

ViewHistogram::~ViewHistogram()
{

}

bool ViewHistogram::sceneEvent(QEvent *event)
{
	qDebug() << "Event";
	return QChart::event(event);
}
