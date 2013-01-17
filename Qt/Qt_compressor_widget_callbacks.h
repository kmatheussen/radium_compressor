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

#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

#include "Qt_compressor_widget.h"

const int min_db = -40;
const int max_db = 40;

const int k_timer_interval = 50;

const int max_attack_release = 500;

const float def_threshold = 0.7f;
const float def_ratio = 0.3;
const float def_makeupgain = 0.3f;

enum{
  RATIO=0,
  THRESHOLD,
  ATTACK,
  RELEASE,
  //INPUT_VOLUME,
  OUTPUT_VOLUME,
  BYPASS
};


struct Comp : public QWidget{
  // All values have the value 0 at top, and 1 at bottom, and:
  // 0 <= val1 <= val2 <= 1
  // (sometimes < intead of <=)

  // slider 1
  double in_val1; // 0
  double in_val2; // threshold

  // slider 2
  double out_val1; // used to calculate ratio ( ratio = in_val2/(out_val1-in_val2) )
  double out_val2; // threshold

  // slider 3
  double vol_val1; // used to calculate volume ( volume = out_val1/vol_val1 )
  double vol_val2; // threshold * volume

  int curr_slider;

  struct Box{
    int x1,y1,x2,y2;
    int p1,p2;

    Box(int _x1, int _y1, int _x2, int _y2, float val1,float val2)
      : x1(_x1)
      , y1(_y1)
      , x2(_x2)
      , y2(_y2)
    {
      p1 = scale(val1,0,1,y1,y2);
      p2 = scale(val2,0,1,y1,y2);
    }

    bool inside(int x,int y){
      return x>=x1 && x<x2 && y>y1 && y<y2;
    }
    bool in_val1(int y){
      int p=(p1+p2)/2;
      return y<p;
    }
    bool in_val2(int y){
      int p=(p1+p2)/2;
      return y>=p;
    }
  };

  struct Peaks{
    const static int num_peaks = 30;//24;
    
    float peaks[num_peaks];
    int peak_pos;
    
    Peaks()
      : peak_pos(0)
    {
      for(int i=0;i<num_peaks;i++)
        peaks[i]=0.0f;
    }

    void add_peaks(float peak){
      //printf("adding peak %.2f. ",peak);
      peaks[peak_pos] = peak;
      peak_pos++;
      if(peak_pos==num_peaks)
        peak_pos=0;
    }
    
    float get_min_peak(){
      float ret=max_db;
      for(int i=0;i<num_peaks;i++)
        if(peaks[i]<ret)
          ret=peaks[i];
      return ret;
    }

    float get_max_peak(){
      float ret=min_db;
      for(int i=0;i<num_peaks;i++)
        if(peaks[i]>ret)
          ret=peaks[i];
      return ret;
    }

    void paint_peak_area(QPainter *p, const Box &box){
      QColor col(40,90,140);
      col.setAlpha(80);

      int min_peak = scale(get_min_peak(),min_db,max_db, box.y2, box.y1);
      int max_peak = scale(get_max_peak(),min_db,max_db, box.y2, box.y1);

      p->fillRect(box.x1, min_peak,
                  box.x2-box.x1, max_peak-min_peak,
                  col);      
    }

  };

  Peaks peaks_in;
  Peaks peaks_out;
  Peaks peaks_vol;

  struct Timer : public QTimer{
    Comp *_comp;
    void timerEvent(QTimerEvent * e){
      //printf("hepp\n");
      _comp->update();
    }
  };

  Timer timer;

  Comp(QWidget *parent)
    : QWidget(parent)
    , in_val1(0.0)
    , in_val2(def_threshold)
    , out_val1(def_ratio)
    , out_val2(def_threshold)
    , vol_val1(def_makeupgain)
    , vol_val2(def_threshold)
    , curr_slider(0)
  {
    timer._comp = this;
    timer.setInterval(k_timer_interval);
    timer.start();

    set_threshold(def_threshold);
    set_compressor_parameters();
  }

  int get_box_width(){
    return (width()-1) / 4;
  }

