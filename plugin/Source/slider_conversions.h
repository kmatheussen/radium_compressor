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


#ifndef SLIDER_CONVERSIONS_H
#define SLIDER_CONVERSIONS_H

#include <math.h>

static inline float scale(float x, float x1, float x2, float y1, float y2){
  return y1 + ( ((x-x1)*(y2-y1))
                /
                (x2-x1)
                );
}

static inline double scale_double(double x, double x1, double x2, double y1, double y2){
  return y1 + ( ((x-x1)*(y2-y1))
                /
                (x2-x1)
                );
}


static const double min_ratio = 1.0/60.0;
static const double max_ratio = 60.0;
static const double max_attack_release = 500.0;
static const double min_threshold = -40;
static const double max_threshold = 40;
static const double min_gain = -80;
static const double max_gain = 80;

static inline float get_exp_value(double val, double max_val, double out_min, double out_max, double how_much_exp){
  return scale_double(exp(how_much_exp*val/max_val),
                      exp(0.0),exp(how_much_exp),
                      out_min,out_max
                      );
}

static inline float get_exp_inverted_value(double out, double max_val, double out_min, double out_max, double how_much_exp){
  return (max_val/how_much_exp) * log(scale_double(out,
                                                   out_min,out_max,
                                                   exp(0.0),exp(how_much_exp/max_val)));
}

#if 0
static inline float get_ratio_from_slider(float slider){
  const float min_output = logf(min_ratio)/logf(max_ratio);
  return powf(max_ratio, scale(slider,
                               0,1,
                               min_output, 1));
#if 0
  x=slider, x1=0, x2=1
  y1=min_output,y2=1
  min_output + slider*(1-min_output)
#endif
}

static inline float get_slider_from_ratio(float ratio){
  const float min_output = logf(min_ratio)/logf(max_ratio);
  const float log_max = logf(max_ratio);
  return (min_output*log_max - log(ratio)) / ((min_output-1.0)*log_max);
}

#endif

#include <stdio.h>

static inline float get_ratio_from_slider(float slider){

  if(slider<0.5)
    return scale(slider,0,0.5,1.0/max_ratio,1);
  //else // got into infinite loop. could be because of inaccurate float conversion, or perhaps that slider (for some reason) isn't between 0 and 1.
  //   return 1.0/get_ratio_from_slider(scale(slider,0.5,1.0,0.5,0.0));
  // workaround:

  slider = scale(slider,0.5,1.0,0.5,0.0);
  double output = 1.0 / scale(slider,0,0.5,1/max_ratio,1);

  return output;
}

/*
(define (get-slider-from-ratio ratio)
  (if (<= ratio 1)
      (scale ratio (/ 1 60.0) 1 0 0.5)
      (let ((m 60))
        (scale (/ (+ m (* -3 ratio) (* 2 m ratio))
                  (* 2 (- m 1) ratio))
               1.5 1.0
               0.5 1.0))))
*/
static inline float get_slider_from_ratio(float ratio){
  float m = max_ratio;
  if(ratio<=1)
    return scale(ratio,1.0/m,1,0,0.5f);
  else
    return scale( (m + 2*m*ratio - 3*ratio) // (got calculation from using the solve function in wolfram alpha)
                  / (2*ratio*(m-1)),
                  1.5, 1.0,
                  0.5, 1.0);
}

static inline float get_attack_release_from_slider(float slider){
  return get_exp_value(slider,1.0,0,max_attack_release,2.0);
}

static inline float get_slider_from_attack_release(float attack_release){
  return get_exp_inverted_value(attack_release,1.0,0,max_attack_release,2.0);
}

static inline float get_threshold_from_slider(float slider){
  return scale(slider,0,1,min_threshold,max_threshold);
}

static inline float get_makeup_gain_from_slider(float slider){
  return scale(slider,0,1,min_gain,max_gain);
}

static inline float get_slider_from_makeup_gain(float value){
  return scale(value,min_gain,max_gain,0,1);
}


#endif //SLIDER_CONVERSIONS_H

