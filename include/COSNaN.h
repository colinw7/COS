#ifndef COSNaN_H
#define COSNaN_H

namespace COSNaN {
  bool has_nan();
  bool is_nan(double real);
  bool set_nan(double *real);
  bool set_nan(double &real);
}

#endif
