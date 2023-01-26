#include "mainwndgendesign.h"
#include "ui_mainwndgendesign.h"

MainWndGenDesign::MainWndGenDesign(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWndGenDesign)
{
    ui->setupUi(this);
}

MainWndGenDesign::~MainWndGenDesign()
{
    delete ui;
    _mesh.toFile("test.stl");
//    solid name

//    facet normal ni nj nk
//        outer loop
//            vertex v1x v1y v1z
//            vertex v2x v2y v2z
//            vertex v3x v3y v3z
//        endloop
//    endfacet

//    endsolid name
}

