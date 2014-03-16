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
#include <signal.h>

#include <string>

//#include "audio/faudiostream/architecture/faust/gui/UI.h"

#include "nonlib/nsm.h"

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

static Compressor_widget *g_compressor_editor = NULL;
static void *g_compressor = NULL;

Compressor_widget *create_compressor_editor(QWidget *parent, void *compressor){
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

void delete_compressor_editor(Compressor_widget *compressor_editor){
  struct Patch *patch=compressor_editor->_patch;
  struct SoundPlugin *plugin=(SoundPlugin*)patch->patchdata;
  COMPRESSOR_delete(plugin->compressor);
  //delete compressor;
}

static nsm_client_t *g_nsm = 0;
static bool g_nsm_quit_now = false;


struct NSMTimer : public QTimer{
  void timerEvent(QTimerEvent * e){
    if(g_nsm_quit_now==true)
      QApplication::quit();
    else
      nsm_check_nowait(g_nsm);
  }
};

NSMTimer g_nsm_timer;

static const char *g_nsm_save_filename;

int rc_nsm_open ( const char *name,                           
                  const char *display_name,                   
                  const char *client_id,                      
                  char **out_msg,                             
                  void *userdata )                            
{                                                         

  if(g_compressor_editor != NULL) {
    fprintf(stderr,"calling nsm open twice is not supported.\n");
    abort();
  }

  //do_open_stuff();
  g_jack_client_name = client_id;
  g_compressor = COMPRESSOR_create(0);

  if(g_nsm_save_filename != NULL)
    free((void*)g_nsm_save_filename);
  g_nsm_save_filename = strdup(name);

  g_compressor_editor = create_compressor_editor(NULL, g_compressor);
  g_compressor_editor->load_button->hide();
  g_compressor_editor->save_button->hide();
  g_compressor_editor->setWindowFlags ( Qt::CustomizeWindowHint | Qt::WindowTitleHint);

  g_compressor_editor->load(name, false);
  g_compressor_editor->show();


  char temp[1000];
  sprintf(temp, "Open. name: %s, display_name: %s, client_id: %s\n",name, display_name, client_id);
  *out_msg = strdup(temp);
  puts(temp);

  return ERR_OK;                                    
}                                                         

int rc_nsm_save ( char **out_msg,                             
              void *userdata )                            
{               
  //do_save_stuff(); 
  g_compressor_editor->save(g_nsm_save_filename);
                                     
  char temp[1000];
  sprintf(temp, "Save. userdata: %p\n", userdata);
  *out_msg = strdup(temp);
  puts(temp);
  return ERR_OK;                                        
}                                                         

void rc_nsm_signalHanlder(int sigNum)
{
  g_nsm_quit_now = true;
}


void start_program(int argc, char **argv){

  QApplication app(argc, argv);

  {


    // nsm
    {
      const char *nsm_url = getenv( "NSM_URL" );
      if ( nsm_url )                                        
        {                                                     
          g_nsm = nsm_new();                                  
          
          printf("NSM: %p\n",g_nsm);

          nsm_set_open_callback( g_nsm, rc_nsm_open, 0 );     
          nsm_set_save_callback( g_nsm, rc_nsm_save, 0 );     
          
          if ( 0 == nsm_init( g_nsm, nsm_url ) )              
            {                                                
              signal(SIGTERM, rc_nsm_signalHanlder);
              nsm_send_announce( g_nsm, "Radium Compressor", "", argv[0] ); 
              g_nsm_timer.setInterval(50);
              g_nsm_timer.start();
              
            }                                                 
          else                                              
            {                                                 
              nsm_free( g_nsm );                              
              g_nsm = 0;                                      
            }                                                 
        }                                                     
    }

    if(g_nsm == NULL){
      if (g_compressor == NULL)
        g_compressor = COMPRESSOR_create(0);
      g_compressor_editor = create_compressor_editor(NULL, g_compressor);
      g_compressor_editor->show();
    }

    app.exec();

    if(g_compressor_editor != NULL)
      delete_compressor_editor(g_compressor_editor);
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
      //OPTARG("--ladspa-slave","-ls") g_shared_mem_key = OPTARG_GETSTRING();
      OPTARG_LAST() g_settings_filename=OPTARG_GETSTRING();
    }OPTARGS_END;

  if(g_shared_mem_key!=NULL){

    g_compressor = COMPRESSOR_create_ladspa(g_shared_mem_key);
    start_program(argc,argv);

  }else{

    start_program(argc,argv);

  }
  return 0;
}

#endif
