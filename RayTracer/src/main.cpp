#include <iostream>
#include "Vector.h"

int main(){
  Vector a = Vector{1,2,3};
  Vector b = Vector{10,20,30};
  Vector c = a;
  Vector d = b;
  d -= a;
  c += a;

  double temp = 2.0;

  std::cout << c << std::endl;
  std::cout << c*temp << std::endl;
  std::cout << d-a << std::endl;
  return 0;
}
