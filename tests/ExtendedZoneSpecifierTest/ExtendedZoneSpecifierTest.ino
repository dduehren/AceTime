#line 2 "ExtendedZoneSpecifierTest.ino"

/*
 * Unit tests for ExtendedZoneSpecifier.
 */

#include <AUnit.h>
#include <AceTime.h>

using namespace aunit;
using namespace ace_time;
using namespace ace_time::zonedbx;

// --------------------------------------------------------------------------
// A simplified version of America/Los_Angeles, using only simple ZoneEras
// (i.e. no references to a ZonePolicy). Valid only for 2018.
// --------------------------------------------------------------------------

// Create simplified ZoneEras which approximate America/Los_Angeles
static const ZoneEra kZoneEraAlmostLosAngeles[] = {
  {
    -32 /*offsetCode*/,
    nullptr,
    0 /*deltaCode*/,
    "PST" /*format*/,
    19 /*untilYearTiny*/,
    3 /*untilMonth*/,
    10 /*untilDay*/,
    2*4 /*untilTimeCode*/,
    'w' /*untilTimeModifier*/
  },
  {
    -32 /*offsetCode*/,
    nullptr,
    4 /*deltaCode*/,
    "PDT" /*format*/,
    19 /*untilYearTiny*/,
    11 /*untilMonth*/,
    3 /*untilDay*/,
    2*4 /*untilTimeCode*/,
    'w' /*untilTimeModifier*/
  },
  {
    -32 /*offsetCode*/,
    nullptr,
    0 /*deltaCode*/,
    "PST" /*format*/,
    20 /*untilYearTiny*/,
    3 /*untilMonth*/,
    8 /*untilDay*/,
    2*4 /*untilTimeCode*/,
    'w' /*untilTimeModifier*/
  },
};

static const ZoneInfo kZoneAlmostLosAngeles = {
  "Almost_Los_Angeles" /*name*/,
  3 /*numEras*/,
  kZoneEraAlmostLosAngeles /*eras*/,
};

// --------------------------------------------------------------------------
// A real ZoneInfo for America/Los_Angeles. Taken from zonedbx/zone_infos.cpp.
// --------------------------------------------------------------------------

static const ZoneRule kZoneRulesTestUS[] = {
  // Rule    US    1967    2006    -    Oct    lastSun    2:00    0    S
  {
    -33 /*fromYearTiny*/,
    6 /*toYearTiny*/,
    10 /*inMonth*/,
    7 /*onDayOfWeek*/,
    0 /*onDayOfMonth*/,
    8 /*atTimeCode*/,
    'w' /*atTimeModifier*/,
    0 /*deltaCode*/,
    'S' /*letter*/,
  },
  // Rule    US    1976    1986    -    Apr    lastSun    2:00    1:00    D
  {
    -24 /*fromYearTiny*/,
    -14 /*toYearTiny*/,
    4 /*inMonth*/,
    7 /*onDayOfWeek*/,
    0 /*onDayOfMonth*/,
    8 /*atTimeCode*/,
    'w' /*atTimeModifier*/,
    4 /*deltaCode*/,
    'D' /*letter*/,
  },
  // Rule    US    1987    2006    -    Apr    Sun>=1    2:00    1:00    D
  {
    -13 /*fromYearTiny*/,
    6 /*toYearTiny*/,
    4 /*inMonth*/,
    7 /*onDayOfWeek*/,
    1 /*onDayOfMonth*/,
    8 /*atTimeCode*/,
    'w' /*atTimeModifier*/,
    4 /*deltaCode*/,
    'D' /*letter*/,
  },
  // Rule    US    2007    max    -    Mar    Sun>=8    2:00    1:00    D
  {
    7 /*fromYearTiny*/,
    126 /*toYearTiny*/,
    3 /*inMonth*/,
    7 /*onDayOfWeek*/,
    8 /*onDayOfMonth*/,
    8 /*atTimeCode*/,
    'w' /*atTimeModifier*/,
    4 /*deltaCode*/,
    'D' /*letter*/,
  },
  // Rule    US    2007    max    -    Nov    Sun>=1    2:00    0    S
  {
    7 /*fromYearTiny*/,
    126 /*toYearTiny*/,
    11 /*inMonth*/,
    7 /*onDayOfWeek*/,
    1 /*onDayOfMonth*/,
    8 /*atTimeCode*/,
    'w' /*atTimeModifier*/,
    0 /*deltaCode*/,
    'S' /*letter*/,
  },

};

