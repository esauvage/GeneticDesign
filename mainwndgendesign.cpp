#include "mainwndgendesign.h"
#include "ui_mainwndgendesign.h"

#include "meshgenerator.h"
#include "meshevolver.h"

MainWndGenDesign::MainWndGenDesign(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWndGenDesign)
{
    ui->setupUi(this);
//    bool ok;
//    ok= _mesh.isIncluding(QVector3D(0.1, 0.1, 0.1));
//    ok = _mesh.isIncluding(QVector3D(0., 0., 0.));
//    ok = _mesh.isIncluding(QVector3D(1, 1, 1));
//    ok = !ok;
	QMap<double, Mesh *> _population;
	for (auto i = 0; i < 50; ++i) {
		_mesh << MeshGenerator::genere();
		ui->lstMeshes->addItem(QString("\n%1\t%2\t%3").arg(_mesh.last()->volume()).arg(_mesh.last()->surface())
						   .arg(_mesh.last()->surface()/_mesh.last()->volume()));
		_population[_mesh.last()->surface()/_mesh.last()->volume()] = _mesh.last();
	}
	ui->lstMeshes->clear();
	QList <Mesh *> _newPop;
	for (auto i = 0; i < 50; ++i)
	{
		auto j = _population.keys().at(i%10);
		_newPop << new Mesh(*_population[j]);
		if (i >= 10)
		{
			MeshEvolver::evolve(_newPop.last());
		}
		ui->lstMeshes->addItem(QString("\n%1\t%2\t%3").arg(_newPop.last()->volume()).arg(_newPop.last()->surface())
						   .arg(_newPop.last()->surface()/_newPop.last()->volume()));
	}
	_mesh = _newPop;
}

MainWndGenDesign::~MainWndGenDesign()
{
    delete ui;
	for (auto i = 0; i < _mesh.size(); ++i) {
		_mesh[i]->toFile(QString("test%1.stl").arg(i));
	}
}

