#include "LaunchpadDriver.h"

void LaunchpadDriver::timerCallback()
{
    flashToDevice();
}

LaunchpadDriver::LaunchpadDriver()
{
    startTimer(10);
}

LaunchpadDriver::~LaunchpadDriver()
{
    if (midiDevice != nullptr)
    {
        midiDevice->sendMessageNow(disableDAW);
        midiDevice = nullptr;
    }
    stopTimer();
}

int LaunchpadDriver::processMidiPitch(const int pitch) const
{
    return layout.pitchMap(pitch);
}

void LaunchpadDriver::initialize(std::unique_ptr<juce::MidiOutput> device)
{
    midiDevice = std::move(device);
    midiDevice->sendMessageNow(disableDAW);
    midiDevice->sendMessageNow(enableDAW);
    midiDevice->sendMessageNow(selectSessionLayout);
    midiDevice->sendMessageNow(clearSession);
    flashToDevice();
}

void LaunchpadDriver::flashToDevice()
{
    if (update == current || midiDevice == nullptr)
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
    if ((++click) % 100 == 0)
    {
        midiDevice->sendMessageNow(layout.getGridItemFlash(0, 2));
        midiDevice->sendMessageNow(layout.getGridItemFlash(0, 1));
        midiDevice->sendMessageNow(layout.getGridItemFlash(0, 0));
    }
    if (click % 100 == 10)
    {
        midiDevice->sendMessageNow(layout.getGridItem(0, 2));
        midiDevice->sendMessageNow(layout.getGridItem(0, 1));
        midiDevice->sendMessageNow(layout.getGridItem(0, 0));
    }
}

const uint8 LaunchpadDriver::enableDAWSysex[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x0D, 0x10, 0x01, 0xF7};
const uint8 LaunchpadDriver::disableDAWSysex[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x0D, 0x10, 0x00, 0xF7};
const uint8 LaunchpadDriver::faderSysex[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x0D, 0x01, 0x00, 0x00,
                                             0x00, 0x00, 0x07, 0x25,
                                             0x01, 0x00, 0x08, 0x26,
                                             0x02, 0x00, 0x09, 0x27,
                                             0x03, 0x00, 0x10, 0x28,
                                             0x04, 0x01, 0x11, 0x29,
                                             0x05, 0x01, 0x12, 0x30,
                                             0x06, 0x01, 0x13, 0x31,
                                             0x07, 0x01, 0x14, 0x32,
                                             0xF7};
const uint8 LaunchpadDriver::enableFaderSysex[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x0D, 0x00, 0x0D, 0xF7};
const uint8 LaunchpadDriver::clearSessionSysex[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x0D, 0x12, 0x01, 0x00, 0x00, 0xF7};
const uint8 LaunchpadDriver::selectSessionLayoutSysex[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x0D, 0x00, 0x00, 0xF7};
const juce::MidiMessage LaunchpadDriver::enableDAW{LaunchpadDriver::enableDAWSysex, 9};
const juce::MidiMessage LaunchpadDriver::disableDAW{LaunchpadDriver::disableDAWSysex, 9};
const juce::MidiMessage LaunchpadDriver::fader{LaunchpadDriver::faderSysex, 10 + 4 * 8};
const juce::MidiMessage LaunchpadDriver::enableFader{LaunchpadDriver::enableFaderSysex, 9};
const juce::MidiMessage LaunchpadDriver::clearSession{LaunchpadDriver::clearSessionSysex, 11};
const juce::MidiMessage LaunchpadDriver::selectSessionLayout{LaunchpadDriver::selectSessionLayoutSysex, 9};