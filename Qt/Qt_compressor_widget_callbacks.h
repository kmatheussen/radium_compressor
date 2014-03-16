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

#include <stdio.h>

#include "Qt_compressor_widget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

#define INCLUDE_MYLINE_AND_MYRECT 1
#  include "compressor_vertical_sliders.cpp"
#undef INCLUDE_MYLINE_AND_MYRECT

namespace cvs{

  struct MyColor{
    QColor col;
    MyColor(int r,int g,int b)
      : col(r,g,b)
    {}

    MyColor(QColor col)
      : col(col)
    {}

    MyColor(int colnum)
#ifdef COMPILING_RADIUM
      : col(static_cast<EditorWidget*>(root->song->tracker_windows->os_visual.widget)->colors[colnum])
#else
        : col(g_colors[colnum])
#endif
    {}

    MyColor()
#ifdef COMPILING_RADIUM
      : col(static_cast<EditorWidget*>(root->song->tracker_windows->os_visual.widget)->colors[11])
#else
        : col(g_colors[11])
#endif
    {}

    MyColor lighter(int p){
      return MyColor(col.light(80));
    }

    MyColor darker(int p){
      return MyColor(col.light(80));
    }

    void setAlpha(int a){
      col.setAlpha(a);
    }
  };

  static inline MyColor mix_colors(const MyColor &c1, const MyColor &c2, float how_much){
    return MyColor(mix_colors(c1.col,c2.col,how_much));
  }

  struct MyImage{
    QImage image;
    QPainter qp;
    MyImage(int width,int height)
      : image(width,height,QImage::Format_RGB32)
      , qp(&image)
    {
    }
  };

  struct MyPainter{
    QPainter *p;
    MyColor col;
    QPen pen;
    QLinearGradient gradient;
    bool use_gradient;
    int _thickness;

    MyPainter(QPainter *p) 
      : p(p) 
      , use_gradient(false)
      , _thickness(1)
    {
      p->setPen(pen);
    }

    MyPainter(MyImage *image)
      : p(&image->qp)
      , use_gradient(false)
      , _thickness(1)
    {
      p->setPen(pen);
    }

    void setGradient(int x1, int y1, int x2, int y2, const MyColor &c1, const MyColor &c2){
      gradient = QLinearGradient(x1,y1,x2,y2);
      gradient.setColorAt(0,c1.col);
      gradient.setColorAt(1,c2.col);
      p->setPen(QPen(QBrush(gradient),_thickness));
      use_gradient = true;
    }

    void unsetGradient(){
      use_gradient=false;
    }

    void drawImage(int x,int y,MyImage *image){
      p->drawImage(x,y,image->image);
    }

    void drawRect(const int x1,const int y1,const int x2,const int y2, const MyColor &col){
      if(use_gradient==false){
        pen.setColor(col.col);
        p->setPen(pen);
      }
      p->drawRect(x1,y1,x2-x1,y2-y1);
    }
    void drawRect(const MyRect &rect,const MyColor &col){
      drawRect(rect.x1,rect.y1,rect.x2,rect.y2,col);
    }


    void fillRect(const int x1,const int y1,const int x2,const int y2, const MyColor &col){
      if(use_gradient==false)
        p->fillRect(x1,y1,x2-x1,y2-y1,col.col);
      else
        p->fillRect(x1,y1,x2-x1,y2-y1,QBrush(gradient));
    }

    void fillRect(const MyRect &rect,const MyColor &col){
      fillRect(rect.x1,rect.y1,rect.x2,rect.y2,col);
    }

    void setThickness(int thickness){
      _thickness=thickness;
      pen.setWidth(thickness);
    }

    void drawLine(const int x1,const int y1,const int x2,const int y2, const MyColor &col){
      if(use_gradient==false){
        pen.setColor(col.col);
        p->setPen(pen);
      }

      bool anti = x1!=x2 && y1!=y2;

      if(anti)
        p->setRenderHints(QPainter::Antialiasing,true);

      p->drawLine(x1,y1,x2,y2);

      if(anti)
        p->setRenderHints(QPainter::Antialiasing,false);
    }

