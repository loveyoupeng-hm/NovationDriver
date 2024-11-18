#include "VstProcessorEditor.h"
using namespace juce;

VstProcessorEditor::VstProcessorEditor(VstProcessor *p)
    : AudioProcessorEditor(p),
      keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
      startTime(juce::Time::getMillisecondCounterHiRes() * 0.001), processor{p}
{
    startTimer(2000);
    setOpaque(true);

    addAndMakeVisible(midiInputListLabel);
    midiInputListLabel.setText("MIDI Input:", juce::dontSendNotification);
    midiInputListLabel.attachToComponent(&midiInputList, true);

    addAndMakeVisible(midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
    auto midiInputs = juce::MidiInput::getAvailableDevices();

    juce::StringArray midiInputNames;

    for (auto input : midiInputs)
        midiInputNames.add(input.name);

    midiInputList.addItemList(midiInputNames, 1);
    midiInputList.onChange = [this]
    { setMidiInput(midiInputList.getSelectedItemIndex()); };

    // find the first enabled device and use that by default
    for (auto input : midiInputs)
    {
        if (processor->getDeviceManager()->isMidiInputDeviceEnabled(input.identifier))
        {
            setMidiInput(midiInputs.indexOf(input));
            break;
        }
    }

    // if no enabled devices were found just use the first one in the list
    if (midiInputList.getSelectedId() == 0)
        setMidiInput(0);

    addAndMakeVisible(keyboardComponent);
    keyboardState.addListener(this);
    addAndMakeVisible(launchpad);
    launchpad.add(*this);
    addAndMakeVisible(midiMessagesBox);
    midiMessagesBox.setSize(600, 400);
    midiMessagesBox.setMultiLine(true);
    midiMessagesBox.setReturnKeyStartsNewLine(true);
    midiMessagesBox.setReadOnly(true);
    midiMessagesBox.setScrollbarsShown(true);
    midiMessagesBox.setCaretVisible(false);
    midiMessagesBox.setPopupMenuEnabled(true);
    midiMessagesBox.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x32ffffff));
    midiMessagesBox.setColour(juce::TextEditor::outlineColourId, juce::Colour(0x1c000000));
    midiMessagesBox.setColour(juce::TextEditor::shadowColourId, juce::Colour(0x16000000));

    setSize(600, 1200);
}

VstProcessorEditor::~VstProcessorEditor()
{
    keyboardState.removeListener(this);
    processor->getDeviceManager()->removeMidiInputDeviceCallback(juce::MidiInput::getAvailableDevices()[midiInputList.getSelectedItemIndex()].identifier, this);
}

void VstProcessorEditor::buttonPressed(uint8 x, uint8 y)
{
    logMessage("Button Pressed " + juce::String(x) + ", " + juce::String(y));
}

void VstProcessorEditor::scenePressed(uint8 scene)
{
    logMessage("Scene Pressed " + juce::String(scene));
}

void VstProcessorEditor::timerCallback()
{
    logMessage(processor->getName() + " " + juce::String(processor->getBpm()));
}

void VstProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);
}

void VstProcessorEditor::resized()
{
    auto area = getLocalBounds();

    midiInputList.setBounds(area.removeFromTop(36).removeFromRight(getWidth() - 150).reduced(8));
    keyboardComponent.setBounds(area.removeFromTop(80).reduced(8));
    midiMessagesBox.setBounds(area.removeFromTop(300).reduced(8));
    launchpad.setBounds(area.reduced(8));
}

void VstProcessorEditor::logMessage(const juce::String m)
{
    if (!isEnabled())
        return;
    midiMessagesBox.moveCaretToEnd();
    midiMessagesBox.insertTextAtCaret(m + juce::newLine);
}

