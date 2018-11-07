#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ZebrafishAnalytics.h"

class ZebrafishAnalytics : public QMainWindow
{
	Q_OBJECT

public:
	ZebrafishAnalytics(QWidget *parent = Q_NULLPTR);

private:
	Ui::ZebrafishAnalyticsClass ui;
};
