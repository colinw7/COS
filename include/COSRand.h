#ifndef COSRand_H
#define COSRand_H

namespace COSRand {
  void   srand();
  void   srand(int seed);
  void   rand(int *r);
  int    rand();
  int    randIn(int min_val, int max_val);
  long   randIn(long min_val, long max_val);
  double randIn(double min_val, double max_val);
  bool   randBool();
  bool   randBool(double weight);
}

#endif
