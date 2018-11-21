#include "BlockWidget.h"

BlockWidget::BlockWidget(QObject *parent)
	: QObject(parent)
{
}

BlockWidget::~BlockWidget()
{
}

void BlockWidget::initialize(int aflags, int awidth, int aheight, QColor acolor, QNEBlock *parent) {
	BackgroundColor = acolor;
	
	generateToolBox(aflags);
	m_block = parent;
}

void BlockWidget::generateToolBox(int aflag) {
	
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

	CloseBtn = new QPushButton;
	CloseBtn->setIcon(QIcon("Resources/icon_blockclose.png"));
	CloseBtn->setIconSize(QSize(15, 15));
	CloseBtn->setFixedSize(QSize(18, 18));
	connect(CloseBtn, SIGNAL(released()), this, SLOT(handleCloseBtn()));

	layout->addWidget(AnnotationBtn);
	layout->addWidget(CloseBtn);

	ToolBox->setLayout(layout);
}
void BlockWidget::generateCenterBox(int width, int height) {


}


void BlockWidget::handleBlockColorBtn() {
	QColorDialogTester color_test;
	QString color_str = color_test.onColor();
	qDebug() << color_str;
	if (color_str.length() > 2) {
		DataColor = QColor(color_str);
		updateColorBox(BucketColorBtn, QColor(color_str));
	}
}

void BlockWidget::handleCloseBtn() {
	qDebug() << "CloseBtn";
	//QGraphicsItem *item = itemAt(me->scenePos());
	emit deleteNode(m_block);
}

void BlockWidget::handleAnnotationBtn() {
	
	qDebug() << "AnnotationBtn";
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

