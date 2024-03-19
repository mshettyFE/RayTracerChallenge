#ifndef CSG_H
#define CSG_H

#include "Shape.h"
#include "Matrix.h"

enum CSG_OPS{UNION,DIFFERENCE, INTERSECTION};

class CSG : public Shape{
private:
    CSG_OPS join_type;
public:
    CSG(CSG_OPS join_type, std::unique_ptr<Shape> left, std::unique_ptr<Shape> right, Matrix Transformation=MatIdentity(4));

    bool intersection_allowed(bool lhit, bool inl, bool inr) const;

    Tuple local_normal_at(const Tuple& pt, const Impact& impt=Impact()) const override;
    std::vector<Impact> local_intersect(const Ray &other) const override;
    void verbose_print() const override;
    std::unique_ptr<AABB> bound() const override;

    const Shape* get_left() const{return children[0].get();};
    const Shape* get_right() const{return children[1].get();};
    const std::string get_type() const{
        switch(join_type){
            case UNION:
                return "union";
                break;
            case DIFFERENCE:
                return "difference";
                break;
            case INTERSECTION:
                return "intersection";
                break;
            default:
                return "Unimplemented";
        }
    }

};

#endif