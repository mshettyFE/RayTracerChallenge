#ifndef COLLISION_INFO_H
#define COLLISION_INFO_H

#include "Shape.h"
#include "Tuple.h"
#include "Impact.h"
#include "Constants.h"
#include <memory>

class CollisionInfo{
private:
    Impact impact;
    Tuple pnt;
    Tuple over_pnt;
    Tuple eye;
    Tuple normal;
    bool inside;
public:
    CollisionInfo(const Impact& impt, const Ray& ray, double res=glob_resolution);
    
    Impact get_impact() const;
    Tuple get_pnt() const;
    Tuple get_over_pnt() const;
    Tuple get_eye() const;
    Tuple get_normal() const;
    bool is_inside() const;

    friend std::ostream& operator << (std::ostream &out, const CollisionInfo& other);

};

#endif