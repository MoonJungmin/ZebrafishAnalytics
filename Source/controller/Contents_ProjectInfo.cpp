#include "Contents_ProjectInfo.h"



Contents_ProjectInfo::Contents_ProjectInfo(QWidget *parent)
	: QObject(parent)
{
	mWidget = parent;
	QVBoxLayout *layout = new QVBoxLayout;
	project_location_layout(layout);
	project_data_dim_layout(layout);
	project_resolution_layout(layout);


	QHBoxLayout *filler_layout = new QHBoxLayout;
	QWidget *filler = new QWidget;
	filler->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	filler_layout->addWidget(filler);

	layout->addLayout(filler_layout);
	mWidget->setLayout(layout);

}

Contents_ProjectInfo::~Contents_ProjectInfo()
{
}

Contents_ProjectInfo::Contents_ProjectInfo(QWidget *parent, QLayout *parent_layout)
	: QObject(parent)
{
	
}

void Contents_ProjectInfo::project_location_layout(QVBoxLayout *layout) {
	QHBoxLayout *proj_name_layout = new QHBoxLayout;
	QHBoxLayout *proj_path_layout = new QHBoxLayout;
	QHBoxLayout *proj_back_layout = new QHBoxLayout;
	QHBoxLayout *proj_cell_layout = new QHBoxLayout;
	
	QFont title_font("Arial", 10, QFont::Bold);
	QLabel *title_label = new QLabel(mWidget);
	title_label->setFont(title_font);
	title_label->setText("Project Location");
	layout->addWidget(title_label);


	QLabel *proj_name_label = new QLabel(mWidget);
	proj_name_label->setAlignment(Qt::AlignCenter);
	proj_name_label->setFixedSize(100, 30);
	proj_name_label->setText("Project Name");
	ProjectName = new QLineEdit(mWidget);
	ProjectName->setReadOnly(true);
	ProjectName->setText(mGlobals.CurrentProject->ProjectName);
	proj_name_layout->addWidget(proj_name_label);
	proj_name_layout->addWidget(ProjectName);

	
	QLabel *proj_path_label = new QLabel(mWidget);
	proj_path_label->setAlignment(Qt::AlignCenter);
	proj_path_label->setFixedSize(100, 30);
	proj_path_label->setText("Project Path");
	ProjectPath = new QLineEdit(mWidget);
	ProjectPath->setReadOnly(true);
	ProjectPath->setText(mGlobals.CurrentProject->ProjectPath);
	proj_path_layout->addWidget(proj_path_label);
	proj_path_layout->addWidget(ProjectPath);


	QLabel *proj_back_label = new QLabel(mWidget);
	proj_back_label->setAlignment(Qt::AlignCenter);
	proj_back_label->setFixedSize(100, 30);
	proj_back_label->setText("EM Path");
	BackPath = new QLineEdit(mWidget);
	BackPath->setReadOnly(true);
	BackPath->setText(QString::fromStdString(mGlobals.CurrentProject->mLayerBack->BackgroundPath));
	proj_back_layout->addWidget(proj_back_label);
	proj_back_layout->addWidget(BackPath);


	QLabel *proj_cell_label = new QLabel(mWidget);
	proj_cell_label->setAlignment(Qt::AlignCenter);
	proj_cell_label->setFixedSize(100, 30);
	proj_cell_label->setText("Cell Path");
	CellPath = new QLineEdit(mWidget);
	CellPath->setReadOnly(true);
	CellPath->setText(QString::fromStdString(mGlobals.CurrentProject->mLayerCell->CellPath));
	proj_cell_layout->addWidget(proj_cell_label);
	proj_cell_layout->addWidget(CellPath);


	QFrame *buffer = new QFrame(mWidget);
	buffer->setFrameStyle(QFrame::HLine | QFrame::Plain);
	buffer->setStyleSheet("color:rgb(100,100,100)");
	buffer->setLineWidth(2.5);
	

	layout->addLayout(proj_name_layout);
	layout->addLayout(proj_path_layout);
	layout->addLayout(proj_back_layout);
	layout->addLayout(proj_cell_layout);
	layout->addWidget(buffer);
	
}
void Contents_ProjectInfo::project_data_dim_layout(QVBoxLayout *layout) {
	QHBoxLayout *dim_x_layout = new QHBoxLayout;
	QHBoxLayout *dim_y_layout = new QHBoxLayout;
	QHBoxLayout *dim_z_layout = new QHBoxLayout;
		
	QFont title_font("Arial", 10, QFont::Bold);
	QLabel *title_label = new QLabel(mWidget);
	title_label->setFont(title_font);
	title_label->setText("Project Data Dimension");
	layout->addWidget(title_label);


	QLabel *dim_x_label = new QLabel(mWidget);
	dim_x_label->setAlignment(Qt::AlignCenter);
	dim_x_label->setFixedSize(100, 30);
	dim_x_label->setText("X");
	DataDimX = new QLineEdit(mWidget);
	DataDimX->setReadOnly(true);
	DataDimX->setAlignment(Qt::AlignCenter);
	DataDimX->setText(QString::fromStdString(std::to_string(mGlobals.CurrentProject->DataSizeX)));
	QLabel *px_x_label = new QLabel(mWidget);
	px_x_label->setAlignment(Qt::AlignCenter);
	px_x_label->setFixedSize(50, 30);
	px_x_label->setText("px");
	dim_x_layout->addWidget(dim_x_label);
	dim_x_layout->addWidget(DataDimX);
	dim_x_layout->addWidget(px_x_label);
	

	QLabel *dim_y_label = new QLabel(mWidget);
	dim_y_label->setAlignment(Qt::AlignCenter);
	dim_y_label->setFixedSize(100, 30);
	dim_y_label->setText("Y");
	DataDimY = new QLineEdit(mWidget);
	DataDimY->setReadOnly(true);
	DataDimY->setAlignment(Qt::AlignCenter);
	DataDimY->setText(QString::fromStdString(std::to_string(mGlobals.CurrentProject->DataSizeY)));
	QLabel *px_y_label = new QLabel(mWidget);
	px_y_label->setAlignment(Qt::AlignCenter);
	px_y_label->setFixedSize(50, 30);
	px_y_label->setText("px");
	dim_y_layout->addWidget(dim_y_label);
	dim_y_layout->addWidget(DataDimY);
	dim_y_layout->addWidget(px_y_label);


	QLabel *dim_z_label = new QLabel(mWidget);
	dim_z_label->setAlignment(Qt::AlignCenter);
	dim_z_label->setFixedSize(100, 30);
	dim_z_label->setText("Z");
	DataDimZ = new QLineEdit(mWidget);
	DataDimZ->setReadOnly(true);
	DataDimZ->setAlignment(Qt::AlignCenter);
	DataDimZ->setText(QString::fromStdString(std::to_string(mGlobals.CurrentProject->DataSizeZ)));
	QLabel *px_z_label = new QLabel(mWidget);
	px_z_label->setAlignment(Qt::AlignCenter);
	px_z_label->setFixedSize(50, 30);
	px_z_label->setText("px");
	dim_z_layout->addWidget(dim_z_label);
	dim_z_layout->addWidget(DataDimZ);
	dim_z_layout->addWidget(px_z_label);

	QFrame *buffer = new QFrame(mWidget);
	buffer->setFrameStyle(QFrame::HLine | QFrame::Plain);
	buffer->setStyleSheet("color:rgb(100,100,100)");
	buffer->setLineWidth(2.5);

	layout->addLayout(dim_x_layout);
	layout->addLayout(dim_y_layout);
	layout->addLayout(dim_z_layout);
	layout->addWidget(buffer);

}
void Contents_ProjectInfo::project_resolution_layout(QVBoxLayout *layout) {
	QHBoxLayout *res_x_layout = new QHBoxLayout;
	QHBoxLayout *res_y_layout = new QHBoxLayout;
	QHBoxLayout *res_z_layout = new QHBoxLayout;


	QFont title_font("Arial", 10, QFont::Bold);
	QLabel *title_label = new QLabel(mWidget);
	title_label->setFont(title_font);
	title_label->setText("Project Resolution");
	layout->addWidget(title_label);

	QLabel *res_x_label = new QLabel(mWidget);
	res_x_label->setAlignment(Qt::AlignCenter);
	res_x_label->setFixedSize(100, 30);
	res_x_label->setText("X");
	ResolutionX = new QLineEdit(mWidget);
	ResolutionX->setAlignment(Qt::AlignCenter);
	ResolutionX->setText(QString::fromStdString(std::to_string(mGlobals.CurrentProject->ResolutionX)));
	ResolutionX_Unit = new QComboBox;
	ResolutionX_Unit->setFixedWidth(50);
	ResolutionX_Unit->addItems(mGlobals.CurrentProject->UnitList);
	res_x_layout->addWidget(res_x_label);
	res_x_layout->addWidget(ResolutionX);
	res_x_layout->addWidget(ResolutionX_Unit);
	
	
	QLabel *res_y_label = new QLabel(mWidget);
	res_y_label->setAlignment(Qt::AlignCenter);
	res_y_label->setFixedSize(100, 30);
	res_y_label->setText("Y");
	ResolutionY = new QLineEdit(mWidget);
	ResolutionY->setAlignment(Qt::AlignCenter);
	ResolutionY->setText(QString::fromStdString(std::to_string(mGlobals.CurrentProject->ResolutionY)));
	ResolutionY_Unit = new QComboBox;
	ResolutionY_Unit->setFixedWidth(50);
	ResolutionY_Unit->addItems(mGlobals.CurrentProject->UnitList);
	res_y_layout->addWidget(res_y_label);
	res_y_layout->addWidget(ResolutionY);
	res_y_layout->addWidget(ResolutionY_Unit);

	QLabel *res_z_label = new QLabel(mWidget);
	res_z_label->setAlignment(Qt::AlignCenter);
	res_z_label->setFixedSize(100, 30);
	res_z_label->setText("Z");
	ResolutionZ = new QLineEdit(mWidget);
	ResolutionZ->setAlignment(Qt::AlignCenter);
	ResolutionZ->setText(QString::fromStdString(std::to_string(mGlobals.CurrentProject->ResolutionZ)));
	ResolutionZ_Unit = new QComboBox;
	ResolutionZ_Unit->setFixedWidth(50);
	ResolutionZ_Unit->addItems(mGlobals.CurrentProject->UnitList);
	res_z_layout->addWidget(res_z_label);
	res_z_layout->addWidget(ResolutionZ);
	res_z_layout->addWidget(ResolutionZ_Unit);


	QFrame *buffer = new QFrame(mWidget);
	buffer->setFrameStyle(QFrame::HLine | QFrame::Plain);
	buffer->setStyleSheet("color:rgb(100,100,100)");
	buffer->setLineWidth(2.5);

	QPushButton *modifybtn = new QPushButton;
	modifybtn->setText("Modify");
	connect(modifybtn, SIGNAL(released()), this, SLOT(handleModifyBtn()));


	layout->addLayout(res_x_layout);
	layout->addLayout(res_y_layout);
	layout->addLayout(res_z_layout);
	layout->addWidget(modifybtn);
		
	
	layout->addWidget(buffer);
}



void Contents_ProjectInfo::handleModifyBtn() {
	qDebug("handleModifyBtn");
	mGlobals.CurrentProject->ResolutionX = std::stod(ResolutionX->text().toStdString());
	mGlobals.CurrentProject->ResolutionY = std::stod(ResolutionY->text().toStdString());
	mGlobals.CurrentProject->ResolutionZ = std::stod(ResolutionZ->text().toStdString());

	mGlobals.CurrentProject->ResolutionUnitX = ResolutionX_Unit->currentIndex();
	mGlobals.CurrentProject->ResolutionUnitY = ResolutionX_Unit->currentIndex();
	mGlobals.CurrentProject->ResolutionUnitZ = ResolutionX_Unit->currentIndex();


}
