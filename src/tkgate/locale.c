/****************************************************************************
    Copyright (C) 1987-2009 by Jeffery P. Hansen

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    Last edit by hansen on Fri Feb 13 22:29:46 2009
****************************************************************************/
#include "tkgate.h"
#include <dirent.h>

extern int is_verbose;

static int gat_msgLookup(ClientData d,Tcl_Interp *tcl,int argc,const char *argv[]);

static void readLongMsg(FILE *f,char *msg,int n,int doFill,Encoder *encoder)
{
  char tmp[8*STRMAX];
  char *p;
  int l;

  p = tmp;
  while (fgets(p,n,f)) {
    if (doFill) {
      char *q = strchr(p,'\n');
      if (q) *q = ' ';
    }

    l = strlen(p);
    if (strncmp(p,"-end-",5) == 0) {
      if (p != tmp)
	p[-1] = 0;
      else
	*p = 0;
      break;
    }
    p += l;
    n -= l;
    if (n <= 0) {
      printf("Fatal Error: Message too long.\n");
      exit(1);
      break;
    }
  }

  recodeText(encoder,msg,8*STRMAX,tmp);
}

static Locale *getLocaleDescriptor(const char *locale)
{
  Locale *l;
  char fileName[STRMAX];
  char buf[STRMAX],tag[STRMAX];
  FILE *f; 
  unsigned loadMask = 0;

  sprintf(fileName,"%s/locale/%s/messages",TkGate.homedir,locale);
  f = fopen(fileName,"r");
  if (!f) {
    logError(ERL_ERROR,"Missing messages file for locale <%s>.",locale);
    return 0;
  }

  /*
   * Set default values
   */
  l = (Locale*)malloc(sizeof(Locale));
  l->l_code = strdup(locale);
  l->l_name = l->l_code;
  l->l_messages = strdup(fileName);
  l->l_encFont = CE_ISO8859_1;
  l->l_encDisplay = CE_ISO8859_1;
  l->l_encMessages = CE_ISO8859_1;
  l->l_encVerilog = CE_ISO8859_1;
  l->l_encPostscript = CE_ISO8859_1;

  /*
   * Read messages file for declarations
   */
  while (fgets(buf,STRMAX,f)) {
    if (sscanf(buf,"\\language %[^\n\r]",tag) == 1) {
      Encoder *e = getEncoder(CE_UTF8,l->l_encMessages);
      recodeText(e,buf,STRMAX,tag);
      l->l_name = strdup(buf);
      loadMask |= 0x1;
    } else if (sscanf(buf,"\\messages-encoding %s",tag) == 1) {
      l->l_encMessages = strdup(tag);
      loadMask |= 0x2;
    } else if (sscanf(buf,"\\verilog-encoding %s",tag) == 1) {
      l->l_encVerilog = strdup(tag);
      loadMask |= 0x4;
    } else if (sscanf(buf,"\\display-encoding %s",tag) == 1) {
      l->l_encDisplay = strdup(tag);
      loadMask |= 0x8;
    } else if (sscanf(buf,"\\font-encoding %s",tag) == 1) {
      l->l_encFont = strdup(tag);
      loadMask |= 0x10;
    } else if (sscanf(buf,"\\postscript-encoding %s",tag) == 1) {
      l->l_encPostscript = strdup(tag);
      loadMask |= 0x20;
    }
    if (loadMask == 0x3f) break;	/* everything loaded */ 
  }

#if LOCALE_DEBUG
  printf("loaded locale %s (%s)\n",l->l_code,l->l_name);
#endif

  return l;
}

/*
 * Find the set of locales that are available 
 */
void init_localeSet()
{
  char dirName[STRMAX];
  DIR *d;
  struct dirent *de;

  TkGate.localeTable = new_SHash_noob();
  TkGate.localeNameTable = new_SHash_noob();

  sprintf(dirName,"%s/locale",TkGate.homedir);
  d = opendir(dirName);
  if (!d) {
    logError(ERL_FATAL,"Can not find locale directory %s/locale.",TkGate.homedir);
    exit(1);
  }
  while ((de = readdir(d))) {
    Locale *l;

    if (*de->d_name == '.') continue;	/* not a locale */ 
    l = getLocaleDescriptor(de->d_name);
    if (l) {
      Locale *x;
      x = SHash_find(TkGate.localeNameTable, l->l_name);
      if (x) {
	logError(ERL_ERROR,"Locale name '%s' for '%s' conflicts with locale '%s'.",l->l_name,l->l_code,x->l_code);
      } else {
	SHash_insert(TkGate.localeTable, l->l_code,l);
	SHash_insert(TkGate.localeNameTable, l->l_name,l);
      }
    }
  }
  closedir(d);

  /*
   * Set default locale.
   */
  TkGate.locale = (Locale*) SHash_find(TkGate.localeTable, "en");
  if (!TkGate.locale) {
    logError(ERL_FATAL,"tkgate: can not find required file locale/en/messages in %s.\n",TkGate.homedir);
    exit(1);
  }
}

