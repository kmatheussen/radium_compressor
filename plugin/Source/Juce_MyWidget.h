#include "MyLineMyRect.h"


namespace cvs{

  struct MyColor{
    Colour col;
    MyColor(int r,int g,int b)
      : col(r,g,b)
    {}

    MyColor(Colour col)
      : col(col)
    {
      incContrast();
    }

    MyColor(int colnum)
#ifdef COMPILING_RADIUM
      : col(static_cast<EditorWidget*>(root->song->tracker_windows->os_visual.widget)->colors[colnum])
#else
      : col(g_colors[colnum])
#endif
    {
      //if(colnum==11)
      //  col = col.brighter(0.77);
      incContrast();
    }

    MyColor()
#ifdef COMPILING_RADIUM
      : col(static_cast<EditorWidget*>(root->song->tracker_windows->os_visual.widget)->colors[11])
#else
      : col(g_colors[11])
#endif
    {
      //incContrast();
    }

    void incContrast(void){
#if 0
      if(col.getBrightness()<0.5)
        col = col.withBrightness(scale(col.getBrightness(),0,5,0,0.25));
      else
        col = col.withBrightness(scale(col.getBrightness(),0.5,1,0.75,1.0));
#endif

#if 0
      //col = g_colors[11].contrasting(col,0.3);
      col = col.withBrightness(43.0/127.0);

      if(col.getBrightness()<0.2)
        col = col.brighter(0.25);
      else
        col = col.darker(0.25);

      //col = col.contrasting(5.0/127.0);
      //col = col.darker(0.25);
#endif
    }

    const MyColor lighter(int p) const{
       return MyColor(col.darker( (100.0-(double)p) / 100.0));
    }

    void setAlpha(int a){
      col = col.withAlpha((uint8)a);
    }
  };

  static inline MyColor mix_mycolors(const MyColor &c1, const MyColor &c2, float how_much){
    float a1 = how_much;
    float a2 = 1.0f-a1;

    if(c1.col.getRed()==0 && c1.col.getGreen()==0 && c1.col.getBlue()==0){ // some of the black lines doesn't look look very good.
      int r = 74*a1 + c2.col.getRed()*a2;
      int g = 74*a1 + c2.col.getGreen()*a2;
      int b = 74*a1 + c2.col.getBlue()*a2;
      
      return MyColor(r,g,b);
    }else{
      
      int r = c1.col.getRed()*a1 + c2.col.getRed()*a2;
      int g = c1.col.getGreen()*a1 + c2.col.getGreen()*a2;
      int b = c1.col.getBlue()*a1 + c2.col.getBlue()*a2;
      
      return MyColor(r,g,b);
    }
  }

#if 0
  static inline MyColor mix_mycolors(const MyColor &c1, const MyColor &c2, float how_much){
    return MyColor(c1.col.interpolatedWith(c2.col,1.0f-how_much));
  }
#endif

  struct MyImage{
    const Image image;
    Graphics g;
    MyImage(int width,int height)
      : image(Image::RGB,width,height,false)
      , g(image)
    {
    }
  };

  struct MyPainter{
    Graphics *g;
    MyColor col;

#if 0
    QLinearGradient gradient;
#endif
    bool use_gradient;
    int _thickness;

    MyPainter(Graphics *g)
      : g(g) 
      , use_gradient(false)
      , _thickness(1)
    {
      init();
    }

    MyPainter(MyImage *image)
      : g(&image->g)
      , use_gradient(false)
      , _thickness(1)
    {
      init();
    }

    void init(){
      Font font = g->getCurrentFont();
      font.setHeight(16);
      g->setFont(font);
    }

    void setGradient(int x1, int y1, int x2, int y2, const MyColor &c1, const MyColor &c2){
      ColourGradient gradient = ColourGradient(c1.col, x1,y1, c2.col, x2,y2, false);
      g->setGradientFill(gradient);

#if 0
      g->setGradientFill
      gradient = QLinearGradient(x1,y1,x2,y2);
      gradient.setColorAt(0,c1.col);
      gradient.setColourAt(1,c2.col);
      g->setPen(QPen(QBrush(gradient),_thickness));
#endif
      use_gradient = true;
    }

    void unsetGradient(){
      use_gradient=false;
    }

    void drawImage(int x,int y,MyImage *image){
      g->setOpacity(1.0f);
      g->drawImageAt(image->image, x,y);
      g->setOpacity(1);
    }

