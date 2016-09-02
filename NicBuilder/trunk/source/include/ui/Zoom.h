#if !defined(__ZOOM__)
#define __Zoom__

extern long PMCUI_API lZoomMode,lPixHor,lPixVer;

void PMCUI_API LPtoUP(long xl,long yl,long *xu, long *yu);
void PMCUI_API UPtoLP(long xu,long yu,long *xl,long *yl);

#endif