/*****************************************************************************
 *
 * Determine our locale and territory.
 *
 *****************************************************************************/
static void getTkGateLocale(Tcl_Interp *tcl,char *lang,char *territory)
{
  char *p = 0;

  /*
   * If Tcl/Tk is running, then look at the environment variables LC_ALL,
   * LC_MESSAGES, and LANG (in that order) to determine the locale.
   */
  if (tcl) {
    p = getenv("TKGATE_LANG");
    if (!p || !*p) p = getenv("LC_ALL");
    if (!p || !*p) p = getenv("LC_MESSAGES");
    if (!p || !*p) p = getenv("LANG");
    if (!p || !*p) {
      /*
       * If we have Japanese support and kinput is running, an invalid locale
       * will cause a crash in the tcl/tk initialization step, even we are running
       * in English mode.  Force LANG it a valid locale to avoid this problem.
       */
      putenv("LANG=ASCII");
      p = "en_US";
    }
    strncpy(lang,p,1024);
  } else 
    strcpy(lang,"en_US");

  /* If locale is any of the following, set the locale to "en" */
  if (strcmp(lang,"ASCII") == 0 ||
      strcmp(lang,"US-ASCII") == 0 ||
      strcmp(lang,"C") == 0 ||
      strcmp(lang,"POSIX") == 0)
    strcpy(lang,"en_US");

  /*
   * Check for a territory, and partition lang into language and territory.
   */
  if (strlen(lang) >= 5) {
    strcpy(territory,lang + 3);
    territory[2] = 0;
  }
  lang[2] = 0;

  if (!*territory)
    strcpy(territory, "US");
}

static SHash *readMessagesFile(Locale *locale)
{
  Encoder *encoder = getEncoder(CE_UTF8, locale->l_encMessages);
  char buf[STRMAX],buf2[STRMAX],tag[STRMAX],msg[8*STRMAX];
  SHash *H = new_SHash_noob();
  FILE *f;

#if LOCALE_DEBUG
  printf("[loading locale %s in <%s> format]\n",locale->l_code, locale->l_encMessages);
#endif

  f = fopen(locale->l_messages,"r");
  assert(f);				/* We've already verified we can open this file. */


  //      TkGate.fontCode = strdup(tag);
  //      TkGate.saveFileEncoding = strdup(tag);

  /*
   * Read the messages file.  We ignore the directives because we have already read them
   * and stored the values in the locale.
   */
  while (fgets(buf2 ,1024,f)) {
    recodeText(encoder,buf,STRMAX,buf2);

    if (sscanf(buf,"\\font-encoding %s",tag) == 1) {
    } else if (sscanf(buf,"\\messages-encoding %s",tag) == 1) {
    } else if (sscanf(buf,"\\verilog-encoding %s",tag) == 1) {
    } else if (sscanf(buf,"\\display-encoding %s",tag) == 1) {
    } else if (sscanf(buf,"\\postscript-encoding %s",tag) == 1) {
    } else if (sscanf(buf,"\\language %s",tag) == 1) {
    } else if (sscanf(buf,"\\%s",tag) == 1) {
      logError(ERL_ERROR,"Unknown command '%s' in messages file '%s'.",tag,locale->l_messages);
    } else if (sscanf(buf,"%s %[^\n]",tag,msg) == 2 && *tag != '#') {
      if (strcmp(msg,"-begin-") == 0)
	readLongMsg(f,msg,8096,0,encoder);
      else if (strcmp(msg,"-fillbegin-") == 0)
	readLongMsg(f,msg,8096,1,encoder);
      else if (strcmp(msg,"-empty-") == 0)
	*msg = 0;

      if (SHash_find(H,tag)) {
	logError(ERL_ERROR,"Duplicate add of message '%s'.",tag);
      }

      SHash_insert(H,tag,ob_strdup(msg));
    }
  }
  fclose(f);

  return H;
}

