#include "AABB.h"
#include "Shape.h"
#include <set>

AABB::AABB(std::initializer_list<double> min_bounds, std::initializer_list<double> max_bounds){
    if(min_bounds.size() != 3){throw std::invalid_argument("min_bounds must have a length of 3");}
    if(max_bounds.size() != 3){throw std::invalid_argument("max_bounds must have a length of 3");}
    std::vector<double> min_temp(min_bounds);
    std::vector<double> max_temp(max_bounds);
    this->min_bounds[0] = min_temp[0];
    this->min_bounds[1] = min_temp[1];
    this->min_bounds[2] = min_temp[2];
    this->max_bounds[0] = max_temp[0];
    this->max_bounds[1] = max_temp[1];
    this->max_bounds[2] = max_temp[2];
}

AABB::AABB(Tuple min_bounds, Tuple max_bounds){
    this->min_bounds[0] = min_bounds.get_x();
    this->min_bounds[1] = min_bounds.get_y();
    this->min_bounds[2] = min_bounds.get_z();
    this->max_bounds[0] = max_bounds.get_x();
    this->max_bounds[1] = max_bounds.get_y();
    this->max_bounds[2] = max_bounds.get_z();
}

bool AABB::operator==(const AABB& other) const{
    if(this->enclosed_shape!=other.enclosed_shape){return false;}
    if(other.center.size() != this->center.size()){return false;}
    for(int i=0; i< other.center.size(); ++i){
        if(other.center[i]->contained_shape() != this->center[i]->contained_shape() ){
            return false;
        }
    }
    if(std::abs(other.min_bounds[0]- min_bounds[0]) > glob_resolution){return false;}
    if(std::abs(other.min_bounds[1]- min_bounds[1]) > glob_resolution){return false;}
    if(std::abs(other.min_bounds[2]- min_bounds[2]) > glob_resolution){return false;}
    if(std::abs(other.max_bounds[0]- max_bounds[0]) > glob_resolution){return false;}
    if(std::abs(other.max_bounds[1]- max_bounds[1]) > glob_resolution){return false;}
    if(std::abs(other.max_bounds[2]- max_bounds[2]) > glob_resolution){return false;}
    return true;
}

bool AABB::operator!=(const AABB& other) const{
    return !(*this == other);
}

void AABB::print() const{
    std::set<const AABB*> visited;
    indented_print();
}

void AABB::indented_print(int indent) const{
    auto indentation = std::string(indent,'\t');
    std::cout << indentation << min_bounds[0] << " " << min_bounds[1] << " "<< min_bounds[2] << std::endl;
    std::cout << indentation << max_bounds[0] << " " << max_bounds[1] << " "<< max_bounds[2] << std::endl;
    std::cout << indentation << enclosed_shape << std::endl;
    for(auto const& box: center){
        if(box != nullptr){
            box->indented_print(indent);            
        }
    }
    indent++;
    if(left!= nullptr){
        left->indented_print(indent);
    }
    if(right!=nullptr){
        right->indented_print(indent);
    }
}

void AABB::set_min_x(double value) { min_bounds[0] = value;}
void AABB::set_min_y(double value) { min_bounds[1] = value;}
void AABB::set_min_z(double value) { min_bounds[2]= value;}
void AABB::set_max_x(double value) { max_bounds[0]= value;}
void AABB::set_max_y(double value) { max_bounds[1]= value;}
void AABB::set_max_z(double value) { max_bounds[2]= value;}


void AABB::add_point(const Tuple new_point){
    if(new_point.type() != TupType::POINT){
        throw std::invalid_argument("can't add vector to bounding box");
    }
    double x =  new_point.get_x();
    double y =  new_point.get_y();
    double z =  new_point.get_z();

    if(x <this->get_min_x()){set_min_x(x);}
    if(y <this->get_min_y()){set_min_y(y);}
    if(z <this->get_min_z()){set_min_z(z);}

    if(x >this->get_max_x()){set_max_x(x);}
    if(y >this->get_max_y()){set_max_y(y);}
    if(z >this->get_max_z()){set_max_z(z);}
}

