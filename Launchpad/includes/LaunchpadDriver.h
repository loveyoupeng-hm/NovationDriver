#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "LaunchpadLayout.h"
#include <memory>

using namespace juce;

class LaunchpadDriver : public LaunchpadLayout::LayoutListener, private juce::Timer
{
public:
    LaunchpadDriver()
    {
        startTimer(2000);
    }
    ~LaunchpadDriver()
    {
        if (midiDevice != nullptr)
        {
            midiDevice->sendMessageNow(disableDAW);
            midiDevice = nullptr;
        }
        stopTimer();
    }
    void initialize(std::unique_ptr<juce::MidiOutput> device)
    {
        midiDevice = std::move(device);
        midiDevice->sendMessageNow(disableDAW);
        midiDevice->sendMessageNow(enableDAW);
        midiDevice->sendMessageNow(selectSessionLayout);
        midiDevice->sendMessageNow(clearSession);
        flashToDevice();
    }

    void flashToDevice()
    {
        if (update == current)
            return;
        current = update;
        for (int row = 0; row < 8; row++)
        {
            for (int col = 0; col < 8; col++)
            {
                midiDevice->sendMessageNow(layout.getGridItem(row, col));
            }
        }
        for (int i = 0; i < 8; i++)
        {
            midiDevice->sendMessageNow(layout.getFunction(i));
            midiDevice->sendMessageNow(layout.getScene(i));
        }
        midiDevice->sendMessageNow(layout.getLogo());
    }

private:
    void timerCallback() override
    {
        flashToDevice();
    }
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