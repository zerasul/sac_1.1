/*
 * languages.h : File That contains all the Languaje Options for the Project SAC.
 *  In this file contains all the Messages for Print in screen in 2 Languajes.
 *
 *  Created on: 16/03/2014
 *      Author: dcuevas
 *      Collaborator: Victor Suarez<suarez.garcia.victor@gmail.com>
 *
 *   SAC: Version 1.1.
 */

#define MAX_LANGUAGE 2
typedef struct TranslatedString {
  const char *languages[MAX_LANGUAGE];
} TranslatedString;

TranslatedString string_db[]={
  /* for languages missing translations, english will be used instead */
  {{"","",""}},
  {{"  F.C."",C.C."}},
  {{"E ","E "}},
  {{"SOIL","HSO:"}},
  {{"H ","H "," "}},
  {{"L ","L ","L"}},
  {{"A ","A ","A"}},
  {{"PU ","RIEGO"}},
  {{"LANGUAGE","IDIOMA"}},
  {{"ENGLISH",         "ESPANOL"}},
  {{"<enabled>",       "<activar>"}},
  {{"<disabled>",      "<inhabilitado>"}},
  {{"TIME",            "HORA"}},
  {{"SOIL MOISTURE",   "HUMEDAD SUELO"}},
  {{"CALIBRATE SAT.","CALIBRACION SAT."}},
  {{"OFF",             "APAGADO"}},
  {{"WATERING",      "RIEGO"}},
  {{"HEATING",         "CALEFACCION",}},
  {{"COOLING",         "REFRIGERACION"}},
  {{"LIGHT",           "ILUMINACION"}},
  {{"AIR EXTRACTION",  "EXTRAC.HUMEDAD"}},
  {{"HUMIDIFIER",      "HUMIDIFICACION"}},
  {{"alarm",           "Alarm"}},
  {{"OUTPUT 1",        "SALIDA 1"}},
  {{"OUTPUT 2",        "SALIDA 2"}},
  {{"OUTPUT 3",        "SALIDA 3"}},
  {{"AIR HUMIDITY",    "HUMEDAD AIRE"}},
  {{"TEMPERATURE",     "TEMPERATURA"}},

  {{"WATERING",      "RIEGO"}},
  {{"CICLES (sec):",  "CICLOS (seg):"}},

  {{"RETURN TO",       "VOLVER"}},
  {{"MAIN MENU",       "MENU PRINCIPAL"}},
  {{"Setup...",        "Configuracion.."}},
  {{"OUTPUTS",          "SALIDAS"}},
  {{"Reset",           "RESET"}},
  {{"ON",              "ENCENDIDO"}},
  {{"START",           "COMIENZA"}},
  {{"DURATION",        "DURACION"}},
  {{"CONFIGURATION",   "CONFIGURACION"}},
  {{"Hysteresis: ",  }},
  {{"Log"}},
  {{"MIN:"}},  {{"MIN:"}},
};

int active_language = 0;