juce::String VstProcessorEditor::getMidiMessageDescription(const juce::MidiMessage &m)
{
    if (m.isNoteOn())
        return "Note on " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
    if (m.isNoteOff())
        return "Note off " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
    if (m.isProgramChange())
        return "Program change " + juce::String(m.getProgramChangeNumber());
    if (m.isPitchWheel())
        return "Pitch wheel " + juce::String(m.getPitchWheelValue());
    if (m.isAftertouch())
        return "After touch " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3) + ": " + juce::String(m.getAfterTouchValue());
    if (m.isChannelPressure())
        return "Channel pressure " + juce::String(m.getChannelPressureValue());
    if (m.isAllNotesOff())
        return "All notes off";
    if (m.isAllSoundOff())
        return "All sound off";
    if (m.isMetaEvent())
        return "Meta event";

    if (m.isController())
    {
        juce::String name(juce::MidiMessage::getControllerName(m.getControllerNumber()));

        if (name.isEmpty())
            name = "[" + juce::String(m.getControllerNumber()) + "]";

        return "Controller " + name + ": " + juce::String(m.getControllerValue());
    }

    return juce::String::toHexString(m.getRawData(), m.getRawDataSize());
}

/** Starts listening to a MIDI input device, enabling it if necessary. */
void VstProcessorEditor::setMidiInput(int index)
{
    auto list = juce::MidiInput::getAvailableDevices();

    for (auto newInput : list)
    {
        processor->getDeviceManager()->removeMidiInputDeviceCallback(newInput.identifier, this);
        if (!processor->getDeviceManager()->isMidiInputDeviceEnabled(newInput.identifier))
            processor->getDeviceManager()->setMidiInputDeviceEnabled(newInput.identifier, true);
        processor->getDeviceManager()->addMidiInputDeviceCallback(newInput.identifier, this);
        midiInputList.setSelectedId(index + 1, juce::dontSendNotification);
    }

    lastInputIndex = index;
}

// These methods handle callbacks from the midi device + on-screen keyboard..
void VstProcessorEditor::handleIncomingMidiMessage(juce::MidiInput *source, const juce::MidiMessage &message)
{
    const juce::ScopedValueSetter<bool> scopedInputFlag(isAddingFromMidiInput, true);
    int note = message.getNoteNumber();
    if (source->getName().equalsIgnoreCase("Launchpad Mini MK3 LPMiniMK3 DA"))
        note = processor->getDriver()->processMidiPitch(note);
    auto msg = juce::MidiMessage(message);
    msg.setNoteNumber(note);
    keyboardState.processNextMidiEvent(msg);
    postMessageToList(msg, source->getName());
}

void VstProcessorEditor::handleNoteOn(juce::MidiKeyboardState *, int midiChannel, int midiNoteNumber, float velocity)
{
    if (!isAddingFromMidiInput)
    {
        auto m = juce::MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
        m.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList(m, "On-Screen Keyboard");
    }
}

void VstProcessorEditor::handleNoteOff(juce::MidiKeyboardState *, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
    if (!isAddingFromMidiInput)
    {
        auto m = juce::MidiMessage::noteOff(midiChannel, midiNoteNumber);
        m.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList(m, "On-Screen Keyboard");
    }
}

void VstProcessorEditor::postMessageToList(const juce::MidiMessage &message, const juce::String &source)
{
    (new IncomingMessageCallback(this, message, source))->post();
}

void VstProcessorEditor::addMessageToList(const juce::MidiMessage &message, const juce::String &source)
{
    auto time = message.getTimeStamp() - startTime;

    auto hours = ((int)(time / 3600.0)) % 24;
    auto minutes = ((int)(time / 60.0)) % 60;
    auto seconds = ((int)time) % 60;
    auto millis = ((int)(time * 1000.0)) % 1000;

    auto timecode = juce::String::formatted("%02d:%02d:%02d.%03d",
                                            hours,
                                            minutes,
                                            seconds,
                                            millis);

    auto description = getMidiMessageDescription(message);

    juce::String midiMessageString(timecode + "  -  " + description + " (" + source + ")"); // [7]
    logMessage(midiMessageString);
}
