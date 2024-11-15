/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             HandlingMidiEventsTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Handles incoming midi events.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2019, linux_make

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>
class VstProcessorEditor;

using namespace juce;

//==============================================================================
class VstProcessor : public juce::AudioProcessor, private juce::ChangeBroadcaster
{
public:
    //==============================================================================
    VstProcessor()
        : AudioProcessor(BusesProperties()) // add no audio buses at all
    {
    }

    //==============================================================================
    void prepareToPlay(double, int) override { }
    void releaseResources() override {}
    void processBlock(AudioBuffer<double> &, MidiBuffer &) override {}
    void processBlock(AudioBuffer<float> &buffer, MidiBuffer &midi) override;

    //==============================================================================
    bool isMidiEffect() const override { return true; }

    //==============================================================================

    bool hasEditor() const override { return true; }

    //==============================================================================
    const String getName() const override { return "VstProcessor"; }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    double getTailLengthSeconds() const override { return 0; }

    //==============================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const String getProgramName(int) override { return "None"; }
    void changeProgramName(int, const String &) override {}
    AudioProcessorEditor *createEditor();

    //==============================================================================
    void getStateInformation(MemoryBlock &) override{}

    void setStateInformation(const void *, int) override {}

private:
    //==============================================================================

    //==============================================================================

    juce::AudioPlayHead *head;

    VstProcessorEditor *editor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VstProcessor)
};
