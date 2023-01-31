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
    QMultiMap<double, Mesh *> _population;
    for (auto i = 0; i < 20; ++i) {
        Mesh * m = MeshGenerator::genere();
//		ui->lstMeshes->addItem(QString("\n%1\t%2\t%3").arg(_mesh.last()->volume()).arg(_mesh.last()->surface())
//						   .arg(_mesh.last()->surface()/_mesh.last()->volume()));
        _population.insert(m->surface(), m);
	}
    for (int k = 0; k < 10; k++)
    {
        ui->lstMeshes->clear();
        QMultiMap <double, Mesh *> _newPop;
        for (auto i = 0; i < _population.size(); ++i)
        {
            auto j = _population.keys().at(i%10);
            auto l = rand()%_population.values(j).size();
            Mesh * m = new Mesh(*_population.values(j).at(l));
            if (i == 10)
            {
                MeshEvolver::evolve(m);
            }
            _newPop.insert(m->surface(), m);
        }
        _population = _newPop;
        QList <double> r;
        for (auto &m : _population)
        {
            r << m->surface();
        }
    }
    for (auto m : _population.values())
    {
        ui->lstMeshes->addItem(QString("\n%1\t%2\t%3").arg(m->volume()).arg(m->surface())
                       .arg(m->surface()/m->volume()));
    }
}

MainWndGenDesign::~MainWndGenDesign()
{
    delete ui;
	for (auto i = 0; i < _mesh.size(); ++i) {
		_mesh[i]->toFile(QString("test%1.stl").arg(i));
	}
}

