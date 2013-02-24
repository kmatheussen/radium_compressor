#ifndef STEREO_CONFIG_H
#define STEREO_CONFIG_H

#define JucePlugin_ManufacturerCode 'RaAu'

#define JucePlugin_Name                   "Radium Compressor Stereo"
#define JucePlugin_Desc                   "Radium Compressor Stereo"

#define JucePlugin_MaxNumInputChannels    2
#define JucePlugin_MaxNumOutputChannels   2
#define JucePlugin_PreferredChannelConfigurations  {2, 2}

#if BUILD_DEMO
#  define JucePlugin_PluginCode             'RdCS'
#else
#  define JucePlugin_PluginCode             'RaCS'
#endif

#endif
