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



#ifdef COMPILING_RADIUM
extern struct Root *root;
#else
extern QColor *g_colors;
#endif


static inline QColor mix_colors(const QColor &c1, const QColor &c2, float how_much){

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


static inline void SLIDERPAINTERPAINTER_paint(QPainter *p, int x1, int y1, int x2, int y2, bool is_enabled, float val, QString text, bool alternative_color){
#ifdef COMPILING_RADIUM
    QColor *colors = static_cast<EditorWidget*>(root->song->tracker_windows->os_visual.widget)->colors;
#else
    QColor *colors = g_colors;
#endif

    int height = y2-y1;
    int width = x2-x1;

    QColor col1;
    QColor col1b;

    int col1num = 11;

    if(is_enabled){
      QColor c(98,59,33);
      
      col1 = c.light(90);
      col1b = colors[13].light(100);
    }else{
      col1 = mix_colors(colors[col1num], Qt::gray, 0.8f);
      col1b = mix_colors(colors[col1num].light(110), Qt::gray, 0.8f);
    }
    
    if(alternative_color==true)
      col1 = QColor(200,200,200);
    
    if(alternative_color==false){
      col1.setAlpha(80);
      col1b.setAlpha(100);
    }else{
      col1.setAlpha(120);
      col1b.setAlpha(120);
    }

    if(height > width){ // i.e. vertical
      int pos=scale(val,0,1,0,height);
      p->fillRect(0,pos,width,height-pos,col1);
    }else{
      int pos=scale(val,0,1,0,width);
      QLinearGradient gradient(0,0,width,height*3/4);
      if(alternative_color==false)
        gradient.setColorAt(0,col1.light(100));
      else
        gradient.setColorAt(0,col1.light(150));
      gradient.setColorAt(1,col1b);

      p->setPen(QPen(QColor(Qt::gray).light(50),1));
      p->setBrush(gradient);
      p->drawRect(0   ,0, pos, height);
      p->setBrush(QBrush());
    }

    p->setPen(QPen(colors[11].light(110),1));
    p->drawRect(0,0,width,height);

    QRect rect(5,2,width-5,height-2);

    if(text!=""){
      QColor c(colors[1]);
      if(is_enabled){
        c.setAlpha(160);
        p->setPen(QPen(c,1));
      }else{
        c.setAlpha(60);
        p->setPen(QPen(c,1));
      }
      p->drawText(rect, Qt::AlignLeft, text);
    }

}

