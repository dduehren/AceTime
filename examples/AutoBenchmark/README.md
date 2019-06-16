# Auto Benchmark

Here are the results from `AutoBenchmark.ino` for various boards.
These results show that integer division and modulus operations are incredibly
slow on 8-bit AVR processors.

## Arduino Nano

IDE: Arduino 1.8.9 (AVR Core 1.6.23)

Memory:

```
sizeof(LocalDate): 3
sizeof(LocalTime): 3
sizeof(LocalDateTime): 6
sizeof(TimeOffset): 1
sizeof(OffsetDateTime): 7
sizeof(basic::ZoneEra): 11
sizeof(basic::ZoneInfo): 8
sizeof(basic::ZoneRule): 9
sizeof(basic::ZonePolicy): 6
sizeof(basic::Transition): 17
sizeof(extended::Transition): 36
sizeof(extended::ZoneMatch): 12
sizeof(ZoneSpecifier): 3
sizeof(ManualZoneSpecifier): 10
sizeof(BasicZoneSpecifier): 95
sizeof(ExtendedZoneSpecifier): 366
sizeof(TimeZone): 3
sizeof(ZonedDateTime): 10
sizeof(TimePeriod): 4
sizeof(SystemTimeKeeper): 17
sizeof(DS3231TimeKeeper): 3
sizeof(SystemTimeSyncLoop): 14
sizeof(SystemTimeHeartbeatLoop): 8
sizeof(SystemTimeSyncCoroutine): 31
sizeof(SystemTimeHeartbeatCoroutine): 18
```

CPU:

```
+--------------------------------------------------+----------+
| Method                                           |   micros |
|--------------------------------------------------|----------|
| Empty loop                                       |    5.200 |
|--------------------------------------------------|----------|
| LocalDate::forEpochDays()                        |  216.400 |
| LocalDate::toEpochDays()                         |   56.800 |
| LocalDate::dayOfWeek()                           |   50.400 |
| OffsetDateTime::forEpochSeconds()                |  321.600 |
| OffsetDateTime::toEpochSeconds()                 |   76.400 |
| ZonedDateTime::toEpochSeconds()                  |   77.200 |
| ZonedDateTime::toEpochDays()                     |   70.400 |
| ZonedDateTime::forEpochSeconds(UTC)              |  332.400 |
| ZonedDateTime::forEpochSeconds(Basic nocache)    | 1046.000 |
| ZonedDateTime::forEpochSeconds(Basic cached)     |  611.600 |
| ZonedDateTime::forEpochSeconds(Extended nocache) | 1865.600 |
| ZonedDateTime::forEpochSeconds(Extended cached)  |  613.200 |
+--------------------------------------------------+----------+
Number of iterations per run: 2500
```

## ESP8266

IDE: Arduino 1.8.9 (ESP Core 2.5.2)

Memory:
```
sizeof(LocalDate): 3
sizeof(LocalTime): 3
sizeof(LocalDateTime): 6
sizeof(TimeOffset): 1
sizeof(OffsetDateTime): 7
sizeof(basic::ZoneEra): 24
sizeof(basic::ZoneInfo): 16
sizeof(basic::ZoneRule): 9
sizeof(basic::ZonePolicy): 16
sizeof(basic::Transition): 24
sizeof(extended::Transition): 44
sizeof(extended::ZoneMatch): 16
sizeof(ZoneSpecifier): 8
sizeof(ManualZoneSpecifier): 20
sizeof(BasicZoneSpecifier): 140
sizeof(ExtendedZoneSpecifier): 472
sizeof(TimeZone): 8
sizeof(ZonedDateTime): 16
sizeof(TimePeriod): 4
sizeof(SystemTimeKeeper): 24
sizeof(DS3231TimeKeeper): 8
sizeof(NtpTimeProvider): 88
sizeof(SystemTimeSyncLoop): 20
sizeof(SystemTimeHeartbeatLoop): 12
sizeof(SystemTimeSyncCoroutine): 52
sizeof(SystemTimeHeartbeatCoroutine): 36
```

CPU:

```
+--------------------------------------------------+----------+
| Method                                           |   micros |
|--------------------------------------------------|----------|
| Empty loop                                       |    5.000 |
|--------------------------------------------------|----------|
| LocalDate::forEpochDays()                        |    7.600 |
| LocalDate::toEpochDays()                         |    4.000 |
| LocalDate::dayOfWeek()                           |    4.000 |
| OffsetDateTime::forEpochSeconds()                |   13.400 |
| OffsetDateTime::toEpochSeconds()                 |    6.300 |
| ZonedDateTime::toEpochSeconds()                  |    6.400 |
| ZonedDateTime::toEpochDays()                     |    5.400 |
| ZonedDateTime::forEpochSeconds(UTC)              |   14.100 |
| ZonedDateTime::forEpochSeconds(Basic nocache)    |   65.100 |
| ZonedDateTime::forEpochSeconds(Basic cached)     |   26.600 |
| ZonedDateTime::forEpochSeconds(Extended nocache) |  138.700 |
| ZonedDateTime::forEpochSeconds(Extended cached)  |   26.900 |
+--------------------------------------------------+----------+
Number of iterations per run: 10000

```