static const ZonePolicy kPolicyTestUS = {
  5 /*numRules*/,
  kZoneRulesTestUS /*rules*/,
  0 /* numLetters */,
  nullptr /* letters */,
};

static const ZoneEra kZoneEraTestLos_Angeles[] = {
  //             -8:00    US    P%sT
  {
    -32 /*offsetCode*/,
    &kPolicyTestUS /*zonePolicy*/,
    0 /*deltaCode*/,
    "P%T" /*format*/,
    127 /*untilYearTiny*/,
    1 /*untilMonth*/,
    1 /*untilDay*/,
    0 /*untilTimeCode*/,
    'w' /*untilTimeModifier*/,
  },

};

static const ZoneInfo kZoneTestLos_Angeles = {
  "America/Los_Angeles" /*name*/,
  1 /*numEras*/,
  kZoneEraTestLos_Angeles /*eras*/,
};

// --------------------------------------------------------------------------
// ExtendedZoneSpecifier
// --------------------------------------------------------------------------

test(ExtendedZoneSpecifierTest, compareEraToYearMonth) {
  ZoneEra era = {0, nullptr, 0, "", 0, 1, 2, 12, 'w'};
  assertEqual(1, ExtendedZoneSpecifier::compareEraToYearMonth(&era, 0, 1));
  assertEqual(1, ExtendedZoneSpecifier::compareEraToYearMonth(&era, 0, 1));
  assertEqual(-1, ExtendedZoneSpecifier::compareEraToYearMonth(&era, 0, 2));
  assertEqual(-1, ExtendedZoneSpecifier::compareEraToYearMonth(&era, 0, 3));

  ZoneEra era2 = {0, nullptr, 0, "", 0, 1, 0, 0, 'w'};
  assertEqual(0, ExtendedZoneSpecifier::compareEraToYearMonth(&era2, 0, 1));
}

test(ExtendedZoneSpecifierTest, createMatch) {
  // UNTIL = 2000-01-02 3:00
  ZoneEra prev = {0, nullptr, 0, "", 0, 1, 2, 3, 'w'};
  // UNTIL = 2002-03-04 5:00
  ZoneEra era = {0, nullptr, 0, "", 2, 3, 4, 5, 'w'};

  YearMonthTuple startYm = {0, 12};
  YearMonthTuple untilYm = {1, 2};
  ZoneMatch match = ExtendedZoneSpecifier::createMatch(
      &prev, &era, startYm, untilYm);
  assertTrue((match.startDateTime == DateTuple{0, 12, 1, 0, 'w'}));
  assertTrue((match.untilDateTime == DateTuple{1, 2, 1, 0, 'w'}));
  assertTrue(&era == match.era);

  startYm = {-1, 12};
  untilYm = {3, 2};
  match = ExtendedZoneSpecifier::createMatch(&prev, &era, startYm, untilYm);
  assertTrue((match.startDateTime == DateTuple{0, 1, 2, 3, 'w'}));
  assertTrue((match.untilDateTime == DateTuple{2, 3, 4, 5, 'w'}));
  assertTrue(&era == match.era);
}