/*
 * If the locale is not English we make sure that we have messages for everything
 * that is in the English file, and we have no tags that aren't in the English
 * file.  We issue warnings for any discrepancies.
 */
static void verifyMessagesFile(SHash *H,Locale *englishLocale)
{
  char buf[STRMAX],buf2[STRMAX],tag[STRMAX],msg[8*STRMAX];
  int no_msg_count = 0;
  FILE *f;
  char *p = 0;
  Encoder *encoder = 0;

  f = fopen(englishLocale->l_messages,"r");
  assert(f);


  while (fgets(buf2,1024,f)) {
    recodeText(encoder,buf,STRMAX,buf2);

    if (sscanf(buf,"\\messages-encoding %s",tag) == 1) {
      encoder = getEncoder(CE_UTF8, tag);
    } else if (sscanf(buf,"\\%s",tag) == 1) {
      /* Ignore */
    } else if (sscanf(buf,"%s %[^\n]",tag,msg) == 2 && *tag != '#') {
      if (strcmp(msg,"-begin-") == 0)
	readLongMsg(f,msg,8096,0,encoder);
      else if (strcmp(msg,"-fillbegin-") == 0)
	readLongMsg(f,msg,8096,1,encoder);

      if (!SHash_find(H,tag)) {
	SHash_insert(H,tag,ob_strdup(msg));

	/*
	 * Warn if there was a missing tag (but is ok not to redefine the @ tags. 
	 */
	if (*tag != '@') {
	  if (is_verbose)
	    logError(ERL_ERROR,"No localized string for symbol '%s'.  Using English value.",tag);
	  else
	    no_msg_count++;
	}
      }
    }
  }
  fclose(f);

  if (!is_verbose && no_msg_count > 0) {
    logError(ERL_ERROR,"No localized strings for %d messages.  Use 'tkgate -v' for details.",no_msg_count);
  }
}

/*
 * Determine which locale we are going to run under and read the appropriate
 * message files.  If the locale is not "en" (English), then the both the
 * locale specific message file and the English message file is read.  If there
 * are any messages in the English message file that are not in the locale 
 * specific message file, a warning is printed and the English message
 * is used in place of the missing locale-specific message.
 */
void localization_Setup(Tcl_Interp *tcl)
{
  char lang[STRMAX],territory[STRMAX];
  Locale *englishLocale;

  /*
   * Make sure we have at least the English locale.
   */
  englishLocale = (Locale*) SHash_find(TkGate.localeTable, "en");
  if (!englishLocale) {
    logError(ERL_FATAL,"Can not find required locale/en/messages in %s.\n",TkGate.homedir);
    exit(1);
  }


  /*
   * Get the locale code
   */
  getTkGateLocale(tcl, lang, territory);

  /*
   * Make sure the requested locale is available.  If not, use English
   */
  TkGate.locale = (Locale*) SHash_find(TkGate.localeTable, lang);
  if (!TkGate.locale) {
    printf("[No support for locale '%s'.  Reverting to English.]\n",lang);
    strcpy(lang,"en");
    TkGate.locale = englishLocale;
  }

  /*
   * Set flag if we have a Japanese locale
   */
  TkGate.japaneseMode = (strcmp(lang,"ja") == 0);

  /*
   * Read the messages for the selected local.
   */
  message_table = readMessagesFile(TkGate.locale);

  /*
   * Test the loaded locale against the English locale to look for missing tags, etc.
   */
  if (strcmp(lang,"en") != 0) {
    verifyMessagesFile(message_table,englishLocale); 
  }


  if (tcl) {
    Tcl_SetVar(tcl,"lang",lang,TCL_GLOBAL_ONLY);
    Tcl_SetVar(tcl,"territory",territory,TCL_GLOBAL_ONLY);
    Tcl_CreateCommand(tcl,"m",gat_msgLookup,(ClientData)0,0);
  }
}

/*
 * tcl handler code for message lookup function 'm'.
 */
static int gat_msgLookup(ClientData d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  char *msg;

  if (argc < 2) return TCL_OK;

  msg = msgLookup(argv[1]);
  if (strlen(msg) < 127)
    strcpy(tcl->result,msg);
  else
    tcl->result = strdup(msg);

  return TCL_OK;
}
