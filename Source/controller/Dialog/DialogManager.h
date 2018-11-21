#pragma once

#include "DialogAddNode.h"
#include "DialogGenBGLayer.h"
#include "DialogGenLBLayer.h"
#include "DialogGenSRLayer.h"
#include "DialogNewProject.h"
#include "DialogPreference.h"

#include "DialogAddFeature.h"
#include "DialogAddSubregion.h"
#include "DialogDetailFeature.h"

#include <QWidget>


class DialogDetailFeature;
class DialogManager
{
public:
	DialogManager();
	DialogManager(QWidget *parent);
	~DialogManager();

	QDialog * mDialogGenBGLayer;
	QDialog * mDialogAddNode;
	QDialog * mDialogGenLBLayer;
	QDialog * mDialogGenSRLayer;
	QDialog * mDialogNewProject;
	QDialog * mDialogPreference;
	QDialog * mDialogAddFeature;
	QDialog * mDialogAddSubregion;
	DialogDetailFeature * mDialogDetailFeature;

private:
	QWidget *mWidget;
	void initializeDialog();

};

