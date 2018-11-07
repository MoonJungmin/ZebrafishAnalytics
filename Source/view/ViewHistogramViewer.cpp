#include "ViewHistogramViewer.h"

ViewHistogramViewer::ViewHistogramViewer(QChart *chart, QWidget *parent) :
	QChartView(chart, parent),
	isPressed(false)
{
 //	setRubberBand(QChartView::RectangleRubberBand);
}

void ViewHistogramViewer::mousePressEvent(QMouseEvent *event)
{	
	if (isPressed)
		return;
	qDebug() << "Mouse down";
	isPressed = true;
	XStartPos = (float)event->x() / (float)chart()->geometry().width();
	emit clear_mouse();

	QChartView::mousePressEvent(event);
}

void ViewHistogramViewer::mouseMoveEvent(QMouseEvent *event)
{
	if (isPressed == false)
		return;
	
	qDebug() << "Mouse move";
	XEndPos = (float)event->x() / (float)chart()->geometry().width();
	qDebug() << XStartPos << " " << XEndPos;
	if (XStartPos > XEndPos) {
		emit update_move_pos(XEndPos, XStartPos);
	}
	else {
		emit update_move_pos(XStartPos, XEndPos);
	}
	
	QChartView::mouseMoveEvent(event);
}

void ViewHistogramViewer::mouseReleaseEvent(QMouseEvent *event)
{
	if (isPressed)
		isPressed = false;
	qDebug() << "Mouse release";
	qDebug() << XStartPos << " " << XEndPos;

	XEndPos = (float)event->x() / (float)chart()->geometry().width();
	if (XStartPos > XEndPos) {
		emit update_release_pos(XEndPos, XStartPos);
	}
	else {
		emit update_release_pos(XStartPos, XEndPos);
	}
	QChartView::mouseReleaseEvent(event);
}

void ViewHistogramViewer::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key_Space:
			XStartPos = 0;
			XEndPos = 1.0;
			emit update_release_pos(XStartPos, XEndPos);
			break;
		case Qt::Key_Escape:
			emit clear_mouse();
			break;

	/*case Qt::Key_Plus:
		chart()->zoomIn();
		break;
	case Qt::Key_Minus:
		chart()->zoomOut();
		break;
	case Qt::Key_Left:
		chart()->scroll(-10, 0);
		break;
	case Qt::Key_Right:
		chart()->scroll(10, 0);
		break;
	case Qt::Key_Up:
		chart()->scroll(0, 10);
		break;
	case Qt::Key_Down:
		chart()->scroll(0, -10);
		break;*/
		default:
			QGraphicsView::keyPressEvent(event);
			break;
	}
}