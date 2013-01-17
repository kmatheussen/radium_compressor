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


#include <math.h>
#include <string>

#include "audio/faudiostream/architecture/faust/audio/dsp.h"
#include "audio/faudiostream/architecture/faust/gui/UI.h"
#include "audio/faudiostream/architecture/faust/audio/jack-dsp.h"

#include <vector>

struct Meta
{
    void declare (const char* key, const char* value) { }
};

inline int 	max (unsigned int a, unsigned int b) { return (a>b) ? a : b; }
inline int 	max (int a, int b)		{ return (a>b) ? a : b; }

inline long 	max (long a, long b) 		{ return (a>b) ? a : b; }
inline long 	max (int a, long b) 		{ return (a>b) ? a : b; }
inline long 	max (long a, int b) 		{ return (a>b) ? a : b; }

inline float 	max (float a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (int a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (float a, int b) 		{ return (a>b) ? a : b; }
inline float 	max (long a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (float a, long b) 		{ return (a>b) ? a : b; }

inline double 	max (double a, double b) 	{ return (a>b) ? a : b; }
inline double 	max (int a, double b) 		{ return (a>b) ? a : b; }
inline double 	max (double a, int b) 		{ return (a>b) ? a : b; }
inline double 	max (long a, double b) 		{ return (a>b) ? a : b; }
inline double 	max (double a, long b) 		{ return (a>b) ? a : b; }
inline double 	max (float a, double b) 	{ return (a>b) ? a : b; }
inline double 	max (double a, float b) 	{ return (a>b) ? a : b; }


inline int	min (int a, int b)		{ return (a<b) ? a : b; }

inline long 	min (long a, long b) 		{ return (a<b) ? a : b; }
inline long 	min (int a, long b) 		{ return (a<b) ? a : b; }
inline long 	min (long a, int b) 		{ return (a<b) ? a : b; }

inline float 	min (float a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (int a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (float a, int b) 		{ return (a<b) ? a : b; }
inline float 	min (long a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (float a, long b) 		{ return (a<b) ? a : b; }

inline double 	min (double a, double b) 	{ return (a<b) ? a : b; }
inline double 	min (int a, double b) 		{ return (a<b) ? a : b; }
inline double 	min (double a, int b) 		{ return (a<b) ? a : b; }
inline double 	min (long a, double b) 		{ return (a<b) ? a : b; }
inline double 	min (double a, long b) 		{ return (a<b) ? a : b; }
inline double 	min (float a, double b) 	{ return (a<b) ? a : b; }
inline double 	min (double a, float b) 	{ return (a<b) ? a : b; }

#include "common/nsmtracker.h"
#include "Qt/Qt_MyQCheckBox.h"
#include "Qt/Qt_MyQSlider.h"
#include "Qt/Qt_MyQButton.h"
#include "audio/system_compressor.cpp"

 
static inline double scale_double(double x, double x1, double x2, double y1, double y2){
  return y1 + ( ((x-x1)*(y2-y1))
                /
                (x2-x1)
                );
}

static QColor das_colors[16];
QColor *g_colors=&das_colors[0];

class MyUI : public UI
{

 public:
  void declare(float* control_port, const char* key, const char* value) {
  }

  const char *_curr_box_name;

  void openFrameBox(const char* label) {_curr_box_name = label;}
  void openTabBox(const char* label) {_curr_box_name = label;}
  void openHorizontalBox(const char* label) {_curr_box_name = label;}
  void openVerticalBox(const char* label) {_curr_box_name = label;}
  void closeBox() {_curr_box_name = NULL;}

  std::vector<float*> _controllers;
  std::vector<float*> _graphs;

  void remove_last_item(){
    printf("Popping last effect\n");
    _controllers.pop_back();
  }

  void addEffect(const char *name, float* control_port, float min_value, float default_value, float max_value){
    printf("Adding effect %s %p\n",name,control_port);
    _controllers.push_back(control_port);
  }

  void addButton(const char* label, float* zone) {
    addEffect(label, zone, 0, 0, 1);
  }
  void addToggleButton(const char* label, float* zone) {
    addEffect(label, zone, 0, 0, 1);
  }
  void addCheckButton(const char* label, float* zone) {
    addEffect(label, zone, 0, 0, 1);
  }
  void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step) {
    addEffect(label, zone,  min, init, max);
  }
  void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step) {
    addEffect(label, zone,  min, init, max);
  }
  void addNumEntry(const char* label, float* zone, float init, float min, float max, float step) {
    addEffect(label, zone, min, init, max); // The INT effect format might not work. Need to go through the code first.
  }
  
  // -- passive widgets

  void addNumDisplay(const char* label, float* zone, int precision) {remove_last_item();}
  void addTextDisplay(const char* label, float* zone, const char* names[], float min, float max) {remove_last_item();}
  void addHorizontalBargraph(const char* label, float* zone, float min, float max) {
    _graphs.push_back(zone);
    //remove_last_item(); // remove metadata
    //next_peak = zone;
  }
  void addVerticalBargraph(const char* label, float* zone, float min, float max) {
    _graphs.push_back(zone);
    //remove_last_item(); // remove metadata
    //next_peak = zone;
  }

};

mydsp *d;
MyUI *ui;

static void set_compressor_parameter(int num,float value){
  float *controller = ui->_controllers.at(num);
  *controller = value;
  printf("Setting controller %d (%p) to %f (%f)\n",num,controller,value,*controller);
}

static float get_compressor_parameter(int num){
  float *controller = ui->_controllers.at(num);
  return *controller;
}

static float get_graph_value(int num){
  float *controller = ui->_graphs.at(num);
  return *controller;
}

#include "mQt_compressor_widget_callbacks.h"

static void set_colors(void){
  das_colors[0] = QColor("#828070");
  das_colors[1] = QColor("#00001d");
  das_colors[2] = QColor("#f1ece6");
  das_colors[3] = QColor("#98006d");
  das_colors[4] = QColor("#6437a7");
  das_colors[5] = QColor("#354353");
  das_colors[6] = QColor("#8b6735");
  das_colors[7] = QColor("#23405d");
  das_colors[8] = QColor("#310062");
  das_colors[9] = QColor("#8a8877");
  das_colors[10] = QColor("#7a7a7a");
  das_colors[11] = QColor("#696c5f");
  das_colors[12] = QColor("#a1b09b");
  das_colors[13] = QColor("#13511f");
  das_colors[14] = QColor("#8a0000");
  das_colors[15] = QColor("#7a7967");
}

int main(int argc, char **argv){
  if(argc>1)
    if(!strcmp(argv[1],"-help")
       || !strcmp(argv[1],"--help")
       || !strcmp(argv[1],"-h")
       )
      {
        printf("radium_compressor <settings filename>\n");
        exit(0);
      }
      

  QApplication app(argc, argv);
  set_colors();

  d = new mydsp;
  d->init(44100);
  ui = new MyUI;
  d->buildUserInterface(ui);
  
  jackaudio audio;
  audio.init("das compressor", d);
  //finterface->recallState(rcfilename);	
  audio.start();

  Compressor_widget compressor;
  if(argc>1)
    compressor.load(argv[1]);

  compressor.show();

  QPalette pal = compressor.palette();
  //pal.setColor(compressor.backgroundRole(), das_colors[11].light(200));
  pal.setColor(compressor.backgroundRole(), das_colors[11].light(100));
  compressor.setPalette(pal);

  app.exec();

  audio.stop();

  return 0;
}
