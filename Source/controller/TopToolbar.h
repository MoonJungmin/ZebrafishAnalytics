#pragma once

#include <QObject>
#include <QPushButton>
#include <QtWidgets>
#include <QIcon>

#include "DialogNewProject.h"
#include "DialogAddNode.h"

class TopToolbar : public QObject
{
	Q_OBJECT

public:
	void initialize(QLayout *parent_layout);
	TopToolbar(QWidget *parent);
	~TopToolbar();

private slots:
	void handleNewProject();
	void handleSaveProject();
	void handleOpenProject();
	void handleAddNode();

private:
	QWidget * mWidget;
};
