#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "VstProcessor.h"
#include "LaunchpadComponent.h"

using namespace juce;

class VstProcessorEditor : public juce::AudioProcessorEditor,
                           private juce::Timer,
                           private juce::MidiInputCallback,
                           private juce::MidiKeyboardStateListener
{
public:
    VstProcessorEditor(VstProcessor *p);
    ~VstProcessorEditor() override;
    void paint(juce::Graphics &g) override;
    void resized() override;

private:
    void timerCallback() override;
    void logMessage(const juce::String m);
    static juce::String getMidiMessageDescription(const juce::MidiMessage &m);

    /** Starts listening to a MIDI input device, enabling it if necessary. */
    void setMidiInput(int index);
    // These methods handle callbacks from the midi device + on-screen keyboard..
    void handleIncomingMidiMessage(juce::MidiInput *source, const juce::MidiMessage &message) override;
    void handleNoteOn(juce::MidiKeyboardState *, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState *, int midiChannel, int midiNoteNumber, float /*velocity*/) override; // This is used to dispach an incoming message to the message thread
    class IncomingMessageCallback : public juce::CallbackMessage
    {
    public:
        IncomingMessageCallback(VstProcessorEditor *o, const juce::MidiMessage &m, const juce::String &s)
            : owner(o), message(m), source(s)
        {
        }

        void messageCallback() override
        {
            if (owner != nullptr)
                owner->addMessageToList(message, source);
        }

        Component::SafePointer<VstProcessorEditor> owner;
        juce::MidiMessage message;
        juce::String source;
    };

    void postMessageToList(const juce::MidiMessage &message, const juce::String &source);
    void addMessageToList(const juce::MidiMessage &message, const juce::String &source);

    //==============================================================================
    juce::AudioDeviceManager deviceManager; // [1]
    juce::ComboBox midiInputList;           // [2]
    juce::Label midiInputListLabel;
    int lastInputIndex = 0;             // [3]
    bool isAddingFromMidiInput = false; // [4]

    juce::MidiKeyboardState keyboardState;         // [5]
    juce::MidiKeyboardComponent keyboardComponent; // [6]
    std::unique_ptr<juce::MidiOutput> midiDevice{nullptr};

    juce::TextEditor midiMessagesBox;
    double startTime;
    VstProcessor *processor;
    LaunchpadComponent launchpad;

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

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VstProcessorEditor)
};