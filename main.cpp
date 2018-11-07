#include "ZebrafishAnalytics.h"
#include <QtWidgets/QApplication>
#include <QDesktopWidget>
#include "Source/view/theme/darkstyle.h"
#include "Source/view/theme/framelesswindow.h"
#include "Source/global.h"

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	// style our application with custom dark style
	a.setStyle(new DarkStyle);

	mGlobals.Application_Path = a.applicationDirPath();
	mGlobals.loadSysinfo();



	// create frameless window (and set windowState or title)
	FramelessWindow framelessWindow;
	framelessWindow.setWindowState(Qt::WindowMaximized);
	framelessWindow.setWindowTitle("Zebrafish Analytics");
	framelessWindow.setWindowIcon(a.style()->standardIcon(QStyle::SP_DesktopIcon));

	// create our mainwindow instance
	ZebrafishAnalytics *mainWindow = new ZebrafishAnalytics;
	// add the mainwindow to our custom frameless window
	framelessWindow.setContent(mainWindow);
	framelessWindow.show();

	return a.exec();
}