  int get_width_between_boxes(){
    return get_box_width() / 2;
  }

  Box get_slider1_parms(){
    return Box(
               0,0,
               get_box_width(),
               height()-1,
               in_val1,in_val2);
  }
  
  Box get_slider2_parms(){
    return Box(
               get_box_width() + get_width_between_boxes(), 0,
               get_box_width() + get_width_between_boxes() + get_box_width(),
               height()-1,
               out_val1,out_val2);
  }

  Box get_slider3_parms(){
    return Box(
               get_box_width() + get_width_between_boxes() + get_box_width() + get_width_between_boxes(), 0,
               get_box_width() + get_width_between_boxes() + get_box_width() + get_width_between_boxes() + get_box_width(),
               height()-1,
               vol_val1, vol_val2);
  }

  double get_threshold(){
    return scale(in_val2,
                 0,1,
                 max_db,min_db);
  }

  void set_threshold_gui(){
    set_threshold(scale(get_compressor_parameter(THRESHOLD),max_db,min_db,0,1));
  }

  double get_ratio(){
    double ratio = fabs(in_val2-out_val1)<0.00001
      ? 60 
      : in_val2 / (in_val2-out_val1);
    //printf("in_val2: %f, out_val1: %f, ratio: %f. (%f)\n",in_val2,out_val1,ratio,abs(in_val2-out_val1));
    if(ratio>60)
      ratio=60;
    return ratio;
  }

  void set_ratio_gui(){
    double ratio = get_compressor_parameter(RATIO);
    set_ratio(in_val2 - in_val2/ratio);
  }

  double get_makeup_gain(){
    return scale(vol_val1,0,1,max_db,min_db) - scale(out_val1,0,1,max_db,min_db);
  }

  // set_ratio must be called first.
  void set_makeup_gain_gui(){
    //printf("out_val1: %f. vol: %f, scale: %f, min/max: %d/%d\n",out_val1,get_compressor_parameter(5),scale(get_compressor_parameter(5),min_db,max_db,0,1),(int)min_db,(int)max_db);
    float vol = get_compressor_parameter(OUTPUT_VOLUME);
    float addvol = scale(out_val1,0,1,max_db,min_db) + vol;
    //printf("out_val1: %f. vol: %f, addvol: %f, scale: %f\n",out_val1,vol,addvol,scale(out_val1,0,1,max_db,min_db));
    set_makeupgain(scale(addvol,max_db,min_db,0,1));
  }

  // After loading.
  void set_gui_parameters(){
    set_threshold_gui();
    set_ratio_gui();
    set_makeup_gain_gui();
  }

  void set_compressor_parameters(){
    double input_volume = 1.0;
    double threshold = get_threshold();
    double ratio = get_ratio();
    double output_volume = get_makeup_gain();
      
    set_compressor_parameter(RATIO,ratio);
    set_compressor_parameter(THRESHOLD, threshold);
    //set_compressor_parameter(2,attack);
    //set_compressor_parameter(3,release);
    //set_compressor_parameter(4,input_volume);
    set_compressor_parameter(OUTPUT_VOLUME,output_volume);

    printf("%f / %f\n%f / %f\n%f / %f\ninput_volume:\t\t %.2f\n"
           "threshold:\t\t %f.2f\n"
           "ratio:\t\t\t %f.2f\n"
           "output_volume:\t\t %.2f\n\n",in_val2,out_val2,scale(vol_val1,0,1,max_db,min_db),scale(out_val1,0,1,max_db,min_db),vol_val1,out_val1,
           input_volume,
           threshold,
           ratio,
           output_volume);
  }

  enum{
    THRESHOLD_SLIDER = 1,
    RATIO_SLIDER,
    MAKEUPGAIN_SLIDER
  };

  double p_startpos;
  int y_startpos;