test(ExtendedZoneSpecifierTest, findMatches_simple) {
  YearMonthTuple startYm = {18, 12};
  YearMonthTuple untilYm = {20, 2};
  const uint8_t kMaxMaches = 4;
  ZoneMatch matches[kMaxMaches];
  uint8_t numMatches = ExtendedZoneSpecifier::findMatches(
      &kZoneAlmostLosAngeles, startYm, untilYm, matches, kMaxMaches);
  assertEqual(3, numMatches);

  assertTrue((matches[0].startDateTime == DateTuple{18, 12, 1, 0, 'w'}));
  assertTrue((matches[0].untilDateTime == DateTuple{19, 3, 10, 8, 'w'}));
  assertTrue(&kZoneEraAlmostLosAngeles[0] == matches[0].era);

  assertTrue((matches[1].startDateTime == DateTuple{19, 3, 10, 8, 'w'}));
  assertTrue((matches[1].untilDateTime == DateTuple{19, 11, 3, 8, 'w'}));
  assertTrue(&kZoneEraAlmostLosAngeles[1] == matches[1].era);

  assertTrue((matches[2].startDateTime == DateTuple{19, 11, 3, 8, 'w'}));
  assertTrue((matches[2].untilDateTime == DateTuple{20, 2, 1, 0, 'w'}));
  assertTrue(&kZoneEraAlmostLosAngeles[2] == matches[2].era);
}

test(ExtendedZoneSpecifierTest, findMatches_named) {
  YearMonthTuple startYm = {18, 12};
  YearMonthTuple untilYm = {20, 2};
  const uint8_t kMaxMaches = 4;
  ZoneMatch matches[kMaxMaches];
  uint8_t numMatches = ExtendedZoneSpecifier::findMatches(
      &kZoneTestLos_Angeles, startYm, untilYm, matches, kMaxMaches);
  assertEqual(1, numMatches);

  assertTrue((matches[0].startDateTime == DateTuple{18, 12, 1, 0, 'w'}));
  assertTrue((matches[0].untilDateTime == DateTuple{20, 2, 1, 0, 'w'}));
  assertTrue(&kZoneEraTestLos_Angeles[0] == matches[0].era);
}

test(ExtendedZoneSpecifierTest, getTransitionTime) {
  const ZoneRule* rule = &kZoneRulesTestUS[4]; // Nov Sun>=1
  DateTuple dt = ExtendedZoneSpecifier::getTransitionTime(18, rule);
  assertTrue((dt == DateTuple{18, 11, 4, 8, 'w'})); // Nov 4 2018
  dt = ExtendedZoneSpecifier::getTransitionTime(19, rule);
  assertTrue((dt == DateTuple{19, 11, 3, 8, 'w'})); // Nov 3 2019
}

test(ExtendedZoneSpecifierTest, createTransitionForYear) {
  const ZoneMatch match = {
    {18, 12, 1, 0, 'w'},
    {20, 2, 1, 0, 'w'},
    &kZoneEraTestLos_Angeles[0]
  };
  const ZoneRule* const rule = &kZoneRulesTestUS[4]; // Nov Sun>=1
  Transition t;
  ExtendedZoneSpecifier::createTransitionForYear(&t, 19, rule, &match);
  assertTrue((t.transitionTime == DateTuple{19, 11, 3, 8, 'w'}));
}

test(ExtendedZoneSpecifierTest, normalizeDateTuple) {
  DateTuple dtp;

  dtp = {0, 1, 1, 0, 'w'};
  ExtendedZoneSpecifier::normalizeDateTuple(&dtp);
  assertTrue((dtp == DateTuple{0, 1, 1, 0, 'w'}));

  dtp = {0, 1, 1, 95, 'w'};
  ExtendedZoneSpecifier::normalizeDateTuple(&dtp);
  assertTrue((dtp == DateTuple{0, 1, 1, 95, 'w'}));

  dtp = {0, 1, 1, 96, 'w'};
  ExtendedZoneSpecifier::normalizeDateTuple(&dtp);
  assertTrue((dtp == DateTuple{0, 1, 2, 0, 'w'}));

  dtp = {0, 1, 1, 97, 'w'};
  ExtendedZoneSpecifier::normalizeDateTuple(&dtp);
  assertTrue((dtp == DateTuple{0, 1, 2, 1, 'w'}));

  dtp = {0, 1, 1, -96, 'w'};
  ExtendedZoneSpecifier::normalizeDateTuple(&dtp);
  assertTrue((dtp == DateTuple{-01, 12, 31, 0, 'w'}));

  dtp = {0, 1, 1, -97, 'w'};
  ExtendedZoneSpecifier::normalizeDateTuple(&dtp);
  assertTrue((dtp == DateTuple{-01, 12, 31, -1, 'w'}));
}