bool AABB::contains(const Tuple& points) const{
    double x = points.get_x();
    double y = points.get_y();
    double z = points.get_z();
    if(x <this->get_min_x()){return false;}
    if(y <this->get_min_y()){return false;}
    if(z <this->get_min_z()){return false;}

    if(x >this->get_max_x()){return false;}
    if(y >this->get_max_y()){return false;}
    if(z >this->get_max_z()){return false;}
    return true;
}

Tuple AABB::get_min() const{    return std::move(GenPoint(get_min_x(),get_min_y(),get_min_z()));}
Tuple AABB::get_max() const{    return std::move(GenPoint(get_max_x(),get_max_y(),get_max_z()));}
Tuple AABB::get_mid() const{return 0.5*(GenVec(min_bounds[0],min_bounds[1],min_bounds[2])+GenVec(max_bounds[0],max_bounds[1],max_bounds[2]));}

bool AABB::contains(const AABB& new_box) const{
    return contains(new_box.get_min()) && contains(new_box.get_max());
}

std::unique_ptr<AABB> AABB::transform(Matrix mat) const{
    double xmin, xmax, ymin,ymax, zmin,zmax;
    xmin = min_bounds[0];
    ymin = min_bounds[1];
    zmin = min_bounds[2];
    xmax = max_bounds[0];
    ymax = max_bounds[1];
    zmax = max_bounds[2];
    std::vector<Tuple> points = {
        GenPoint(xmin,ymin, zmin),
        GenPoint(xmin,ymin, zmax),
        GenPoint(xmin,ymax, zmin),
        GenPoint(xmin,ymax, zmax),
        GenPoint(xmax,ymin, zmin),
        GenPoint(xmax,ymin, zmax),
        GenPoint(xmax,ymax, zmin),
        GenPoint(xmax,ymax, zmax)
    };
    AABB new_box;
    for(auto const& pt : points){
        new_box.add_point(mat*pt);
    }
    return std::make_unique<AABB>(std::move(new_box));
}

void AABB::expand_box(const AABB* new_box){
    add_point(new_box->get_min());
    add_point(new_box->get_max());
}

std::unique_ptr<AABB> parent_space_bounds(const Shape* shape){
    return shape->bound()->transform(shape->get_transform());
}

std::vector<double> AABB::check_axis(double origin, double direction, double min, double max) const{
    double tmin_num = (min-origin);
    double tmax_num = (max-origin);
    double tmin, tmax;
    if(std::abs(direction) >= glob_resolution){
        tmin = tmin_num/direction;
        tmax = tmax_num/direction;
    }
    else{
        tmin = tmin_num*INFTY;
        tmax = tmax_num*INFTY;
    }
    if(tmin>tmax){
        double temp = tmin;
        tmin = tmax;
        tmax = temp;
    }
    return {tmin,tmax};
}

bool AABB::intersect(const Ray &other) const{
    std::vector<double> minima;
    std::vector<double> maxima;
    auto bounds = check_axis(other.get_origin()[0],other.get_direction()[0], get_min_x(), get_max_x());
    minima.push_back(bounds[0]);
    maxima.push_back(bounds[1]);
    bounds = check_axis(other.get_origin()[1],other.get_direction()[1], get_min_y(), get_max_y());
    minima.push_back(bounds[0]);
    maxima.push_back(bounds[1]);
    bounds = check_axis(other.get_origin()[2],other.get_direction()[2], get_min_z(), get_max_z());
    minima.push_back(bounds[0]);
    maxima.push_back(bounds[1]);
    double largest_min = NEG_INFTY;
    for(auto x: minima){
        if(x>largest_min){
            largest_min = x;
        }    
    }
    double smallest_max = INFTY;
    for(auto x: maxima){
        if(x<smallest_max){
            smallest_max = x;
        }    
    }
    if(largest_min > smallest_max){
        return false;
    }
    return true;
}


