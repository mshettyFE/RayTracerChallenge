// Checks World's BVH against brute-force intersection of every leaf shape.
#include <gtest/gtest.h>
#include <random>
#include <algorithm>
#include "World.h"
#include "Sphere.h"
#include "Cube.h"
#include "Plane.h"
#include "Group.h"
#include "CSG.h"

static void leaves(const Shape* s, std::vector<const Shape*>& out){
    if(s->get_name()=="Group"){ for(int i=0;i<s->get_total_children();++i) leaves(s->get_child(i), out); }
    else out.push_back(s);
}
static std::vector<double> brute(const World& w, const Ray& r){
    std::vector<const Shape*> ls; for(int i=0;i<w.number_of_shapes();++i) leaves(w.get_shape(i), ls);
    std::vector<double> ts; for(auto s: ls) for(auto h: s->intersect(r)) ts.push_back(h.get_t());
    std::sort(ts.begin(), ts.end()); return ts;
}
static int compare(World& w, int n, unsigned seed=1){
    w.build_bvh();
    std::mt19937 g(seed); std::uniform_real_distribution<double> u(-1,1);
    int bad=0;
    for(int i=0;i<n;++i){
        Tuple o({u(g)*3, u(g)*3+2, -8}, TupType::POINT);
        Tuple d({u(g)*0.6, u(g)*0.6, 1}); d.normalize();
        Ray r(o,d);
        auto ref = brute(w,r);
        std::vector<double> got; for(auto h: w.intersect(r)) got.push_back(h.get_t());
        bool ok = ref.size()==got.size();
        for(size_t k=0; ok && k<ref.size(); ++k) ok = std::abs(ref[k]-got[k])<1e-6;
        if(!ok){ if(bad<3) std::cout<<"mismatch ref="<<ref.size()<<" got="<<got.size()<<"\n"; ++bad; }
    }
    return bad;
}

TEST(BVHCheck, TopLevelSpheres){
    World w; std::mt19937 g(7); std::uniform_real_distribution<double> u(-3,3);
    for(int i=0;i<60;++i) w.add_shape(std::make_unique<Sphere>(MatTranslation(u(g),u(g)+2,u(g))*MatScaling(0.3,0.3,0.3)));
    EXPECT_EQ(compare(w,300),0);
}
// Groups with non-identity transforms, including a nested one. This failed before the fix:
// Group::bound() returned group-local child boxes that the world BVH treated as world-space.
TEST(BVHCheck, GroupTransformed){
    World w; auto outer = std::make_unique<Group>(MatTranslation(0.5,0.5,0)*MatRotateY(0.4)*MatScaling(0.7,0.7,0.7));
    std::mt19937 g(3); std::uniform_real_distribution<double> u(-3,3);
    for(int i=0;i<40;++i) outer->add_child(std::make_unique<Sphere>(MatTranslation(u(g),u(g)+2,u(g))*MatScaling(0.3,0.3,0.3)));
    auto inner = std::make_unique<Group>(MatTranslation(0.4,-0.3,0)*MatScaling(0.8,1.2,0.9));
    for(int i=0;i<20;++i) inner->add_child(std::make_unique<Cube>(MatTranslation(u(g),u(g)+2,u(g))*MatScaling(0.25,0.25,0.25)));
    outer->add_child(std::move(inner));
    w.add_shape(std::move(outer));
    EXPECT_EQ(compare(w,300),0);
}
TEST(BVHCheck, PlaneInsideGroup){
    World w; auto grp = std::make_unique<Group>(MatRotateZ(0.3));
    grp->add_child(std::make_unique<Plane>(MatRotateX(pi/2)*MatTranslation(0,4,0)));
    grp->add_child(std::make_unique<Sphere>(MatTranslation(0,2,0)));
    w.add_shape(std::move(grp));
    EXPECT_EQ(compare(w,300),0);
}
TEST(BVHCheck, TransformedCSG){
    World w;
    auto csg = std::make_unique<CSG>(DIFFERENCE, std::make_unique<Cube>(), std::make_unique<Sphere>(MatScaling(1.3,1.3,1.3)),
                                     MatTranslation(0,2,0)*MatRotateY(0.5));
    w.add_shape(std::move(csg));
    w.build_bvh();
    std::mt19937 g(9); std::uniform_real_distribution<double> u(-1,1);
    int bad=0;
    for(int i=0;i<300;++i){
        Ray r(Tuple({u(g)*2,u(g)*2+2,-8},TupType::POINT), Tuple({u(g)*0.2,u(g)*0.2,1}));
        // reference: CSG intersected directly (no BVH)
        auto ref = w.get_shape(0)->intersect(r); auto got = w.intersect(r);
        if(ref.size()!=got.size()) ++bad;
    }
    EXPECT_EQ(bad,0);
}
TEST(BVHCheck, GroupIntersectDirect){
    auto grp = std::make_unique<Group>(MatScaling(2,2,2));
    grp->add_child(std::make_unique<Sphere>(MatTranslation(5,0,0)));
    Ray r(Tuple({10,0,-10},TupType::POINT), Tuple({0,0,1}));
    EXPECT_EQ(grp->intersect(r).size(), 2u);   // Ray Tracer Challenge "intersecting a transformed group"
}
