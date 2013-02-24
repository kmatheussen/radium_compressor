/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "slider_conversions.h"

#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace radium_compressor{

#include "system_compressor_wrapper.cpp"

}


//==============================================================================
RadiumCompressorAudioProcessor::RadiumCompressorAudioProcessor()
  : is_bypassing(false)
  , was_on(true)
  , was_off(false)
#if BUILD_DEMO
  , is_silencing(false)
  , time_since_last_silence(0)
  , time_since_last_sound(0)
#endif
{
  // We need to call the faust init function here in order to get the default parameter values.
  // However, since we don't know the sample rate yet, we just use 48000 for now. The real samplerate will be set when prepareToPlay is called.
  wrapper = new radium_compressor::Compressor_wrapper(48000.0f);
}

RadiumCompressorAudioProcessor::~RadiumCompressorAudioProcessor()
{
  delete wrapper;
}

//==============================================================================
const String RadiumCompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int RadiumCompressorAudioProcessor::getNumParameters()
{
  return radium_compressor::COMP_EFF_NUM_PARAMETERS;
}

float RadiumCompressorAudioProcessor::getParameter (int index)
{
  if(index==radium_compressor::COMP_EFF_BYPASS)
    return is_bypassing==true ? 1.0 : 0.0;

  float value = wrapper->get_parameter(index);

  switch(index){

  case radium_compressor::COMP_EFF_RATIO:
    //printf("Returning radium_compressor::COMP_EFF_RATIO. Value: %f. Slider value: %f. value2: %f\n",value,get_slider_from_ratio(value),get_ratio_from_slider(get_slider_from_ratio(value)));
    //return scale(value,0,60,0,1);
    return get_slider_from_ratio(value);

  case radium_compressor::COMP_EFF_THRESHOLD:
    return scale(value,min_threshold,max_threshold,0,1);

  case radium_compressor::COMP_EFF_ATTACK:
  case radium_compressor::COMP_EFF_RELEASE:
    return get_slider_from_attack_release(value);

  case radium_compressor::COMP_EFF_OUTPUT_VOLUME:
    return get_slider_from_makeup_gain(value);
  }

  return value;
}

void RadiumCompressorAudioProcessor::setParameter (int index, float newValue)
{
  bool updated = false;

  switch(index){

  case radium_compressor::COMP_EFF_RATIO:
    updated=wrapper->update_parameter_if_different(index,get_ratio_from_slider(newValue));
    break;
  case radium_compressor::COMP_EFF_THRESHOLD:
    updated=wrapper->update_parameter_if_different(index,get_threshold_from_slider(newValue));
    break;

  case radium_compressor::COMP_EFF_ATTACK:
    updated=wrapper->update_parameter_if_different(index,get_attack_release_from_slider(newValue));
    break;

  case radium_compressor::COMP_EFF_RELEASE:
    updated=wrapper->update_parameter_if_different(index,get_attack_release_from_slider(newValue));
    break;

  case radium_compressor::COMP_EFF_OUTPUT_VOLUME:
    updated=wrapper->update_parameter_if_different(index,get_makeup_gain_from_slider(newValue));
    break;

  case radium_compressor::COMP_EFF_BYPASS:
    {
      bool old=is_bypassing;
      is_bypassing = newValue>=0.5 ? true : false;
      updated = old!=is_bypassing;
      break;
    }
  }

  if(updated==true){
    RadiumCompressorAudioProcessorEditor *editor = dynamic_cast<RadiumCompressorAudioProcessorEditor *>(getActiveEditor()); // I guess we can use static_cast instead of dynamic_cast though.
    if(editor!=NULL)
      editor->update_gui(); // (update_gui() is RT safe.)
  }
}

const String RadiumCompressorAudioProcessor::getParameterName (int index)
{
  switch(index){
  case radium_compressor::COMP_EFF_RATIO:
    return "Ratio";
  case radium_compressor::COMP_EFF_THRESHOLD:
    return "Threshold";
  case radium_compressor::COMP_EFF_ATTACK:
    return "Attack";
  case radium_compressor::COMP_EFF_RELEASE:
    return "Release";
  case radium_compressor::COMP_EFF_OUTPUT_VOLUME:
    return "Makeup Gain";
  case radium_compressor::COMP_EFF_BYPASS:
    return "Bypass";
  default:
    return String::empty;
  }
}

