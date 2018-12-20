#include "DialogCellInfo.h"



DialogCellInfo::DialogCellInfo(QWidget *parent) : QDialog(parent)
{
	pWidget = parent;
	setModal(true);
	setFocusPolicy(Qt::StrongFocus);
	setFocus();
	int nWidth = 1920 / 2;
	int nHeight = 1080 / 4;
	if (parent != NULL) {
		//setGeometry(parent->x() + parent->width() / 2 - nWidth / 2, parent->y() + parent->height() / 2 - nHeight / 2, nWidth, nHeight);
		setGeometry(0 + 1920 / 2 - nWidth / 2, 0 + 1080 / 2 - nHeight / 2, nWidth, nHeight);
	}
	else {
		resize(nWidth, nHeight);
	}
	main_layout = new QHBoxLayout;
	main_layout->setMargin(5);
	this->setLayout(main_layout);
}


DialogCellInfo::~DialogCellInfo()
{
}



int DialogCellInfo::exec()
{
	qDebug("BG");
	activateWindow();
	this->show();
	return 1;
}
void DialogCellInfo::done(int val)
{
	qDebug("done.");

}

void DialogCellInfo::accept()
{
	qDebug("Accept.");
	// here!!!!!
	this->hide();
}
void DialogCellInfo::reject()
{
	emit thread_kill();

	qDebug("Reject.");
	this->hide();
}

void DialogCellInfo::setIndexAndDraw(unsigned int index) {
	mIndex = index;
	int feature_size = mGlobals.CurrentProject->mFeature.size();
	
	QFont title_font("Arial", 15, QFont::Bold);
	QFont name_font("Arial", 11, QFont::Normal);
	QFont value_font("Arial", 10, QFont::Normal);

	QWidget *container_widget = new QWidget;
	QVBoxLayout *master_layout = new QVBoxLayout;
	
	QLabel *title_label = new QLabel;
	title_label->setFont(title_font);
	title_label->setText("Cell Information");
	
	QHBoxLayout *information_layout = new QHBoxLayout;
	
	for (int i = 0; i < feature_size + 1; ++i) {
		QVBoxLayout *data_layout = new QVBoxLayout;
		QLabel *name_label = new QLabel;
		QLabel *value_label = new QLabel;
		if (i == 0) {
			name_label->setFont(name_font);
			name_label->setText("Cell Index");
			value_label->setFont(value_font);
			value_label->setText(QString::fromStdString(std::to_string(mIndex)));
		}
		else {
			name_label->setFont(name_font);
			name_label->setText(QString::fromStdString(mGlobals.CurrentProject->mFeature[i - 1].FeatureName));
			value_label->setFont(value_font);
			value_label->setText(QString::fromStdString(std::to_string(mGlobals.CurrentProject->mFeature[i - 1].FeatureMap[mIndex])));
		}
		data_layout->addWidget(name_label);
		data_layout->addWidget(value_label);
		information_layout->addLayout(data_layout);
	}
	QWidget *filler = new QWidget;
	filler->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

	master_layout->addWidget(title_label);
	master_layout->addLayout(information_layout);
	master_layout->addWidget(filler);
	

	container_widget->setLayout(master_layout);
	QLayoutItem* item;
	while ((item = main_layout->takeAt(0)) != NULL)
	{
		delete item->widget();
		delete item;
	}
	main_layout->addWidget(container_widget);

}
