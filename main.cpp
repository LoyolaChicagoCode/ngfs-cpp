// main.cpp
#include <ctime>

#include "ngfs.hpp"

int main (int argc, char **argv) {

  srand((unsigned) time(0));

  NumguessFS numguess;

  // The first 3 parameters are identical to the fuse_main function.
  // The last parameter gives a pointer to a class instance, which is
  // required for static methods to access instance variables/ methods.
  return numguess.main (argc, argv, NULL, &numguess);
}
