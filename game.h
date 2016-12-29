#ifndef __GAME_H__
#define __GAME_H__

#include <set>
#include <QPainter>
#include <iostream>

class Object;
typedef std::set<Object*> ObjSet;

class Object{
    protected:
    double size_x, size_y;

    public:
    double x,y;
    double dx,dy;
    int id;
    double Starting_Y;
    enum {Type = 0};
    virtual int type() const {return Type;}
    virtual ~Object(){}
    virtual void Show(QPainter&) = 0;
    virtual void Next(ObjSet& world) = 0;
    virtual void Commit() = 0;
    Object* collides(ObjSet& objects) const;
    void translate(double delta) {
        y -= delta;
    }
};

class Plank_Green;
class Doodler;

class Game
{
    private:
        ObjSet objects;
        Plank_Green* Green_ptr;
        Doodler* Doodler_ptr;
    public:
        Game();
        void Next();
        void Show(QPainter& p);
        void keyPressed(int key);
        void keyReleased(int key);
};

#endif
