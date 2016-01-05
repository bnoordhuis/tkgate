#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "hash.h" 

#define STRMAX 1024
#define LONGSTRMAX 8096

typedef struct Message_str {
  char		*m_key;
  char		*m_str;
  int		m_isFill;
  int           m_isLong;
  int		m_renamed;
  int		m_isMatched;
  int		m_dupStr;
} Message;

typedef struct MsgTable_str {
  SHash		mt_keytab;
  SHash		mt_strtab;
} MsgTable;

/*
 * These undoable object functions are only used in the main tkgate code, but
 * since they are used in the common hash table code we provide stubs here for
 * the hash table code to use.
 */
void *ob_malloc(int s,char *x) { return malloc(s); }
void *ob_calloc(int n,int s,char *x) { return calloc(n,s); }
void ob_free(void *p) { free(p); }
char *ob_strdup(char *s) { return strdup(s); }
void ob_set_type(void *o,char *n) {}
void ob_touch(void *o) {}


Message *new_Message(const char *key,const char *str, int isFill, int isLong)
{
  Message *m = (Message*) malloc(sizeof(Message));

  m->m_key = strdup(key);
  m->m_str = strdup(str);
  m->m_isFill = isFill;
  m->m_isLong = isLong;
  m->m_renamed = 0;
  m->m_isMatched = 0;
  m->m_dupStr = 0;

  return m;
}
 

/*****************************************************************************
 *
 * Read a long message from a message file.
 *
 *****************************************************************************/
void readLongMsg(FILE *f,char *msg,int n,int doFill)
{
  char *p;
  int l;

  p = msg;
  while (fgets(p,n,f)) {
    if (doFill) {
      char *q = strchr(p,'\n');
      if (q) *q = ' ';
    }

    l = strlen(p);
    if (strncmp(p,"-end-",5) == 0) {
      if (p != msg)
	p[-1] = 0;
      else
	*p = 0;
      break;
    }
    p += l;
    n -= l;
    if (n <= 0) {
      fprintf(stderr, "Fatal Error: Message too long.\n");
      exit(1);
      break;
    }
  }
}

void MsgTable_init(MsgTable *mt)
{
  SHash_init(&mt->mt_keytab);
  SHash_init(&mt->mt_strtab);
}

MsgTable *new_MsgTable()
{
  MsgTable *mt = (MsgTable *) malloc(sizeof(MsgTable));
  
  MsgTable_init(mt);

  return mt;
}

/*****************************************************************************
 *
 * key		Message key
 * str		Message value
 * isFill	Fill code (0=normal, 1=filled)
 *
 *****************************************************************************/
void MsgTable_add(MsgTable *mt,const char *key,const char *str,int isFill,int isLong)
{
  Message *m = new_Message(key,str,isFill,isLong);

  SHash_insert(&mt->mt_keytab,key,m);

  if (SHash_find(&mt->mt_strtab,str))
    m->m_dupStr = 1;

  SHash_insert(&mt->mt_strtab,str,m);
}

int MsgTable_read(MsgTable *mt,const char *fileName)
{
  char buf[STRMAX], tag[STRMAX], msg[LONGSTRMAX];
  FILE *f;

  f = fopen(fileName,"r");
  if (f) {
    while (fgets(buf,1024,f)) {
      if (sscanf(buf,"\\font-encoding %s",tag) == 1) {
	/*ignore*/
      } else if (sscanf(buf,"\\%s",tag) == 1) {
	/*ignore*/
      } else if (sscanf(buf,"%s %[^\n]",tag,msg) == 2 && *tag != '#') {
	int isFill = 0;
	int isLong = 0;

	if (strcmp(msg,"-begin-") == 0) {
	  readLongMsg(f,msg,LONGSTRMAX,1);
	  isFill = 0;
	  isLong = 1;
	} else if (strcmp(msg,"-fillbegin-") == 0) {
	  readLongMsg(f,msg,LONGSTRMAX,1);
	  isFill = 1;
	  isLong = 1;
	}

	MsgTable_add(mt, tag, msg, isFill, isLong);
      }
    }
    fclose(f);
    return 0;
  } else
    return -1;
}

