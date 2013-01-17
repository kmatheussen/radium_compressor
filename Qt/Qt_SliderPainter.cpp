/* Copyright 2012 Kjetil S. Matheussen

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

#include <QWidget>
#include <QGraphicsItem>
#include <QTimer>
#include <QSlider>
#include <QTimerEvent>

#include "EditorWidget.h"

#include "Qt_SliderPainter_proc.h"


const int k_timer_interval = 50;

#ifdef COMPILING_RADIUM
extern struct Root *root;
#else
extern QColor *g_colors;
#endif



static int scale(int x, int x1, int x2, int y1, int y2){
  return (int)scale((float)x,(float)x1,(float)x2,(float)y1,(float)y2);
}

static float gain2db(float val){
  if(val<=0.0f)
    return -100.0f;

  return 20*log10(val);
}

static float db2linear(float db){
  if(db<-70)
    return 0.0f;
  else if(db>40)
    return 1.0f;
  else
    return scale(db,-70.0f,40.0f,0.0f,1.0f);
}

#if 0
// Function iec_scale picked from meterbridge by Steve Harris.
// db is a value between 0 and 1.

static float iec_scale(float db) {
  
  db = 20.0f * log10f(db);


         float def = 0.0f; /* Meter deflection %age */
 
         if (db < -70.0f) {
                 def = 0.0f;
         } else if (db < -60.0f) {
                 def = (db + 70.0f) * 0.25f;
         } else if (db < -50.0f) {
                 def = (db + 60.0f) * 0.5f + 5.0f;
         } else if (db < -40.0f) {
                 def = (db + 50.0f) * 0.75f + 7.5;
         } else if (db < -30.0f) {
                 def = (db + 40.0f) * 1.5f + 15.0f;
         } else if (db < -20.0f) {
                 def = (db + 30.0f) * 2.0f + 30.0f;
         } else if (db < 0.0f) {
                 def = (db + 20.0f) * 2.5f + 50.0f;
         } else {
                 def = 100.0f;
         }
 
         return def * 2.0f / 200.0f;
}
#endif


static QColor mix_colors(const QColor &c1, const QColor &c2, float how_much){

  float a1 = how_much;
  float a2 = 1.0f-a1;

  if(c1.red()==0 && c1.green()==0 && c1.blue()==0){ // some of the black lines doesn't look look very good.
    int r = 74*a1 + c2.red()*a2;
    int g = 74*a1 + c2.green()*a2;
    int b = 74*a1 + c2.blue()*a2;

    return QColor(r,g,b);
  }else{

    int r = c1.red()*a1 + c2.red()*a2;
    int g = c1.green()*a1 + c2.green()*a2;
    int b = c1.blue()*a1 + c2.blue()*a2;

    return QColor(r,g,b);
  }
}



#define MAX_CHANNELS 8

//namespace{ // stupid c++ with all this useless syntax necessary to make a program appear to work. C++ is only great if you like a challenge and don't want things done.

namespace{
struct AutomationOrPeakData{
  float *value;
  int requested_pos;
  int last_drawn_pos;

  int *color;

  int ch;
  int num_ch;

  bool is_automation;
};
}

static int DATA_get_y1(AutomationOrPeakData *data, int height){
  return scale(data->ch,0,data->num_ch,1,height-1);
}

static int DATA_get_y2(AutomationOrPeakData *data, int height){
  return scale(data->ch+1,0,data->num_ch,1,height-1);
}

struct SliderPainter{

  struct Timer : public QTimer{
    SliderPainter *_painter;

