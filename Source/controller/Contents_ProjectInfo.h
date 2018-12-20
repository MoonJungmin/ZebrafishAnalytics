#pragma once

#include <QObject>
#include <QPushButton>
#include <QtWidgets>
#include <QIcon>

#include "Dialog/DialogManager.h"
class DialogManager;

class Contents_ProjectInfo : public QObject
{
	Q_OBJECT

public:
	Contents_ProjectInfo(QWidget *parent);
	Contents_ProjectInfo(QWidget *parent, QLayout *parent_layout);
	~Contents_ProjectInfo();
	
	QLineEdit *ProjectName;
	QLineEdit *ProjectPath;
	QLineEdit *BackPath;
	QLineEdit *CellPath;

	QLineEdit *DataDimX;
	QLineEdit *DataDimY;
	QLineEdit *DataDimZ;

	QLineEdit *ResolutionX;
	QLineEdit *ResolutionY;
	QLineEdit *ResolutionZ;

	QComboBox *ResolutionX_Unit;
	QComboBox *ResolutionY_Unit;
	QComboBox *ResolutionZ_Unit;
	
	

private slots:
	void handleModifyBtn();
	


private:
	QWidget * mWidget;

	void project_location_layout(QVBoxLayout *layout);
	void project_data_dim_layout(QVBoxLayout *layout);
	void project_resolution_layout(QVBoxLayout *layout);


};
