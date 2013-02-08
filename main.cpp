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

//#include "audio/faudiostream/architecture/faust/gui/UI.h"

#include <vector>

#define OPTARGS_CHECK_GET(wrong,right) lokke==argc-1?(fprintf(stderr,"Must supply argument for '%s'\n",argv[lokke]),exit(-4),wrong):right

#define OPTARGS_BEGIN(das_usage) {int lokke;const char *usage=das_usage;for(lokke=1;lokke<argc;lokke++){char *a=argv[lokke];if(!strcmp("--help",a)||!strcmp("-h",a)){fprintf(stderr,"%s",usage);exit(0);
#define OPTARG(name,name2) }}else if(!strcmp(name,a)||!strcmp(name2,a)){{
#define OPTARG_GETINT() OPTARGS_CHECK_GET(0,atoll(argv[++lokke]))
//int optargs_inttemp;
//#define OPTARG_GETINT() OPTARGS_CHECK_GET(0,(optargs_inttemp=strtol(argv[++lokke],(char**)NULL,10),errno!=0?(perror("strtol"),0):optargs_inttemp))
#define OPTARG_GETFLOAT() OPTARGS_CHECK_GET(0.0f,atof(argv[++lokke]))
#define OPTARG_GETSTRING() OPTARGS_CHECK_GET("",argv[++lokke])
#define OPTARG_LAST() }}else if(lokke==argc-1 && argv[lokke][0]!='-'){lokke--;{
#define OPTARGS_ELSE() }else if(1){
#define OPTARGS_END }else{fprintf(stderr,"%s",usage);exit(-1);}}}

#include "common/nsmtracker.h"
#include "Qt/Qt_MyQCheckBox.h"
#include "Qt/Qt_MyQSlider.h"
#include "Qt/Qt_MyQButton.h"

static QColor das_colors[16];
QColor *g_colors=&das_colors[0];

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

bool g_autoconnect = false;
const char *g_jack_client_name = "radium_compressor";
const char *g_settings_filename = NULL;
const char *g_shared_mem_key = NULL;

QWidget *create_compressor_editor(QWidget *parent, void *compressor){
  // A minimal radium patch environment.
  struct Patch *patch = new Patch;
  SoundPlugin *plugin = new SoundPlugin;
  patch->patchdata = plugin;
  plugin->compressor = compressor;
  
  Compressor_widget *compressor_widget = new Compressor_widget(patch,parent);

  if(g_settings_filename!=NULL)
    compressor_widget->load(g_settings_filename);

  set_colors();

  QPalette pal = compressor_widget->palette();
  //pal.setColor(compressor.backgroundRole(), das_colors[11].light(200));
  pal.setColor(compressor_widget->backgroundRole(), das_colors[11].light(100));
  compressor_widget->setPalette(pal);

  return compressor_widget;
}

void delete_compressor_editor(QWidget *widget){
  Compressor_widget *compressor = static_cast<Compressor_widget *>(widget);
  struct Patch *patch=compressor->_patch;
  struct SoundPlugin *plugin=(SoundPlugin*)patch->patchdata;
  COMPRESSOR_delete(plugin->compressor);
  //delete compressor;
}

void start_program(int argc, char **argv, void *compressor){

  QApplication app(argc, argv);

  {
    QWidget *compressor_editor = create_compressor_editor(NULL, compressor);

    compressor_editor->show();

    app.exec();

    delete_compressor_editor(compressor_editor);
  }
}

#ifndef COMPILING_VST

void *COMPRESSOR_create_ladspa(const char *key);

int main(int argc, char **argv){

  OPTARGS_BEGIN("radium_compressor [--autoconnect] [--client-name s] [--settings-filename s] [settings-filename]\n"
                "                  [ -ac         ] [ -cn          s] [ -sn                s] [settings filename]\n"
                "\n"
                "\"autoconnect\"       connects radium_compressor to physical in and out ports.\n"
                "\"client-name\"       sets the name of the jack client.\n"
                "\"settings-filename\" is the name of a settings file to load at startup.\n"
                )
    {

      OPTARG("--settings-filename","-sn") g_settings_filename=OPTARG_GETSTRING();
      OPTARG("--autoconnect","-ac") g_autoconnect=true;
      OPTARG("--client-name","-cn") g_jack_client_name=OPTARG_GETSTRING();
      OPTARG("--ladspa-slave","-ls") g_shared_mem_key = OPTARG_GETSTRING();
      OPTARG_LAST() g_settings_filename=OPTARG_GETSTRING();
    }OPTARGS_END;

  if(g_shared_mem_key!=NULL){

    start_program(argc,argv,COMPRESSOR_create_ladspa(g_shared_mem_key));

  }else{

    start_program(argc,argv, COMPRESSOR_create(0));

  }
  return 0;
}

#endif
