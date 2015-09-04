#ifndef triangles_test_histogram_h
#define triangles_test_histogram_h

#include "gtest/gtest.h"
#include "histogram.h"


TEST(Histogram, int) {
    Histogram<int> histogram(0, 1024, 1024);

    for (unsigned int i = 0; i <= 1024; i++)
        ASSERT_EQ(i, histogram.bin(i));
    ASSERT_EQ(0, histogram.bin(-1));
    ASSERT_EQ(1024, histogram.bin(1025));

    for (unsigned int i = 0; i <= 1024; i++)
        ASSERT_EQ(i, histogram.value(i));

    for (unsigned int i = 1; i <= 1024; i++) {
        ASSERT_EQ(i, histogram.value(histogram.bin(i)));
        ASSERT_EQ(i - 1, histogram.value(histogram.bin(i) - 1));
    }

    histogram.add(1025);
    ASSERT_EQ(histogram.bin(1025), 1024);
    ASSERT_EQ(histogram[1024], 1);
}


TEST(Histogram, float) {
    Histogram<double> histogram(0, 1024, 1024);

    for (unsigned int i = 0; i <= 1024; i++)
        ASSERT_EQ(i, histogram.bin(i));
        ASSERT_EQ(0, histogram.bin(-1));
        ASSERT_EQ(1024, histogram.bin(1025));

    for (unsigned int i = 0; i <= 1024; i++)
        ASSERT_EQ(i, histogram.value(i));

    for (unsigned int i = 0; i < 1024; i++) {
        ASSERT_EQ(i, histogram.value(histogram.bin(i)));
    }
}

TEST(Histogram, floatUniform) {
    Histogram<double> histogram(0, 1, 10);

    ASSERT_EQ(0, histogram.bin(-0.1));
    ASSERT_EQ(0, histogram.bin(0));
    ASSERT_EQ(0, histogram.bin(0.0000001));
    ASSERT_EQ(0, histogram.bin(0.0999999));
    ASSERT_EQ(5, histogram.bin(0.5000001));
    ASSERT_EQ(5, histogram.bin(0.5999999));
    ASSERT_EQ(9, histogram.bin(0.9000001));
    ASSERT_EQ(9, histogram.bin(0.9999999));
    ASSERT_EQ(10, histogram.bin(1));
    ASSERT_EQ(10, histogram.bin(1.1));

    EXPECT_NEAR(0.1, histogram.value(histogram.bin(0.1000001)), 0.00001);
    EXPECT_NEAR(0.1, histogram.value(histogram.bin(0.1999999)), 0.00001);
    EXPECT_NEAR(0.5, histogram.value(histogram.bin(0.5000001)), 0.00001);
    EXPECT_NEAR(0.5, histogram.value(histogram.bin(0.5999999)), 0.00001);
}

#endif