  void set_makeupgain(float val){
    vol_val1=val;
#if 0 // allow this.
    if(vol_val1<0.0)
      vol_val1=0.0;
#endif
    if(vol_val1>0.9999)
      vol_val1=0.9999;
    vol_val2 = vol_val1 + (out_val2-out_val1);// scale(out_val2, out_val1, 1.0, vol_val1, 1.0);
  }

  void set_ratio(float val){
    double vol_diff = vol_val1 - out_val1;
    out_val1=val;
    if(out_val1<0.0)
      out_val1=0.0;
    if(out_val1>in_val2-0.0001)
      out_val1=in_val2-0.0001;
    set_makeupgain(vol_diff + out_val1);
  }

  void set_threshold(float val){
    double old_ratio_factor = out_val1 / in_val2;

    in_val2 = val;
    if(in_val2<=0.0)
      in_val2=0.00001;
    if(in_val2>0.9999)
      in_val2=0.9999;
    
    out_val2 = in_val2;
    set_ratio(old_ratio_factor*in_val2);
  }

  void handle_mouse_event ( QMouseEvent * event ){
    //printf("Got mouse press event %d / %d\n",(int)event->x(),(int)event->y());

    double new_val = p_startpos + scale(event->y()-y_startpos,0,height(),0,1);
    printf("p_startpos: %f\n",p_startpos);

    switch(curr_slider){
    case THRESHOLD_SLIDER:
      set_threshold(new_val);
      break;
    case RATIO_SLIDER:
      set_ratio(new_val);
      break;
    case MAKEUPGAIN_SLIDER:
      set_makeupgain(new_val);
      break;
    }

    set_compressor_parameters();

    update();

    event->accept();
  }

  void mousePressEvent ( QMouseEvent * event )
  {
    Box in_box = get_slider1_parms();
    Box out_box = get_slider2_parms();
    Box vol_box = get_slider3_parms();

    int x = event->x();
    int y = event->y();

    if(in_box.inside(x,y)){

      curr_slider = THRESHOLD_SLIDER;
      p_startpos = in_box.p2;

    }else if(out_box.inside(x,y)){

      curr_slider = RATIO_SLIDER;
      p_startpos = out_box.p1;

    }else if(vol_box.inside(x,y)){

      curr_slider = MAKEUPGAIN_SLIDER;
      p_startpos = vol_box.p1;

    }

    p_startpos = scale(p_startpos,0,height(),0,1);
    y_startpos = y;
  }

  void mouseMoveEvent ( QMouseEvent * event )
  {
    handle_mouse_event(event);
  }

  void mouseReleaseEvent ( QMouseEvent * event )
  {
    handle_mouse_event(event);
    curr_slider = 0;
  }

  void paint_slider(QPainter *p, const Box &b){
    p->drawRect(b.x1, b.y1,
                b.x2-b.x1, b.y2-b.y1);

    p->drawLine(b.x1,b.p1,
                b.x2,b.p1);
    p->drawLine(b.x1,b.p2,
                b.x2,b.p2);
  }

