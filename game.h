#ifndef __GAME_H__
#define __GAME_H__

#include <list>
#include <QPainter>

class Object;
typedef std::list<Object*> ObjList;

class Object
{
    protected:
    double x,y;
    double size;

    public:
    int id;
    virtual ~Object(){}
    virtual void Show(QPainter&) = 0;
    virtual void Next(ObjList& world, double dt) = 0;
    virtual void Commit(ObjList& world) = 0;

    Object* collides(ObjList& objects) const;
    double  distance(const Object& other) const;
};

class Kaban;
class Plank_Green;
class Plank_Blue;
class Doodler;
class Game
{
    private:
        ObjList objects;
        Kaban* kaban_ptr;
        Plank_Green* Green_ptr;
        Plank_Blue* Blue_ptr;
    public:
        Game();
        void Next(double dt);
        void Show(QPainter& p);
        void keyPressed(int key);
};

#endif