test(ExtendedZoneSpecifierTest, expandDateTuple) {
  DateTuple tt;
  DateTuple tts;
  DateTuple ttu;
  int8_t offsetCode = 8;
  int8_t deltaCode = 4;

  tt = {0, 1, 30, 12, 'w'};
  ExtendedZoneSpecifier::expandDateTuple(&tt, &tts, &ttu,
      offsetCode, deltaCode);
  assertTrue((tt == DateTuple{0, 1, 30, 12, 'w'}));
  assertTrue((tts == DateTuple{0, 1, 30, 8, 's'}));
  assertTrue((ttu == DateTuple{0, 1, 30, 0, 'u'}));

  tt = {0, 1, 30, 8, 's'};
  ExtendedZoneSpecifier::expandDateTuple(&tt, &tts, &ttu,
      offsetCode, deltaCode);
  assertTrue((tt == DateTuple{0, 1, 30, 12, 'w'}));
  assertTrue((tts == DateTuple{0, 1, 30, 8, 's'}));
  assertTrue((ttu == DateTuple{0, 1, 30, 0, 'u'}));

  tt = {0, 1, 30, 0, 'u'};
  ExtendedZoneSpecifier::expandDateTuple(&tt, &tts, &ttu,
      offsetCode, deltaCode);
  assertTrue((tt == DateTuple{0, 1, 30, 12, 'w'}));
  assertTrue((tts == DateTuple{0, 1, 30, 8, 's'}));
  assertTrue((ttu == DateTuple{0, 1, 30, 0, 'u'}));
}

test(ExtendedZoneSpecifierTest, calcInteriorYears) {
  const uint8_t kMaxInteriorYears = 4;
  int8_t interiorYears[kMaxInteriorYears];

  uint8_t num = ExtendedZoneSpecifier::calcInteriorYears(
      interiorYears, kMaxInteriorYears, -2, -1, 0, 2);
  assertEqual(0, num);

  num = ExtendedZoneSpecifier::calcInteriorYears(
      interiorYears, kMaxInteriorYears, 3, 5, 0, 2);
  assertEqual(0, num);

  num = ExtendedZoneSpecifier::calcInteriorYears(
      interiorYears, kMaxInteriorYears, -2, 0, 0, 2);
  assertEqual(1, num);
  assertEqual(0, interiorYears[0]);

  num = ExtendedZoneSpecifier::calcInteriorYears(
      interiorYears, kMaxInteriorYears, 2, 4, 0, 2);
  assertEqual(1, num);
  assertEqual(2, interiorYears[0]);

  num = ExtendedZoneSpecifier::calcInteriorYears(
      interiorYears, kMaxInteriorYears, 1, 2, 0, 2);
  assertEqual(2, num);
  assertEqual(1, interiorYears[0]);
  assertEqual(2, interiorYears[1]);

  num = ExtendedZoneSpecifier::calcInteriorYears(
      interiorYears, kMaxInteriorYears, -1, 3, 0, 2);
  assertEqual(3, num);
  assertEqual(0, interiorYears[0]);
  assertEqual(1, interiorYears[1]);
  assertEqual(2, interiorYears[2]);
}

test(ExtendedZoneSpecifierTest, getMostRecentPriorYear) {
  int8_t yearTiny;

  yearTiny = ExtendedZoneSpecifier::getMostRecentPriorYear(-2, -1, 0, 2);
  assertEqual(-1, yearTiny);

  yearTiny = ExtendedZoneSpecifier::getMostRecentPriorYear(3, 5, 0, 2);
  assertEqual(LocalDate::kInvalidYearTiny, yearTiny);

  yearTiny = ExtendedZoneSpecifier::getMostRecentPriorYear(-2, 0, 0, 2);
  assertEqual(-1, yearTiny);

  yearTiny = ExtendedZoneSpecifier::getMostRecentPriorYear(2, 4, 0, 2);
  assertEqual(LocalDate::kInvalidYearTiny, yearTiny);

  yearTiny = ExtendedZoneSpecifier::getMostRecentPriorYear(1, 2, 0, 2);
  assertEqual(LocalDate::kInvalidYearTiny, yearTiny);

  yearTiny = ExtendedZoneSpecifier::getMostRecentPriorYear(-1, 3, 0, 2);
  assertEqual(-1, yearTiny);
}

