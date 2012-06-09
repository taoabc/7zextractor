/*
** Êı×Ö¼ÆËã
** author
**   taoabc@gmail.com
*/
#ifndef ULT_NUMBER_H_
#define ULT_NUMBER_H_

#include <ctime>
#include <vector>

namespace ult {
namespace number {

inline void SwapInteger(int* x, int *y) {
  if (*x != *y) {
    *x = *x ^ *y;
    *y = *x ^ *y;
    *x = *x ^ *y;
  }
}

inline int GetRandomInteger(int min_number, int max_number) {
  static bool seeded = false;
  if (!seeded) {
    srand((unsigned int)time(NULL));
    seeded = true;
  }
  if (min_number > max_number) {
    SwapInteger(&min_number, &max_number);
  }
  return (int)((double)rand() / RAND_MAX * (max_number - min_number) + min_number);
}

inline void DisorderInteger(int begin_number, int end_number, std::vector<int>* vec) {
  if (begin_number > end_number) {
    SwapInteger(&begin_number, &end_number);
  }
  for (int i = begin_number; i <= end_number; ++i) {
    vec->push_back(i);
  }

  int vec_size = vec->size();
  int r;
  for (int i = 0; i < vec_size; ++i) {
    r = GetRandomInteger(0, vec_size - 1);
    SwapInteger(&(*vec)[i], &(*vec)[r]);
  }
}

inline void DecreaseVals(unsigned __int64* numberator, unsigned __int64* denominator) {
  static const unsigned __int64 kMax = (unsigned __int64)1 << 31;
  while (*numberator > kMax) {
    *numberator >>= 1;
    *denominator >>= 2;
  }
}

//to calculate numberator1*numberator2 / denominator
inline unsigned __int64 UIntMultDiv(unsigned __int64 numberator1,
                                    unsigned __int64 numberator2,
                                    unsigned __int64 denominator) {
  DecreaseVals(&numberator1, &denominator);
  DecreaseVals(&numberator2, &denominator);
  if (denominator == 0) {
    denominator = 1;
  }
  return numberator1 * numberator2 / denominator;
}

} //namespace number

using namespace number;

} //namespace ult

#endif