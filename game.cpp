#include <game.h>
#include <cmath>
#include <cstdlib>
#include <typeinfo> // ƒл€ std::bad_cast
#include <iostream> // ƒл€ std::cerr и др.
#include <QImage>
#include <QKeyEvent>
#include <iostream>
using namespace std;
double Doodler_dx = 0;
double Doodler_dy = 0;

double Object::distance(const Object& other) const
{
    double dx = other.x - x;
    double dy = other.y - y;
    return sqrt(dx*dx + dy*dy);
}

Object* Object::collides(ObjList& objects) const
{
    for(ObjList::const_iterator it = objects.begin(); it != objects.end(); ++it)
    {
        if(*it == this) continue;
    if(distance(**it) < (size+(*it)->size))
            return *it;
    }
    return 0;
}

void Game::Next(double dt)
{
    for(ObjList::const_iterator it = objects.begin(); it != objects.end(); ++it)
        (*it)->Next(objects, dt);

    for(ObjList::const_iterator it = objects.begin(); it != objects.end(); ++it)
        (*it)->Commit(objects);
}


void Game::Show(QPainter& p)
{
    for(ObjList::const_iterator it = objects.begin(); it != objects.end(); ++it)
    (*it)->Show(p);
}

//====== Objects

class Slonopotam : public Object
{
    double dx,dy;
    double x_new, y_new;
    static QImage sprite;
    public:
    Slonopotam(int X, int Y): dx(0), dy(0)
    {
        x = X; y = Y; size = 150;
    }

    void change_direction()
    {
        dx = rand() % 20 - 10;
        dy = rand() % 20 - 10;
    }

    void Show(QPainter& p)
    {
        p.save();

        p.translate(x,y);
        p.drawImage(-sprite.width()/2,-sprite.height()/2,sprite);

        p.restore();
    }

    void Next(ObjList& objects, double dt)
    {
        if(x < 50 || x > 750 || y < 50 || y > 750)
        {
            if(x < 50)  dx =  fabs(dx);
            if(y < 50)  dy =  fabs(dy);
            if(x > 750) dx = -fabs(dx);
            if(y > 750) dy = -fabs(dy);
        }
        else if(collides(objects))
        {
                dx = -dx; dy = -dy;
        }
        else if(rand() % 1000 < 10)
            change_direction();



        x_new = x+dx; y_new = y+dy;
    }

    void Commit(ObjList& objects)
    {
        x = x_new; y = y_new;
    }
};

QImage Slonopotam::sprite("C:/Users/Gimarr/Documents/DoodleMain/slonopotam.png");

class Kaban : public Object
{
    double dx,dy;
    double x_new, y_new;
    static QImage sprite;
    public:
    Kaban(int X, int Y): dx(0), dy(0)
    {
        x = X; y = Y; size = 60;
    }

    void Show(QPainter& p)
    {
        p.save();

        p.translate(x,y);
        p.drawImage(-sprite.width()/2,-sprite.height()/2,sprite);

        p.restore();
    }

    void Next(ObjList& objects, double dt)
    {
        if(x < 50 || x > 750 || y < 50 || y > 750)
        {
            dx =  0;
            dy =  0;
        }
        else if(collides(objects))
        {
            dx = 0; dy = 0;
            std::cout << "Stopping due to collision" << std::endl;
        }
        x_new = x+dx; y_new = y+dy;
    }

    void Commit(ObjList& objects)
    {
        x = x_new; y = y_new;
    }
    void do_command(int key)
    {
    std::cout << "Do_command: " << key << std::endl;
        switch(key)
        {
            case Qt::Key_A:
                dx = -5;
                std::cout << "Going left" << std::endl;
                break;
            case Qt::Key_W:
                dy = -5;
                std::cout << "Going up" << std::endl;
                break;
            case Qt::Key_S:
                dy =  5;
                std::cout << "Going down" << std::endl;
                break;
            case Qt::Key_D:
                dx =  5;
                std::cout << "Going right" << std::endl;
                break;
            case Qt::Key_Space:
                dx = 0; dy = 0;
                std::cout << "Stopping" << std::endl;
                break;
        }
    }


};

QImage Kaban::sprite("C:/Users/Gimarr/Documents/DoodleMain/kaban.png");

class Plank_Green : public Object
{
    double dx,dy;
    double x_new, y_new;
    static QImage sprite;
    public:
    Plank_Green(int Y)
    {
        id = 1;
        y = Y;
        x = 400;//rand()% 700 + 50;
        dx = 0;//rand() % 20 - 10 ;
        dy = 0;
    }


