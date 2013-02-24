#ifndef MONO_CONFIG_H
#define MONO_CONFIG_H

#define JucePlugin_ManufacturerCode 'RaAu'

#define JucePlugin_Name                   "Radium Compressor Mono"
#define JucePlugin_Desc                   "Radium Compressor Mono"

#define JucePlugin_MaxNumInputChannels    1
#define JucePlugin_MaxNumOutputChannels   1
#define JucePlugin_PreferredChannelConfigurations  {1, 1}

#if BUILD_DEMO
#  define JucePlugin_PluginCode             'RdCM'
#else
#  define JucePlugin_PluginCode             'RaCM'
#endif

#endif
