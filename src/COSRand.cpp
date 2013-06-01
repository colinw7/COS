#include <COSRand.h>

#include <time.h>
#include <stdlib.h>
#include <algorithm>

void
COSRand::
srand()
{
  srand48(time(NULL));
}

void
COSRand::
srand(int seed)
{
  srand48(seed);
}

void
COSRand::
rand(int *r)
{
  *r = rand();
}

int
COSRand::
rand()
{
  return lrand48();
}

int
COSRand::
randIn(int min_val, int max_val)
{
  int number = (abs(rand()) % (max_val - min_val + 1)) + min_val;

  return std::min(std::max(number, min_val), max_val);
}

double
COSRand::
randIn(double min_val, double max_val)
{
  return (max_val - min_val)*((1.0*rand())/RAND_MAX) + min_val;
}

bool
COSRand::
randBool()
{
  static const int RND_TOL = (RAND_MAX >> 1);

  int r = rand();

  return (r > RND_TOL);
}

bool
COSRand::
randBool(double weight)
{
  double r = randIn(0.0, 1.0);

  return (r <= weight);
}
