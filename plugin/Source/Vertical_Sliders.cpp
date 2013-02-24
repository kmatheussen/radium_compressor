/*
  ==============================================================================

    Radium_Compressor(2).h
    Created: 8 Feb 2013 8:02:02pm
    Author:  kjetil

  ==============================================================================
*/

#ifndef __RADIUM_COMPRESSOR2_H_31885997__
#define __RADIUM_COMPRESSOR2_H_31885997__

//#include "../JuceLibraryCode/JuceHeader.h"

#include "system_compressor_wrapper_proc.h"

//struct Patch;



#if 0 // testing
static void set_compressor_parameter(Patch *patch, int num,float value){
}

static float get_compressor_parameter(Patch *patch, int num){
return Random::getSystemRandom().nextFloat()*25;
  return 0.0f;
}

static float get_graph_value(Patch *patch, int num){
return Random::getSystemRandom().nextFloat()*25;
  return 0.0;
}
#endif


namespace vertical_sliders{

#include "Juce_MyWidget.h"
#include "compressor_vertical_sliders.cpp"

}

#endif  // __RADIUM_COMPRESSOR2_H_31885997__
