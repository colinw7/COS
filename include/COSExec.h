#ifndef COSExec_H
#define COSExec_H

#include <sys/types.h>
#include <string>

struct COSExecData {
  int pipes[2];
  int save_stdout { -1 };
  int save_stderr { -1 };

  COSExecData() {
    init();
  }

  void init() {
    pipes[0]    = -1;
    pipes[1]    = -1;
    save_stdout = -1;
    save_stderr = -1;
  }
};

namespace COSExec {
  void grabOutput  (bool std_out=true, bool std_err=true);
  void ungrabOutput();

  bool checkGrabbedOutput(uint msecs=10);

  void readGrabbedOutput(std::string &str, uint msecs=10);
  void readGrabbedOutput(std::string &ostr, std::string &estr, uint msecs=10);

  void grabStdout  (COSExecData *exec_data);
  void ungrabStdout(COSExecData *exec_data);
  void grabStderr  (COSExecData *exec_data);
  void ungrabStderr(COSExecData *exec_data);
  void openPipes   (COSExecData *exec_data);
  void closePipes  (COSExecData *exec_data);

  int getSaveStdOut();
  int getSaveStdErr();
}

#endif
