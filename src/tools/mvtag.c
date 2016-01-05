#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "hash.h" 

#define STRMAX 4096

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

void usage()
{
  printf("Usage: mvtag -L code mapfile\n");
  printf("       mvtag -a mapfile\n");
  printf("\n");
  printf("Renames tags in one or more tkgate messages files.  Must be run\n");
  printf("from the tkgate home directory.  The 'mapfile' is a list of rename\n");
  printf("pairs with one pair on each line.  The first word is the old tag value\n");
  printf("and the second is the new tag value.\n");
  exit(1);
}

void renameTag(const char *lang,SHash *msgMap)
{
  FILE *rf, *wf;
  char readFile[STRMAX],bakFile[STRMAX],tmpFile[STRMAX];
  char line[STRMAX],tag[STRMAX];

  sprintf(readFile,"locale/%s/messages",lang);
  sprintf(bakFile,"locale/%s/messages.bak",lang);
  sprintf(tmpFile,"locale/%s/messages.tmp",lang);

  rf = fopen(readFile,"r");
  if (!rf) {
    printf("mvtag: no messages file for locale <%s>.  skipping...\n",lang);
    return;
  }

  wf = fopen(tmpFile,"w");
  while (fgets(line,STRMAX,rf)) {
    if (sscanf(line,"%s",tag) == 1 && *tag != '#' && *tag != '\\') {
      char *newTag = SHash_find(msgMap,tag);
      if (newTag) {
	char *p = line+strspn(line," \t\r\n");
	int l1 = strlen(tag);
	int l2 = strlen(newTag);
	printf("in <%s> renaming <%s> to <%s>\n",lang,tag,newTag);
	memmove(p+l2,p+l1,strlen(p+l1)+1);
	memmove(p,newTag,l2);
      }
    }

    fputs(line,wf);
  }
  fclose(wf);
  fclose(rf);
  rename(readFile,bakFile);
  rename(tmpFile,readFile);
}

void readMapFile(const char *name,SHash *msgMap)
{
  FILE *f;
  char line[STRMAX],fromTag[STRMAX],toTag[STRMAX];

  f = fopen(name,"r");
  if (!f) {
    printf("can not open map file '%s'\n",name);
    exit(1);
  }

  while (fgets(line,STRMAX,f))
    if (sscanf(line,"%s %s",fromTag,toTag) == 2) 
      SHash_insert(msgMap,fromTag,strdup(toTag));
}

int main(int argc,char *argv[])
{
  const char *lang;
  SHash msgMap;

  SHash_init(&msgMap);

  if (argc < 2)
    usage();

  if (strcmp(argv[1],"-a") == 0) {
    if (argc != 3) usage();
    lang = 0;
    readMapFile(argv[2],&msgMap);
  } else if (strcmp(argv[1],"-L") == 0) {
    if (argc != 4) usage();
    lang = argv[2];
    readMapFile(argv[3],&msgMap);
  } else
    usage();

  if (lang == 0) {
    DIR *d;
    struct dirent *de;

    d = opendir("locale");
    if (!d) {
      printf("mvtag: no 'locale' sub-directory in current directory.\n");
      exit(1);
    }
    while ((de = readdir(d))) {
      if (*de->d_name == '.') continue;	/* not a locale */ 
      renameTag(de->d_name,&msgMap);
    }
  } else
    renameTag(lang,&msgMap);

  return 0;
}
