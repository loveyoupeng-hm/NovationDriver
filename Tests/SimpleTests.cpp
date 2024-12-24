#include <gtest/gtest.h>
#include <juce_dsp/juce_dsp.h>
#include <atomic>
#define M_PI 3.14159265358979323846
#include <cmath>
#include <juce_audio_basics/juce_audio_basics.h>

namespace test_plugins
{
    struct A
    {
        int a[2];
    };
    struct B
    {
        int a[5];
    };
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
                       { return std::sin(x); });
        EXPECT_TRUE(osc.isInitialised());
        osc.setFrequency(1);
        EXPECT_EQ(1.0, osc.getFrequency());
        osc.reset();
        for (int i = 0; i < 48000 / 4; i++)
            osc.processSample(.0f);
        EXPECT_NEAR(-1.0f, osc.processSample(.0f), 0.0001);

        for (int i = 0; i < 48000 / 4; i++)
            osc.processSample(.0f);
        EXPECT_NEAR(0, osc.processSample(.0f), 0.0001);

        for (int i = 0; i < 48000 / 4; i++)
            osc.processSample(.0f);
        EXPECT_NEAR(1.f, osc.processSample(.0f), 0.0001);

        for (int i = 0; i < 48000 / 4; i++)
            osc.processSample(.0f);

        EXPECT_NEAR(0.f, osc.processSample(.0f), 0.01);
        for (int i = 0; i < 48000 / 4; i++)
            osc.processSample(.0f);

        EXPECT_NEAR(-1.f, osc.processSample(.0f), 0.01);
    }

    TEST(Cpp, sin)
    {
        EXPECT_DOUBLE_EQ(0.0, std::sin(0));
        EXPECT_DOUBLE_EQ(1.0, std::sin(0.5 * M_PI));
    }

    TEST(Juce, ADSR)
    {
        juce::ADSR adsr;
        juce::ADSR::Parameters parameters{1, 1, 0.5, 1};
        adsr.setSampleRate(10.0);
        adsr.setParameters(parameters);
        EXPECT_FLOAT_EQ(0.0, adsr.getNextSample());
        adsr.noteOn();
        for (int i = 0; i < 10; i++)
            EXPECT_FLOAT_EQ(0.1 * (i + 1), adsr.getNextSample());

        for (int i = 0; i < 10; i++)
            EXPECT_FLOAT_EQ(1.0 - 0.05 * (i + 1), adsr.getNextSample());

        for (int i = 0; i < 100; i++)
            EXPECT_FLOAT_EQ(0.5, adsr.getNextSample());

        adsr.noteOff();
        for (int i = 0; i < 10; i++)
            EXPECT_NEAR(0.5 - 0.05 * (i + 1), adsr.getNextSample(), 0.0001);
    }

} // namespace test_plugins