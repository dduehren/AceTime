# ExtendedHinnantDateTest

This unit test compares the DST transitions calculated by the
`BasicZoneProcessor` class (which uses the `zonedb` data files) with the
`validation_data.cpp` file generated by the
`../../tools/compare_cpp/test_data_generator.cpp` program which uses the
[Hinnant Date Library](https://github.com/HowardHinnant/date).

## Running the Test

Assuming that you have `g++` and `make` installed, just type:
```
$ make clean && make -j 4

$ make runtests
TestRunner started on 387 test(s).
Test ExtendedTransitionTest_Africa_Abidjan passed.
...
Test ExtendedTransitionTest_WET passed.
TestRunner duration: 2.561 seconds.
TestRunner summary: 268 passed, 0 failed, 0 skipped, 0 timed out, out of 268 test(s).

$ make clean
```

## Compiling the HinnantDate Test Data Generator

The `Makefile` assumes that you have already compiled the
`../../../tools/compare_cpp/test_data_generator.out` program. Type the
following:

```
$ cd ../../../tools/compare_cpp
$ make
```
