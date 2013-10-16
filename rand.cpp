#include <iostream>
#include <ctime>
using namespace std;
main() {
  srand((unsigned) time(0));
  cout << rand() % 100 << endl;
}
