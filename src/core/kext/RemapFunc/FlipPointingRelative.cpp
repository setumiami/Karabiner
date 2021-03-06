#include <IOKit/IOLib.h>

#include "EventOutputQueue.hpp"
#include "FlipPointingRelative.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFunc {
    FlipPointingRelative::FlipPointingRelative(void) : flipHorizontal_(false), flipVertical_(false)
    {}

    FlipPointingRelative::~FlipPointingRelative(void)
    {}

    void
    FlipPointingRelative::add(AddDataType datatype, AddValue newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_OPTION:
        {
          Option option(newval);
          /*  */ if (Option::FLIPPOINTINGRELATIVE_HORIZONTAL == option) {
            flipHorizontal_ = true;
          } else if (Option::FLIPPOINTINGRELATIVE_VERTICAL == option) {
            flipVertical_ = true;
          } else {
            IOLOG_ERROR("FlipPointingRelative::add unknown option:%u\n", static_cast<unsigned int>(newval));
          }
          break;
        }

        default:
          IOLOG_ERROR("FlipPointingRelative::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
          break;
      }
    }

    bool
    FlipPointingRelative::remap(RemapParams& remapParams)
    {
      Params_RelativePointerEventCallback* params = remapParams.paramsUnion.get_Params_RelativePointerEventCallback();
      if (! params) return false;

      if (remapParams.isremapped) return false;
      if (params->dx == 0 && params->dy == 0) return false;
      remapParams.isremapped = true;

      int dx = params->dx;
      int dy = params->dy;

      if (flipHorizontal_) {
        dx = -dx;
      }
      if (flipVertical_) {
        dy = -dy;
      }

      EventOutputQueue::FireRelativePointer::fire(ButtonStatus::makeButtons(), dx, dy);

      return true;
    }
  }
}
