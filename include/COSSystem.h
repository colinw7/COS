#ifndef COSSystem_H
#define COSSystem_H

#include <string>
#include <sys/types.h>

namespace COSSystem {
  std::string getSystemName();
  std::string getSystemRelease();
  std::string getSystemMachine();

  uint getWordSize();
}

#endif
