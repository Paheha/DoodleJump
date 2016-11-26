#ifndef __GAME_H__
#define __GAME_H__

#include <set>
#include <QPainter>
#include <iostream>


class Object;
typedef std::set<Object*> ObjSet;

class Object
{
    protected:

    double size, size_x, size_y;
    public:
    double x,y;
    double dx,dy;
    int id;
    double Starting_Y;
    enum {Type = 0};
    virtual int type() const {return Type;}
    virtual ~Object(){}
    virtual void Show(QPainter&) = 0;
    virtual void Next(ObjSet& world, double dt) = 0;
    virtual void Commit(ObjSet& world) = 0;

    Object* collides(ObjSet& objects) const;
    double  distance(const Object& other) const;

    void translate(double delta) {

        y = Starting_Y + delta;
    }

};

class Plank_Green;
class Plank_Blue;
class Doodler;

class Game
{
    private:
        ObjSet objects;
        Plank_Green* Green_ptr;
        Plank_Blue* Blue_ptr;
        Doodler* Doodler_ptr;
    public:
        Game();
        void Next(double dt);
        void Show(QPainter& p);
        void keyPressed(int key);
        void keyReleased(int key);
};


#endif
