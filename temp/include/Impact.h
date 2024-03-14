#ifndef IMPACT_H
#define IMPACT_H

#include <vector>
#include <iostream>

class Shape;

class Impact{
private:
    double time;
    const Shape* obj;
public:
    Impact(double time, const Shape* obj);
    Impact();
    
    double get_t() const;
    const Shape* get_obj() const;

    bool operator==(const Impact& other) const;
    bool operator!=(const Impact& other) const;

    friend std::ostream& operator << (std::ostream &out, const Impact& other);

};

Impact first_hit(std::vector<Impact> intersections);

#endif