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
