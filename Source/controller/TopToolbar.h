#pragma once

#include <QObject>
#include <QPushButton>
#include <QtWidgets>
#include <QIcon>

#include "Dialog/DialogManager.h"
//#include "DialogNewProject.h"
//#include "DialogAddNode.h"
class DialogManager;

class TopToolbar : public QObject
{
	Q_OBJECT

public:
	TopToolbar(QWidget *parent);
	TopToolbar(QWidget *parent, QLayout *parent_layout);
	~TopToolbar();

	//void setDialogManager(DialogManager *ptr);

private slots:
	void handleNewProject();
	void handleSaveProject();
	void handleOpenProject();
	void handleAddNode();


private:
	QWidget * mWidget;
	//DialogManager *mDialogManager;
	
};
