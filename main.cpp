#include <QApplication>
#include <game.h>

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>

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
};

void GameView::timerEvent(QTimerEvent*)
{
    the_game.Next(50);
    update();
}

void GameView::paintEvent(QPaintEvent*)
{
    QPainter p(this);
  the_game.Show(p);
}

void GameView::keyPressEvent(QKeyEvent* e)
{
  the_game.keyPressed(e->key());
}

int main(int ac, char* av[])
{
    QApplication a(ac,av);

    GameView gv; gv.show();

    QObject::connect(
            &a,SIGNAL(lastWindowClosed()),
            &a,SLOT(quit()));

    return a.exec();
}
