#include "Shape.h"
#include "Matrix.h"

unsigned long Shape::GlobalID = 0;

Shape::Shape(){
    Transformation = MatIdentity(4);
    id = ++GlobalID;
}
Shape::Shape(Matrix a_Transformation){
    Transformation = a_Transformation;
    id = ++GlobalID;
}