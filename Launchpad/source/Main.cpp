#include "VstProcessor.h"

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new VstProcessor();
}
