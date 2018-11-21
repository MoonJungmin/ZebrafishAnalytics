#pragma once

#include <QObject>
#include <QPushButton>
#include <QtWidgets>
#include <QLabel>
#include <QComboBox>



#include "Source/global.h"
class TopInterface : public QObject
{
	Q_OBJECT

public:
	TopInterface(QWidget *parent);
	TopInterface(QWidget *parent, QLayout *parent_layout);

	~TopInterface();

public slots:
	void update_value(int x, int y, int z, float level);
	
signals:
	void update_view(bool);


private:
	QLineEdit * x_value_edit;
	QLineEdit * y_value_edit;
	QLineEdit * z_value_edit;
	QLineEdit * level_value_edit;
	QComboBox * rendering_menu;

	QPushButton * interface_set_btn;

	QWidget * mWidget;

private slots:
	void handleComboMenu(int index);
	void handleButton();

};