  void paint_gradient(QPainter *p, const Box &in, const Box &out, const Box &vol){
    QColor col1(30,170,33);
    QColor col2(159,58,33);
    col2=col2.light(80);

    // in slider
    {
      QLinearGradient gradient(0,in.p1,0,in.y2);
      gradient.setColorAt(0,col1);
      gradient.setColorAt(1,col2);
      p->fillRect(in.x1 ,in.p1, in.x2-in.x1 ,in.y2-in.p1,QBrush(gradient));
    }

    float in_val = scale(in_val2,in_val1,1.0f,0.0f,1.0f);
    QColor col = mix_colors(col1,col2,1.0f-in_val);

    // out slider top
    {
      QLinearGradient gradient(0,out.p1,0,out.p2);
      gradient.setColorAt(0,col1);
      gradient.setColorAt(1,col);
      p->fillRect(out.x1, out.p1,
                  out.x2-out.x1,
                  out.p2-out.p1,
                  QBrush(gradient));
    }

    // out slider bot.
    {
      QLinearGradient gradient(0,out.p2,0,out.y2);
      gradient.setColorAt(0,col);
      gradient.setColorAt(1,col2);
      p->fillRect(out.x1, out.p2,
                  out.x2-out.x1,
                  out.y2-out.p2,
                  QBrush(gradient));
    }

    // area between in slider and out slider, below threshold 
    {
      QLinearGradient gradient(0,out.p2,
                               0,out.y2);
      gradient.setColorAt(0,col);
      gradient.setColorAt(1,col2);
      p->fillRect(in.x2, out.p2,
                  out.x1-in.x2,
                  out.y2-out.p2,
                  QBrush(gradient));
    }

    // area between in slider and out slider, above threshold
    for(int x=in.x2;x<out.x1;x++){
      int y1 = scale(x,
                     in.x2,out.x1,
                     in.p1,out.p1);
      int y2 = scale(x,
                     in.x2,out.x1,
                     in.p2,out.p2);
      QLinearGradient gradient(x,y1,x,y2);
      gradient.setColorAt(0,col1);
      gradient.setColorAt(1,col);
      p->setPen(QPen(QBrush(gradient),1));
      p->drawLine(x,y1,x,y2);
    }

    // volume slider
    {
      int p1 = vol.p1;
      int p2 = vol.p2;//scale(out.p2, out.p1, out.y2, vol.p1, vol.y2);
      int p3 = p2 + (out.y2-out.p2);

      // top
      {
        QLinearGradient gradient(0,p1,0,p2);
        gradient.setColorAt(0,col1);
        gradient.setColorAt(1,col);
        p->fillRect(vol.x1, p1,
                    vol.x2-vol.x1,
                    p2-p1,
                    QBrush(gradient));
      }
      
      // bot.
      {
        QLinearGradient gradient(0,p2,0,p3);
        gradient.setColorAt(0,col);
        gradient.setColorAt(1,col2);
        p->fillRect(vol.x1, p2,
                    vol.x2-vol.x1,
                    p3-p2,
                    QBrush(gradient));
      }

      QColor col3=col2.light(scale(p3,vol.y2,vol.y1,100,40));

      // below bot.
      {
        QLinearGradient gradient(0,p3,0,vol.y2);
        gradient.setColorAt(0,col2);
        gradient.setColorAt(1,col3);
        p->fillRect(vol.x1,p3,
                    vol.x2-vol.x1,vol.y2-p3,
                    QBrush(gradient));
      }

      // area between out slider and vol slider, above threshold
      for(int x=out.x2;x<vol.x1;x++){
        int y1 = scale(x,
                       out.x2,vol.x1,
                       out.p1,vol.p1);
        int y2 = scale(x,
                       out.x2,vol.x1,
                       out.p2,vol.p2);
        QLinearGradient gradient(x,y1,x,y2);
        gradient.setColorAt(0,col1);
        gradient.setColorAt(1,col);
        p->setPen(QPen(QBrush(gradient),1));
        p->drawLine(x,y1,x,y2);
      }

      // area between out slider and vol slider, below threshold
      for(int x=out.x2;x<vol.x1;x++){
        int y1 = scale(x,
                       out.x2,vol.x1,
                       out.p2,vol.p2);
        int y2 = vol.y2-1;
        QLinearGradient gradient(x,y1,x,y2);
        gradient.setColorAt(0,col);
        gradient.setColorAt(1,mix_colors(col2,col3,scale(x,out.x2,vol.x1,1.0f,0.0f)));
        p->setPen(QPen(QBrush(gradient),1));
        p->drawLine(x,y1,x,y2);
      }

    }

    //p->setBrush(QBrush());
    p->setPen(QPen());
  }