test(ExtendedZoneSpecifierTest, compareTransitionToMatchFuzzy) {
  const ZoneMatch match = {
    {0, 1, 1, 0, 'w'} /* startDateTime */,
    {1, 1, 1, 0, 'w'} /* untilDateTime */,
    nullptr
  };

  Transition transition = {
    &match /*match*/, nullptr /*rule*/, {-1, 11, 1, 0, 'w'} /*transitionTime*/,
    {}, {}, {}, {0}, 0, false
  };
  assertEqual(-1, ExtendedZoneSpecifier::compareTransitionToMatchFuzzy(
      &transition, &match));

  transition = {
    &match /*match*/, nullptr /*rule*/, {-1, 12, 1, 0, 'w'} /*transitionTime*/,
    {}, {}, {}, {0}, 0, false
  };
  assertEqual(1, ExtendedZoneSpecifier::compareTransitionToMatchFuzzy(
      &transition, &match));

  transition = {
    &match /*match*/, nullptr /*rule*/, {0, 1, 1, 0, 'w'} /*transitionTime*/,
    {}, {}, {}, {0}, 0, false
  };
  assertEqual(1, ExtendedZoneSpecifier::compareTransitionToMatchFuzzy(
      &transition, &match));

  transition = {
    &match /*match*/, nullptr /*rule*/, {1, 1, 1, 0, 'w'} /*transitionTime*/,
    {}, {}, {}, {0}, 0, false
  };
  assertEqual(1, ExtendedZoneSpecifier::compareTransitionToMatchFuzzy(
      &transition, &match));

  transition = {
    &match /*match*/, nullptr /*rule*/, {1, 2, 1, 0, 'w'} /*transitionTime*/,
    {}, {}, {}, {0}, 0, false
  };
  assertEqual(1, ExtendedZoneSpecifier::compareTransitionToMatchFuzzy(
      &transition, &match));

  transition = {
    &match /*match*/, nullptr /*rule*/, {1, 3, 1, 0, 'w'} /*transitionTime*/,
    {}, {}, {}, {0}, 0, false
  };
  assertEqual(2, ExtendedZoneSpecifier::compareTransitionToMatchFuzzy(
      &transition, &match));
}


test(ExtendedZoneSpecifierTest, compareTransitionToMatch) {
  const ZoneMatch match = {
    {0, 1, 1, 0, 'w'} /*startDateTime*/,
    {1, 1, 1, 0, 'w'} /*untilDateTime*/,
    nullptr /*era*/
  };

  Transition transition = {
    &match /*match*/, nullptr /*rule*/, {-1, 12, 31, 0, 'w'} /*transitionTime*/,
    {}, {}, {}, {0}, 0, false
  };
  assertEqual(-1, ExtendedZoneSpecifier::compareTransitionToMatch(
      &transition, &match));

  transition = {
    &match /*match*/, nullptr /*rule*/, {0, 1, 1, 0, 'w'} /*transitionTime*/,
    {}, {}, {}, {0}, 0, false
  };
  assertEqual(0, ExtendedZoneSpecifier::compareTransitionToMatch(
      &transition, &match));

  transition = {
    &match /*match*/, nullptr /*rule*/, {0, 1, 2, 0, 'w'} /*transitionTime*/,
    {}, {}, {}, {0}, 0, false
  };
  assertEqual(1, ExtendedZoneSpecifier::compareTransitionToMatch(
      &transition, &match));

  transition = {
    &match /*match*/, nullptr /*rule*/, {1, 1, 2, 0, 'w'} /*transitionTime*/,
    {}, {}, {}, {0}, 0, false
  };
  assertEqual(2, ExtendedZoneSpecifier::compareTransitionToMatch(
      &transition, &match));
}

