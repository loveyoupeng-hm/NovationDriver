#include "VstProcessor.h"
#include "VstProcessorEditor.h"
#include <iostream>
using namespace juce;

VstProcessor::VstProcessor()
    : AudioProcessor(BusesProperties()) // add no audio buses at all
{
}

VstProcessor::~VstProcessor()
{
  if (deviceManager != nullptr)
    delete deviceManager;
  deviceManager = nullptr;
  if (driver != nullptr)
    delete driver;
  driver = nullptr;
}

void VstProcessor::initialize()
{
  if (driver != nullptr)
    return;
  sessionMode = true;
  driver = new LaunchpadDriver();
  auto list = juce::MidiInput::getAvailableDevices();
  deviceManager = new juce::AudioDeviceManager();
  for (auto newInput : list)
  {
    if (newInput.name.equalsIgnoreCase("Launchpad Mini MK3 LPMiniMK3 DA") || newInput.name.equalsIgnoreCase("LPMiniMK3 MIDI"))
    {
      auto output = juce::MidiOutput::openDevice(newInput.identifier);
      if (output)
        driver->initialize(std::move(output));
      deviceManager->removeMidiInputDeviceCallback(newInput.identifier, this);
      if (!deviceManager->isMidiInputDeviceEnabled(newInput.identifier))
        deviceManager->setMidiInputDeviceEnabled(newInput.identifier, true);
      deviceManager->addMidiInputDeviceCallback(newInput.identifier, this);
    }
  }
}

void VstProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  ignoreUnused(samplesPerBlock);
  initialize();
  midiNotes.clear();
  lastNoteValue = -1;
  time = 0;
  rate = static_cast<float>(sampleRate);
  numSamples = samplesPerBlock;
  currentNote = 0;
  notes.clear();
  notes.push_back(48);
  notes.push_back(48 + 2 + 2);
  notes.push_back(48 + 2 + 2 + 1 + 2);
  notes.push_back(48 + 12);
}

LaunchpadDriver *VstProcessor::getDriver()
{
  return driver;
}

void VstProcessor::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midi)
{
  updatePlayHead();

  juce::ignoreUnused(buffer);
  juce::ignoreUnused(midi);
  // A pure MIDI plugin shouldn't be provided any audio data
  // midi.clear();

  // however we use the buffer to get timing information
  // auto numSamples = buffer.getNumSamples();
  if (sessionMode.load())
  {
    midiNotes.clear();
    for (const auto metadata : midi)
    {
      auto msg = metadata.getMessage();
      int note = msg.getNoteNumber();
      note = driver->processMidiPitch(note);
      auto copy = juce::MidiMessage(msg);
      copy.setNoteNumber(note);
      midiNotes.addEvent(copy, metadata.samplePosition);
    }

    midi.clear();
    midi.swapWith(midiNotes);
  }

  // get note duration
  // 60.0 / bpm * 0.5

  auto noteDuration = static_cast<int>(std::ceil(juce::jmap(60.0f / bpm * 0.5f, 0.0f, 1.0f, 0.f, rate)));

  if (play && (time + numSamples) >= noteDuration)
  {
    auto offset = jmax(0, jmin((int)(noteDuration - time), numSamples -
                                                               1));

    if (lastNoteValue > 0)
    {
      midi.addEvent(MidiMessage::noteOff(1, lastNoteValue), offset);
      lastNoteValue = -1;
    }

    if (notes.size() > 0)
    {
      currentNote %= static_cast<int>(notes.size());
      lastNoteValue = notes[static_cast<size_t>(currentNote++)];
      midi.addEvent(MidiMessage::noteOn(1, lastNoteValue, (uint8)127),
                    offset);
    }
  }

  time = (time + numSamples) % noteDuration;
}

juce::AudioDeviceManager *VstProcessor::getDeviceManager()
{
  return deviceManager;
}

void VstProcessor::handleIncomingMidiMessage(juce::MidiInput *source, const juce::MidiMessage &message)
{
  if (!source->getName().equalsIgnoreCase("Launchpad Mini MK3 LPMiniMK3 DA"))
    return;
  if (message.isController())
  {
    const int cc = message.getControllerNumber();
    if (cc >= 96 && cc <= 98)
      sessionMode = false;
    else if (cc == 95)
      sessionMode = true;
  }
}

void VstProcessor::updatePlayHead()
{
  auto *head = getPlayHead();
  if (head != nullptr)
  {
    auto position = head->getPosition();
    if (position.hasValue())
    {
      //bpm = static_cast<float>(*(position->getBpm()));
      if (bpm < 0.1)
        bpm = 100.0f;

      play = position->getIsPlaying();
    }
  }
}

AudioProcessorEditor *VstProcessor::createEditor()
{
  initialize();
  editor = new VstProcessorEditor(this);
  return editor;
}