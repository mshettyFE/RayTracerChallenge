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
    virtual Tuple local_normal_at(const Tuple& pt, const Impact& impt=Impact()) const =0;
    virtual std::vector<Impact> local_intersect(const Ray &other) const =0;
    Matrix get_aggregate_transform(std::set<const Shape*>& visited, bool verbose=false, int count=0) const;
protected:
    unsigned long id;
    Material mat;
    const Shape* parent;
    std::vector<std::unique_ptr<Shape>> children;
    std::string name;
public:
    Shape(const Matrix& Transformation=MatIdentity(4), const Material& material=Material(), const Shape* parent=nullptr, std::string name="PLACEHOLDER");
//    Shape(const Shape&) = delete;
//    Shape& operator= (const Shape&) = delete;
    Shape(Shape&& mS)            = default;
    Shape& operator=(Shape&& mS) = default;
    virtual ~Shape() {}

    virtual void verbose_print() const=0;

    Tuple normal_at(const Tuple& pt, const Impact& impt=Impact()) const;
    std::vector<Impact> intersect(const Ray &other) const;

    void print(unsigned int indent=0, std::set<const Shape*> visited={}) const;

    int get_total_children() const;
    const Shape* get_node(int id, int count=0, bool debug=false) const;

    Matrix get_transform() const;
    Matrix get_aggregate_transform(bool verbose=false) const;
    void set_transform(Matrix Tranformation);

    std::string get_name() const;

    void set_material(Material new_mat);
    Material get_material() const;
    unsigned long get_id() const;

    const Shape* get_parent() const;
    void set_parent(const Shape* new_parent);

    const std::vector<std::unique_ptr<Shape>>& get_children() const;

    const Shape* get_this() const{return this;}

};


#endif