#include "Config.hpp"
#include "IOLogWrapper.hpp"
#include "KeyCode.hpp"
#include "NumHeldDownKeys.hpp"

namespace org_pqrs_Karabiner {
  int NumHeldDownKeys::num_ = 0;

  void
  NumHeldDownKeys::log(void)
  {
    IOLOG_DEVEL("NumHeldDownKeys = %d\n", num_);
  }

  void
  NumHeldDownKeys::set(int diff) {
    num_ += diff;
    log();
  }

  void
  NumHeldDownKeys::reset(void)
  {
    IOLOG_DEVEL("NumHeldDownKeys::reset\n");
    num_ = 0;
  }

  bool
  NumHeldDownKeys::iszero(void)
  {
    return num_ <= 0;
  }
}
