#ifndef MAINWNDGENDESIGN_H
#define MAINWNDGENDESIGN_H

#include <QMainWindow>
#include "mesh.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWndGenDesign; }
QT_END_NAMESPACE

class MainWndGenDesign : public QMainWindow
{
    Q_OBJECT

public:
    MainWndGenDesign(QWidget *parent = nullptr);
    ~MainWndGenDesign();

private:
    Ui::MainWndGenDesign *ui;
	QList <Mesh> _mesh;
};
#endif // MAINWNDGENDESIGN_H