  void paintPeaks(QPainter *p, const Box &in_box, const Box &out_box, const Box &vol_box){
    float in_peak_value = get_graph_value(0);
    float out_peak_value = in_peak_value + get_graph_value(1);
    float vol_peak_value = out_peak_value + get_makeup_gain();

    // 1. Lines
    int in_peak = scale(in_peak_value,min_db,max_db, in_box.y2, in_box.y1);
    int out_peak = scale(out_peak_value,min_db,max_db, out_box.y2, out_box.y1);
    int vol_peak = scale(vol_peak_value,min_db,max_db, vol_box.y2, vol_box.y1);

    {
      QColor col(0,90,180);
      col.setAlpha(80);
      QPen pen(col);
      pen.setWidth(4);
      p->setPen(pen);
    }

    // in
    p->drawLine(in_box.x1, in_peak,
                in_box.x2, in_peak);

    // between in and out
    p->drawLine(in_box.x2, in_peak,
                out_box.x1, out_peak);

    // out
    p->drawLine(out_box.x1, out_peak,
                out_box.x2, out_peak);


    // between out and vol
    p->drawLine(out_box.x2, out_peak,
                vol_box.x1, vol_peak);


    // vol
    p->drawLine(vol_box.x1, vol_peak,
                vol_box.x2, vol_peak);


    // 2. Areas
    peaks_in.add_peaks(in_peak_value);
    peaks_out.add_peaks(out_peak_value);
    peaks_vol.add_peaks(vol_peak_value);

    peaks_in.paint_peak_area(p,in_box);
    peaks_out.paint_peak_area(p,out_box);
    peaks_vol.paint_peak_area(p,vol_box);
  }

  int get_text_width(QString text){
    const QFontMetrics fn = QFontMetrics(QFont());
    return fn.width(text);
  }

  void paintSliderText(QPainter *p, const Box &box, QString text, double value, QString unit){
    QString value_text = QString::number(value,'g',3) + unit;
    QString one_line = text + " " + value_text;
    
    QRect rect(QPoint(box.x1+2,box.y1+2),QPoint(box.x2-4,box.y2-4));

    if(get_text_width(text) < rect.width() && get_text_width(value_text) < rect.width()){
      // horizontal

      p->drawText(rect,Qt::AlignTop|Qt::AlignLeft,
                  get_text_width(one_line)<rect.width()
                    ? one_line
                    : text+"\n"+value_text);

    }else{
      // vercial

      p->save();
      QPoint point(box.x1,box.y1); 
      p->translate(box.x1,0);//point);//x, y);
      p->rotate(90); // or 270
      //p->scale((box.x2-box.x1)/12,1.0);
      //point = p->xFormDev(point);
      p->drawText(5,-5, one_line);
      p->restore();

    }
    //p->drawText(box.x1,box.y2-20,QString::number(value));
  }

  void paint_box_fill(QPainter *p, const Box &box){
    QColor col(150,190,10);
    col.setAlpha(10);
    
    p->fillRect(box.x1, box.y1,
                box.x2-box.x1, box.y2-box.y1,
                col);      
  }