    void timerEvent(QTimerEvent * e){
      //if(_painter->isVisible())
      //  printf("oh yeah. value: %f. last_drawn: %d, visible? %d\n",*data->value, data->last_drawn_pos,_painter->isVisible());

      if(_painter->isVisible()==false)
        return;

      for(unsigned int i=0;i<_painter->_data.size();i++){
        AutomationOrPeakData *data = _painter->_data.at(i);

        float gain;

        if(data->is_automation)
          gain = *data->value;
        else{
          float db = gain2db(*data->value);
          if(db>4.0f)
            _painter->_peak_color = 14;
          else if(db>0.0f)
            _painter->_peak_color = 6;
          else
            _painter->_peak_color = 13;

          gain = db2linear(db);
        }

        data->requested_pos = scale(gain,0.0f,1.0f,
                                    0.0f,(float)_painter->width())
                              - 1;

        if(data->last_drawn_pos != data->requested_pos){

          //printf("Painting. Last drawn: %d. requested: %d\n",data->last_drawn_pos,data->requested_pos);


          int y1 = DATA_get_y1(data,_painter->height());
          int y2 = DATA_get_y2(data,_painter->height());
          int height = y2-y1;

          //printf("y1: %d, y2: %d, height: %d. req: %d, last: %d\n",y1,y2,height,data->requested_pos,data->last_drawn_pos);
          _painter->update(data->requested_pos,
                           y1,4,height+1);
          _painter->update(data->last_drawn_pos,
                           y1,4,height+1);

          //printf("%d - %d\n",data->requested_pos,data->last_drawn_pos);
        }
      }

      //e->accept();
    }
  }; // struct Timer

  std::vector<AutomationOrPeakData*> _data;
  
  QSlider *_qslider;
  QGraphicsItem *_graphics_item;  // Either widget or _graphics_item must be set.

  int _x1;
  int _y1;
  int _x2;
  int _y2;

  float *_peak_values;
  bool _local_peak_values;
  float _automation_value;
  int _automation_color;
  int _peak_color;

  bool _alternative_color;

  int _value;

  int _num_automations;
  Timer _timer;

  QString _display_string;

  int _num_channels;


  int value(){
    if(_qslider!=NULL)
      return _qslider->value();
    else
      return _value;
  }

  int minimum(){
    if(_qslider!=NULL)
      return _qslider->minimum();
    else
      return 0;
  }

  int maximum(){
    if(_qslider!=NULL)
      return _qslider->maximum();
    else
      return 10000;
  }

  int orientation(){
    if(_qslider!=NULL)
      return _qslider->orientation();
    else
      return Qt::Horizontal;
  }

  bool isEnabled(){
    if(_qslider!=NULL)
      return _qslider->isEnabled();
    else
      return _num_channels>0;
  }

  bool isVisible(){
    if(_qslider!=NULL)
      return _qslider->isVisible() && !_qslider->isHidden();
    else
      return _graphics_item->isVisible();
  }

  void update(int x, int y, int width, int height){
    //fprintf(stderr,"x: %d. _qslider: %p\n",x,_qslider);
    if(x<0)
      x=0;
    if(y<0)
      y=0;

    //fprintf(stderr,"y: %d. _qslider: %p\n",y,_qslider);
    if(_qslider!=NULL){
      return _qslider->update(x,y,width,height);
    }else
      return _graphics_item->update(_x1+x, _y1+y, width, height);
  }

  int width(){
    if(_qslider!=NULL)
      return _qslider->width();
    else
      return _x2-_x1;
  }

  int height(){
    if(_qslider!=NULL)
      return _qslider->height();
    else
      return _y2-_y1;
  }

  void init(){
    _value = 0;
    _num_automations = 0;
    _num_channels = 0;
    _peak_values = NULL;
    _local_peak_values=false;
    _automation_color = 13;
    _peak_color = 13;
    _alternative_color = false;
  }

  SliderPainter(QSlider *widget)
    : _qslider(widget)
    , _graphics_item(NULL)
    , _automation_value(0.0f)
  {
    init();
  }

  SliderPainter(QGraphicsItem *graphics_item)
    : _qslider(NULL)
    , _graphics_item(graphics_item)
  {
    init();
  }

  ~SliderPainter(){
    if(_local_peak_values==true)
      free(_peak_values);

    for(unsigned int i=0;i<_data.size();i++) // Don't like iterators. Rather free memory manually than using them.
      delete _data.at(i);
  }

