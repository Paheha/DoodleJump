#include <game.h>
#include <cmath>
#include <cstdlib>
#include <iostream> // Для std::cerr и др.
#include <QImage>
#include <QBitmap>
#include <QPixmap>
#include <QKeyEvent>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

double Line = 300;

Object* Object::collides(ObjSet& objects) const
{
    for(ObjSet::const_iterator it = objects.begin(); it != objects.end(); ++it)
    {
        if(*it == this) continue;
        double x1 = x - size_x; // Координата х верхнего края этого объекта
        double y1 = y - size_y; // Координата y левого края этого объекта
        double x2 = (*it)->x - (*it)->size_x; // Координата х верхнего края другого объекта
        double y2 = (*it)->y - (*it)->size_y; // Координата y левого края другого объекта



       QRect This(x1,y1,size_x*2,size_y*2);
       QRect Other(x2,y2,((*it)->size_x)*2,((*it)->size_y)*2);

       if(This.intersects(Other))
        {
                cout<<"COLLISION!!! "<<endl;
                return *it;
        }
    }
    return 0;
}

void Game::Next()
{
    for(ObjSet::const_iterator it = objects.begin(); it != objects.end(); ++it)
        (*it)->Next(objects);

    for(ObjSet::const_iterator it = objects.begin(); it != objects.end(); ++it)
        (*it)->Commit();
}

class Plank_Green : public Object{
    double x_new, y_new;
    QPixmap sprite;
    public:

    Plank_Green(int X,int Y): sprite(":/images/planks/normal.png"){
        size_x = sprite.width()/2;
        size_y = sprite.height()/2;
        cout<<size_x<<", "<<size_y<<endl;
        id = 1;
        dx = 0; dy = 0;
        x = X;
        y = Y;
        Starting_Y = Y;
    }

    ~Plank_Green() {}

    enum {Type = 10};
    virtual int type() const {return Type;}

    void Show(QPainter& p){
        p.save();
        p.translate(x,y);
        p.drawPixmap(-sprite.width()/2,-sprite.height()/2,sprite);
        p.restore();
    }

    void Next(ObjSet& objects){
        if (y >= 800) {
            y = 50;
            x = rand()%700 + 50 ;

        }
        x_new = x+dx; y_new = y+dy;
    }

    void Commit(){
        x = x_new; y = y_new;
    }
};

class Doodler : public Object
{
    double ddx, ddy;
    double x_new, y_new, dx_new, dy_new;
    QPixmap sprite;

    public:
    double score;
    double dx, dy;
    bool KEY_A_PRESSED,KEY_D_PRESSED, KEY_SPACE_PRESSED;
    enum {Type = 1};
    virtual int type() const {return Type;}
    Doodler(): sprite(":/images/doodler.png"){
        score = 0;
        KEY_A_PRESSED = false;
        KEY_D_PRESSED = false;
        KEY_SPACE_PRESSED = false;
        sprite.setMask(sprite.createHeuristicMask());
        x = 400; y = 400; ddx = 0.4, ddy = 0.09;
        dx = 0; dy = 0;
        size_x = sprite.width()/2;
        size_y = sprite.height()/8;
        cout<<size_x<<", "<<size_y<<endl;
    }

    void Show(QPainter& p)
    {
        p.save();
        p.drawText( QPoint(700,20),"Score " + QString::number(score));
        p.translate(x,y);
        p.drawPixmap(-sprite.width()/2,-sprite.height()*7/8,sprite);

        p.drawText( QPoint(40,-40), "x = " + QString::number(x));
        p.drawText( QPoint(40,-30), "y = " + QString::number(y));
        p.restore();
    }

    void Next(ObjSet& objects)
    {
        if (Object* Obj_c = collides(objects)) {
            std::cout << Obj_c->id << std::endl;
            switch(Obj_c->id){
                case 1:{
                    cout<< rand()<<endl;
                    if (dy > 0)
                    {
                        dy = -6;
                        std::cout << "Jump" << std::endl;
                    }
                    std::cout << "Green Plank" << std::endl;
                    break;
                }

            }
        }

        if (dx > 0 ) { dx = dx - ddx ;}
        if (dx < 0 ) { dx = dx + ddx ;}

        if (abs(dx)<0.4) dx = 0;

        if (( x >= 750 )&&(dx > 0)) x = 51;
        if (( x <= 50 )&&(dx < 0)) x = 749;
        if (( y >= 750 )&&(dy > 0)) {y = Line;}

        if (KEY_A_PRESSED) {
               dx = -4;
               std::cout << "Going left" << std::endl;
        }
        if (KEY_D_PRESSED) {
            dx =  4;
            std::cout << "Going right" << std::endl;
        }
        if (KEY_SPACE_PRESSED) {
            x = 400; y = 400;
            std::cout << "Stopping" << std::endl;
        }
        dy = dy + ddy;
        if (( y <= Line)&&(dy < 0)) {
            score -= dy;
            y = Line;
        }
        else y_new = y + dy;

        x_new = x+dx;

    }

    void Commit(){
        x = x_new; y = y_new;
    }

};

void Game::keyPressed(int key)
{
    if(Doodler_ptr) {
        switch(key) {
            case (Qt::Key_A):
                Doodler_ptr->KEY_A_PRESSED = true;
                break;
            case Qt::Key_D:
                Doodler_ptr->KEY_D_PRESSED = true;
                break;
            case Qt::Key_Space:
                Doodler_ptr->KEY_SPACE_PRESSED = true;
                break;
            case Qt::Key_Left:
                Doodler_ptr->KEY_A_PRESSED = true;
                break;
            case Qt::Key_Right:
                Doodler_ptr->KEY_D_PRESSED = true;
                break;
        }
    }
}
void Game::keyReleased(int key)
{
    if(Doodler_ptr) {
        switch(key)
        {
            case Qt::Key_A:
                Doodler_ptr->KEY_A_PRESSED = false;
                break;
            case Qt::Key_D:
                Doodler_ptr->KEY_D_PRESSED = false;
                break;
            case Qt::Key_Space:
                Doodler_ptr->KEY_SPACE_PRESSED = false;
                break;
            case Qt::Key_Left:
                Doodler_ptr->KEY_A_PRESSED = false;
                break;
            case Qt::Key_Right:
                Doodler_ptr->KEY_D_PRESSED = false;
                break;
        }
    }
}

Game::Game()
{
    Doodler_ptr = new Doodler;
    objects.insert(Doodler_ptr);

    for (int i = 1; i < 8; i++) {
        objects.insert(new Plank_Green(rand()%700 + 50,i*90));
    }
}

void Game::Show(QPainter& p)
{
    bool translatePlanks = false;
    double delta = 0;
    if((Doodler_ptr)->y <= Line ) {
        translatePlanks = true;
        delta = Doodler_ptr->dy;
    }
    else translatePlanks = false;

    for(ObjSet::const_iterator it = objects.begin(); it != objects.end(); ++it) {
        if(((*it)->type() == Plank_Green::Type) && translatePlanks ) {
           (*it)->translate(delta);
        }
        else (*it)->dy = 0;

        (*it)->Show(p);
    }
}
