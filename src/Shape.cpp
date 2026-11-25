#include "Shape.h"
#include "Matrix.h"
#include <set>
#include <stdexcept>

Shape::Shape(const Matrix& Transformation, const Material& material, const Shape* parent, std::string name)
    : Transformation(Transformation), id(GlobalID++), mat(material), parent(parent), name(std::move(name)){
    update_world_transform();
}

Shape::Shape(Shape&& other)
    : Transformation(std::move(other.Transformation)), id(other.id), mat(std::move(other.mat)),
      parent(other.parent), children(std::move(other.children)), name(std::move(other.name)){
    update_world_transform();
}

Shape& Shape::operator=(Shape&& other){
    if(this != &other){
        Transformation = std::move(other.Transformation);
        id = other.id;
        mat = std::move(other.mat);
        parent = other.parent;
        children = std::move(other.children);
        name = std::move(other.name);
        update_world_transform();
    }
    return *this;
}

void Shape::set_transform(Matrix a_Tranformation){
    this->Transformation = a_Tranformation;
    update_world_transform();
}

// Recomputes the cached world transform for this shape, then for each descendant. Also points
// every child's parent at this, which keeps children correct after this shape is moved.
void Shape::update_world_transform(){
    world_transform = (parent == nullptr) ? Transformation : parent->world_transform * Transformation;
    world_inverse = world_transform.Inverse();
    for(auto& child: children){
        child->parent = this;
        child->update_world_transform();
    }
}

Matrix Shape::get_transform() const{
    return Transformation;
}

Matrix Shape::get_aggregate_transform(bool verbose) const{
    std::set<const Shape *> visited;
    return get_aggregate_transform(visited, verbose);
}


Matrix Shape::get_aggregate_transform(std::set<const Shape*>& visited , bool verbose, int count) const{
    Matrix output = MatIdentity(4);
    if(verbose){
        if(get_parent() == nullptr){
            std::cout << "Level: " << count << " Current: " << this->get_id() <<   " Parent: NULL" << std::endl;
        }
        else{
            std::cout << "Level: " << count << " Current: " << this->get_id() <<  " Parent: " << this->get_parent()->get_id() << std::endl;
        }
    }
    if(visited.count(this)){
        throw std::invalid_argument("Cycle detected in when calculating aggregate transformation. Check add_child arguments in Group (or CSG)");
    }
    visited.insert(this);
    count++;
    if(get_parent() == nullptr){
        output = Transformation;
        return output;
    }
    Matrix parent_matrix = get_parent()->get_aggregate_transform(visited,verbose,count);
    output = parent_matrix*this->Transformation;
    return output;
}

void Shape::set_material(Material new_mat){
    this->mat = new_mat;
}


const Shape* Shape::get_parent() const{
    return this->parent;
}

void Shape::set_parent(const  Shape* new_parent){
    this->parent = new_parent;
    update_world_transform();
}

unsigned long Shape::get_id() const{return id;}

std::string Shape::get_name() const{
    return name;
}

void Shape::print(unsigned int indent, std::set<const Shape*> visited) const{
    auto indentation = std::string(indent,'\t');
    if(parent == nullptr){
        std::cout << indentation <<"Shape: " <<  name << "Loc: " << this << " ID: " << get_id() << " Parent: " << parent <<  std::endl;
    }
    else{
        std::cout << indentation <<"Shape: " <<  name << "Loc: " << this << " ID: " << get_id() << " Parent: " << parent <<  " Parent ID: " << parent->get_id() <<  std::endl;
    }
    indent++;
    visited.insert(this);
    if(get_total_children() != 0){
        for(int i=0; i< get_total_children(); ++i){
            if(visited.count(children[i].get())){
                throw std::invalid_argument("Cycle detected in print. Check add_child arguments in Group (or CSG)");
            }
            children[i]->print(indent);
        }
    }
}

Tuple Shape::normal_at(const Tuple& world_pt, const Impact& impt) const{
    Tuple local_point = world_inverse*world_pt;
    local_point.set_type(TupType::POINT);
    Tuple local_normal  = local_normal_at(local_point,impt);
    Tuple output = world_inverse.Transpose()*local_normal;
    output.set_type(TupType::VECTOR);
    output.normalize();
    return output;
}

std::vector<Impact> Shape::intersect(const Ray &other) const{
    Ray TransformedRay = other.transform(world_inverse);
    return local_intersect(TransformedRay);
}

int Shape::get_total_children() const{
    return this->children.size();
}

bool Shape::includes(const Shape* test) const{
    if(this==test){
        return true;
    }
    for(auto const& child: children){
        if(child->includes(test)){
            return true;
        }
    }
    return false;
}

std::ostream& operator << (std::ostream &out, const Shape* other){
    if(other==nullptr){
        out << "No Shape" << std::endl;
        return out;
    }
    out << other->get_name() << " " << other->get_id() << std::endl;
    return out;
}