    void drawLine(const MyLine &l, const MyColor &col){
      drawLine(l.x1,l.y1,l.x2,l.y2, col);
    }

    int getTextWidth(std::string text){
      const QFontMetrics fn = QFontMetrics(QFont());
      return fn.width(QString::fromStdString(text));
    }

    void drawText(const MyRect &rect, std::string text, const MyColor &col){
      if(use_gradient==false){
        pen.setColor(col.col);
        p->setPen(pen);
      }

      p->drawText(rect.x1,rect.y1,rect.width(),rect.height(),
                  Qt::AlignTop|Qt::AlignLeft,
                  QString::fromStdString(text));
    }

    void drawVerticalText(int x, int y, std::string text, const MyColor &col){
      if(use_gradient==false){
        pen.setColor(col.col);
        p->setPen(pen);
      }
      p->save();
      QPoint point(x,y); 
      p->translate(x,0);//point);//x, y);
      p->rotate(90); // or 270
      //p->scale((box.x2-box.x1)/12,1.0);
      //point = p->xFormDev(point);
      p->drawText(5,-5,QString::fromStdString(text));
      p->restore();
    }
  };

  struct MyWidget{
    struct MyQWidget : public QWidget{

      MyWidget *mywidget;
      MyQWidget(QWidget *parent, MyWidget *mywidget) : QWidget(parent),mywidget(mywidget){
      }

      void mousePressEvent ( QMouseEvent * event ){
        if(mywidget->mousePress(event->x(),event->y()))
          event->accept();
      }
      void mouseMoveEvent ( QMouseEvent * event ){
        if(mywidget->mouseMove(event->x(),event->y()))
          event->accept();
      }
      void mouseReleaseEvent ( QMouseEvent * event ){
        if(mywidget->mouseRelease(event->x(),event->y()))
          event->accept();
      }
      void paintEvent ( QPaintEvent * ev ){
        QPainter qp(this);
        MyPainter p(&qp);
        mywidget->repaint(&p);
      }
      virtual void resizeEvent ( QResizeEvent * event ){
        mywidget->resized();
      }
    };

    MyQWidget w;

    MyWidget(void *parent)
      : w(static_cast<QWidget*>(parent),this)
    {}

    void update(int x1,int y1,int x2,int y2){
      w.update(x1,y1,x2-x1,y2-y1);
    }
    void update(const MyRect &r){
      update(r.x1,r.y1,r.x2,r.y2);
    }

    void update(){
      w.update();
    }

    int width(){return w.width();}
    int height(){return w.height();}

    bool isVisible(){return w.isVisible();}
    bool isEnabled(){
      //printf("w is %s\n",w.isEnabled()?"enabled":"disabled");
      return w.isEnabled();
    }

    virtual void resized() = 0;

    //virtual void repaint(MyPainter *p){}
    virtual void repaint(MyPainter *p) = 0;

    virtual bool mousePress(int x, int y) = 0;

    virtual bool mouseMove(int x, int y) = 0;

    virtual bool mouseRelease(int x, int y) = 0;
  };

  struct MyTimer{

    struct MyQTimer : public QTimer{
      MyTimer *mytimer;

      MyQTimer(MyTimer *mytimer) : mytimer(mytimer) {}

      void timerEvent(QTimerEvent * e){ // virtual method from QTimer
        mytimer->timer();
      }
    };

    MyQTimer qtimer;

    MyTimer() : qtimer(this) {}

    virtual void timer() = 0;

    void startTimer(int interval_in_milliseconds){
      qtimer.setInterval(interval_in_milliseconds);
      qtimer.start();
    }
  };

} // namespace cvs



#include "../audio/system_compressor_wrapper_proc.h"


#ifdef COMPILING_RADIUM

// I'm pretty sure it's not necessary to test for plugin!=NULL in these three functons.
// Normally, I'm 100% sure it's safe, but these functions can also be called from a timer.
//
static void set_compressor_parameter(struct Patch *patch, int num,float value){
  SoundPlugin *plugin = (SoundPlugin*)patch->patchdata;
  if(plugin!=NULL)
    PLUGIN_set_effect_value(plugin, 0, plugin->type->num_effects+EFFNUM_COMP_RATIO+num, value, PLUGIN_NONSTORED_TYPE, PLUGIN_STORE_VALUE);
}

