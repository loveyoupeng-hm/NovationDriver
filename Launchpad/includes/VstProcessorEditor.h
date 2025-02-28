#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "VstProcessor.h"
#include "LaunchpadComponent.h"
#include "LaunchpadDriver.h"

using namespace juce;

class VstProcessorEditor : public juce::AudioProcessorEditor,
                           private LaunchpadBroadcaster::Listener,
                           private juce::Timer,
                           private juce::MidiInputCallback,
                           private juce::MidiKeyboardStateListener

{
public:
    VstProcessorEditor(VstProcessor *p);
    ~VstProcessorEditor() override;
    void paint(juce::Graphics &g) override;
    void resized() override;
    void buttonPressed(uint8 x, uint8 y) override;
    void scenePressed(uint8 scene) override;
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
    juce::ComboBox midiInputList;           // [2]
    juce::Label midiInputListLabel;
    int lastInputIndex = 0;             // [3]
    bool isAddingFromMidiInput = false; // [4]

    juce::MidiKeyboardState keyboardState;         // [5]
    juce::MidiKeyboardComponent keyboardComponent; // [6]
    juce::TextEditor midiMessagesBox;
    double startTime;
    VstProcessor *_processor;
    LaunchpadComponent launchpad;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VstProcessorEditor)
};