void usage()
{
  fprintf(stderr,"Usage: msgdiff -l <msgs>\n");
  fprintf(stderr,"       msgdiff <old> <new>\n");
  fprintf(stderr,"       msgdiff <old> <new> <edit>\n");
  fprintf(stderr,"       msgdiff -a <base> <edit>\n");
  fprintf(stderr,"\n");
  fprintf(stderr,"The first form lists the tags in a messages file.  The second form\n");
  fprintf(stderr,"lists the differences between <old> and <new>.  The third form looks\n");
  fprintf(stderr,"for differences between <old> and <new> and makes notes about those changes\n");
  fprintf(stderr,"in <edit> with a 'TODO' notation.  The last form looks for tags in <base>\n");
  fprintf(stderr,"that are not in <edit> and adds 'TODO' entries for the missing tags.\n");
  fprintf(stderr,"The last two forms write the new version of <edit> to standard output.\n");
  exit(1);
}

void tagList(MsgTable *MT)
{
  HashElem *he;

  for (he = Hash_first(&MT->mt_keytab);he;he = Hash_next(&MT->mt_keytab,he)) {
    Message *nm = (Message*) HashElem_obj(he);
    printf("%s\n",nm->m_key);
  }
}

void messageDiff(MsgTable *oldMT,MsgTable *newMT,MsgTable *editMT)
{
  HashElem *he;

  for (he = Hash_first(&newMT->mt_keytab);he;he = Hash_next(&newMT->mt_keytab,he)) {
    Message *nm = (Message*) HashElem_obj(he);
    Message *om = (Message*) SHash_find(&oldMT->mt_keytab,nm->m_key);
    Message *em = editMT ? (Message*) SHash_find(&editMT->mt_keytab,nm->m_key) : NULL;

    if (om) {
      if (strcmp(nm->m_str,om->m_str) != 0) {
	fprintf(stderr, "%-25s : content changed\n",nm->m_key);
      } else if (!om->m_isFill && nm->m_isFill) {
	if (em)
	  em->m_isFill = 1;

	fprintf(stderr, "%-25s : changed to -fillbegin- style\n",nm->m_key);
      } else if (om->m_isFill && !nm->m_isFill) {
	if (em)
	  em->m_isFill = 0;

	fprintf(stderr, "%-25s : changed to -begin- style\n",nm->m_key);
      }
      om->m_isMatched = 1;
    }
  }

  for (he = Hash_first(&newMT->mt_keytab);he;he = Hash_next(&newMT->mt_keytab,he)) {
    Message *nm = (Message*) HashElem_obj(he);
    Message *om = (Message*) SHash_find(&oldMT->mt_keytab,nm->m_key);
    Message *em = NULL;

    if (!om) {
      om = (Message*) SHash_find(&oldMT->mt_strtab,nm->m_str);
      if (om) {
	if (!om->m_isMatched) {
	  if (om->m_dupStr || nm->m_dupStr)
	    fprintf(stderr, "%-25s : possibly renamed to %s\n",om->m_key,nm->m_key);
	  else {
	    /* rename in edit if present under old name */
	    em = editMT ? (Message *) SHash_find(&editMT->mt_keytab,om->m_key) : NULL;
	    if (em) {
      	      SHash_remove(&editMT->mt_keytab,em->m_key);
	      free(em->m_key);
	      em->m_key = strdup(nm->m_key);
	      SHash_insert(&editMT->mt_keytab,em->m_key,em);
	    }

	    fprintf(stderr, "%-25s : renamed to %s\n",om->m_key,nm->m_key);
	  }
	  om->m_renamed = 1;
	}
      } else {
	fprintf(stderr, "%-25s : new message\n",nm->m_key);

	if (editMT) {
	  em = (Message *) malloc(sizeof(Message));
	  memcpy(em, nm, sizeof(Message));
	  em->m_key = strdup(nm->m_key);
	  em->m_str = malloc(strlen(nm->m_str) + sizeof("TODO: ") + 1);
	  strcpy(em->m_str, "TODO: ");
	  strcat(em->m_str, nm->m_str);
	  
	  SHash_insert(&editMT->mt_keytab,em->m_key,em);
	}
      }
    }
  }

  for (he = Hash_first(&oldMT->mt_keytab);he;he = Hash_next(&oldMT->mt_keytab,he)) {
    Message *om = (Message*) HashElem_obj(he);
    Message *nm = (Message*) SHash_find(&newMT->mt_keytab,om->m_key);
    Message *em = editMT ? (Message*) SHash_find(&editMT->mt_keytab,om->m_key) : NULL;

    if (!nm && !om->m_renamed) {
      if (em)
	SHash_remove(&editMT->mt_keytab,om->m_key);

      fprintf(stderr, "%-25s : message deleted\n",om->m_key);
    }
  }

  if (editMT) {
    for (he = Hash_first(&newMT->mt_keytab);he;he = Hash_next(&newMT->mt_keytab\
,he)) {
      Message *nm = (Message*) HashElem_obj(he);
      Message *em = (Message*) SHash_find(&editMT->mt_keytab,nm->m_key);
  
      /* added if in new but not the edit source (the odd corner case) */
      if (!em) {
        em = (Message *) malloc(sizeof(Message));
        memcpy(em, nm, sizeof(Message));
        em->m_key = strdup(nm->m_key);
        em->m_str = malloc(strlen(nm->m_str) + sizeof("TODO: ") + 1);
        strcpy(em->m_str, "TODO: ");
        strcat(em->m_str, nm->m_str);

        SHash_insert(&editMT->mt_keytab,em->m_key,em);

        fprintf(stderr, "%-25s : new message (in old, but not edit)\n",nm->m_key);
      }
    }
  }

}

