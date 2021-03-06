# BasicPythonTest

This unit test compares the DST transitions calculated by the
`BasicZoneProcessor` class with the `validation_data.cpp` file generated by the
Python `../../tools/compare_pytz/test_data_generator.py` module which uses the
[pytz](https://pypi.org/project/pytz/) library.

The Python tool generates about 130,000 data points spanning the year 2000 to
2038. It is too large to run on any Arduino board that I am aware of, including
the ESP32.

The unit test does run on a Linux machine using the
[UnixHostDuino](https://github.com/bxparks/UnixHostDuino) adapter layer.
Assuming that you have `g++` and `make` installed, just type:

```
$ make

$ make runtests
TestRunner started on 270 test(s).
Test BasicTransitionTest_Africa_Abidjan passed.
...
Test BasicTransitionTest_WET passed.
TestRunner duration: 1.614 seconds.
TestRunner summary: 270 passed, 0 failed, 0 skipped, 0 timed out, out of 270 test(s).

$ make clean
```