  void paintEvent ( QPaintEvent * ev ){
    QPainter p(this);

    //printf("Paint\n");

    Box in_box = get_slider1_parms();
    Box out_box = get_slider2_parms();
    Box vol_box = get_slider3_parms();

    paint_gradient(&p,in_box,out_box,vol_box);

    QColor black(0,0,0);
    black.setAlpha(80);
    QPen pen(black);
    //pen.setWidth(2);

    p.setPen(pen);

    paint_slider(&p, in_box);
    paint_slider(&p, out_box);
    paint_slider(&p, vol_box);

    // bottom line between sliders
    {
      p.drawLine(in_box.x2,in_box.y2,out_box.x1,out_box.y2);
      p.drawLine(out_box.x2,out_box.y2,vol_box.x1,vol_box.y2);
    }

    // threshold line between sliders
    {
      // between in and out
      p.drawLine(in_box.x2,in_box.p2,
                 out_box.x1,out_box.p2);

      if(out_box.p2!=vol_box.p2)
        p.setRenderHints(QPainter::Antialiasing,true);
 
      // between out and vol
      p.drawLine(out_box.x2,out_box.p2,
                 vol_box.x1,vol_box.p2);

      if(out_box.p2!=vol_box.p2)
        p.setRenderHints(QPainter::Antialiasing,false);
    }

    paint_box_fill(&p,in_box);
    paint_box_fill(&p,out_box);
    paint_box_fill(&p,vol_box);

    paintPeaks(&p, in_box, out_box, vol_box);

    // border line between sliders
    {
      black.setAlpha(60);
      QPen pen(black);
      p.setPen(pen);

      // between in and out
      {
        if(in_box.p1!=out_box.p1){
          p.setRenderHints(QPainter::Antialiasing,true);
          pen.setWidth(2);
        }

        p.drawLine(in_box.x2,in_box.p1,
                   out_box.x1,out_box.p1);
        
        if(in_box.p1!=out_box.p1){
          p.setRenderHints(QPainter::Antialiasing,false);
          pen.setWidth(1);     
        }
      }

      // between out and vol
      {
        if(out_box.p1!=vol_box.p1){
          p.setRenderHints(QPainter::Antialiasing,true);
          pen.setWidth(2);
        }

        p.drawLine(out_box.x2,out_box.p1,
                   vol_box.x1,vol_box.p1);
        
        if(out_box.p1!=vol_box.p1){
          p.setRenderHints(QPainter::Antialiasing,false);
          pen.setWidth(1);
        }
      }
    }

    p.setPen(QPen());

    paintSliderText(&p,in_box,"Threshold: ",get_threshold(), "dB");
    paintSliderText(&p,out_box,"Ratio: ",get_ratio(),":1");
    paintSliderText(&p,vol_box,"Makeup Gain: ",get_makeup_gain(),"dB");
  }

};

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
  Comp *comp;

 Compressor_widget(QWidget *parent=NULL)
    : QWidget(parent)
  {
    initing = true;
    setupUi(this);
    comp = new Comp(this);
    verticalLayout->insertWidget(1,comp);
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
    attack_slider->setValue(get_exp_inverted_value(get_compressor_parameter(ATTACK),1000,0,max_attack_release));
    release_slider->setValue(get_exp_inverted_value(get_compressor_parameter(RELEASE),1000,0,max_attack_release));
  }

  void load(QString filename){
    FILE *file = fopen(filename,"r");

    if(file==NULL){
      QMessageBox msgBox;
      msgBox.setText("Could not open file \""+filename+"\".");
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
      return;
    }
    
    set_compressor_parameter(RATIO,read_float(file)); // ratio
    set_compressor_parameter(THRESHOLD,read_float(file)); // threshold
    set_compressor_parameter(ATTACK,read_float(file)); // attack
    set_compressor_parameter(RELEASE,read_float(file)); // release
    //set_compressor_parameter(INPUT_VOLUME,read_float(file)); // input volume (currently not used)
    read_float(file); // input volume (currently not used)
    set_compressor_parameter(OUTPUT_VOLUME,read_float(file)); // output volume

    fclose(file);

    update_gui();
  }

public slots:

  void on_attack_slider_valueChanged(int val){
    float attack = get_exp_value(val,1000,0,max_attack_release);
    set_compressor_parameter(ATTACK,attack);
    char temp[512];
    sprintf(temp,"Attack: %.2fms",attack);
    SLIDERPAINTER_set_string(attack_slider->_painter, temp);
  }

  void on_release_slider_valueChanged(int val){
    float release = get_exp_value(val,1000,0,max_attack_release);
    set_compressor_parameter(RELEASE,release);
    char temp[512];
    sprintf(temp,"Release: %.2fms",release);
    SLIDERPAINTER_set_string(release_slider->_painter, temp);
  }

  void on_bypass_toggled(bool val){
    printf("bypass: %d\n",(int)val);
    set_compressor_parameter(BYPASS,val==true?1.0f:0.0f);
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
            get_compressor_parameter(RATIO),
            get_compressor_parameter(THRESHOLD),
            get_compressor_parameter(ATTACK),
            get_compressor_parameter(RELEASE),
            //get_compressor_parameter(INPUT_VOLUME),
            0.0f, // input volume (in dB)
            get_compressor_parameter(OUTPUT_VOLUME)
            );

    fclose(file);
  }
};