/* 
 * use newfile and newMT as a template to write an edited
 * message file out
 *
 */
int dumpEdited(char *newfile, MsgTable *newMT, MsgTable *editMT, FILE *out) {
  char buf[STRMAX], tag[STRMAX], msg[STRMAX];
  FILE *f;

  f = fopen(newfile, "r");
  if (!f) return -1; /* shouldn't really happen */
  
  while (fgets(buf,sizeof(buf),f)) {
    if (sscanf(buf,"\\font-encoding %s",tag) == 1) {
      fprintf(out,"%s",buf);
      /*ignore*/
    } else if (sscanf(buf,"\\%s",tag) == 1) {
      fprintf(out,"%s",buf);
      /*ignore*/
    } else if (sscanf(buf,"%s %[^\n]",tag,msg) == 2 && *tag != '#') {
      Message *nm = (Message *)SHash_find(&newMT->mt_keytab, tag);
      Message *em = (Message *)SHash_find(&editMT->mt_keytab, tag);
      
      if (!em) { /* old file has message that edit file doesn't */
	/* discard entire message -- right thing to do? */
	fprintf(stderr, "Warning: new message '%s' not found in edited\n", tag);
	if (nm->m_isLong) {
	  while (fgets(buf,sizeof(buf),f))
	    if (strncmp(buf,"-end-",5) == 0) break;
	}
	continue;
      }

      if (nm->m_isLong) {
	if (nm->m_isFill) {
	  fprintf(out, "%s\t\t-fillbegin-\n%s\n-end-\n", tag, em->m_str);
	} else {
	  fprintf(out, "%s\t\t-begin-\n%s\n-end-\n", tag, em->m_str);
	}
	/* discard until -end- */
	while (fgets(buf,sizeof(buf),f))
	  if (strncmp(buf,"-end-",5) == 0) break;
	
      } else {
	fprintf(out, "%s\t\t%s\n", tag, em->m_str);
      }

    } else {
      /* copy everything else to edited file */
      fprintf(out,"%s",buf);
    }
  }

  fclose(f);
  return 0;
}

