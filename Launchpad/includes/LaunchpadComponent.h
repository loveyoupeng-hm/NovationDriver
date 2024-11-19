#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "LaunchpadBroadcaster.h"

using namespace juce;

class LaunchpadComponent : public juce::Component
                        
{
public:
    LaunchpadComponent();
    ~LaunchpadComponent() override;
    void paint(juce::Graphics &g) override;
    void resized() override;
    void mouseDown(const MouseEvent& event) override;
    void
    add(LaunchpadBroadcaster::Listener &callback)
    {
        broadcaster->addListener(callback);
    }

private:
    bool clicked[8][8];
    bool upclicked[8];
    bool rightclicked[8];
    LaunchpadBroadcaster* broadcaster{nullptr};

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LaunchpadComponent)
};