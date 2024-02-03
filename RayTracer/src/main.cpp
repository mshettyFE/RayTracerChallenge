#include <iostream>
#include "Tuple.h"

int main(){
  Tuple a = Tuple{1,2,3};
  Tuple b = Tuple{10,20,30};
  Tuple c = a;
  Tuple d = b;
  d -= a;
  c += a;

  double temp = 2.0;

  std::cout << c << std::endl;
  std::cout << c*temp << std::endl;
  a.normalize();
  std::cout << a << std::endl;
  std::cout << d-a << std::endl;
  return 0;
}