  AutomationOrPeakData *create_automation_data(){
    AutomationOrPeakData *data = new AutomationOrPeakData;

    data->value          = &_automation_value;
    data->requested_pos  = 0;
    data->last_drawn_pos = 0;

    data->color = &_automation_color;

    data->ch     = 0;
    data->num_ch = 1;

    data->is_automation = true;

    _data.push_back(data);
    if(_num_automations==0){
      _timer._painter = this;
      _timer.setInterval(k_timer_interval);
      _timer.start();
    }
    _num_automations++;
    
    return data;
  }

  float *obtain_automation_value_pointer(){
    create_automation_data();
    return &_automation_value;
  }

  int *obtain_automation_color_pointer(){
    return &_automation_color;
  }

  float *obtain_peak_value_pointers(int num_channels, float *peak_values){
    bool is_replacing = _peak_values!=NULL;

    if(peak_values==NULL) {
      _peak_values = (float*)calloc(sizeof(float*),num_channels);

      _local_peak_values = true;

    } else {

      _peak_values = peak_values;

    } 
    
    if(is_replacing){
      while(_num_automations>0)
        release_automation_pointers(); // Warning! This works only since _is_replacing==true just for the bottom bar volume slider (which doesn't have automations).
      _data.clear();
    }

    for(int ch=0;ch<num_channels;ch++){
      AutomationOrPeakData *data = create_automation_data();
      data->ch            = ch;
      data->num_ch        = num_channels;
      data->is_automation = false;
      data->value         = &_peak_values[ch];
      data->color         = &_peak_color;
    }

    return _peak_values;
  }

  void set_peak_value_pointers(int num_channels, float *pointers){
    obtain_peak_value_pointers(num_channels, pointers);
  }

  void release_automation_pointers(){
    _num_automations--;
    if(_num_automations==0)
      _timer.stop();
  }

  void paint(QPainter *p){
#ifdef COMPILING_RADIUM
    QColor *colors = static_cast<EditorWidget*>(root->song->tracker_windows->os_visual.widget)->colors;
#else
    QColor *colors = g_colors;
#endif

    QColor col1;
    QColor col1b;
    QColor col2;

    int col1num = 11;
    int col2num = 9;
    //int col1num = qrand() % 15;
    //int col2num = qrand() % 15;

    if(false && _qslider!=NULL){
      if(isEnabled()){
        QColor c1(70,70,33);
        QColor c2(59,98,33);
        col1 = mix_colors(c1,colors[col1num].light(90),0.5);
        col1b = mix_colors(c2,col1,0.3);
        ///col2 = mix_colors(editor->colors[col2num],editor->colors[col1num],0.8).light(95);//.light(90);
        col2 = colors[15];
      }else{
        col1 = colors[col1num].light(105);
        col1b = col1;
        col2 = colors[col2num].light(102);
      }
    }else{
      if(isEnabled()){
        QColor c(98,59,33);

        int colnum = 8;
        col1 = c.light(90);
        col1b = colors[13].light(100);
        //int colnum = 8;
        //col1 = editor->colors[colnum].light(90);
        //col1b = editor->colors[13].light(100);

        col2 = colors[colnum];
      }else{
        //col1 = editor->colors[col1num].light(105);
        col1 = mix_colors(colors[col1num], Qt::gray, 0.8f);
        col1b = mix_colors(colors[col1num].light(110), Qt::gray, 0.8f);
        col2 = mix_colors(colors[col2num], Qt::gray, 0.8f);
        //col2 = editor->colors[col2num].light(102);
      }

      if(_alternative_color==true){
        col1 = QColor(200,200,200);
        col2 = QColor(100,100,100);
      }

      if(_qslider!=NULL){
        col1.setAlpha(80);
        col1b.setAlpha(100);
      }else{
        col1.setAlpha(120);
        col1b.setAlpha(120);
      }
      col2.setAlpha(0);
    }

    if(orientation() == Qt::Vertical){
      int pos=scale(value(),maximum(),minimum(),0,height());
      p->fillRect(0,0,width(),height(),col2);
      p->fillRect(0,pos,width(),height()-pos,col1);
    }else{
      int pos=scale(value(),minimum(),maximum(),0,width());
      {
        QLinearGradient gradient(0,0,width(),height()*3/4);
        if(_qslider!=NULL){
          //gradient.setColorAt(0,col1.light(90));
          //gradient.setColorAt(1,col1.light(110));
          gradient.setColorAt(0,col1.light(100));
          gradient.setColorAt(1,col1b);
        }else{
          gradient.setColorAt(0,col1.light(150));
          gradient.setColorAt(1,col1b);
        }
        p->setPen(QPen(QColor(Qt::gray).light(50),1));
        p->setBrush(gradient);
        //p->fillRect(0   ,0, pos         ,height(),col1);
        p->drawRect(0   ,0, pos, height());
        p->setBrush(QBrush());
      }
      p->setPen(QPen(Qt::gray,1));
      p->fillRect(pos ,0, width()-pos ,height(),col2);
    }

    for(unsigned int i=0;i<_data.size();i++){
      AutomationOrPeakData *data = _data.at(i);
      int y1 = DATA_get_y1(data,height());
      int y2 = DATA_get_y2(data,height());
      int height = y2-y1;
      
      p->fillRect(data->requested_pos+1 ,y1+1,
                  2,                    height-1,
                  colors[*data->color]);
      
      //p->setPen(editor->colors[2].dark(10));
      p->setPen(QPen(colors[11].light(120),1));
      p->drawRect(data->requested_pos, y1,
                  3,                   height);
      
      data->last_drawn_pos = data->requested_pos;
    }

    if(1){
      p->setPen(QPen(colors[11].light(110),1));
      p->drawRect(0,0,width(),height());
    }

    QRect rect(5,2,width()-5,height()-2);

    if(_display_string!=""){
      QColor c(colors[1]);
      if(isEnabled()){
        c.setAlpha(160);
        p->setPen(QPen(c,1));//editor->colors[2].darker(500));
      }else{
        c.setAlpha(60);
        p->setPen(QPen(c,1));
      }
      p->drawText(rect, Qt::AlignLeft, _display_string);
    }
  }

};
//}

