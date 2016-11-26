#include <QApplication>
#include <game.h>
#include <QResource>
#include <QDir>
#include <QFile>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <iostream>
using namespace std;
class GameView : public QWidget
{
        Game the_game;
        public:
        GameView():QWidget(0)
        {
                setFixedSize(800,800);
                startTimer(10);
        }
        void timerEvent(QTimerEvent*);
        void paintEvent(QPaintEvent*);
        void keyPressEvent(QKeyEvent*);
        //void doodler_jump(QPaintEvent*,Game the_game);
};

void GameView::timerEvent(QTimerEvent*)
{
        the_game.Next(10);
        update();
}

void GameView::paintEvent(QPaintEvent*)
{
        QPainter p(this);
        p.drawLine(0,200,800,200);
        p.drawLine(0,550,800,550);
        the_game.Show(p);
}



void GameView::keyPressEvent(QKeyEvent* e)
{
  the_game.keyPressed(e->key());
}

int main(int ac, char* av[])
{
        setlocale( LC_ALL,"Russian" );
        QApplication a(ac,av);
        QResource::registerResource("myresource.rcc");
        GameView gv; gv.show();

        QObject::connect(
                        &a,SIGNAL(lastWindowClosed()),
                        &a,SLOT(quit()));

        return a.exec();
}
