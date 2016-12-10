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
                setFixedSize(800,1200);
                startTimer(10);
        }
        void timerEvent(QTimerEvent*);
        void paintEvent(QPaintEvent*);
        void keyPressEvent(QKeyEvent*);
        void keyReleaseEvent(QKeyEvent*);
};

void GameView::timerEvent(QTimerEvent*)
{
        the_game.Next();
        update();
}

void GameView::paintEvent(QPaintEvent*)
{
        QPainter p(this);
        for (int i = 0; i<= 1200; i+=100) {
            p.drawText( QPoint(10,i+10),QString::number(i));
            p.drawLine(0,i,800,i);
        }
        the_game.Show(p);
}


void GameView::keyPressEvent(QKeyEvent* e)
{
  the_game.keyPressed(e->key());
}
void GameView::keyReleaseEvent(QKeyEvent* e)
{
  the_game.keyReleased(e->key());
}

int main(int ac, char* av[])
{
        setlocale( LC_ALL,"Russian" );
        QApplication a(ac,av);
        QResource::registerResource("myresource.rcc");
        GameView gv; gv.show();
       /* QBrush br(Qt::TexturePattern);
            br.setTextureImage(QImage(":/images/backgr.png"));
            QPalette plt = gv.palette();
            plt.setBrush(QPalette::Background, br);
            gv.setPalette(plt);
            gv.show(); */
        QObject::connect(
                        &a,SIGNAL(lastWindowClosed()),
                        &a,SLOT(quit()));

        return a.exec();
}
