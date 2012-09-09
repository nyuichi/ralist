#include "ralist.h"

#include <stdlib.h>
#include <iostream>
#include <vector>


int main() {
  using namespace std;

  const size_t SIZE = 10000;
  const size_t COUNT = 100000;

  srand((unsigned int)time(NULL));

  clock_t start, end;

  /***************
   * std::vector *
   ***************/

  vector<int> vec(SIZE);

  // init
  for (size_t i = 0; i < SIZE; ++i) {
    vec[i] = i;
  }

  // go!
  start = clock();
  for (size_t i = 0; i < COUNT; ++i) {
    size_t j = rand() % SIZE;
    size_t k = rand() % SIZE;
    int v1 = vec[j];
    int v2 = vec[k];
    vec[j] = v2;
    vec[k] = v1;
  }
  end = clock();

  printf("vector: %f\n", (double)(end-start) / CLOCKS_PER_SEC);

  /********************
   * RandomAccessList *
   ********************/

  const RandomAccessList<int> *ralist = new RandomAccessList<int>();

  // init
  for (size_t i = 0; i < SIZE; ++i) {
    ralist = ralist->cons(i);
  }

  // go!
  start = clock();
  for (size_t i = 0; i < COUNT; ++i) {
    size_t j = rand() % SIZE;
    size_t k = rand() % SIZE;
    int v1 = ralist->lookup(j);
    int v2 = ralist->lookup(k);
    ralist = ralist->update(j, v2);
    ralist = ralist->update(k, v1);
  }
  end = clock();

  printf("ralist: %f\n", (double)(end-start) / CLOCKS_PER_SEC);
}