## ESP32

IDE: Arduino 1.8.9 (ESP32 Core 1.0.2)

Memory:

```
sizeof(LocalDate): 3
sizeof(LocalTime): 3
sizeof(LocalDateTime): 6
sizeof(TimeOffset): 1
sizeof(OffsetDateTime): 7
sizeof(basic::ZoneEra): 24
sizeof(basic::ZoneInfo): 16
sizeof(basic::ZoneRule): 9
sizeof(basic::ZonePolicy): 16
sizeof(basic::Transition): 24
sizeof(extended::Transition): 44
sizeof(extended::ZoneMatch): 16
sizeof(ZoneSpecifier): 8
sizeof(ManualZoneSpecifier): 20
sizeof(BasicZoneSpecifier): 140
sizeof(ExtendedZoneSpecifier): 472
sizeof(TimeZone): 8
sizeof(ZonedDateTime): 16
sizeof(TimePeriod): 4
sizeof(SystemTimeKeeper): 24
sizeof(DS3231TimeKeeper): 8
sizeof(NtpTimeProvider): 116
sizeof(SystemTimeSyncLoop): 20
sizeof(SystemTimeHeartbeatLoop): 12
sizeof(SystemTimeSyncCoroutine): 52
sizeof(SystemTimeHeartbeatCoroutine): 36
```

CPU:

```
+--------------------------------------------------+----------+
| Method                                           |   micros |
|--------------------------------------------------|----------|
| Empty loop                                       |    1.400 |
|--------------------------------------------------|----------|
| LocalDate::forEpochDays()                        |    0.440 |
| LocalDate::toEpochDays()                         |    0.330 |
| LocalDate::dayOfWeek()                           |    0.430 |
| OffsetDateTime::forEpochSeconds()                |    1.470 |
| OffsetDateTime::toEpochSeconds()                 |    1.260 |
| ZonedDateTime::toEpochSeconds()                  |    1.280 |
| ZonedDateTime::toEpochDays()                     |    1.000 |
| ZonedDateTime::forEpochSeconds(UTC)              |    1.910 |
| ZonedDateTime::forEpochSeconds(Basic nocache)    |   13.170 |
| ZonedDateTime::forEpochSeconds(Basic cached)     |    2.980 |
| ZonedDateTime::forEpochSeconds(Extended nocache) |   29.100 |
| ZonedDateTime::forEpochSeconds(Extended cached)  |    3.050 |
+--------------------------------------------------+----------+
Number of iterations per run: 100000
```

Note: Once the benchmark of the function under test becomes smaller than the
duration of an empty loop, the numbers become suspect.

## Teensy 3.2

IDE: Arduino 1.8.9 (Teensyduino 1.46)
Compiler Optimization: "Faster"

Memory:

```
sizeof(LocalDate): 3
sizeof(LocalTime): 3
sizeof(LocalDateTime): 6
sizeof(TimeOffset): 1
sizeof(OffsetDateTime): 7
sizeof(basic::ZoneEra): 24
sizeof(basic::ZoneInfo): 16
sizeof(basic::ZoneRule): 9
sizeof(basic::ZonePolicy): 16
sizeof(basic::Transition): 24
sizeof(extended::Transition): 44
sizeof(extended::ZoneMatch): 16
sizeof(ZoneSpecifier): 8
sizeof(ManualZoneSpecifier): 20
sizeof(BasicZoneSpecifier): 140
sizeof(ExtendedZoneSpecifier): 472
sizeof(TimeZone): 8
sizeof(ZonedDateTime): 16
sizeof(TimePeriod): 4
sizeof(SystemTimeKeeper): 24
sizeof(DS3231TimeKeeper): 8
sizeof(SystemTimeSyncLoop): 20
sizeof(SystemTimeHeartbeatLoop): 12
sizeof(SystemTimeSyncCoroutine): 52
sizeof(SystemTimeHeartbeatCoroutine): 36
```

CPU:

```
+--------------------------------------------------+----------+
| Method                                           |   micros |
|--------------------------------------------------|----------|
| Empty loop                                       |    0.570 |
|--------------------------------------------------|----------|
| LocalDate::forEpochDays()                        |    0.940 |
| LocalDate::toEpochDays()                         |    0.970 |
| LocalDate::dayOfWeek()                           |    1.960 |
| OffsetDateTime::forEpochSeconds()                |    2.130 |
| OffsetDateTime::toEpochSeconds()                 |    0.170 |
| ZonedDateTime::toEpochSeconds()                  |    0.340 |
| ZonedDateTime::toEpochDays()                     |    0.300 |
| ZonedDateTime::forEpochSeconds(UTC)              |    2.300 |
| ZonedDateTime::forEpochSeconds(Basic nocache)    |   26.150 |
| ZonedDateTime::forEpochSeconds(Basic cached)     |    5.470 |
| ZonedDateTime::forEpochSeconds(Extended nocache) |   61.880 |
| ZonedDateTime::forEpochSeconds(Extended cached)  |    5.610 |
+--------------------------------------------------+----------+
Number of iterations per run: 100000
```

Note: Once the benchmark of the function under test becomes smaller than the
duration of an empty loop, the numbers become suspect.