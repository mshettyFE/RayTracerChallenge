#ifndef IMPACT_H
#define IMPACT_H

#include "Shape.h"
#include "Tuple.h"
#include <memory>

class Impact{
private:
    double time;
    std::shared_ptr<Shape> obj;
public:
    Impact(double time,const std::shared_ptr<Shape>& obj);
    Impact();
    
    double get_t() const;
    std::shared_ptr<Shape> get_obj() const;

    bool operator==(const Impact& other) const;
    bool operator!=(const Impact& other) const;

    friend std::ostream& operator << (std::ostream &out, const Impact& other);

};

Impact first_hit(std::vector<Impact> intersections);

#endif