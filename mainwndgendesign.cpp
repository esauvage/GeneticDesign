#include "mainwndgendesign.h"
#include "ui_mainwndgendesign.h"

#include "meshgenerator.h"
#include "meshevolver.h"
#include <iostream>

MainWndGenDesign::MainWndGenDesign(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWndGenDesign)
{
    ui->setupUi(this);
	srand(time(nullptr));
	for (auto i = 0; i < 100; ++i) {
        Mesh * m = MeshGenerator::genere();
		double l = m->longueurMax();
		l = l>1 ? l : 1/l;
		_population.insert(m->surface()/m->volume() * l, m);
	}
    for (auto i = 0; i < _population.size(); ++i) {
        _population.values()[i]->toFile(QString("test%1.stl").arg(i));
    }
	auto nbGenerations = 500;
    for (int k = 0; k < nbGenerations; k++)
    {
		QMultiMap <double, Mesh *> _newPop;
		std::cout << "Génération " << k << " Meilleure : " << _population.keys().first() << std::endl;
        for (auto i = 0; i < _population.size(); ++i)
        {
			auto j = _population.keys().at(i%20);
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