void addTags(const char *editFile,MsgTable *oldMT, MsgTable *editMT)
{
  HashElem *he;
  FILE *f;
  char buf[STRMAX], tag[STRMAX], msg[STRMAX];
  FILE *out = stdout;

  f = fopen(editFile, "r");
  if (!f) return; /* shouldn't really happen */

  while (fgets(buf,STRMAX, f))
    fputs(buf, stdout);
  fclose(f);
  fprintf(out,"\n");
  fprintf(out,"# New Tags\n");
  fprintf(out,"\n");
    


  for (he = Hash_first(&oldMT->mt_keytab);he;he = Hash_next(&oldMT->mt_keytab,he)) {
    Message *nm = (Message*) HashElem_obj(he);
    Message *em = (Message*) SHash_find(&editMT->mt_keytab,nm->m_key);
    if (!em) {
      fprintf(stderr,"add missing tag %s\n",nm->m_key);


      if (nm->m_isLong) {
	if (nm->m_isFill) {
	  fprintf(out, "%s\t\t-fillbegin-\nTODO: %s\n-end-\n", nm->m_key, nm->m_str);
	} else {
	  fprintf(out, "%s\t\t-begin-\nTODO: %s\n-end-\n", nm->m_key, nm->m_str);
	}
      } else {
	fprintf(out, "%s\t\tTODO: %s\n", nm->m_key, nm->m_str);
      }

    }
  }
}


/*
 *
 * Usage: msgdiff oldmsgs newmsgs [editmsgs]
 *        msgdiff -l msgs
 *        msgdiff -a eng-msgs editmsgs
 *
 */
int main(int argc,char *argv[])
{
  char *oldFile = 0,*newFile = 0, *editFile = 0;
  MsgTable *oldMT = 0,*newMT = 0, *editMT = 0;

  if (argc < 2)
    usage();

  if (strcmp(argv[1],"-l") == 0) {
    if (argc != 3) usage();
    oldFile = argv[2];

    oldMT = new_MsgTable();
    if (MsgTable_read(oldMT, oldFile) < 0) {
      fprintf(stderr,"msgdiff: could not read message file '%s'.\n",oldFile);
      exit(1);
    }
    tagList(oldMT);
  } else  if (strcmp(argv[1],"-a") == 0) {
    if (argc != 4) usage();
    oldFile = argv[2];
    editFile = argv[3];

    oldMT = new_MsgTable();
    if (MsgTable_read(oldMT, oldFile) < 0) {
      fprintf(stderr,"msgdiff: could not read message file '%s'.\n",oldFile);
      exit(1);
    }

    editFile = argv[3];
    editMT = new_MsgTable();
    if (MsgTable_read(editMT, editFile) < 0) {
      fprintf(stderr, "msgdiff: could not read message file '%s'.\n", editFile);
      exit(1);
    }

    addTags(editFile, oldMT, editMT);
  } else {
    if (argc !=3 && argc != 4) usage();
    oldFile = argv[1];
    newFile = argv[2];

    oldMT = new_MsgTable();
    if (MsgTable_read(oldMT, oldFile) < 0) {
      fprintf(stderr,"msgdiff: could not read message file '%s'.\n",oldFile);
      exit(1);
    }

    newMT = new_MsgTable();
    if (MsgTable_read(newMT, newFile) < 0) {
      fprintf(stderr,"msgdiff: could not read message file '%s'.\n",newFile);
      exit(1);
    }

    if (argc == 4) {
      editFile = argv[3];
      editMT = new_MsgTable();
      if (MsgTable_read(editMT, editFile) < 0) {
	fprintf(stderr, "msgdiff: could not read message file '%s'.\n", editFile);
	exit(1);
      }
    }

    messageDiff(oldMT, newMT, editMT);

    if (editMT) {
      if (dumpEdited(newFile, newMT, editMT, stdout) < 0) {
	fprintf(stderr, "msgdiff: failed to write out edited messages\n");
	exit(1);
      }
    }
  }
  
  return 0;
}
