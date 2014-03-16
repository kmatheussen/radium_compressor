PREFIX ?= /usr/local
bindir ?= $(PREFIX)/bin
libdir ?= $(PREFIX)/lib

#FIX locally. Line below removed from dsp file.
#effect.bypass2(checkbox("[6] bypass"), compressor) 

# These two commands are used by me, since most of the source files are stored in the Radium tree.
# You would normally just run "make" to compile the program.
#
# make copy_files && make all && ./radium_compressor 
# make copy_files && rm -f benchmark && make benchmark && ./benchmark && ./benchmark && ./benchmark

OPT = -O3 -ffast-math
OPT_DSP = -O3 -ffast-math -Wno-strict-aliasing
#-funroll-loops
CPP = g++ -DDEBUG  -Wall -msse -mfpmath=sse -DUSE_QT_REQTYPE -DUSE_QT4 -g -I. -IQt
#  -fpredictive-commoning -ftree-vectorize 
# -funroll-loops -fira-loop-pressure -fipa-cp-clone -ftree-loop-distribute-patterns

#-finline-functions 

#FAUST = /home/kjetil/faudiostream/compiler/faust -vec
FAUST = faust -vec

# only used for copy files
RADIUM_PATH = /home/kjetil/radium-qt4

all: system_compressor_wrapper.o myladspa.o
	cd Qt && ./create_source_from_ui.sh `../find_moc_and_uic_paths.sh uic` `../find_moc_and_uic_paths.sh moc` compressor_widget
	$(CPP) Qt/Qt_SliderPainter.cpp `pkg-config --cflags Qt3Support` -c  $(OPT)
	$(CPP) main.cpp Qt_SliderPainter.o system_compressor_wrapper.o myladspa.o -DCOMPILING_STANDALONE -Iaudio/faudiostream/architecture/ `pkg-config --libs --cflags Qt3Support``pkg-config --cflags --libs liblo`  -ljack -o radium_compressor  $(OPT)
# /usr/lib64/libprofiler.so.0
# make copy_files && make all && CPUPROFILE=ls.prof ./radium_compressor 

clean:
	rm -f *.o

install:
	install -d $(DESTDIR)$(bindir)
	cp radium_compressor $(DESTDIR)$(bindir)/


uninstall:
	rm -f $(DESTDIR)$(bindir)/radium_compressor

# ladpa plugin
radium_compressor.so: audio/system_compressor.cpp myladspa.o system_compressor_wrapper_ladspa.o
	$(CPP) Qt/Qt_SliderPainter.cpp `pkg-config --cflags Qt3Support` -c -fPIC $(OPT)
	$(CPP) -DCOMPILING_LADSPA main.cpp -Iaudio/faudiostream/architecture/ `pkg-config --libs --cflags Qt3Support` -c -fPIC  $(OPT)
	$(CPP) `pkg-config --libs Qt3Support` myladspa.o system_compressor_wrapper_ladspa.o -shared -fPIC -o radium_compressor.so

system_compressor_wrapper.o: audio/system_compressor_wrapper.cpp audio/system_compressor.cpp
	$(CPP) -Ifaudiostream/architecture `pkg-config --cflags QtCore` -DCOMPILING_STANDALONE audio/system_compressor_wrapper.cpp -c -fPIC $(OPT_DSP)

system_compressor_wrapper_ladspa.o: audio/system_compressor_wrapper.cpp audio/system_compressor.cpp
	$(CPP) -Ifaudiostream/architecture `pkg-config --cflags QtCore` -DCOMPILING_LADSPA audio/system_compressor_wrapper.cpp -c -fPIC -o system_compressor_wrapper_ladspa.o $(OPT_DSP)

myladspa.o: myladspa.cpp audio/system_compressor.cpp
	$(CPP) -DCOMPILING_LADSPA -Ifaudiostream/architecture `pkg-config --cflags QtCore` myladspa.cpp -fPIC -c  $(OPT_DSP)

audio/system_compressor.cpp: audio/system_compressor.dsp standalone_compressor.dsp
	cp standalone_compressor.dsp audio/
	$(FAUST) -cn Faust_system_compressor audio/standalone_compressor.dsp >audio/system_compressor.cpp

#audio/system_compressor_ladspa.cpp: audio/system_compressor.dsp standalone_compressor.dsp
#	cp standalone_compressor.dsp audio/
#	$(FAUST) -cn Faust_system_compressor -a myladspa.cpp audio/standalone_compressor.dsp >audio/system_compressor_ladspa.cpp


