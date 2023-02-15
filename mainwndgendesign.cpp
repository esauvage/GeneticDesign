#include "mainwndgendesign.h"
#include "ui_mainwndgendesign.h"

#include "meshgenerator.h"
#include "meshevolver.h"

MainWndGenDesign::MainWndGenDesign(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWndGenDesign)
{
    ui->setupUi(this);
	srand(time(nullptr));
//    bool ok;
//    ok= _mesh.isIncluding(QVector3D(0.1, 0.1, 0.1));
//    ok = _mesh.isIncluding(QVector3D(0., 0., 0.));
//    ok = _mesh.isIncluding(QVector3D(1, 1, 1));
//    ok = !ok;
	for (auto i = 0; i < 100; ++i) {
        Mesh * m = MeshGenerator::genere();
//		ui->lstMeshes->addItem(QString("\n%1\t%2\t%3").arg(_mesh.last()->volume()).arg(_mesh.last()->surface())
//						   .arg(_mesh.last()->surface()/_mesh.last()->volume()));
		double l = m->longueurMax();
		l = l>1 ? l : 1/l;
		_population.insert(m->surface()/m->volume() * l, m);
	}
    for (auto i = 0; i < _population.size(); ++i) {
        _population.values()[i]->toFile(QString("test%1.stl").arg(i));
    }
	auto nbGenerations = 100;
    for (int k = 0; k < nbGenerations; k++)
    {
		QMultiMap <double, Mesh *> _newPop;
        for (auto i = 0; i < _population.size(); ++i)
        {
			auto j = _population.keys().at(i%20);
//            auto l = rand()%_population.values(j).size();
			Mesh * m = new Mesh(*_population.values(j).at(rand()%_population.values(j).size()));
			if (i >= 0.2*_population.size())
            {
                MeshEvolver::evolve(m);
            }
			double l = m->longueurMax();
			l = l>1 ? l : 1/l;
			auto cout = m->surface()/m->volume() * l;
			if (cout < 0)
			{
				i--;
			}
			else
			{
				_newPop.insert(cout, m);
			}
        }
        _population = _newPop;
        for (auto i = 0; i < _population.size(); ++i) {
            _population.values()[i]->toFile(QString("test%1.stl").arg(i));
        }
    }
    for (auto m : _population.values())
    {
		double l = m->longueurMax();
		l = l>1 ? l : 1/l;
		ui->lstMeshes->addItem(QString("\n%1\t%2\t%4\t%3").arg(m->volume()).arg(m->surface())
					   .arg(m->surface()/m->volume() * l).arg(l));
    }
}

MainWndGenDesign::~MainWndGenDesign()
{
    delete ui;
	for (auto i = 0; i < _population.size(); ++i) {
		_population.values()[i]->toFile(QString("test%1.stl").arg(i));
	}
}

