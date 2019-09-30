# .pri file for including the Google Test sources in a build.

INCLUDEPATH += $$PWD/googletest/googletest/include
INCLUDEPATH += $$PWD/googletest/googletest/src
INCLUDEPATH += $$PWD/googletest/googletest

HEADERS += \
    $$PWD/googletest/googletest/include/gtest/gtest-death-test.h \
    $$PWD/googletest/googletest/include/gtest/gtest-matchers.h \
    $$PWD/googletest/googletest/include/gtest/gtest-message.h \
    $$PWD/googletest/googletest/include/gtest/gtest-param-test.h \
    $$PWD/googletest/googletest/include/gtest/gtest-printers.h \
    $$PWD/googletest/googletest/include/gtest/gtest-spi.h \
    $$PWD/googletest/googletest/include/gtest/gtest-test-part.h \
    $$PWD/googletest/googletest/include/gtest/gtest-typed-test.h \
    $$PWD/googletest/googletest/include/gtest/gtest.h \
    $$PWD/googletest/googletest/include/gtest/gtest_pred_impl.h \
    $$PWD/googletest/googletest/include/gtest/gtest_prod.h \
    $$PWD/googletest/googletest/include/gtest/internal/custom/gtest-port.h \
    $$PWD/googletest/googletest/include/gtest/internal/custom/gtest-printers.h \
    $$PWD/googletest/googletest/include/gtest/internal/custom/gtest.h \
    $$PWD/googletest/googletest/src/gtest-internal-inl.h

SOURCES += \
    $$PWD/googletest/googletest/src/gtest-death-test.cc \
    $$PWD/googletest/googletest/src/gtest-filepath.cc \
    $$PWD/googletest/googletest/src/gtest-matchers.cc \
    $$PWD/googletest/googletest/src/gtest-port.cc \
    $$PWD/googletest/googletest/src/gtest-printers.cc \
    $$PWD/googletest/googletest/src/gtest-test-part.cc \
    $$PWD/googletest/googletest/src/gtest-typed-test.cc \
    $$PWD/googletest/googletest/src/gtest.cc \
    $$PWD/googletest/googletest/src/gtest_main.cc
