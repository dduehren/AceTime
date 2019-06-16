#ifndef ACE_TIME_TESTABLE_SYSTEM_TIME_KEEPER_H
#define ACE_TIME_TESTABLE_SYSTEM_TIME_KEEPER_H

#include <stdint.h>
#include "../provider/SystemTimeKeeper.h"

namespace ace_time {
namespace testing {

/**
 * A version of SystemTimeKeeper that allows the millis() function to be
 * manually set for testing purposes.
 */
class TestableSystemTimeKeeper: public provider::SystemTimeKeeper {
  public:
    explicit TestableSystemTimeKeeper(
            TimeProvider* syncTimeProvider /* nullable */,
            TimeKeeper* backupTimeKeeper /* nullable */,
            FakeMillis* fakeMillis):
        SystemTimeKeeper(syncTimeProvider, backupTimeKeeper),
        mFakeMillis(fakeMillis) {}

    unsigned long millis() const override {
      return mFakeMillis->millis();
    }

  private:
    FakeMillis* mFakeMillis;
};

}
}

#endif
