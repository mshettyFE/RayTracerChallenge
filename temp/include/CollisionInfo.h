#ifndef COLLISION_INFO_H
#define COLLISION_INFO_H

#include "Shape.h"
#include "Tuple.h"
#include "Impact.h"
#include "Constants.h"
#include <memory>
#include <vector>

class CollisionInfo{
private:
    std::unique_ptr<Impact> impact;
    Tuple pnt;
    Tuple over_pnt;
    Tuple under_pnt;
    Tuple eye;
    Tuple normal;
    Tuple reflect_v;
    bool inside;
    double n1;
    double n2;
    void calculate_indices(const std::vector<Impact>& all_hits);
public:
    CollisionInfo(std::unique_ptr<Impact> impt, const Ray& ray, const std::vector<Impact>& all_hits= {});
    
    const Impact get_impact() const;
    Tuple get_pnt() const;
    Tuple get_over_pnt() const;
    Tuple get_under_pnt() const;
    Tuple get_eye() const;
    Tuple get_normal() const;
    Tuple get_reflect() const;
    bool is_inside() const;

    double get_n1() const;
    double get_n2() const;

    double schlick() const;

    friend std::ostream& operator << (std::ostream &out, const CollisionInfo& other);

};

#endif