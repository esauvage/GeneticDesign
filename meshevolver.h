#ifndef MESHEVOLVER_H
#define MESHEVOLVER_H

#include "mesh.h"

class MeshEvolver
{
public:
	MeshEvolver();
	static void evolve(Mesh* m);
	static void addFacet(Mesh *m);
};

#endif // MESHEVOLVER_H
