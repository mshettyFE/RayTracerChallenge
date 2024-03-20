#include "Shape.h"
#include "Matrix.h"
#include <set>
#include <stdexcept>

Shape::Shape(const Matrix& Transformation, const Material& material, const Shape* parent, std::string name){
    if (Transformation.get_dim() != 4){
        throw std::invalid_argument("Invalid Transformation Matrix");
    }
    mat = material;
    this->parent = parent;
    id = GlobalID;
    GlobalID += 1;
    this->name = name;
    this->Transformation = Transformation;
}

void Shape::set_transform(Matrix a_Tranformation){
    this->Transformation = a_Tranformation;
}

Matrix Shape::get_transform() const{
    return Transformation;
}

Matrix Shape::get_aggregate_transform(bool verbose) const{
    std::set<const Shape *> visited;
    return get_aggregate_transform(visited, verbose);
}


Matrix Shape::get_aggregate_transform(std::set<const Shape*>& visited , bool verbose, int count) const{
    Matrix output;
    if(verbose){
        if(get_parent() == nullptr){
            std::cout << "Level: " << count << " Current: " << this->get_id() <<   " Parent: NULL" << std::endl;
        }
        else{
            std::cout << "Level: " << count << " Current: " << this->get_id() <<  " Parent: " << this->get_parent()->get_id() << std::endl;
        }
    }
    if(visited.count(this)){
        throw std::invalid_argument("Cycle detected in when caculating aggregate transformation. Check add_child arguments in Group (or CSG)");
    }
    visited.insert(this);
    count++;
    if(get_parent() == nullptr){
        output = Transformation;
    }
    else{
        Matrix parent_matrix = get_parent()->get_aggregate_transform(visited,verbose,count);
        output = parent_matrix*this->Transformation;
    }
    return output;
}

void Shape::set_material(Material new_mat){
    this->mat = new_mat;
}

Material Shape::get_material() const{return mat;}

const Shape* Shape::get_parent() const{
    return this->parent;
}

void Shape::set_parent(const  Shape* new_parent){
    this->parent = new_parent;
    for(int index=0; index<get_total_children(); ++index){
        this->children[index]->set_parent(this);
    }
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
    Matrix agg_trans_inv = this->get_aggregate_transform().Inverse();
    Tuple local_point = agg_trans_inv*world_pt;
    local_point.set_type(TupType::POINT);
    Tuple local_normal  = local_normal_at(local_point,impt);
    Tuple output = agg_trans_inv.Transpose()*local_normal;
    output.set_type(TupType::VECTOR);
    output.normalize();
    return output;
}

std::vector<Impact> Shape::intersect(const Ray &other) const{
    Ray TransformedRay = other.transform(get_transform().Inverse());
    return local_intersect(TransformedRay);
}

int Shape::get_total_children() const{
    return this->children.size();
}

const std::vector<std::unique_ptr<Shape>>& Shape::get_children() const{
    return children;
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
    out << other->get_name() << " " << other->get_id() << std::endl;
    return out;
}
