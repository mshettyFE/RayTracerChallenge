#include "AABB.h"
#include "Shape.h"
#include <utility>
#include <set>
#include <algorithm>

AABB::AABB(std::initializer_list<double> min_bounds, std::initializer_list<double> max_bounds, const Shape* shp){
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
    this->set_shape(shp);
}

AABB::AABB(Tuple min_bounds, Tuple max_bounds, const Shape* shp){
    this->min_bounds[0] = min_bounds.get_x();
    this->min_bounds[1] = min_bounds.get_y();
    this->min_bounds[2] = min_bounds.get_z();
    this->max_bounds[0] = max_bounds.get_x();
    this->max_bounds[1] = max_bounds.get_y();
    this->max_bounds[2] = max_bounds.get_z();
    this->set_shape(shp);
}

bool AABB::operator==(const AABB& other) const{
    if(this->enclosed_shape!=other.enclosed_shape){return false;}
    if(other.center.size() != this->center.size()){return false;}
    for(unsigned int i=0; i< other.center.size(); ++i){
        if(other.center[i]->get_shape() != this->center[i]->get_shape() ){
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

void AABB::print(bool verbose) const{
    indented_print(this,0, verbose);
}

void AABB::indented_print(const AABB* cur_box, int indent, bool verbose) const{
    auto indentation = std::string(indent,'\t');
    std::cout << indentation << "Depth: " << indent << std::endl;
    std::cout << indentation << "Shape: " <<  cur_box->get_shape() << std::endl;
    if(verbose){
        std::cout << indentation << "Min Bounds: " << cur_box->get_min_x() << " " << cur_box->get_min_y() << " "<< cur_box->get_min_z()<< std::endl;
        std::cout << indentation << "Max Bounds: " << cur_box->get_max_x() << " " << cur_box->get_max_y()<< " "<< cur_box->get_max_z() << std::endl;
    }
    indent++;
    indentation = std::string(indent,'\t');
    for(auto const& box: cur_box->center){
        if(box != nullptr){
            std::cout << indentation << "Center " << std::endl;
            indented_print( box.get(), indent,verbose);
        }
    }
    if(cur_box->left!= nullptr){
        std::cout << indentation << "Left" << std::endl;
        indented_print(cur_box->left.get(), indent,verbose);
    }
    if(cur_box->right!=nullptr){
        std::cout << indentation << "Right" << std::endl;
        indented_print(cur_box->right.get(), indent,verbose);
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

bool AABB::contains(const AABB& new_box) const{
    return contains(new_box.get_min()) && contains(new_box.get_max());
}

void AABB::transform(Matrix mat) {
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
    new_box.set_shape(this->get_shape());
    this->set_min_x(new_box.get_min_x());
    this->set_min_y(new_box.get_min_y());
    this->set_min_z(new_box.get_min_z());
    this->set_max_x(new_box.get_max_x());
    this->set_max_y(new_box.get_max_y());
    this->set_max_z(new_box.get_max_z());
}

void AABB::expand_box(const AABB* new_box){
    add_point(new_box->get_min());
    add_point(new_box->get_max());
}

bool AABB::is_finite() const{
    for(int i=0; i<3; ++i){
        if(!std::isfinite(min_bounds[i]) || !std::isfinite(max_bounds[i])){return false;}
    }
    return true;
}

// Infinite boxes are left untransformed: transforming them computes 0*inf = NaN.
// Callers check is_finite() and treat such shapes as unbounded.
AABB parent_space_bounds(const Shape* shape){
    AABB s = shape->bound();
    if(s.is_finite()){s.transform(shape->get_transform());}
    return s;
}

AABB world_space_bounds(const Shape* shape){
    AABB s = shape->bound();
    if(s.is_finite()){s.transform(shape->get_world_transform());}
    return s;
}

bool AABB::intersect(const Ray &other) const{
    const Tuple& o = other.get_origin();
    const Tuple& d = other.get_direction();
    const double origin[3] = {o.get_x(), o.get_y(), o.get_z()};
    const double direction[3] = {d.get_x(), d.get_y(), d.get_z()};
    double largest_entry = NEG_INFTY;
    double smallest_exit = INFTY;
    for(int axis = 0; axis < 3; ++axis){
        if(std::abs(direction[axis]) < glob_resolution){
            // Parallel to this slab: the ray is either always inside it (no constraint) or
            // never (a miss). A ray lying exactly in a face's plane counts as inside.
            if(origin[axis] < min_bounds[axis] || origin[axis] > max_bounds[axis]){return false;}
            continue;
        }
        double t_enter = (min_bounds[axis] - origin[axis]) / direction[axis];
        double t_exit = (max_bounds[axis] - origin[axis]) / direction[axis];
        if(t_enter > t_exit){std::swap(t_enter, t_exit);}
        if(t_enter > largest_entry){largest_entry = t_enter;}
        if(t_exit < smallest_exit){smallest_exit = t_exit;}
        // The overlap only shrinks as axes are added, so once empty it stays empty.
        if(largest_entry > smallest_exit){return false;}
    }
    return true;
}

void AABB::collect_candidates(const Ray& r, std::vector<const Shape*>& candidates) const {
    if (!intersect(r)) return;

    if (enclosed_shape != nullptr)
        candidates.push_back(enclosed_shape);

    for (auto const& box : center)
        if (box) box->collect_candidates(r, candidates);

    if (left)  left->collect_candidates(r, candidates);
    if (right) right->collect_candidates(r, candidates);
}

std::ostream& operator << (std::ostream &out, const AABB& other){
    out << other.get_min() << std::endl;
    out << other.get_max() << std::endl;
    return out;
}

std::unique_ptr<AABB> AABB::build_bvh(std::vector<std::unique_ptr<AABB>> boxes){
    if(boxes.empty()){return nullptr;}
    return build_range(boxes, 0, boxes.size());
}

// Builds the node for boxes[begin, end). Only ever looks at those boxes, and reorders them in place.
std::unique_ptr<AABB> AABB::build_range(std::vector<std::unique_ptr<AABB>>& boxes, size_t begin, size_t end){
    const size_t count = end - begin;
    if(count == 1){return std::move(boxes[begin]);}   

    auto node = std::make_unique<AABB>();
    for(size_t i = begin; i < end; ++i){node->expand_box(boxes[i].get());}
    if(count <= MAX_LEAF_OBJECTS){
        for(size_t i = begin; i < end; ++i){node->center.push_back(std::move(boxes[i]));}
        return node;
    }

    // Split along the axis where the objects' centre points are most spread out. Centres, not
    // box extents, so a few huge objects can't skew the choice.
    auto centre = [](const AABB& b, int axis){return 0.5 * (b.min_bounds[axis] + b.max_bounds[axis]);};
    std::array<double,3> lo = {INFTY, INFTY, INFTY}, hi = {NEG_INFTY, NEG_INFTY, NEG_INFTY};
    for(size_t i = begin; i < end; ++i){
        for(int axis = 0; axis < 3; ++axis){
            lo[axis] = std::min(lo[axis], centre(*boxes[i], axis));
            hi[axis] = std::max(hi[axis], centre(*boxes[i], axis));
        }
    }
    int axis = 0;
    for(int a = 1; a < 3; ++a){
        if(hi[a] - lo[a] > hi[axis] - lo[axis]){axis = a;}
    }

    // Split by count at the median, not by space at the midpoint: both halves are always
    // non-empty, even if many centres coincide, so the recursion always terminates.
    // nth_element puts the median in place and partitions around it in linear time.
    const size_t mid = begin + count / 2;
    std::nth_element(boxes.begin() + begin, boxes.begin() + mid, boxes.begin() + end,
                     [&](const std::unique_ptr<AABB>& a, const std::unique_ptr<AABB>& b){
                         return centre(*a, axis) < centre(*b, axis);
                     });
    node->left = build_range(boxes, begin, mid);
    node->right = build_range(boxes, mid, end);
    return node;
}
