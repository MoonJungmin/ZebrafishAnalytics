#pragma once

#include <QObject>
#include <QDialog>
#include <QtWidgets>
#include "Source/global.h"
#include "ZebrafishAnalytics.h"

class DialogNewProject : public QDialog
{
	Q_OBJECT

public slots:
	int exec();
	void done(int val);
	void accept();
	void reject();
	void find_save_dir();
	void find_open_bgl();
	void find_open_lb();
	void find_open_analytics();

public:
	DialogNewProject(QWidget *parent);
	~DialogNewProject();

private:
	QWidget * pWidget;
	QLineEdit * proj_location_edit;
	QLineEdit *proj_name_edit;
	QLineEdit *background_location_edit;
	QLineEdit *label_location_edit;
	QLineEdit *analytics_location_edit;
};
