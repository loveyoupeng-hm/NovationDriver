#include <gtest/gtest.h>
#include <juce_dsp/juce_dsp.h>
#include <atomic>

namespace test_plugins
{
    struct A {int a[2];};
    struct B {int a[5];};
    TEST(Cpp, lock_free)
    {
        std::atomic<A> a;
        std::atomic<B> b;
        EXPECT_TRUE(a.is_lock_free());
        EXPECT_FALSE(b.is_lock_free());
    }

    TEST(Juce, Oscillator)
    {
        juce::dsp::Oscillator<float> osc;
        EXPECT_FALSE(osc.isInitialised());
        osc.initialise([](float x)
                       { return x; });
        EXPECT_TRUE(osc.isInitialised());
        osc.setFrequency(1);
        EXPECT_EQ(1.0, osc.getFrequency());
    }

} // namespace test_plugins