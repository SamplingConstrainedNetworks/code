#ifndef test_histogram_h
#define test_histogram_h

#include "gtest/gtest.h"
#include "histogram.h"


class TestHistogram : public ::testing::Test
{
public:
	Histogram<double> * histogram;

	void SetUp() {
		histogram = new Histogram<double>(0, 1, 11);
	}

	void TearDown() {
		delete histogram;
	}
};


TEST_F(TestHistogram, add) {
    histogram->add(0.01);

	ASSERT_EQ(1, histogram->counts(0));
    ASSERT_EQ(0, histogram->counts(1));

    histogram->add(0.1001);
    EXPECT_EQ(1, histogram->counts(0));
    EXPECT_EQ(1, histogram->counts(1));
}


#endif
