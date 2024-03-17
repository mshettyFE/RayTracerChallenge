#Compiling

```
mkdir build
cd build
cmake ../RayTracer/ -DTESTING=True
make
```
 
# Running
## Without generating images
./Tracer --gtest_filter=-GenImage.*
## Generating images
./Tracer --gtest_filter=GenImage.*