test(ExtendedZoneSpecifierTest, processActiveTransition) {
  Transition* prior = nullptr;
  const ZoneMatch match = {
    {0, 1, 1, 0, 'w'} /*startDateTime*/,
    {1, 1, 1, 0, 'w'} /*untilDateTime*/,
    nullptr /*era*/
  };

  // This transition occurs before the match, so prior should be filled.
  Transition transition0 = {
    &match /*match*/, nullptr /*rule*/, {-1, 12, 31, 0, 'w'} /*transitionTime*/,
    {}, {}, {}, {0}, 0, false
  };
  ExtendedZoneSpecifier::processActiveTransition(&match, &transition0, &prior);
  assertTrue(transition0.active);
  assertTrue(prior == &transition0);

  // This occurs at exactly match.startDateTime, so should replace
  Transition transition1 = {
    &match /*match*/, nullptr /*rule*/, {0, 1, 1, 0, 'w'} /*transitionTime*/,
    {}, {}, {}, {0}, 0, false
  };
  ExtendedZoneSpecifier::processActiveTransition(&match, &transition1, &prior);
  assertTrue(transition1.active);
  assertTrue(prior == &transition1);

  // An interior transition. Prior should not change.
  Transition transition2 = {
    &match /*match*/, nullptr /*rule*/, {0, 1, 2, 0, 'w'} /*transitionTime*/,
    {}, {}, {}, {0}, 0, false
  };
  ExtendedZoneSpecifier::processActiveTransition(&match, &transition2, &prior);
  assertTrue(transition2.active);
  assertTrue(prior == &transition1);

  // Occurs after match.untilDateTime, so should be rejected.
  Transition transition3 = {
    &match /*match*/, nullptr /*rule*/, {1, 1, 2, 0, 'w'} /*transitionTime*/,
    {}, {}, {}, {0}, 0, false
  };
  assertFalse(transition3.active);
  assertTrue(prior == &transition1);
}

test(ExtendedZoneSpecifierTest, findCandidateTransitions) {
  const ZoneMatch match = {
    {18, 12, 1, 0, 'w'},
    {20, 2, 1, 0, 'w'},
    &kZoneEraTestLos_Angeles[0]
  };

  // Reserve storage for the Transitions
  const uint8_t kMaxStorage = 8;
  TransitionStorage<kMaxStorage> storage;
  storage.init();

  // Verify compareTransitionToMatchFuzzy() elminates various transitions
  // to get down to 5:
  //    * 2018 Mar Sun>=8 (11)
  //    * 2019 Nov Sun>=1 (4)
  //    * 2019 Mar Sun>=8 (10)
  //    * 2019 Nov Sun>=1 (3)
  //    * 2020 Mar Sun>=8 (8)
  storage.resetCandidatePool();
  ExtendedZoneSpecifier::findCandidateTransitions(storage, &match);
  assertEqual(5,
      (int) (storage.getCandidatePoolEnd() - storage.getCandidatePoolBegin()));
  Transition** t = storage.getCandidatePoolBegin();
  assertTrue(((*t++)->transitionTime == DateTuple{18, 3, 11, 8, 'w'}));
  assertTrue(((*t++)->transitionTime == DateTuple{18, 11, 4, 8, 'w'}));
  assertTrue(((*t++)->transitionTime == DateTuple{19, 3, 10, 8, 'w'}));
  assertTrue(((*t++)->transitionTime == DateTuple{19, 11, 3, 8, 'w'}));
  assertTrue(((*t++)->transitionTime == DateTuple{20, 3, 8, 8, 'w'}));
}

