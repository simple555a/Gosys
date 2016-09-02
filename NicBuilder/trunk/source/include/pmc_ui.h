#ifndef __pmcui_h__
#define __pmcui_h__

#ifdef PMCUI_EXPORTS
	#define PMCUI_API __declspec(dllexport)
#else
	#define PMCUI_API __declspec(dllimport)
	#ifndef RTK_NO_LIBS
		#pragma comment(lib, "pmcui.lib")
	#endif
#endif

#include <ui/Arc.h>
#include <ui/Button.h>
#include <ui/Cell.h>
#include <ui/Cursor.h>
#include <ui/Dnyattr.h>
#include <ui/Element.h>
#include <ui/Ellipse.h>
#include <ui/Line.h>
#include <ui/Meta.h>
#include <ui/Polygon.h>
#include <ui/PolyLine.h>
#include <ui/Rectangle.h>
#include <ui/RoundRect.h>
#include <ui/Text.h>
#include <ui/Trend.h>
#include <ui/Zoom.h>

#ifdef __cplusplus
extern "C"{
#endif

PMCUI_API HANDLE _stdcall pmcui_open_metafile(const char * fileName);

#ifdef __cplusplus
}
#endif

#endif