SliderPainter *SLIDERPAINTER_create(QSlider *qslider){
  return new SliderPainter(qslider);
}

SliderPainter *SLIDERPAINTER_create(QGraphicsItem *graphics_item, int x1, int y1, int x2, int y2){
  SliderPainter *painter = new SliderPainter(graphics_item);
  painter->_x1=x1;
  painter->_y1=y1;
  painter->_x2=x2;
  painter->_y2=y2;

  return painter;
}


void SLIDERPAINTER_delete(SliderPainter *painter){
  delete painter;
}

void SLIDERPAINTER_setValue(SliderPainter *painter, int value){
  painter->_value = value;
}

void SLIDERPAINTER_set_num_channels(SliderPainter *painter, int num_channels){
  painter->_num_channels = num_channels;
}

void SLIDERPAINTER_paint(SliderPainter *painter, QPainter *p){
  painter->paint(p);
}

float *SLIDERPAINTER_obtain_peak_value_pointers(SliderPainter *painter, int num_channels){
  return painter->obtain_peak_value_pointers(num_channels,NULL);
}

void SLIDERPAINTER_set_peak_value_pointers(SliderPainter *painter, int num_channels, float *pointers){
  painter->set_peak_value_pointers(num_channels, pointers);
}

float *SLIDERPAINTER_obtain_automation_value_pointer(SliderPainter *painter){
  return painter->obtain_automation_value_pointer();
}
int *SLIDERPAINTER_obtain_automation_color_pointer(SliderPainter *painter){
  return painter->obtain_automation_color_pointer();
}
void SLIDERPAINTER_release_automation_pointers(SliderPainter *painter){
  painter->release_automation_pointers();
}

// Used for chips where the slider controls input volume instead of output volume.
void SLIDERPAINTER_set_alternative_color(SliderPainter *painter){
  painter->_alternative_color = true;
}

void SLIDERPAINTER_set_string(SliderPainter *painter,QString string){
  painter->_display_string = string;
}

QString SLIDERPAINTER_get_string(SliderPainter *painter){
  return painter->_display_string;
}
