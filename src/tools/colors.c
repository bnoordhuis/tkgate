/*
 * This program is used to sort the colors into classes of similar colors
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RGBFILE "/usr/X11R6/lib/X11/rgb.txt"


#define NUMCLASSES 7
#define NUMCOLORSINCLASS 11

#define NUMCOLORS (NUMCLASSES*NUMCOLORSINCLASS)

#define DTHRESH 10

typedef struct { 
  char *name;
  int r,g,b;
  int cclass;
  int cstrength;
} ColorInfo;


char *colors[] = {
  "red","tan4","RosyBrown", "orange", "brown", "sienna", "MistyRose3", "firebrick", "red3", "coral1", "seashell",
  "green","green4", "chartreuse4", "YellowGreen", "aquamarine", "OliveDrab","honeydew", "DarkSeaGreen3", "green2", "turquoise", "aquamarine3",
  "blue", "RoyalBlue", "purple", "SkyBlue", "lavender", "navy", "GhostWhite", "MediumBlue", "SlateBlue", "MediumOrchid4", "CornflowerBlue",
  "magenta","magenta4", "thistle2", "plum3", "orchid4", "violet", "magenta2", "magenta3", "plum3", "DarkMagenta", "LavenderBlush2",
  "yellow", "yellow2", "LemonChiffon", "cornsilk", "beige", "khaki3", "khaki1", "yellow3", "yellow4", "seashell2", "ivory4",
  "cyan", "cyan4", "PowderBlue", "AliceBlue", "azure", "DarkSlateGray", "LightCyan3", "cyan2", "LightSeaGreen", "CadetBlue", "PaleTurquoise",
  "black","gray10","gray20","gray30", "gray40", "gray50","gray60", "gray70", "gray80","gray90","white",
};

ColorInfo colorTable[NUMCOLORS];

ColorInfo classColors[NUMCLASSES][NUMCOLORSINCLASS];
int numInClass[NUMCLASSES];

#define ApproxEq(a,b) ( ((a) < (b) + DTHRESH) &&  ((a) > (b) - DTHRESH) )

int isMixColor(int c1,int c2,int c3)
{
  return (ApproxEq(c1,c2) && c1 > c3 && c2 > c3);
}

int chooseClass(ColorInfo *C)
{
  if (C->r == C->g && C->r == C->b)
    return 6;				// Black/white/gray color


  if (isMixColor(C->r,C->b,C->g))
    return 3;				// magenta-like color
  if (isMixColor(C->r,C->g,C->b))
    return 4;				// yellow-like color
  if (isMixColor(C->g,C->b,C->r))
    return 5;				// cyan-like color

  if ( (C->r > C->g) && (C->r > C->b))
    return 0;				// red-like color
  if ( (C->g > C->r) && (C->g > C->b))
    return 1;				// green-like color
  if ( (C->b > C->g) && (C->b > C->r))
    return 2;				// blue-like color

  return 6;				// Don't know		
}

int computeStrength(ColorInfo *C)
{
  switch (C->cclass) {
  case 0 :
    return C->r - C->g - C->b;
  case 1 :
    return C->g - C->r - C->b;
  case 2 :
    return C->b - C->r - C->g;
  case 3 :
    return C->r + C->b - C->g;
  case 4 :
    return C->r + C->g - C->b;
  case 5 :
    return C->g + C->b - C->r;
  case 6 :
    return 255 - C->r;
  }
  return 0;
}

int lookup(ColorInfo *C)
{
  FILE *f = fopen(RGBFILE,"r");
  char buf[1024],name[1024];
  int r,g,b;

  while (fgets(buf,1024,f)) {
    if (sscanf(buf,"%d %d %d %[^\n]",&r,&g,&b,name) == 4 && strcmp(C->name,name) == 0) {
      fclose(f);
      C->r = r;
      C->g = g;
      C->b = b;
      C->cclass = chooseClass(C);
      C->cstrength = computeStrength(C);
      return 1;
    }
  }

  fclose(f);
  return 0;
}

int colorCompare(const void *vA,const void *vB)
{
  ColorInfo *A = (ColorInfo *) vA;
  ColorInfo *B = (ColorInfo *) vB;

  return B->cstrength - A->cstrength;
}

void showColors()
{
  int c,i;
  int N = sizeof(colors)/sizeof(colors[0]);

  for (c = 0;c < NUMCLASSES;c++)
    numInClass[c] = 0;

  for (i = 0;i < N;i++) {

    colorTable[i].name = colors[i];
    if (!lookup(&colorTable[i])) {
      printf("bad color %s\n",colors[i]);
      exit(1);
    }

    c = colorTable[i].cclass;
    if (numInClass[c] < NUMCOLORSINCLASS)
      classColors[c][numInClass[c]++] = colorTable[i];
  }

  printf("set colors {\n");
  for (c = 0;c < NUMCLASSES;c++) {
    qsort(classColors[c],numInClass[c],sizeof(classColors[0][0]),colorCompare);
    printf("    ");
    for (i = 0;i < numInClass[c];i++) {
      printf(" %s",classColors[c][i].name);
    }
    printf(" \n");
  }
  printf("}\n");
}

void listClassColors(int c)
{
  ColorInfo table[5000];
  int N = 0;
  FILE *f = fopen(RGBFILE,"r");
  char buf[1024],name[1024];
  int r,g,b;
  ColorInfo C;
  int i;

  while (fgets(buf,1024,f)) {
    if (sscanf(buf,"%d %d %d %[^\n]",&r,&g,&b,name) == 4) {
      C.name = strdup(name);
      C.r = r;
      C.g = g;
      C.b = b;
      C.cclass = chooseClass(&C);
      C.cstrength = computeStrength(&C);
      if (C.cclass == c)
	table[N++] = C;
    }
  }
  fclose(f);

  qsort(table,N,sizeof(table[0]),colorCompare);
  for (i = 0;i < N;i++) {
    if (strchr(table[i].name,' ') == 0)
      printf("%s\n",table[i].name);
  }
}

int main(int argc,char *argv[])
{
  int c;

  if (argc <= 1) {
    showColors();
    exit(0);
  }

  if (sscanf(argv[1],"%d",&c) == 1)
    listClassColors(c);

  return 0;
}
