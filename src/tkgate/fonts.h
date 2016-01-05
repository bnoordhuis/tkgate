#ifndef __tkgate_fonts_h
#define __tkgate_fonts_h

/*
 * Font family codes
 */
#define FF_COURIER	0
#define FF_HELVETICA	1
#define FF_TIMES	2
#define FF_SYMBOL	3
#define FF_NOKANJI_MAX	4
#define FF_KANJI	4
#define FF_MAX		5

/*
 * Font property codes
 */
#define FP_ROMAN	0
#define FP_BOLD		1
#define FP_ITALIC	2
#define FP_MAX		4

/*
 * Font size codes
 */
#define FS_TINY		0
#define FS_VSMALL	1
#define FS_SMALL	2
#define FS_NORMAL	3
#define FS_LARGE	4
#define FS_HUGE		5
#define FS_XHUGE	6
#define FS_MAX		7

typedef unsigned int fontfamily_t;
typedef unsigned int fontprop_t;
typedef unsigned int fontsize_t;

XFontStruct *GetXFont(fontfamily_t ff,fontprop_t fp,fontsize_t fs,int zoom);
XFontStruct **GetXFonts(fontfamily_t,fontprop_t,fontsize_t);
void UnloadAllFonts();

int getKanjiFontSize(int fs);
int getFontSize(int fs);
const char *getFontFamilyName(fontfamily_t ff);

#endif
