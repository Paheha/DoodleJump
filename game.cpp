#include <game.h>
#include <cmath>
#include <cstdlib>
#include <typeinfo> // Для std::bad_cast
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

class Plank_Green ;

double Object::distance(const Object& other) const
{


    double delta_x = other.x - x;
    double delta_y = other.y - y;
    return sqrt(delta_x*delta_x + delta_y*delta_y);
}

Object* Object::collides(ObjSet& objects) const
{
    /*
    for(ObjSet::const_iterator it = objects.begin(); it != objects.end(); ++it)
    {
        if(*it == this) continue;
        if (distance(**it) < (size+(*it)->size)) {
                cout<<"Distance = "<<distance(**it)<<endl;
                return *it;
        }
    } */

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

void Game::Next(double dt)
{

    for(ObjSet::const_iterator it = objects.begin(); it != objects.end(); ++it)
        (*it)->Next(objects, dt);

    for(ObjSet::const_iterator it = objects.begin(); it != objects.end(); ++it)
        (*it)->Commit(objects);
}

//====== Objects

class Plank_Green : public Object
{

    double x_new, y_new;
    static QImage sprite;

    public:


    Plank_Green(int X,int Y)
    {
        size_x = sprite.width()/2;
        size_y = sprite.height()/2;
        cout<<size_x<<", "<<size_y<<endl;
        size = 10;
        id = 1;
        x = X;
        y = Y;
        dx = 0;//rand() % 20 - 10 ;
        dy = 0;
        Starting_Y = Y;
    }

    ~Plank_Green() {


    }

    enum {Type = 10};
    virtual int type() const {return Type;}


    void Show(QPainter& p)
    {
        p.save();

        p.translate(x,y);
        p.drawImage(-sprite.width()/2,-sprite.height()/2,sprite);

        p.restore();
    }

    void Next(ObjSet& objects, double dt)
    {

       // if (( x >= 750 )&&(dx > 0)) x = 51;
       // if (( x <= 50 )&&(dx < 0)) x = 749;
        x_new = x+dx; y_new = y+dy;

    }

    void Commit(ObjSet& objects)
    {
        x = x_new; y = y_new;
    }
};


QImage Plank_Green::sprite(":/images/planks/normal.png");

class Plank_Blue : public Object
{
    double dx,dy;
    double x_new, y_new;
    static QImage sprite;
    public:

    Plank_Blue(int Y)
    {
        size = 50;
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

    void Next(ObjSet& objects, double dt)
    {
        if (( x >= 750 )&&(dx > 0)) x = 51;
        if (( x <= 50 )&&(dx < 0)) x = 749;
        x_new = x+dx;
        dy = dy + 0.06;
        y_new =y +  5* sin(dy);

    }

    void Commit(ObjSet& objects)
    {
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
    double y_fake, dy_fake;
    enum {Type = 1};
    virtual int type() const {return Type;}
    double dx,dy;
    Doodler(): dx(0), dy(0), sprite(":/images/doodler.png")
    {
       sprite.setMask(sprite.createHeuristicMask());
       x = 400; y = 300; ddx = 0.1, ddy = 0.06; size = 80;
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

    void do_command(int key)
        {
            std::cout << "Do_command: " << key << std::endl;
            switch(key)
            {
               case Qt::Key_A:
                       dx = -6;
                       std::cout << "Going left" << std::endl;
                       break;
               case Qt::Key_D:
                       dx =  6;
                       std::cout << "Going right" << std::endl;
                       break;
               case Qt::Key_Space:
                       x = 400; y =400;
                       std::cout << "Stopping" << std::endl;
                       break;
                }
           }

    void Next(ObjSet& objects, double dt)
    {
        if (Object* Obj_c = collides(objects)) {

            std::cout << Obj_c->id << std::endl;

            switch(Obj_c->id)
            {
            case 1:
            {
                cout<< rand()<<endl;
                //std::cout << collides(objects)->id << std::endl;
                if (dy > 0)
                {
                    dy = -5;
                    std::cout << "Jump" << std::endl;
                }
                std::cout << "Green Plank" << std::endl;
                break;
            }
            case 2:
            {
                //std::cout << collides(objects)->id << std::endl;
                if (dy > 0)
                {
                    dy = -5;
                    std::cout << "Jump" << std::endl;
                }
                std::cout << "Blue Plank" << std::endl;
                break;
            }
            }
        }

        if (dx > 0 ) { dx = dx - ddx ;}
        if (dx < 0 ) { dx = dx + ddx ;}
        if (abs(dx)<0.07) dx = 0;
        if (( x >= 750 )&&(dx > 0)) x = 51;
        if (( x <= 50 )&&(dx < 0)) x = 749;
        //if (( y < 50 )&&(dy < 0)) y = 749;
        if (( y >= 750 )&&(dy > 0)) y = 400;


        if ( y_fake < 200 ) {
            dy = dy_fake;

            dy_fake = dy_fake + ddy;
            y_fake = y_fake + dy_fake;

            //cout<<" y_f= "<<y_fake<<" y= "<<y<<endl;
        }
        else
        {
            dy_fake = dy;
            y_fake = y;
            //cout<<" y_f= "<<y_fake<<" y= "<<y<<endl;
            y_new = y + dy;
        }

        dy = dy + ddy;

        x_new = x+dx;
    }

    void Commit(ObjSet& objects)
    {
        x = x_new; y = y_new;
    }

};

//QPixmap Doodler::sprite(":/images/doodler.png");
//=========

void Game::keyPressed(int key)
{
    if(Doodler_ptr) Doodler_ptr->do_command(key);

}


Game::Game()
{
    Doodler_ptr = new Doodler;
    objects.insert(Doodler_ptr);
    objects.insert(new Plank_Green(150,700));
    objects.insert(new Plank_Blue(150));
    objects.insert(new Plank_Green(400,400));
}

void Game::Show(QPainter& p)
{

    bool translatePlanks = false;
    double delta = 0;
    if((Doodler_ptr)->y <= Line ) {
        //const Doodler* doodler = static_cast<const Doodler*>(*it);
        translatePlanks = true;
        //dood_dy = Line - Doodler_ptr->y_fake;
        delta = Line - Doodler_ptr->y_fake;
        //objects.push_back(new Plank_Green(400,400));
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
