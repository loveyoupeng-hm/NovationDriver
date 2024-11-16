#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>

using namespace juce;

class LaunchpadComponent : public juce::Component
{
public:
    LaunchpadComponent();
    ~LaunchpadComponent() override {};
    void paint(juce::Graphics &g) override;
    void resized() override;
};