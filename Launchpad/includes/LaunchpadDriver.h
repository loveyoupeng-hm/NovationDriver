#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "LaunchpadLayout.h"
#include <memory>

using namespace juce;

class LaunchpadDriver : public LaunchpadLayout::LayoutListener, private juce::Timer
{
public:
    LaunchpadDriver();

    ~LaunchpadDriver();

    void initialize(std::unique_ptr<juce::MidiOutput> device);

    void flashToDevice();

    int processMidiPitch(const int pitch) const;

private:
    void timerCallback();
    uint64 click = 0;
    std::unique_ptr<juce::MidiOutput> midiDevice;
    uint64 current = 1;
    uint64 update = 0;
    LaunchpadLayout layout;
    static const uint8 enableDAWSysex[];
    static const uint8 disableDAWSysex[];
    static const uint8 faderSysex[];
    static const uint8 enableFaderSysex[];
    static const uint8 clearSessionSysex[];
    static const uint8 selectSessionLayoutSysex[];
    static const juce::MidiMessage enableDAW;
    static const juce::MidiMessage disableDAW;
    static const juce::MidiMessage fader;
    static const juce::MidiMessage enableFader;
    static const juce::MidiMessage clearSession;
    static const juce::MidiMessage selectSessionLayout;
};