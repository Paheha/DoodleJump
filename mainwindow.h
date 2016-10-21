#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  //  setMouseTracking(true);

 //   m_plblX = new QLabel(this);
//    m_plblY = new QLabel(this);
 //   statusBar()->addWidget(m_plblY);
 //   statusBar()->addWidget(m_plblX);

private:
    Ui::MainWindow *ui;
    QLabel* m_plblX;
    QLabel* m_plblY;

protected:
    virtual void mouseMoveEvent(QMouseEvent* pe)
    {
  //      m_plblX->setText("X=" + QString().setNum(pe->x()));
 //       m_plblY->setText("Y=" + QString().setNum(pe->y()));
    }
};

#endif // MAINWINDOW_H