bool AABB::is_leaf() const{
// AABB is leaf is no left pointer, no right pointer, no  center boxes, and enclosed_shape is not null
    return (left==nullptr) && (right==nullptr) && (center.size() ==0) && (enclosed_shape !=nullptr);
}

bool AABB::straddle(const AABB* new_box) const{
    auto middle = this->get_mid();
    double x = middle.get_x();
    double y = middle.get_y();
    double z = middle.get_z();
    bool straddle_x = (new_box->get_min_x()< x) && (x < new_box->get_max_x());
    bool straddle_y = (new_box->get_min_y()< y) && (y < new_box->get_max_y());
    bool straddle_z = (new_box->get_min_z()< z) && (z < new_box->get_max_z());
    return straddle_x&&straddle_y&&straddle_z ;
}

void AABB::split(){
    auto middle = this->get_mid();
    Tuple delta = this->get_max()-this->get_min();
    double current_max = NEG_INFTY;
    int index = -1;
    for(int i=0; i<3; ++i){
        if(delta[i] > current_max){
            current_max=  delta[i];
            index = i;
        }
    }
    Tuple left_max;
    Tuple right_min;
// To split box, divide, split along largest axis, and get the new min and max of left and right
// The largest axis coordinate gets replaced by the corresponding middle coordinate, while the others get replaced by the min/max
    if(index==0){
        left_max = GenPoint(middle.get_x(),this->get_max_y(), this->get_max_z());
        right_min = GenPoint(middle.get_x(), this->get_min_y(), this->get_min_z());
    }
    else if(index==1){
        left_max = GenPoint(this->get_max_x(),middle.get_y(), this->get_max_z());
        right_min = GenPoint(this->get_max_x(), middle.get_y(), this->get_min_z());
    }
    else if(index==2){
        left_max = GenPoint(this->get_max_x(),this->get_max_y(), middle.get_z());
        right_min = GenPoint(this->get_min_x(), this->get_min_y(), middle.get_z());
    }
    else{
        throw std::invalid_argument("Something went horribly wrong when splitting");
    }
    std::cout << index << "\n" << left_max << "\n" << right_min << std::endl;
// assign left and right bounding box
    this->left = std::make_unique<AABB>(this->get_min(), left_max);
    this->right = std::make_unique<AABB>(right_min, this->get_max());
}


bool AABB::insert(std::unique_ptr<AABB>& new_box, unsigned int depth){
// box needs to be big enough to hold new_box. If the box is not big enough, don't change the state of the current box
// for recursive calls, prevents too-big boxes from affecting the state of the current box
    if(!this->contains(*new_box.get())){
        return false;
    }
// If the box is small enough, and straddles  the center, you can't really assign it to either left or right
// Hence, run through vector of center boxes.
// If it does fit a box, then recurse on said box, and when stack eventually unwinds, return immediately
    if(straddle(new_box.get())){
        for(int i=0; i<center.size(); ++i){
            if(center[i]->contains(*new_box.get())){
                center[i]->insert(new_box);
                return true;
            }
        }
// If you got here, then the new_box doesn't fit in any of the present sub boxes. Add to vector, then return
        center.push_back(std::move(new_box));
        return true;
    }
    else{
// OK. The box doesn't go in the center. Check if both left and right are nullptr (having only one be null shouldn't happen with this setup?)
// If both  are nullptr, then split the current box in half, and recurse on both halves
        if(this->left==nullptr && this->right==nullptr){
            split();
// return if the new_box fits in a sub-box. No need to check the other
            if(this->left->insert(new_box)){
                return true;
            }
            if(this->right->insert(new_box)){
                return true;
            }
        }
    }
// assuming you got here, this means that the box doesn't fit in the center, left, or right groups.
// Also, due to the first contains call, we know the new_box fits in the current, so it must be in one of the groups
// What the above means is that something went horribly wrong for you to arrive here...
    throw std::invalid_argument("Something went horrible wrong in AABB::insert");
}

std::ostream& operator << (std::ostream &out, const AABB& other){
    out << other.get_min() << std::endl;
    out << other.get_max() << std::endl;
    return out;
}
