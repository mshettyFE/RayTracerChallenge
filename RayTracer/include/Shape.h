#ifndef SHAPE_H
#define SHAPE_H

#include "Matrix.h"
#include "Tuple.h"
#include "Material.h"
#include "Ray.h"
#include "Impact.h"
#include <memory>
#include <set>

class Shape{
private:
    static inline unsigned long GlobalID{0};
    Matrix Transformation;
    virtual Tuple local_normal_at(const Tuple& pt) const =0;
    virtual std::vector<Impact> local_intersect(const Ray &other) const =0;
protected:
    unsigned long id;
    Material mat;
    const Shape* parent;
    std::vector<const Shape*> children;
    std::string name;
    Matrix get_only_this_transform() const;
public:
    Shape(const Matrix& Transformation=MatIdentity(4), const Material& material=Material(), const Shape* parent=nullptr, std::string name="PLACEHOLDER");
    virtual ~Shape() {}

    virtual void verbose_print() const=0;

    Tuple normal_at(const Tuple& pt) const;
    std::vector<Impact> intersect(const Ray &other) const;

    void print(unsigned int indent=0, std::set<const Shape*> visited={}) const;

    int get_total_children() const;

    Matrix get_transform() const;
    Matrix get_aggregate_transform(int count=0, bool verbose=false) const;
    void set_transform(Matrix Tranformation);

    std::string get_name() const;

    void set_material(Material new_mat);
    Material get_material() const;
    unsigned long get_id() const;

    const Shape* get_parent() const;
    void set_parent(const Shape* new_parent);

};

#endif