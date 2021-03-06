#ifndef EVENTINPUTQUEUE_HPP
#define EVENTINPUTQUEUE_HPP

#include "CallBackWrapper.hpp"
#include "FromEvent.hpp"
#include "IntervalChecker.hpp"
#include "KeyCode.hpp"
#include "List.hpp"
#include "ParamsUnion.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFunc {
    class SimultaneousButtonPresses;
    class SimultaneousKeyPresses;
  }

  class EventInputQueue {
    friend class RemapFunc::SimultaneousButtonPresses;
    friend class RemapFunc::SimultaneousKeyPresses;

  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);

    static uint64_t currentSerialNumber(void) { return serialNumber_; }

    // ------------------------------------------------------------
    static void push_KeyboardEventCallback(OSObject* target,
                                           unsigned int eventType,
                                           unsigned int flags,
                                           unsigned int key,
                                           unsigned int charCode,
                                           unsigned int charSet,
                                           unsigned int origCharCode,
                                           unsigned int origCharSet,
                                           unsigned int keyboardType,
                                           bool repeat,
                                           AbsoluteTime ts,
                                           OSObject* sender,
                                           void* refcon);

    static void push_UpdateEventFlagsCallback(OSObject* target,
                                              unsigned flags,
                                              OSObject* sender,
                                              void* refcon);

    static void push_KeyboardSpecialEventCallback(OSObject* target,
                                                  unsigned int eventType,
                                                  unsigned int flags,
                                                  unsigned int key,
                                                  unsigned int flavor,
                                                  UInt64 guid,
                                                  bool repeat,
                                                  AbsoluteTime ts,
                                                  OSObject* sender,
                                                  void* refcon);

    static void push_RelativePointerEventCallback(OSObject* target,
                                                  int buttons,
                                                  int dx,
                                                  int dy,
                                                  AbsoluteTime ts,
                                                  OSObject* sender,
                                                  void* refcon);

    static void push_ScrollWheelEventCallback(OSObject* target,
                                              short deltaAxis1,
                                              short deltaAxis2,
                                              short deltaAxis3,
                                              IOFixed fixedDelta1,
                                              IOFixed fixedDelta2,
                                              IOFixed fixedDelta3,
                                              SInt32 pointDelta1,
                                              SInt32 pointDelta2,
                                              SInt32 pointDelta3,
                                              SInt32 options,
                                              AbsoluteTime ts,
                                              OSObject* sender,
                                              void* refcon);

    // ------------------------------------------------------------
    class Item : public List::Item {
    public:
      Item(const Params_KeyboardEventCallBack& p,        bool r, const DeviceIdentifier& di, uint32_t d) :
        params(p), retainFlagStatusTemporaryCount(r), deviceIdentifier(di), delayMS(d), enqueuedFrom(ENQUEUED_FROM_HARDWARE) {}
      Item(const Params_KeyboardSpecialEventCallback& p, bool r, const DeviceIdentifier& di, uint32_t d) :
        params(p), retainFlagStatusTemporaryCount(r), deviceIdentifier(di), delayMS(d), enqueuedFrom(ENQUEUED_FROM_HARDWARE) {}
      Item(const Params_RelativePointerEventCallback& p, bool r, const DeviceIdentifier& di, uint32_t d) :
        params(p), retainFlagStatusTemporaryCount(r), deviceIdentifier(di), delayMS(d), enqueuedFrom(ENQUEUED_FROM_HARDWARE) {}
      Item(const Params_ScrollWheelEventCallback& p,     bool r, const DeviceIdentifier& di, uint32_t d) :
        params(p), retainFlagStatusTemporaryCount(r), deviceIdentifier(di), delayMS(d), enqueuedFrom(ENQUEUED_FROM_HARDWARE) {}
      virtual ~Item(void) {}

      ParamsUnion params;
      bool retainFlagStatusTemporaryCount;
      DeviceIdentifier deviceIdentifier;

      uint32_t delayMS;

      // To avoid recursive enqueueing from blockedQueue_.
      enum EnqueuedFrom {
        ENQUEUED_FROM_HARDWARE,
        ENQUEUED_FROM_BLOCKEDQUEUE,
      } enqueuedFrom;
    };

  private:
    enum DelayType {
      DELAY_TYPE_KEY,
      DELAY_TYPE_POINTING_BUTTON,
    };

    class BlockUntilKeyUpHander {
    public:
      static void initialize(IOWorkLoop& workloop);
      static void terminate(void);

      // Return true if you need to call doFire.
      static bool doBlockUntilKeyUp(void);

    private:
      static bool isTargetDownEventInBlockedQueue(const Item& front);
      static void endBlocking(void);
      static void setIgnoreToAllPressingEvents(void);
      static void blockingTimeOut_timer_callback(OSObject* owner, IOTimerEventSource* sender);

      class PressingEvent : public List::Item {
      public:
        PressingEvent(const ParamsUnion& paramsUnion) :
          paramsUnion_(paramsUnion),
          fromEvent_(paramsUnion),
          ignore_(false) {}
        virtual ~PressingEvent(void) {}

        const ParamsUnion& getParamsUnion(void) const { return paramsUnion_; }
        const FromEvent& getFromEvent(void) const { return fromEvent_; }

        bool ignore(void) const { return ignore_; }
        void setIgnore(void) { ignore_ = true; }

      private:
        ParamsUnion paramsUnion_;
        FromEvent fromEvent_;
        bool ignore_;
      };

      static List* blockedQueue_;
      static List* pressingEvents_;
      static TimerWrapper blockingTimeOut_timer_;
    };

    static uint32_t calcdelay(DelayType type);

    // ------------------------------------------------------------
    static void enqueue_(const Params_KeyboardEventCallBack& p,
                         bool retainFlagStatusTemporaryCount, const DeviceIdentifier& di, bool push_back);
    static void enqueue_(const Params_KeyboardSpecialEventCallback& p,
                         bool retainFlagStatusTemporaryCount, const DeviceIdentifier& di);
    static void enqueue_(const Params_RelativePointerEventCallback& p,
                         bool retainFlagStatusTemporaryCount, const DeviceIdentifier& di);
    static void enqueue_(const Params_ScrollWheelEventCallback& p,
                         bool retainFlagStatusTemporaryCount, const DeviceIdentifier& di);
    static void fire_timer_callback(OSObject* owner, IOTimerEventSource* sender);
    static void doFire(void);
    static void setTimer(void);

    static List* queue_;
    static IntervalChecker ic_;
    static TimerWrapper fire_timer_;
    // Increment at fire_timer_callback.
    static uint64_t serialNumber_;
  };
}

#endif