    void Show(QPainter& p)
    {
        p.save();

        p.translate(x,y);
        p.drawImage(-sprite.width()/2,-sprite.height()/2,sprite);

        p.restore();
    }

    void Next(ObjList& objects, double dt)
    {
        if (( x >= 750 )&&(dx > 0)) x = 51;
        if (( x <= 50 )&&(dx < 0)) x = 749;
        x_new = x+dx; y_new = y+dy;

    }

    void Commit(ObjList& objects)
    {
        x = x_new; y = y_new;
    }
};

QImage Plank_Green::sprite("C:/Users/Gimarr/Documents/DoodleMain/Planks/normal.png");

class Plank_Blue : public Object
{
    double dx,dy;
    double x_new, y_new;
    static QImage sprite;
    public:
    Plank_Blue(int Y)
    {
        id = 2;
        y = 100;
        x = 400;//rand()% 700 + 50;
        dy = 0;
        dx = 5;

    }


    void Show(QPainter& p)
    {
        p.save();

        p.translate(x,y);
        p.drawImage(-sprite.width()/2,-sprite.height()/2,sprite);

        p.restore();
    }

    void Next(ObjList& objects, double dt)
    {
        if (( x >= 750 )&&(dx > 0)) x = 51;
        if (( x <= 50 )&&(dx < 0)) x = 749;
        x_new = x+dx;
        dy = dy + 0.06;
        y_new =400 +  50* sin(dy);

    }

    void Commit(ObjList& objects)
    {
        x = x_new; y = y_new;
    }
};

QImage Plank_Blue::sprite("C:/Users/Gimarr/Documents/DoodleMain/Planks/blue.png");

class Doodler : public Object
{

    double dx,dy, ddx, ddy;
    double x_new, y_new, dx_new, dy_new;
    static QImage sprite;
    public:
    Doodler(): dx(0), dy(0)
    {
        x = 400; y = 300; ddx = 0, ddy = 0.06; size = 80;
    }

    void Show(QPainter& p)
    {
        p.save();

        p.translate(x,y);
        p.drawImage(-sprite.width()/2,-sprite.height()/2,sprite);

        p.restore();
    }

    void do_command(int key)
        {
            std::cout << "Do_command: " << key << std::endl;
            switch(key)
            {
               case Qt::Key_A:
                       dx = -5;
                       std::cout << "Going left" << std::endl;
                       break;
               case Qt::Key_D:
                       dx =  5;
                       std::cout << "Going right" << std::endl;
                       break;
               case Qt::Key_Space:
                       dx = 0; dy = 0;
                       std::cout << "Stopping" << std::endl;
                       break;
                }
           }

    void Next(ObjList& objects, double dt)
    {
        if (( x >= 750 )&&(dx > 0)) x = 51;
        if (( x <= 50 )&&(dx < 0)) x = 749;
        if ( y > 750 ) y = 51;

        if (collides(objects)) {

            std::cout << collides(objects)->id << std::endl;

            switch((collides(objects))->id)
            {
            case 1:
            {

                std::cout << collides(objects)->id << std::endl;
                if (dy > 0)
                {
                    dy = -6;
                    std::cout << "Jump" << std::endl;
                }
                std::cout << "Green Plank" << std::endl;
                break;
            }
            case 2:
            {
                std::cout << collides(objects)->id << std::endl;
                if (dy > 0)
                {
                    dy = -6;
                    std::cout << "Jump" << std::endl;
                }
                std::cout << "Blue Plank" << std::endl;
                break;
            }
            }
        }

        dx = dx + ddx; dy = dy + ddy;

        x_new = x+dx; y_new = y + dy;

    }

    void Commit(ObjList& objects)
    {
        x = x_new; y = y_new;
    }

};

QImage Doodler::sprite("C:/Users/Gimarr/Documents/DoodleMain/doodler.png");
//=========

void Game::keyPressed(int key)
{
    if(kaban_ptr) kaban_ptr->do_command(key);
}


Game::Game(): kaban_ptr(0)
{
    //objects.push_back(new Slonopotam(100,400));
    //kaban_ptr = new Kaban(400,400);
    //objects.push_back(kaban_ptr);
    objects.push_back(new Plank_Green(600));
    objects.push_back(new Plank_Blue(200));
    objects.push_back(new Doodler);
}