    void drawRect(const int x1,const int y1,const int x2,const int y2, const MyColor &col){
      if(use_gradient==false)
        g->setColour(col.col);

      g->drawRect(x1,y1,x2-x1,y2-y1,_thickness);
    }
    void drawRect(const MyRect &rect,const MyColor &col){
      drawRect(rect.x1,rect.y1,rect.x2,rect.y2,col);
    }

    void fillRect(const int x1,const int y1,const int x2,const int y2, const MyColor &col){
      if(use_gradient==false)
        g->setColour(col.col);

      g->fillRect(x1,y1,x2-x1,y2-y1);
    }

    void fillRect(const MyRect &rect,const MyColor &col){
      fillRect(rect.x1,rect.y1,rect.x2,rect.y2,col);
    }

    void setThickness(int thickness){
      _thickness=thickness*1;
    }

    void drawLine(const int x1,const int y1,const int x2,const int y2, const MyColor &col){
      if(use_gradient==false)
        g->setColour(col.col.darker(1));

      g->drawLine(x1,y1,x2,y2,_thickness);
    }

    void drawLine(const MyLine &l, const MyColor &col){
      drawLine(l.x1,l.y1,l.x2,l.y2, col);
    }

    int getTextWidth(std::string text){
      return g->getCurrentFont().getStringWidth(text.c_str());
    }

    int getTextHeight(){
      return g->getCurrentFont().getHeight();
    }

    void drawText(const MyRect &rect, std::string text, const MyColor &col){
      MyColor dascol(0,0,75);
      dascol.setAlpha(210);

      if(use_gradient==false)
        g->setColour(dascol.col);

#if 1
      g->drawText(text.c_str(),
                  rect.x1+2,rect.y1+2,rect.width()-4,rect.height()-4,
                  Justification::topLeft,
                  false);
#endif
      //g->drawFittedText(
    }

    void drawVerticalText(int x, int y, std::string text, const MyColor &col){
#if 0
      if(use_gradient==false){
        pen.setColour(col.col);
        g->setPen(pen);
      }
      g->save();
      QPoint point(x,y); 
      g->translate(x,0);//point);//x, y);
      g->rotate(90); // or 270
      //g->scale((box.x2-box.x1)/12,1.0);
      //point = g->xFormDev(point);
      g->drawText(5,-5,QString::fromStdString(text));
      g->restore();
#endif
    }
  };

  struct MyWidget{
    struct MyQWidget : public Component{

      MyWidget *mywidget;
      MyQWidget(MyWidget *mywidget) : mywidget(mywidget){
      }

      void mouseDown 	(	const MouseEvent & 	event	){
        mywidget->mousePress(event.x,event.y);
      }
      void mouseDrag 	(	const MouseEvent & 	event	){
        mywidget->mouseMove(event.x,event.y);
      }
      void mouseUp 	(	const MouseEvent & 	event	){
        mywidget->mouseRelease(event.x,event.y);
      }
      void paint ( Graphics &g){
        //printf("got paint\n");
        g.setImageResamplingQuality(Graphics::lowResamplingQuality);
        MyPainter p(&g);
        mywidget->repaint(&p);
      }
      virtual void resized(){
        //printf("got resize\n");
        mywidget->resized();
      }
    };

    MyQWidget w;

    MyWidget(void *parent)
      : w(this)
    {
      //printf("MyWidget created. parent: %p\n",parent);
    }

    virtual ~MyWidget(){
    }

    void update(int x1,int y1,int x2,int y2){
      w.repaint(x1,y1,x2-x1,y2-y1);
    }
    void update(const MyRect &r){
      update(r.x1,r.y1,r.x2,r.y2);
    }

    void update(){
      w.repaint();
    }

    int width(){return R_MAX(10,w.getWidth());}
    int height(){return R_MAX(10,w.getHeight());}

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

    struct MyQTimer : public Timer{
      MyTimer *mytimer;

      MyQTimer(MyTimer *mytimer) : mytimer(mytimer) {}

      void timerCallback(){
        mytimer->timer();
      }
    };

    MyQTimer qtimer;

    MyTimer() : qtimer(this) {}

    virtual ~MyTimer() {}

    virtual void timer() = 0;

    void startTimer(int interval_in_milliseconds){
      qtimer.startTimer(interval_in_milliseconds);
    }
  };




} // namespace cvs
