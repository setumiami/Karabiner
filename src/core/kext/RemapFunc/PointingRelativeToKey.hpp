#ifndef POINTINGRELATIVETOKEY_HPP
#define POINTINGRELATIVETOKEY_HPP

#include "DeltaBuffer.hpp"
#include "IntervalChecker.hpp"
#include "KeyToKey.hpp"
#include "RemapFuncClasses.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFunc {
    class PointingRelativeToKey {
    public:
      PointingRelativeToKey(void);
      ~PointingRelativeToKey(void);

      bool remap(RemapParams& remapParams);

      void add(AddDataType datatype, AddValue newval);

    private:
      PointingRelativeToKey(const PointingRelativeToKey& rhs); // Prevent copy-construction
      PointingRelativeToKey& operator=(const PointingRelativeToKey& rhs); // Prevent assignment

      Vector_ModifierFlag fromModifierFlags_;

      DeltaBuffer deltaBuffer_dx_;
      DeltaBuffer deltaBuffer_dy_;
      IntervalChecker keyrepeat_ic_;

      enum {
        KEYTOKEY_UP,
        KEYTOKEY_DOWN,
        KEYTOKEY_LEFT,
        KEYTOKEY_RIGHT,
        KEYTOKEY__END__,
      };
      KeyToKey keytokey_[KEYTOKEY__END__];
      KeyToKey* current_keytokey_;
    };
  }
}

#endif
