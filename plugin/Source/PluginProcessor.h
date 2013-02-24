/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PLUGINPROCESSOR_H_DA1601D__
#define __PLUGINPROCESSOR_H_DA1601D__

#ifndef BUILD_DEMO
#  error "BUILD_DEMO not defined"
#endif

#ifndef BUILD_DEMO
#  error "BUILD_MONO not defined"
#endif


#if BUILD_MONO==1
#include "mono_config.h"
#else
#include "stereo_config.h"
#endif

#include "../JuceLibraryCode/JuceHeader.h"

class RadiumCompressorAudioProcessorEditor;

namespace radium_compressor{
  class Compressor_wrapper;
}

//==============================================================================
/**
*/
class RadiumCompressorAudioProcessor  : public AudioProcessor
{
public:
  radium_compressor::Compressor_wrapper *wrapper;
  bool is_bypassing;
  bool was_on;
  bool was_off;

#if BUILD_DEMO
  bool is_silencing;
  int time_since_last_silence;
  int time_since_last_sound;
#endif

    //==============================================================================
    RadiumCompressorAudioProcessor();
    ~RadiumCompressorAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
  void reset();
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RadiumCompressorAudioProcessor)
};

#endif  // __PLUGINPROCESSOR_H_DA1601D__
