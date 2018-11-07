#include "TopInterface.h"

TopInterface::TopInterface(QWidget *parent)
	: QObject(parent)
{
	mWidget = parent;

}

void TopInterface::initialize(QLayout *parent_layout) {
	QLabel *label1 = new QLabel(mWidget);
	label1->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	label1->setAlignment(Qt::AlignCenter);
	label1->setFixedSize(40,30);
	label1->setText("X");
	//label1->setAlignment(Qt::AlignBottom | Qt::AlignRight);
	x_value_edit = new QLineEdit(mWidget);
	x_value_edit->setFixedSize(70, 30);
	x_value_edit->setFocus();

	QLabel *label2 = new QLabel(mWidget);
	label2->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	label2->setAlignment(Qt::AlignCenter);
	label2->setFixedSize(40, 30);
	label2->setText("Y");
	//label2->setAlignment(Qt::AlignBottom | Qt::AlignRight);
	y_value_edit = new QLineEdit(mWidget);
	y_value_edit->setFixedSize(70, 30);
	y_value_edit->setFocus();

	QLabel *label3 = new QLabel(mWidget);
	label3->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	label3->setAlignment(Qt::AlignCenter);
	label3->setFixedSize(40, 30);
	label3->setText("Z");
	//label3->setAlignment(Qt::AlignBottom | Qt::AlignRight);
	z_value_edit = new QLineEdit(mWidget);
	z_value_edit->setFixedSize(70, 30);
	z_value_edit->setFocus();

	QLabel *label4 = new QLabel(mWidget);
	label4->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	label4->setAlignment(Qt::AlignCenter);
	label4->setFixedSize(100, 30);
	label4->setText("Zoom-level");
	//label4->setAlignment(Qt::AlignBottom | Qt::AlignRight);
	level_value_edit = new QLineEdit(mWidget);
	level_value_edit->setFixedSize(70, 30);
	level_value_edit->setFocus();

	interface_set_btn = new QPushButton("Set", mWidget);
	interface_set_btn->setFixedSize(QSize(40, 30));
	connect(interface_set_btn, SIGNAL(released()), this, SLOT(handleButton()));
	
	QFrame *buffer = new QFrame(mWidget);
	buffer->setFrameStyle(QFrame::VLine | QFrame::Plain);
	buffer->setStyleSheet("color:rgb(72,72,72)");
	buffer->setLineWidth(1.5);
	buffer->setFixedWidth(30);

	QLabel *label5 = new QLabel(mWidget);
	label5->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	label5->setAlignment(Qt::AlignCenter);
	label5->setFixedSize(150, 30);
	label5->setText("Cell Visualize Method");
	rendering_menu = new QComboBox(mWidget);
	QStringList commands = { "Random color", "Morpological feature histogram threshold", "Selected cell"};
	rendering_menu->addItems(commands);
	rendering_menu->setFixedWidth(250);
	connect(rendering_menu, SIGNAL(currentIndexChanged(int)), this, SLOT(handleComboMenu(int)));
	//connect(interface_set_btn, SIGNAL(released()), this, SLOT(handleButton()));



	parent_layout->addWidget(label1);
	parent_layout->addWidget(x_value_edit);
	parent_layout->addWidget(label2);
	parent_layout->addWidget(y_value_edit);
	parent_layout->addWidget(label3);
	parent_layout->addWidget(z_value_edit);
	parent_layout->addWidget(label4);
	parent_layout->addWidget(level_value_edit);
	parent_layout->addWidget(interface_set_btn);
	parent_layout->addWidget(buffer);
	parent_layout->addWidget(label5);
	parent_layout->addWidget(rendering_menu);


}
void TopInterface::handleButton() {
	if (abs(mGlobals.CurrentProject->ViewZoomLevel - 1.0/level_value_edit->text().toFloat()) < 0.001) {
		mGlobals.CurrentProject->ViewPos_X = x_value_edit->text().toFloat();
		mGlobals.CurrentProject->ViewPos_Y = y_value_edit->text().toFloat();
		mGlobals.CurrentProject->ViewPos_Z = z_value_edit->text().toFloat();
		emit update_view(false);
	}
	else {
		mGlobals.CurrentProject->ViewZoomLevel = 1.0/level_value_edit->text().toFloat();
		emit update_view(true);
	}
}
void TopInterface::handleComboMenu(int index) {
	mGlobals.CurrentProject->VisualizeMethod_Index = index;
}

void TopInterface::update_value(int x, int y, int z, float level) {
	//qDebug("update value");
	x_value_edit->setText(QString::fromStdString(std::to_string(x)));
	y_value_edit->setText(QString::fromStdString(std::to_string(y)));
	z_value_edit->setText(QString::fromStdString(std::to_string(z)));
	level_value_edit->setText(QString::fromStdString(std::to_string(1.0 / level)));
}

TopInterface::~TopInterface()
{

}