copy_files:
	cp $(RADIUM_PATH)/Qt/qt4_compressor_widget.ui Qt/
	cp $(RADIUM_PATH)/Qt/compressor_vertical_sliders.cpp Qt/
	cp $(RADIUM_PATH)/Qt/Qt_compressor_widget_callbacks.h Qt/
	cp $(RADIUM_PATH)/Qt/Qt_MyQCheckBox.h Qt/
	cp $(RADIUM_PATH)/Qt/Qt_MyQButton.h Qt/
	cp $(RADIUM_PATH)/Qt/Qt_MyQSlider.* Qt/
	cp $(RADIUM_PATH)/Qt/Qt_SliderPainter* Qt/
	cp $(RADIUM_PATH)/Qt/create_source_from_ui.sh Qt/
	cp $(RADIUM_PATH)/Qt/EditorWidget.h Qt/
	cp $(RADIUM_PATH)/Qt/OS_Visual.h Qt/
	cp $(RADIUM_PATH)/Qt/Qt_ReqType.h Qt/
	cp $(RADIUM_PATH)/Qt/Qt_instruments_proc.h Qt/

	cp $(RADIUM_PATH)/audio/system_compressor.dsp audio/
	cp $(RADIUM_PATH)/audio/fast_log_exp.dsp audio/
	cp $(RADIUM_PATH)/audio/system_compressor_wrapper_proc.h audio/
	cp $(RADIUM_PATH)/audio/system_compressor_wrapper.cpp audio/
	cp $(RADIUM_PATH)/audio/typepunning.h audio/
	cp $(RADIUM_PATH)/audio/undo_audio_effect_proc.h audio/
	cp $(RADIUM_PATH)/audio/SoundPlugin.h audio/
	cp $(RADIUM_PATH)/audio/SoundPlugin_proc.h audio/
	cp $(RADIUM_PATH)/audio/Smooth_proc.h audio/

	cp $(RADIUM_PATH)/common/nsmtracker.h common/
	cp $(RADIUM_PATH)/common/debug_proc.h common/
	cp $(RADIUM_PATH)/common/memory_proc.h common/
	cp $(RADIUM_PATH)/common/nsmtracker_events.h common/
	cp $(RADIUM_PATH)/common/keyboard_sub_ids.h common/
	cp $(RADIUM_PATH)/common/OS_error_proc.h common/
	cp $(RADIUM_PATH)/common/OS_Semaphores.h common/
	cp $(RADIUM_PATH)/common/mouse.h common/
	cp $(RADIUM_PATH)/common/windows_proc.h common/
	cp $(RADIUM_PATH)/common/visual_proc.h common/
	cp $(RADIUM_PATH)/common/OS_visual_input.h common/
	cp $(RADIUM_PATH)/common/visual_op_queue_proc.h common/
	cp $(RADIUM_PATH)/common/undo_patchvoice_proc.h common/
	cp $(RADIUM_PATH)/common/instruments_proc.h common/
	cp $(RADIUM_PATH)/common/vector_proc.h common/
	cp $(RADIUM_PATH)/common/settings_proc.h common/
	cp $(RADIUM_PATH)/common/hashmap_proc.h common/

	cp $(RADIUM_PATH)/find_moc_and_uic_paths.sh .
	cp -a $(RADIUM_PATH)/audio/faudiostream audio/


# Note that 0.9.55 runs the benchmark program a bit faster than 0.9.46.
# I guess it's because of more min/max functions in 0.9.55, but it could also
# be because of some castings.
benchmark:
	$(FAUST) -a bench.cpp audio/system_compressor.dsp >compressor_benchmark.cpp 
	$(CPP) benchmark.cpp -Iaudio -o benchmark -lpthread $(OPT_DSP)

# original db2linear/linear2db: 60 MB/s
# fast pow2/log: 420 MB/s
# faster pow2/log: 530 MB/s
# lookup table: 420 MB/s
# no db<->linear conversion: 610 MB/s

# Relative accuracy of faster pow2 / faster log: 0.0152579 / 0.0130367
# Relative accuracy of fast pow2 / fast log: 1.58868e-05 / 2.09348e-05

#/home/kjetil/faudiostream/compiler/faust -a ladspa.cpp system_compressor.dsp >ladspa_compressor.cpp
#g++ ladspa_compressor.cpp -I /home/kjetil/faudiostream/architecture/ -shared -fPIC -o ladspa_compressor.so

VSTPATH = /home/kjetil/Dropbox/radium_build/vstsdk2.4
vst:
	$(FAUST) audio/standalone_compressor.dsp >compressor_vst.cpp
	$(CPP) Qt/Qt_SliderPainter.cpp `pkg-config --cflags Qt3Support` -c -fPIC
	$(CPP) -DCOMPILING_VST main.cpp -Iaudio/faudiostream/architecture/ `pkg-config --libs --cflags Qt3Support` -c -fPIC
	$(CPP) -I $(VSTPATH)/public.sdk/source/vst2.x/ -I $(VSTPATH)/pluginterfaces/vst2.x/ -I $(VSTPATH) `pkg-config --cflags QtGui` `pkg-config --libs QtGui` vstplugin.cpp -Iaudio $(VSTPATH)/public.sdk/source/vst2.x/audioeffectx.cpp $(VSTPATH)/public.sdk/source/vst2.x/audioeffect.cpp  $(VSTPATH)/public.sdk/source/vst2.x/vstplugmain.cpp main.o Qt_SliderPainter.o -shared -o radium_compressor.so -fPIC