const String RadiumCompressorAudioProcessor::getParameterText (int index)
{
  if(index==radium_compressor::COMP_EFF_BYPASS)
    return is_bypassing==true ? "On" : "Off";

  float value = wrapper->get_parameter(index);

  if(value==-0.0)
    value=0.0;

  switch(index){
  case radium_compressor::COMP_EFF_RATIO:
    if(value<1.0f)
      return "1:"+String(1.0/value,1);
    else
      return String(value,1)+":1";
  case radium_compressor::COMP_EFF_THRESHOLD:
    return String(value>0.0 ? "+" : "") + String(value,1) + " dB";
  case radium_compressor::COMP_EFF_ATTACK:
    return String(value,1) + " ms";
  case radium_compressor::COMP_EFF_RELEASE:
    return String(value,1) + " ms";
  case radium_compressor::COMP_EFF_OUTPUT_VOLUME:
    return String(value>0.0 ? "+" : "") + String(value,1) + " dB";
  default:
    return String::empty;
  }
}

const String RadiumCompressorAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String RadiumCompressorAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool RadiumCompressorAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool RadiumCompressorAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool RadiumCompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RadiumCompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RadiumCompressorAudioProcessor::silenceInProducesSilenceOut() const
{
    return true;
}

int RadiumCompressorAudioProcessor::getNumPrograms()
{
    return 0;
}

int RadiumCompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RadiumCompressorAudioProcessor::setCurrentProgram (int index)
{
}

const String RadiumCompressorAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void RadiumCompressorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void RadiumCompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

  wrapper->init(sampleRate);
  //printf("***** \n\n\n **** samplearet: %f / %f  **** \n\n\n",getSampleRate(),sampleRate);
}

void RadiumCompressorAudioProcessor::reset(){
  wrapper->reset();
}

void RadiumCompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

static void RT_fade_out(float *sound, int num_frames){
  float num_frames_plus_1 = num_frames+1.0f;
  int i;
  float val = (num_frames / num_frames_plus_1);
  float inc = val - ( (num_frames-1) / num_frames_plus_1);

  for(i=0;i<num_frames;i++){
    sound[i] *= val;
    val -= inc;
  }
}

static void RT_fade_in(float *sound, int num_frames){
  float num_frames_plus_1 = num_frames+1.0f;
  for(int i=0;i<num_frames;i++)
    sound[i] *= (i+1.0f)/num_frames_plus_1;
}


// fade in 'input'
static void RT_crossfade_in(float *input, float *output, int num_frames){
  RT_fade_in(input, num_frames);
  RT_fade_out(output, num_frames);
  
  for(int i=0;i<num_frames;i++)
    output[i] += input[i];
}

// fade out 'input'
static void RT_crossfade_out(float *input, float *output, int num_frames){
  RT_fade_out(input, num_frames);
  RT_fade_in(output, num_frames);
  
  for(int i=0;i<num_frames;i++)
    output[i] += input[i];
}

#define MAX_BUF_SIZE 1024

static void process(RadiumCompressorAudioProcessor *processor, float **buf, int num_frames){
  bool is_on = processor->is_bypassing==false;
  bool was_on = processor->was_on;
  bool was_off = processor->was_off;

  if(is_on==false && was_on==false) // bypass
    return;

  if(is_on==true && was_off==false){

    COMPRESSOR_process(processor->wrapper, buf, buf, num_frames);
    //memset(buf[0],0,num_frames*sizeof(float)); // for testing fade-in / fade-out
    //memset(buf[1],0,num_frames*sizeof(float));

  } else {


#if BUILD_MONO
    float tempsound[MAX_BUF_SIZE];
    float *temp[1] = {&tempsound[0]};
#else
    float tempsound[MAX_BUF_SIZE*2];
    float *temp[2] = {&tempsound[0],
                      &tempsound[num_frames]};
#endif
    
    COMPRESSOR_process(processor->wrapper, buf, temp, num_frames);
    //memset(temp[0],0,num_frames*sizeof(float));
    //memset(temp[1],0,num_frames*sizeof(float));
  
    if(is_on==true && processor->was_off==true){ // fade in from bypass

      RT_crossfade_in(temp[0], buf[0], num_frames);
#if BUILD_MONO==0
      RT_crossfade_in(temp[1], buf[1], num_frames);
#endif

      processor->was_off = false;
      processor->was_on  = true;
      
    } else { // fade out to bypass
      
      RT_crossfade_out(temp[0], buf[0], num_frames);
#if BUILD_MONO==0
      RT_crossfade_out(temp[1], buf[1], num_frames);
#endif
      
      processor->was_off = true;
      processor->was_on = false;
      
    }
  }
}

