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

double Line = 200;

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
        if ( y >= 1100 ) {
            y = 200 ;
            x = rand()%600 + 100;
            Starting_Y = 200;
        }
        x_new = x+dx; y_new = y+dy;
    }

    void Commit(){
        x = x_new; y = y_new;
    }
};

class Plank_Blue : public Object{
    double dx,dy;
    double x_new, y_new;
    static QImage sprite;
    public:

    Plank_Blue(int Y){
        id = 2;
        y = Y;
        x = 400;//rand()% 700 + 50;
        dy = 0;
        dx = 5;
    }
    double Starting_Y;
    enum {Type = 11};
    virtual int type() const {return Type;}

    void Show(QPainter& p)
    {
        p.save();
        p.translate(x,y);
        p.drawImage(-sprite.width()/2,-sprite.height()/2,sprite);
        p.restore();
    }

    void Next(ObjSet& objects)
    {
        if (( x >= 750 )&&(dx > 0)) x = 51;
        if (( x <= 50 )&&(dx < 0)) x = 749;
        x_new = x+dx;
        dy = dy + 0.06;
        y_new =y +  5* sin(dy);

    }

    void Commit(){
        x = x_new; y = y_new;
    }
};

QImage Plank_Blue::sprite(":/images/planks/blue.png");

class Doodler : public Object
{
    double ddx, ddy;
    double x_new, y_new, dx_new, dy_new;
    QPixmap sprite;

    public:
    bool KEY_A_PRESSED,KEY_D_PRESSED, KEY_SPACE_PRESSED;
    double y_fake, dy_fake;
    enum {Type = 1};
    virtual int type() const {return Type;}
    double dx = 0 , dy = 0;
    Doodler(): sprite(":/images/doodler.png"){
       KEY_A_PRESSED = false;
       KEY_D_PRESSED = false;
       KEY_SPACE_PRESSED = false;
       sprite.setMask(sprite.createHeuristicMask());
       x = 400; y = 900; ddx = 0.4, ddy = 0.06;
       dy_fake = 0;
       y_fake = y;
       size_x = sprite.width()/2;
       size_y = sprite.height()/2;
       cout<<size_x<<", "<<size_y<<endl;
    }

    void Show(QPainter& p)
    {
        p.save();
        p.translate(x,y);
        p.drawPixmap(-sprite.width()/2,-sprite.height()/2,sprite);
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
                        dy = -4;
                        std::cout << "Jump" << std::endl;
                    }
                    std::cout << "Green Plank" << std::endl;
                    break;
                }
                case 2:{
                    if (dy > 0)
                    {
                        dy = -4;
                        std::cout << "Jump" << std::endl;
                    }
                    std::cout << "Blue Plank" << std::endl;
                    break;
                }
            }
        }

        if (dx > 0 ) { dx = dx - ddx ;}
        if (dx < 0 ) { dx = dx + ddx ;}
        if (abs(dx)<0.4) dx = 0;
        if (( x >= 750 )&&(dx > 0)) x = 51;
        if (( x <= 50 )&&(dx < 0)) x = 749;
        if (( y >= 1200 )&&(dy > 0)) {y = 200;}

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
        if ( y_fake < 200 ) {
            dy = dy_fake;
            dy_fake = dy_fake + ddy;
            y_fake = y_fake + dy_fake;
        }
        else {
            dy_fake = dy;
            y_fake = y;
            y_new = y + dy;
        }
        dy = dy + ddy;
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
    objects.insert(new Plank_Green(300,500));
    objects.insert(new Plank_Green(200,700));
    objects.insert(new Plank_Green(400,1100));
    for (int i = 0; i < 7; i++) {
        objects.insert(new Plank_Green(rand()%800,i*120+30));
    }
}

void Game::Show(QPainter& p)
{
    bool translatePlanks = false;
    double delta = 0;
    if((Doodler_ptr)->y <= Line ) {
        translatePlanks = true;
        delta = Line - Doodler_ptr->y_fake;
    }
    else translatePlanks = false;

    for(ObjSet::const_iterator it = objects.begin(); it != objects.end(); ++it) {
        if(((*it)->type() == Plank_Green::Type) && translatePlanks ) {
           (*it)->translate(delta);
        }
        else (*it)->dy = 0;

        (*it)->Show(p);
    }

    for(ObjSet::const_iterator it = objects.begin(); it != objects.end(); ++it) {
        int Greens = 0;
        int Blues = 0;
        if ((*it)->type() == Plank_Green::Type) {Greens++;}
        if ((*it)->type() == Plank_Blue::Type) {Blues++;}
        //cout<<"Greens : "<<Greens<<endl;
        //cout<<"Blues : "<<Blues<<endl;
    }
}