#else // COMPILING_RADIUM

static void set_compressor_parameter(struct Patch *patch, int num,float value){
  SoundPlugin *plugin = (SoundPlugin*)patch->patchdata;
  if(plugin!=NULL)
    COMPRESSOR_set_parameter(plugin->compressor,num,value);
}

#endif // #else COMPILING_RADIUM

static float get_compressor_parameter(struct Patch *patch, int num){
  SoundPlugin *plugin = (SoundPlugin*)patch->patchdata;
  if(plugin!=NULL)
    return COMPRESSOR_get_parameter(plugin->compressor,num);
  else
    return 0.0f;
}

static float get_graph_value(struct Patch *patch, int num){
  SoundPlugin *plugin = (SoundPlugin*)patch->patchdata;
  if(plugin!=NULL)
    return COMPRESSOR_get_graph_value(plugin->compressor,num);
  else
    return 0.0;
}


//#ifdef COMPILING_RADIUM
//namespace radium_comp{ // got an error trying to use anonymous namespace here.
  //#endif


#include "compressor_vertical_sliders.cpp"


static double OS_get_double_from_string(const char *s){
  QLocale::setDefault(QLocale::C);
  QString string(s);
  return string.toDouble();
}

static float read_float(FILE *file){
  char temp[512] = {0};

  fgets(temp,500,file);

  return OS_get_double_from_string(temp);
}


class Compressor_widget : public QWidget, public Ui::Compressor_widget{
  Q_OBJECT

 public:
  bool initing;
  cvs::Comp *comp;
  struct Patch *_patch;

 Compressor_widget(struct Patch *patch, QWidget *parent=NULL)
    : QWidget(parent)
    , _patch(patch)
  {
    initing = true;
    setupUi(this);

    QWidget *parent_for_comp = this;
    comp = new cvs::Comp(patch,parent_for_comp);

#ifdef COMPILING_RADIUM
    comp->w.setEnabled(false);
#else
    attack_slider->setEnabled(true);
    release_slider->setEnabled(true);
    enable_checkbox->setVisible(false);
#endif
    verticalLayout->insertWidget(1,&comp->w);

#ifdef COMPILING_RADIUM
    // Enable MyQSlider and MyQCheckBox to take care of undo/redo.
    {
      SoundPlugin *plugin = (SoundPlugin*)_patch->patchdata;
      attack_slider->_patch = patch;
      attack_slider->_effect_num = plugin->type->num_effects+EFFNUM_COMP_ATTACK;
      release_slider->_patch = patch;
      release_slider->_effect_num = plugin->type->num_effects+EFFNUM_COMP_RELEASE;
      enable_checkbox->_patch = patch;
      enable_checkbox->_effect_num = plugin->type->num_effects+EFFNUM_COMP_ONOFF;
    }
#endif

    update_gui();
    initing = false;
  }

  float get_exp_value(double val, double max_val, double y1, double y2){
    return scale_double(exp(val/max_val),
                        exp(0.0),expf(1.0),
                        y1,y2
                        );
  }

  float get_exp_inverted_value(double y, double max_val, double y1, double y2){
    return max_val * log(scale_double(y,y1,y2,exp(0.0),exp(1.0)));
  }

  void update_gui(){
    comp->set_gui_parameters();
    attack_slider->setValue(get_exp_inverted_value(get_compressor_parameter(_patch, COMP_EFF_ATTACK),1000,0,max_attack_release));
    release_slider->setValue(get_exp_inverted_value(get_compressor_parameter(_patch, COMP_EFF_RELEASE),1000,0,max_attack_release));

    SoundPlugin *plugin = (SoundPlugin*)_patch->patchdata;
    enable_checkbox->setChecked(plugin->comp.is_on);

    //paint_all=true;
    //updateBackgroundImage();
    update();
  }