static const float silence_duration = 5.0f;
static const float demo_interval = 30.0f;

#if BUILD_DEMO
void processDemoSilence(RadiumCompressorAudioProcessor *processor, float **buf, int num_frames){

  if(processor->is_silencing==true){

    processor->time_since_last_sound += num_frames;

    if(processor->time_since_last_sound > processor->getSampleRate()*silence_duration){
      processor->time_since_last_sound = 0;
      processor->is_silencing = false;

      RT_fade_in(buf[0],num_frames);
#if BUILD_MONO==0
      RT_fade_in(buf[1],num_frames);
#endif

      RadiumCompressorAudioProcessorEditor *editor = dynamic_cast<RadiumCompressorAudioProcessorEditor *>(processor->getActiveEditor()); // I guess we can use static_cast instead of dynamic_cast though.
      if(editor!=NULL)
        editor->show_demo_text(false);

    }else{
      memset(buf[0],0,num_frames*sizeof(float));
#if BUILD_MONO==0
      memset(buf[1],0,num_frames*sizeof(float));
#endif

    }

  }else{ // i.e silencing==false

    processor->time_since_last_silence += num_frames;

    if(processor->time_since_last_silence > processor->getSampleRate()*demo_interval){

      processor->is_silencing = true;
      processor->time_since_last_silence = 0;
      RT_fade_out(buf[0],num_frames);
#if BUILD_MONO==0
      RT_fade_out(buf[1],num_frames);
#endif

      RadiumCompressorAudioProcessorEditor *editor = dynamic_cast<RadiumCompressorAudioProcessorEditor *>(processor->getActiveEditor()); // I guess we can use static_cast instead of dynamic_cast though.
      if(editor!=NULL)
        editor->show_demo_text(true);
    }
  }
}
#endif //BUILD_DEMO


void RadiumCompressorAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
  const bool is_on = is_bypassing==false;

  if(is_on==false && was_on==false) // bypass
    return;

  const int num_frames = buffer.getNumSamples();

#if BUILD_MONO
  float *buf[1] = {buffer.getSampleData (0)};
#else
  float *buf[2] = {buffer.getSampleData (0),
                   buffer.getSampleData (1)};
#endif

#if BUILD_DEMO
  processDemoSilence(this,buf,num_frames);
#endif

  for(int i=0 ; i<num_frames ; i+=MAX_BUF_SIZE){      
    int num_frames_here;
    if(i+MAX_BUF_SIZE>=num_frames)
      num_frames_here = num_frames - i;
    else
      num_frames_here = MAX_BUF_SIZE;
      
    //fprintf(stderr,"i: %d, num_frames_here: %d, MAX_BUF_SIZE: %d\n",i,num_frames_here,MAX_BUF_SIZE);    
    process(this,buf,num_frames_here);
    //fprintf(stderr,"finished\n");
      
    buf[0] += MAX_BUF_SIZE;
#if BUILD_MONO==0
    buf[1] += MAX_BUF_SIZE;
#endif
  }
}

//==============================================================================
bool RadiumCompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* RadiumCompressorAudioProcessor::createEditor()
{
    return new RadiumCompressorAudioProcessorEditor (this);
}

//==============================================================================
void RadiumCompressorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RadiumCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RadiumCompressorAudioProcessor();
}
