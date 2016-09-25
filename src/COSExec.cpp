#include <COSExec.h>
#include <COSRead.h>

#include <fcntl.h>
#include <unistd.h>

// pipe: write=0, read=1

static COSExecData cos_exec_data;

void
COSExec::
grabOutput(bool std_out, bool std_err)
{
  cos_exec_data.init();

  if (std_out) grabStdout(&cos_exec_data);
  if (std_err) grabStderr(&cos_exec_data);
}

void
COSExec::
ungrabOutput()
{
  ungrabStdout(&cos_exec_data);
  ungrabStderr(&cos_exec_data);
}

bool
COSExec::
checkGrabbedOutput(uint msecs)
{
  if      (cos_exec_data.pipes[0] != -1 && cos_exec_data.pipes[1] != -1) {
    int fds[2] = { cos_exec_data.pipes[0], cos_exec_data.pipes[1] };

    return COSRead::wait_read(fds, 2, 0, msecs);
  }
  else if (cos_exec_data.pipes[0] != -1) {
    int fds[1] = { cos_exec_data.pipes[0] };

    return COSRead::wait_read(fds, 1, 0, msecs);
  }
  else if (cos_exec_data.pipes[1] != -1) {
    int fds[2] = { cos_exec_data.pipes[1] };

    return COSRead::wait_read(fds, 1, 0, msecs);
  }
  else
    return false;
}

void
COSExec::
readGrabbedOutput(std::string &str)
{
  int  num_read;
  char buffer[512];

  str = "";

  buffer[0] = '\0';

  if (cos_exec_data.pipes[0] != -1) {
    if (COSRead::wait_read(&cos_exec_data.pipes[0], 1, 0, 10)) {
      while ((num_read = ::read(cos_exec_data.pipes[0], buffer, 511)) != -1) {
        buffer[num_read] = '\0';

        str += buffer;
      }
    }

    if (COSRead::wait_read(&cos_exec_data.pipes[1], 1, 0, 10)) {
      while ((num_read = ::read(cos_exec_data.pipes[1], buffer, 511)) != -1) {
        buffer[num_read] = '\0';

        str += buffer;
      }
    }
  }
}

void
COSExec::
grabStdout(COSExecData *exec_data)
{
  if (exec_data->pipes[1] == -1)
    openPipes(exec_data);

  exec_data->save_stdout = ::dup(1);

  ::dup2(exec_data->pipes[1], 1);

  int flags = ::fcntl(1, F_GETFL, 0);

  ::fcntl(1, F_SETFL, flags | O_NDELAY | O_NONBLOCK);
}

void
COSExec::
ungrabStdout(COSExecData *exec_data)
{
  if (exec_data->save_stdout != -1) {
    ::dup2(exec_data->save_stdout, 1);

    close(exec_data->save_stdout);
  }

  closePipes(exec_data);
}

void
COSExec::
grabStderr(COSExecData *exec_data)
{
  if (exec_data->pipes[1] == -1)
    openPipes(exec_data);

  exec_data->save_stderr = ::dup(2);

  ::dup2(exec_data->pipes[1], 2);

  int flags = fcntl(2, F_GETFL, 0);

  ::fcntl(2, F_SETFL, flags | O_NDELAY | O_NONBLOCK);
}

void
COSExec::
ungrabStderr(COSExecData *exec_data)
{
  if (exec_data->save_stderr != -1) {
    ::dup2(exec_data->save_stderr, 2);

    ::close(exec_data->save_stderr);
  }

  closePipes(exec_data);
}

void
COSExec::
openPipes(COSExecData *exec_data)
{
  ::pipe(exec_data->pipes);

  int flags = ::fcntl(exec_data->pipes[0], F_GETFL, 0);

  ::fcntl(exec_data->pipes[0], F_SETFL, flags | O_NONBLOCK);
}

void
COSExec::
closePipes(COSExecData *exec_data)
{
  if (exec_data->pipes[0] != -1)
    ::close(exec_data->pipes[0]);

  if (exec_data->pipes[1] != -1)
    ::close(exec_data->pipes[1]);

  exec_data->pipes[0] = -1;
  exec_data->pipes[1] = -1;
}
