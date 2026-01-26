#ifndef IMPACT_H
#define IMPACT_H

#include <vector>
#include <iostream>

class Shape;

class Impact{
private:
    double time;
    const Shape* obj;
    double u;
    double v;
public:
    Impact(double time, const Shape* obj, double u=0, double v=0);
    Impact();
    
    double get_t() const;
    double get_u() const;
    double  get_v() const;
    const Shape* get_obj() const;

    bool operator==(const Impact& other) const;
    bool operator!=(const Impact& other) const;

    bool operator<(const Impact& other) const;
    bool operator<=(const Impact& other) const;
    bool operator>(const Impact& other) const;
    bool operator>=(const Impact& other) const;

    friend std::ostream& operator << (std::ostream &out, const Impact& other);

};

Impact first_hit(std::vector<Impact> intersections);

#endif