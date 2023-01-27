#include "mainwndgendesign.h"
#include "ui_mainwndgendesign.h"

#include "meshgenerator.h"

MainWndGenDesign::MainWndGenDesign(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWndGenDesign)
{
    ui->setupUi(this);
	ui->label->setText(QString("%1").arg(_mesh.volume()));
//    bool ok;
//    ok= _mesh.isIncluding(QVector3D(0.1, 0.1, 0.1));
//    ok = _mesh.isIncluding(QVector3D(0., 0., 0.));
//    ok = _mesh.isIncluding(QVector3D(1, 1, 1));
//    ok = !ok;
    _mesh = MeshGenerator::genere();
}

MainWndGenDesign::~MainWndGenDesign()
{
    delete ui;
    _mesh.toFile("test.stl");
}

