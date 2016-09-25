#ifndef COSExec_H
#define COSExec_H

#include <sys/types.h>
#include <string>

struct COSExecData {
  int pipes[2];
  int save_stdout;
  int save_stderr;

  COSExecData() :
   save_stdout(0), save_stderr() {
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

  bool checkGrabbedOutput(uint msecs);
  void readGrabbedOutput(std::string &str);

  void grabStdout  (COSExecData *exec_data);
  void ungrabStdout(COSExecData *exec_data);
  void grabStderr  (COSExecData *exec_data);
  void ungrabStderr(COSExecData *exec_data);
  void openPipes   (COSExecData *exec_data);
  void closePipes  (COSExecData *exec_data);
}

#endif
