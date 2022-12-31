# Tiny SmartPointer

An optimized reimplementation of C++ STL-version `smartpointer`, contains `shared_ptr` and `weak_ptr`. The implementation is mainly for educational purpose, thus its design principle is to recap modern C++ features as more as possible and keep simple project layout.

# Layout

```shell
memory/
├── Makefile        
├── README.md       (Doc)
├── shared_ptr.h    
├── test.cc         (Runtime Test)
├── u_ptr.h
└── weak_ptr.h

0 directories, 6 files
```

# Build and Test

You could modify `test.cc` to add more test features, and run them by typing following command in your `terminal` apps:

```shell
$ make test
```

**Tips:** Don't foget run `make clean` after you modify `test.cc`, and then run `make test` to test your code.


