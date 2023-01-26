#ifndef MESH_H
#define MESH_H

#include <QString>

class Mesh
{
public:
    Mesh();
    void toFile(QString filename);
private:
    QString _nom{"generation"};
};

#endif // MESH_H
