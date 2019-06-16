#ifndef ACE_TIME_SYSTEM_TIME_SYNC_LOOP_H
#define ACE_TIME_SYSTEM_TIME_SYNC_LOOP_H

#include <stdint.h>
#include "SystemTimeKeeper.h"

namespace ace_time {
namespace provider {

/**
 * A class that periodically that syncs the SystemTimeKeeper with its
 * syncTimeProvider.
 */
class SystemTimeSyncLoop {
  public:
    /**
     * Constructor.
     *
     * @param systemTimeKeeper the system time keeper to sync up
     * @param syncPeriodSeconds seconds between normal sync attempts
     *    (default 3600)
     * @param initialSyncPeriodSeconds seconds between sync attempts when
     *    the systemTimeKeeper is not initialized (default 5)
     * @param requestTimeoutMillis number of milliseconds before the request to
     *    syncTimeProvider times out
     */
    SystemTimeSyncLoop(SystemTimeKeeper& systemTimeKeeper,
          uint16_t syncPeriodSeconds = 3600,
          uint16_t initialSyncPeriodSeconds = 5,
          uint16_t requestTimeoutMillis = 1000):
      mSystemTimeKeeper(systemTimeKeeper),
      mSyncPeriodSeconds(syncPeriodSeconds),
      mInitialSyncPeriodSeconds(initialSyncPeriodSeconds),
      mRequestTimeoutMillis(requestTimeoutMillis),
      mCurrentSyncPeriodSeconds(initialSyncPeriodSeconds) {}

    /**
     * If AceRoutine coroutine infrastructure is not used, then call this from
     * the global loop() method.
     */
    void loop() {
      if (mSystemTimeKeeper.mSyncTimeProvider == nullptr) return;

      unsigned long nowMillis = millis();
      unsigned long timeSinceLastSync = nowMillis - mLastSyncMillis;

      if (timeSinceLastSync >= mCurrentSyncPeriodSeconds * 1000UL
          || mSystemTimeKeeper.getNow() == 0) {
        acetime_t nowSeconds = mSystemTimeKeeper.mSyncTimeProvider->getNow();

        if (nowSeconds == 0) {
          // retry with exponential backoff
          if (mCurrentSyncPeriodSeconds >= mSyncPeriodSeconds / 2) {
            mCurrentSyncPeriodSeconds = mSyncPeriodSeconds;
          } else {
            mCurrentSyncPeriodSeconds *= 2;
          }
        } else {
          mSystemTimeKeeper.sync(nowSeconds);
          mCurrentSyncPeriodSeconds = mSyncPeriodSeconds;
        }

        mLastSyncMillis = nowMillis;
      }
    }

    /**
     * Return the number of seconds since last sync. Mostly for
     * debugging purposes.
     */
    uint16_t getSecondsSinceLastSync() const {
      unsigned long elapsedMillis = millis() - mLastSyncMillis;
      return elapsedMillis / 1000;
    }

  private:
    SystemTimeKeeper& mSystemTimeKeeper;
    uint16_t const mSyncPeriodSeconds;
    uint16_t const mInitialSyncPeriodSeconds;
    uint16_t const mRequestTimeoutMillis;

    unsigned long mLastSyncMillis = 0; // should be the same type as millis()
    uint16_t mCurrentSyncPeriodSeconds;
};

}
}

#endif