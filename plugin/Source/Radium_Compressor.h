/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  24 Feb 2013 3:20:37pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_RADIUMCOMPRESSOR_RADIUMCOMPRESSOR_DBD6CB93__
#define __JUCER_HEADER_RADIUMCOMPRESSOR_RADIUMCOMPRESSOR_DBD6CB93__

//[Headers]     -- You can add your own extra header files here --

/* Copyright 2013 Kjetil S. Matheussen

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. */

#include "JuceHeader.h"

#include "slider_conversions.h"

namespace custom_gui{
namespace cvs{
  struct Comp;
}
}

namespace su{
  struct SliderUpdater;
}

struct MySlider : public Component{
  Patch *patch;
  std::string slider_name;
  double value;
  int effect_num;

 MySlider(String componentName)
   : patch(NULL)
   , value(0.0)
   , effect_num(-1)
  {
  }


 double getValue(){
   return value;
 }

 void setValue(double val){
   //printf("setting %f. old: %f\n",val,value);
   value = val;
   if(patch!=NULL && effect_num!=-1)
     set_compressor_parameter(patch, effect_num, value);
   repaint();
 }

 void handle_mouse(int x){
   float val = scale(x,0,getWidth(),0,1);
   if(val<0)
     val=0;
   if(val>1)
     val=1;
   setValue(get_attack_release_from_slider(val));
 }

 void mouseDown 	(	const MouseEvent & 	event	){
   handle_mouse(event.x);
 }
 void mouseDrag 	(	const MouseEvent & 	event	){
   handle_mouse(event.x);
 }
 void mouseUp 	(	const MouseEvent & 	event	){
   handle_mouse(event.x);
 }

 void setRange(double a,double b, double c){}

 void setSliderStyle(Slider::SliderStyle newStyle){}

 void setTextBoxStyle(Slider::TextEntryBoxPosition 	newPosition,
		bool 	isReadOnly,
		int 	textEntryBoxWidth,
		int 	textEntryBoxHeight )
 {}

 void setColor(	int 	colourId,
		const Colour & 	colour ){}

 void addListener(	Slider::Listener * 	listener){}

 void setSkewFactor(double skew){}

#if 0
  String getTextFromValue(double value){
    //return "";
    return String("Attack: ") + String(value) + " ms";
  }
#endif

  void paint (Graphics& g);

};


//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Radium_Compressor  : public Component,
                           public SliderListener,
                           public ButtonListener
{
public:
    //==============================================================================
    Radium_Compressor (Patch *patch);
    ~Radium_Compressor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    custom_gui::cvs::Comp *comp;
    Patch *patch;
    su::SliderUpdater *slider_updater;

#if BUILD_DEMO

    bool show_demo_text;

    struct DemoTextTimer : Timer{
      bool last_show_value;
      Radium_Compressor *compressor;

      void init(Radium_Compressor *compressor){
  this->compressor = compressor;
  last_show_value = false;
  startTimer(1000);
      }

      void timerCallback(){
         bool compressor_show = compressor->show_demo_text;
         if(compressor_show!=last_show_value){
            compressor->repaint();
            last_show_value = compressor_show;
         }
      }
    };

    DemoTextTimer demo_text_timer;

#endif

  void check_sliders_and_checkbox_updates(){
    float attack_in_processor = get_compressor_parameter(patch, COMP_EFF_ATTACK);
    float attack_in_slider = attack_slider->getValue();
    if(fabs(attack_in_processor - attack_in_slider) > 0.1)
      attack_slider->setValue(attack_in_processor);

    float release_in_processor = get_compressor_parameter(patch, COMP_EFF_RELEASE);
    float release_in_slider = release_slider->getValue();
    if(fabs(release_in_processor - release_in_slider) > 0.1)
      release_slider->setValue(release_in_processor);

    if(patch->is_bypassing != bypass_button->getToggleState())
      bypass_button->setToggleState(patch->is_bypassing,false);
  }

  int getRealWidth(){
    return attack_slider->getX()*2 + attack_slider->getWidth();
  }

  int getRealHeight(){
    return bypass_button->getY() + release_slider->getY() + release_slider->getHeight();
  }

    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);

    // Binary resources:
    static const char* radium_256x256x32_transparent_png;
    static const int radium_256x256x32_transparent_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    MySlider* attack_slider;
    MySlider* release_slider;
    ToggleButton* bypass_button;
    HyperlinkButton* hyperlinkButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Radium_Compressor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCER_HEADER_RADIUMCOMPRESSOR_RADIUMCOMPRESSOR_DBD6CB93__
