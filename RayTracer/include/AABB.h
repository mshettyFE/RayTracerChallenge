#ifndef AABB_H
#define AABB_H

#include "Ray.h"
#include "Impact.h"
#include "Constants.h"
#include <vector>
#include <memory>
#include <map>
#include <type_traits>

class Shape;

class AABB{
private:
    const Shape* enclosed_shape=nullptr;
    std::vector<std::unique_ptr<AABB>> center{};
    std::unique_ptr<AABB> left{};
    std::unique_ptr<AABB> right{};
    std::array<double,3> min_bounds={INFTY,INFTY,INFTY};
    std::array<double,3> max_bounds={NEG_INFTY,NEG_INFTY,NEG_INFTY};
    void indented_print( const AABB* current_box, int indent=0, bool verbose=false) const;
    std::vector<double> check_axis(double origin, double direction, double min, double max) const;
public:
    bool intersect(const Ray &other) const;

    AABB(std::initializer_list<double> min_bounds, std::initializer_list<double> max_bounds, const Shape* shp=nullptr);
    AABB(Tuple min_bounds, Tuple max_bounds, const Shape* shp=nullptr);
    AABB(){}

    void add_point(const Tuple new_point);

    Tuple get_min() const;
    Tuple get_max() const;
    Tuple get_mid() const;

    double get_min_x() const{return min_bounds[0];}
    double get_min_y() const{return min_bounds[1];}
    double get_min_z() const{return min_bounds[2];}
    double get_max_x() const{return max_bounds[0];}
    double get_max_y() const{return max_bounds[1];}
    double get_max_z() const{return max_bounds[2];}

    void set_min_x(double value) ;
    void set_min_y(double value) ;
    void set_min_z(double value) ;
    void set_max_x(double value) ;
    void set_max_y(double value) ;
    void set_max_z(double value) ;

    bool operator==(const AABB& other) const;
    bool operator!=(const AABB& other) const;

    bool contains(const Tuple& points) const;
    bool contains(const AABB& new_box) const;

    const AABB* get_left() const{return this->left.get();}
    const AABB* get_right() const{return this->right.get();}

    std::vector<const AABB*> get_center() const{
        std::vector<const AABB*> output;
        for(auto const& child: center){
            output.push_back(child.get());
        }
        return output;
    }

    std::vector<const AABB*> get_center(){
        std::vector<const AABB*> output;
        for(auto const& box: center){output.push_back(box.get());}
        return output;
    }

    const Shape* get_shape() const {return enclosed_shape;}
    void set_shape(const Shape* new_shape){this->enclosed_shape = new_shape;}

    void expand_box(const AABB* new_box);
    bool insert(std::unique_ptr<AABB>& new_box, unsigned int depth=0, unsigned int max_depth = 20);

    void split();

    bool straddle(const AABB* new_box) const;

    void print(bool verbose=false) const;

    void transform(Matrix mat);

    friend std::ostream& operator << (std::ostream &out, const AABB& other);

};

std::unique_ptr<AABB> parent_space_bounds(const Shape* shape);

#endif