test(ExtendedZoneSpecifierTest, findTransitionsFromNamedMatch) {
  const ZoneMatch match = {
    {18, 12, 1, 0, 'w'},
    {20, 2, 1, 0, 'w'},
    &kZoneEraTestLos_Angeles[0]
  };

  // Reserve storage for the Transitions
  const uint8_t kMaxStorage = 8;
  TransitionStorage<kMaxStorage> storage;
  storage.init();

  ExtendedZoneSpecifier::findTransitionsFromNamedMatch(storage, &match);
  assertEqual(3,
      (int) (storage.getActivePoolEnd() - storage.getActivePoolBegin()));
  Transition** t = storage.getActivePoolBegin();
  assertTrue(((*t++)->transitionTime == DateTuple{18, 12, 1, 0, 'w'}));
  assertTrue(((*t++)->transitionTime == DateTuple{19, 3, 10, 8, 'w'}));
  assertTrue(((*t++)->transitionTime == DateTuple{19, 11, 3, 8, 'w'}));
}

test(ExtendedZoneSpecifierTest, fixTransitionTimes_generateStartUntilTimes) {
  // Create 3 matches for the AlmostLosAngeles test zone.
  YearMonthTuple startYm = {18, 12};
  YearMonthTuple untilYm = {20, 2};
  const uint8_t kMaxMaches = 4;
  ZoneMatch matches[kMaxMaches];
  uint8_t numMatches = ExtendedZoneSpecifier::findMatches(
      &kZoneAlmostLosAngeles, startYm, untilYm, matches, kMaxMaches);
  assertEqual(3, numMatches);

  TransitionStorage<4> storage;
  storage.init();

  // Create 3 Transitions corresponding to the matches.
  // Implements ExtendedZoneSpecifier::findTransitionsFromSimpleMatch().
  Transition* transition1 = storage.getFreeAgent();
  transition1->match = &matches[0];
  transition1->rule = nullptr;
  transition1->transitionTime = matches[0].startDateTime;
  transition1->active = true;
  storage.addFreeAgentToCandidatePool();

  Transition* transition2 = storage.getFreeAgent();
  transition2->match = &matches[1];
  transition2->rule = nullptr;
  transition2->transitionTime = matches[1].startDateTime;
  transition2->active = true;
  storage.addFreeAgentToCandidatePool();

  Transition* transition3 = storage.getFreeAgent();
  transition3->match = &matches[2];
  transition3->rule = nullptr;
  transition3->transitionTime = matches[2].startDateTime;
  transition3->active = true;
  storage.addFreeAgentToCandidatePool();

  // Move actives to Active pool.
  storage.addActiveCandidatesToActivePool();
  Transition** begin = storage.getActivePoolBegin();
  Transition** end = storage.getActivePoolEnd();
  assertEqual(3, (int) (end - begin));
  assertTrue(begin[0] == transition1);
  assertTrue(begin[1] == transition2);
  assertTrue(begin[2] == transition3);

  // Fix the transition times, expanding to 's' and 'u'
  ExtendedZoneSpecifier::fixTransitionTimes(begin, end);

  // Verify. The first Transition is extended to -infinity.
  assertTrue((transition1->transitionTime == DateTuple{18, 12, 1, 0, 'w'}));
  assertTrue((transition1->transitionTimeS == DateTuple{18, 12, 1, 0, 's'}));
  assertTrue((transition1->transitionTimeU == DateTuple{18, 12, 1, 32, 'u'}));

  // Second transition uses the UTC offset of the first.
  assertTrue((transition2->transitionTime == DateTuple{19, 3, 10, 8, 'w'}));
  assertTrue((transition2->transitionTimeS == DateTuple{19, 3, 10, 8, 's'}));
  assertTrue((transition2->transitionTimeU == DateTuple{19, 3, 10, 40, 'u'}));

  // Third transition uses the UTC offset of the second.
  assertTrue((transition3->transitionTime == DateTuple{19, 11, 3, 8, 'w'}));
  assertTrue((transition3->transitionTimeS == DateTuple{19, 11, 3, 4, 's'}));
  assertTrue((transition3->transitionTimeU == DateTuple{19, 11, 3, 36, 'u'}));

  // Generate the startDateTime and untilDateTime of the transitions.
  ExtendedZoneSpecifier::generateStartUntilTimes(begin, end);

  // Verify. The first transition startTime should be the same as its
  // transitionTime.
  assertTrue((transition1->transitionTime == DateTuple{18, 12, 1, 0, 'w'}));
  assertTrue((transition1->startDateTime == DateTuple{18, 12, 1, 0, 'w'}));
  assertTrue((transition1->untilDateTime == DateTuple{19, 3, 10, 8, 'w'}));
  acetime_t epochSecs = OffsetDateTime::forComponents(
      2018, 12, 1, 0, 0, 0, UtcOffset::forOffsetCode(-32)).toEpochSeconds();
  assertEqual(epochSecs, transition1->startEpochSeconds);

  // Second transition startTime is shifted forward one hour into PDT.
  assertTrue((transition2->transitionTime == DateTuple{19, 3, 10, 8, 'w'}));
  assertTrue((transition2->startDateTime == DateTuple{19, 3, 10, 12, 'w'}));
  assertTrue((transition2->untilDateTime == DateTuple{19, 11, 3, 8, 'w'}));
  epochSecs = OffsetDateTime::forComponents(
      2019, 3, 10, 3, 0, 0, UtcOffset::forOffsetCode(-28)).toEpochSeconds();
  assertEqual(epochSecs, transition2->startEpochSeconds);

  // Third transition startTime is shifted back one hour into PST.
  assertTrue((transition3->transitionTime == DateTuple{19, 11, 3, 8, 'w'}));
  assertTrue((transition3->startDateTime == DateTuple{19, 11, 3, 4, 'w'}));
  assertTrue((transition3->untilDateTime == DateTuple{20, 2, 1, 0, 'w'}));
  epochSecs = OffsetDateTime::forComponents(
      2019, 11, 3, 1, 0, 0, UtcOffset::forOffsetCode(-32)).toEpochSeconds();
  assertEqual(epochSecs, transition3->startEpochSeconds);
}

