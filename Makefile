
# These two commands are used by me, since most of the source files are stored in the Radium tree.
# To compile this program, just running "make" should be enough.
#
# make copy_files && make all && ./radium_compressor 
# make copy_files && rm -f benchmark && make benchmark && ./benchmark && ./benchmark && ./benchmark


CPP = g++ -DDEBUG -O3 -Wall -msse -mfpmath=sse -DUSE_QT_REQTYPE -DUSE_QT4 -g -I. -ffast-math -IQt

FAUST = /home/kjetil/faudiostream/compiler/faust -vec

# only used for copy files
RADIUM_PATH = /home/kjetil/radium-qt4

all: audio/system_compressor.cpp
	cd Qt && ./create_source_from_ui.sh `../find_moc_and_uic_paths.sh uic` `../find_moc_and_uic_paths.sh moc` compressor_widget
	$(CPP) Qt/Qt_SliderPainter.cpp `pkg-config --cflags Qt3Support` -c
	$(CPP) main.cpp Qt_SliderPainter.o -Iaudio/faudiostream/architecture/ `pkg-config --libs --cflags Qt3Support` -ljack -o radium_compressor


copy_files:
	cp $(RADIUM_PATH)/Qt/qt4_compressor_widget.ui Qt/
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


audio/system_compressor.cpp: audio/system_compressor.dsp
	$(FAUST) audio/system_compressor.dsp >audio/system_compressor.cpp

benchmark:
	$(FAUST) -a bench.cpp system_compressor.dsp >compressor_benchmark.cpp
	g++ benchmark.cpp $(OPTS) -o benchmark -lpthread

# original db2linear/linear2db: 60 MB/s
# fast pow2/log: 420 MB/s
# faster pow2/log: 530 MB/s
# lookup table: 420 MB/s
# no db<->linear conversion: 610 MB/s

# Relative accuracy of faster pow2 / faster log: 0.0152579 / 0.0130367
# Relative accuracy of fast pow2 / fast log: 1.58868e-05 / 2.09348e-05

