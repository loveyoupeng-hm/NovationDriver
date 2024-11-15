#include "VstProcessor.h"
#include "VstProcessorEditor.h"
using namespace juce;

void VstProcessor::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midi)
{
    if(!midi.isEmpty())
    {
        this->sendChangeMessage();
    }

    if (!head)
    {
        auto tmp = getPlayHead();
        if (tmp != head && tmp != nullptr)
        {
            head = tmp;
            if (editor)
            {
                auto position = head->getPosition();
                if (position->getIsPlaying())
                {
                    this->sendChangeMessage();
                }

                // if (static_cast<bool>(position))
                // editor->logMessage("BPM : ");
            }
        }
    }

    juce::ignoreUnused(buffer);
    juce::ignoreUnused(midi);
    // A pure MIDI plugin shouldn't be provided any audio data
    jassert(buffer.getNumChannels() == 0);
    // midi.clear();

    // however we use the buffer to get timing information
    // auto numSamples = buffer.getNumSamples();

    // get note duration
    // auto noteDuration = static_cast<int>(std::ceil(rate * 0.25f * (0.1f + (1.0f - (*speed)))));

    // for (const auto metadata : midi)
    // {
    //     const auto msg = metadata.getMessage();
    //     if (msg.isNoteOn())
    //         notes.add(msg.getNoteNumber());
    //     else if (msg.isNoteOff())
    //         notes.removeValue(msg.getNoteNumber());
    // }

    // if ((time + numSamples) >= noteDuration)
    // {
    //     auto offset = jmax(0, jmin((int)(noteDuration - time), numSamples - 1));

    //     if (lastNoteValue > 0)
    //     {
    //         midi.addEvent(MidiMessage::noteOff(1, lastNoteValue), offset);
    //         lastNoteValue = -1;
    //     }

    //     if (notes.size() > 0)
    //     {
    //         currentNote = (currentNote + 1) % notes.size();
    //         lastNoteValue = notes[currentNote];
    //         midi.addEvent(MidiMessage::noteOn(1, lastNoteValue, (uint8)127), offset);
    //     }
    // }

    // time = (time + numSamples) % noteDuration;
}

AudioProcessorEditor *VstProcessor::createEditor()
{
    auto result = new VstProcessorEditor(this);
    addChangeListener(result);
    return result;
}