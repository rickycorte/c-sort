# C sort algorithms

Still working for more implementations :heart:

You can find the sort implementations in `src/sortlibs` folder


 Algorithm | Implemented | Implementation type
 :---: | :---: | :---:
 Bubble sort | Yes | Iterative
 Insertion sort | Yes | Iterative
 Merge sort | Yes | Iterative
 Heapsort | Yes | Recursive
 Quicksort | Yes | Recursive


## Build

### Linux: 

To build the benchmark program
```
cmake .
make
```

If you also want to run the benchmark after compiling
(`cmake .` must be run at least once before!)
```
make run
```

or 
```
make srun
```
to save benchmark output to `bin/output.txt`

If you want to generate a comparision table run:
```
make cmprun
```

### Other OS

This repo should be 100% cross platform and compilable everywhere, but I haven't tested the compatibility!


## Generate tests

To generate benchmark tests run 
```
python3 testgen.py
```
this script will generate a brand new `test_data.txt` file to use!

## Parameters

```
./sort -c
```
to generate a comparision table in html format!


```
./sort -v
```
to print every benchmark and test result on the console

It's not possibile to combile these two options!


