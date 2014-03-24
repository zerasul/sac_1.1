/*
 * languages.h : File That contains all the Language Options for the Project SAC.
 *  In this file contains all the Messages for Print in screen in 2 Languages.
 *
 *  Created on: 16/03/2014
 *      Author: David Cuevas <mr.cavern@gmail.com>
 *      Collaborator: Victor Suarez<suarez.garcia.victor@gmail.com>
 *
 *   SAC: Version 1.1.
 */

#define MAX_LANGUAGE 4
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
  {{"CICLES:",  	"CICLOS:"}},

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
  {{"STMAX:"}},  {{"TSMAX:"}},
};

int active_language = 0;

/* returns a translated string; if no translation found - return the original
 * string.
 */
static const char *translate(int stringno)
{
  if (active_language < 0)
    active_language = 0;
  if (active_language >= MAX_LANGUAGE)
    active_language = MAX_LANGUAGE-1;

  if (string_db[stringno].languages[active_language])
    return string_db[stringno].languages[active_language];
  else
    return string_db[stringno].languages[0];
}