test(ExtendedZoneSpecifierTest, createAbbreviation) {
  const uint8_t kDstSize = 6;
  char dst[kDstSize];

  ExtendedZoneSpecifier::createAbbreviation(dst, kDstSize, "SAST", 0, nullptr);
  assertEqual("SAST", dst);

  ExtendedZoneSpecifier::createAbbreviation(dst, kDstSize, "P%T", 4, "D");
  assertEqual("PDT", dst);

  ExtendedZoneSpecifier::createAbbreviation(dst, kDstSize, "P%T", 0, "S");
  assertEqual("PST", dst);

  ExtendedZoneSpecifier::createAbbreviation(dst, kDstSize, "P%T", 0, "");
  assertEqual("PT", dst);

  ExtendedZoneSpecifier::createAbbreviation(dst, kDstSize, "GMT/BST", 0, "");
  assertEqual("GMT", dst);

  ExtendedZoneSpecifier::createAbbreviation(dst, kDstSize, "GMT/BST", 4, "");
  assertEqual("BST", dst);

  ExtendedZoneSpecifier::createAbbreviation(dst, kDstSize, "P%T3456", 4, "DD");
  assertEqual("PDDT3", dst);

  ExtendedZoneSpecifier::createAbbreviation(dst, kDstSize, "%", 4, "CAT");
  assertEqual("CAT", dst);

  ExtendedZoneSpecifier::createAbbreviation(dst, kDstSize, "%", 0, "WAT");
  assertEqual("WAT", dst);
}

test(ExtendedZoneSpecifierTest, calcAbbreviations) {
  // TODO: Implement
}

// --------------------------------------------------------------------------

void setup() {
#if defined(ARDUINO)
  delay(1000); // wait for stability on some boards to prevent garbage Serial
#endif
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while(!Serial); // for the Arduino Leonardo/Micro only

#if 0
  TestRunner::exclude("*");
  TestRunner::include("ExtendedZoneSpecifierTest",
      "findTransitionsFromNamedMatch");
#endif
}

void loop() {
  TestRunner::run();
}