  void load(QString filename, bool complain_if_file_not_found = true){
    FILE *file = fopen(filename,"r");

    if(file==NULL){
      if(complain_if_file_not_found){
        QMessageBox msgBox;
        msgBox.setText("Could not open file \""+filename+"\".");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
      }
      return;
    }
    
    set_compressor_parameter(_patch, COMP_EFF_RATIO,read_float(file)); // ratio
    set_compressor_parameter(_patch, COMP_EFF_THRESHOLD,read_float(file)); // threshold
    set_compressor_parameter(_patch, COMP_EFF_ATTACK,read_float(file)); // attack
    set_compressor_parameter(_patch, COMP_EFF_RELEASE,read_float(file)); // release
    //set_compressor_parameter(INPUT_VOLUME,read_float(file)); // input volume (currently not used)
    read_float(file); // input volume (currently not used)
    set_compressor_parameter(_patch, COMP_EFF_OUTPUT_VOLUME,read_float(file)); // output volume

    int x = read_float(file);
    int y = read_float(file);
    int width = read_float(file);
    int height = read_float(file);

    if(width>0 && height>0){
      move(x,y);
      resize(width,height);
    }

    fclose(file);

    update_gui();
  }

  void save(QString filename){
    FILE *file = fopen(filename,"w");

    if(file==NULL){
      QMessageBox msgBox;
      msgBox.setText("Could not save file.");
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
      return;
    }

    fprintf(file,"%f\n%f\n%f\n%f\n%f\n%f\n",
            get_compressor_parameter(_patch, COMP_EFF_RATIO),
            get_compressor_parameter(_patch, COMP_EFF_THRESHOLD),
            get_compressor_parameter(_patch, COMP_EFF_ATTACK),
            get_compressor_parameter(_patch, COMP_EFF_RELEASE),
            //get_compressor_parameter(INPUT_VOLUME),
            0.0f, // input volume (in dB)
            get_compressor_parameter(_patch, COMP_EFF_OUTPUT_VOLUME)
            );

    fprintf(file,"%d\n%d\n%d\n%d\n",
            pos().x(),pos().y(),
            width(),height()
            );

    fclose(file);
  }

public slots:

void on_enable_checkbox_toggled(bool val){
#ifdef COMPILING_RADIUM
  SoundPlugin *plugin = (SoundPlugin*)_patch->patchdata;
  PLUGIN_set_effect_value(plugin, 0, plugin->type->num_effects+EFFNUM_COMP_ONOFF, val==true?1.0:0.0, PLUGIN_NONSTORED_TYPE, PLUGIN_STORE_VALUE);

  attack_slider->setEnabled(val);
  release_slider->setEnabled(val);
  comp->background_image_must_be_updated=true;
  comp->w.setEnabled(val);
#endif
}

  void on_attack_slider_valueChanged(int val){
    float attack = get_exp_value(val,1000,0,max_attack_release);
    set_compressor_parameter(_patch, COMP_EFF_ATTACK,attack);
    char temp[512];
    sprintf(temp,"Attack: %.2fms",attack);
    SLIDERPAINTER_set_string(attack_slider->_painter, temp);
  }

  void on_release_slider_valueChanged(int val){
    float release = get_exp_value(val,1000,0,max_attack_release);
    set_compressor_parameter(_patch, COMP_EFF_RELEASE,release);
    char temp[512];
    sprintf(temp,"Release: %.2fms",release);
    SLIDERPAINTER_set_string(release_slider->_painter, temp);
  }

  void on_bypass_toggled(bool val){
    printf("bypass: %d\n",(int)val);
    set_compressor_parameter(_patch, COMP_EFF_BYPASS,val==true?1.0f:0.0f);
  }

  void on_load_button_pressed(void){
    printf("load pressed\n");

    QString filename = QFileDialog::getOpenFileName(this, "Load Effect configuration", "", "Radium Compressor Configuration (*.rcc)");

    if(filename=="")
      return;

    load(filename);
  }

  void on_save_button_pressed(void){
    printf("save pressed\n");

    QString filename = QFileDialog::getSaveFileName(this, "Save Effect configuration", "", "Radium Compressor Configuration (*.rcc)");

    if(filename=="")
      return;

    save(filename);
  }
};

  //#ifdef COMPILING_RADIUM
  //} // radium_comp namespace
  //#endif

