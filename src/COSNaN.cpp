#include <COSNaN.h>
#include <NaN.h>

#include <std_os.h>

bool
COSNaN::
has_nan()
{
#ifdef OS_UNIX
  return true;
#else
  return false;
#endif
}

bool
COSNaN::
is_nan(double real)
{
#ifdef OS_UNIX
  if (IsNaN(real))
    return true;
#endif

  return false;
}

bool
COSNaN::
set_nan(double *real)
{
#ifdef OS_UNIX
  SetNaN(*real);

  return true;
#else
  return false;
#endif
}

bool
COSNaN::
set_nan(double &real)
{
#ifdef OS_UNIX
  SetNaN(real);

  return true;
#else
  return false;
#endif
}

//------

bool
COSNaN::
is_inf(double real)
{
#ifdef OS_UNIX
  if (IsInf(real))
    return true;
#endif

  return false;
}

bool
COSNaN::
is_pos_inf(double real)
{
#ifdef OS_UNIX
  if (IsPosInf(real))
    return true;
#endif

  return false;
}

bool
COSNaN::
is_neg_inf(double real)
{
#ifdef OS_UNIX
  if (IsNegInf(real))
    return true;
#endif

  return false;
}

bool
COSNaN::
set_pos_inf(double &real)
{
#ifdef OS_UNIX
  SetPosInf(real);

  return true;
#else
  return false;
#endif
}

bool
COSNaN::
set_neg_inf(double &real)
{
#ifdef OS_UNIX
  SetNegInf(real);

  return true;
#else
  return false;
#endif
}
