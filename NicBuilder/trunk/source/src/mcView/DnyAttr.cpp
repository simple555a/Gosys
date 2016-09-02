#include "stdafx.h"
#include "DnyAttr.h"

CDnyAttr::CDnyAttr()
{
	m_Linked=false;
	m_rank=0;
	m_na=false;
	m_vi=true;
	m_en=true;
	m_push=0;
	m_ClsPop=FALSE;
	//触动链接
	m_InputD.m_Used=false;
	m_InputA.m_Used=false;
		m_InputA.m_MinVal=0.;
		m_InputA.m_MaxVal=100.;
	m_SlideH.m_Used=false;
		m_SlideH.m_Ref=0;
		m_SlideH.m_LTO=0.;
		m_SlideH.m_LTT=100.;
		m_SlideH.m_RBO=0.;
		m_SlideH.m_RBT=100.;
	m_SlideV.m_Used=false;
		m_SlideV.m_Ref=2;
		m_SlideV.m_LTO=0.;
		m_SlideV.m_LTT=100.;
		m_SlideV.m_RBO=0.;
		m_SlideV.m_RBT=100.;
	m_ButtonD.m_Used=false;
		m_ButtonD.m_Action=0;
		m_ButtonD.m_pulseWidth = -1;
	m_ButtonA.m_Used=false;
		m_ButtonA.m_Fuction=0;
		m_ButtonA.m_DownLimit=0.;
		m_ButtonA.m_UpLimit=100.;
		m_ButtonA.m_Delta=1.;
	m_ButtonWnd.m_Used=false;
		m_ButtonWnd.m_Mode=0;
	//显示链接
	m_ValueD.m_Used=false;
	m_ValueA.m_Used=false;
	m_ColorPenD.m_Used=false;
		m_ColorPenD.m_ColorOff=0;
		m_ColorPenD.m_ColorOn=0;
	m_ColorPenA.m_Used=false;
		m_ColorPenA.m_Color0=0;
		m_ColorPenA.m_Color1=0;
		m_ColorPenA.m_Color2=0;
		m_ColorPenA.m_Color3=0;
		m_ColorPenA.m_Color4=0;
		m_ColorPenA.m_Split1=0.;
		m_ColorPenA.m_Split2=0.;
		m_ColorPenA.m_Split3=0.;
		m_ColorPenA.m_Split4=0.;
	m_ColorPenDA.m_Used=false;
		m_ColorPenDA.m_ColorAlarm=0;
		m_ColorPenDA.m_ColorNomal=0;
	m_ColorPenAA.m_Used=false;
		m_ColorPenAA.m_ColorH2=0;
		m_ColorPenAA.m_ColorH1=0;
		m_ColorPenAA.m_ColorN=0;
		m_ColorPenAA.m_ColorL1=0;
		m_ColorPenAA.m_ColorL2=0;
		m_ColorPenAA.m_ColorR=0;
	m_ColorBrushD.m_Used=false;
		m_ColorBrushD.m_ColorOff=0;
		m_ColorBrushD.m_ColorOn=0;
	m_ColorBrushA.m_Used=false;
		m_ColorBrushA.m_Color0=0;
		m_ColorBrushA.m_Color1=0;
		m_ColorBrushA.m_Color2=0;
		m_ColorBrushA.m_Color3=0;
		m_ColorBrushA.m_Color4=0;
		m_ColorBrushA.m_Split1=0.;
		m_ColorBrushA.m_Split2=0.;
		m_ColorBrushA.m_Split3=0.;
		m_ColorBrushA.m_Split4=0.;
	m_ColorBrushDA.m_Used=false;
		m_ColorBrushDA.m_ColorAlarm=0;
		m_ColorBrushDA.m_ColorNomal=0;
	m_ColorBrushAA.m_Used=false;
		m_ColorBrushAA.m_ColorH2=0;
		m_ColorBrushAA.m_ColorH1=0;
		m_ColorBrushAA.m_ColorN=0;
		m_ColorBrushAA.m_ColorL1=0;
		m_ColorBrushAA.m_ColorL2=0;
		m_ColorBrushAA.m_ColorR=0;
	m_ColorTextD.m_Used=false;
		m_ColorTextD.m_ColorOff=0;
		m_ColorTextD.m_ColorOn=0;
	m_ColorTextA.m_Used=false;
		m_ColorTextA.m_Color0=0;
		m_ColorTextA.m_Color1=0;
		m_ColorTextA.m_Color2=0;
		m_ColorTextA.m_Color3=0;
		m_ColorTextA.m_Color4=0;
		m_ColorTextA.m_Split1=0.;
		m_ColorTextA.m_Split2=0.;
		m_ColorTextA.m_Split3=0.;
		m_ColorTextA.m_Split4=0.;
	m_ColorTextDA.m_Used=false;
		m_ColorTextDA.m_ColorAlarm=0;
		m_ColorTextDA.m_ColorNomal=0;
	m_ColorTextAA.m_Used=false;
		m_ColorTextAA.m_ColorH2=0;
		m_ColorTextAA.m_ColorH1=0;
		m_ColorTextAA.m_ColorN=0;
		m_ColorTextAA.m_ColorL1=0;
		m_ColorTextAA.m_ColorL2=0;
		m_ColorTextAA.m_ColorR=0;
	m_SizeH.m_Used=false;
		m_SizeH.m_LTO=0.;
		m_SizeH.m_LTT=0.;
		m_SizeH.m_RBO=100.;
		m_SizeH.m_RBT=100.;
		m_SizeH.m_Ref=0;
	m_SizeV.m_Used=false;
		m_SizeV.m_LTO=0.;
		m_SizeV.m_LTT=0.;
		m_SizeV.m_RBO=100.;
		m_SizeV.m_RBT=100.;
		m_SizeV.m_Ref=2;
	m_PosH.m_Used=false;
		m_PosH.m_LTO=0.;
		m_PosH.m_LTT=0.;
		m_PosH.m_RBO=100.;
		m_PosH.m_RBT=100.;
		m_PosH.m_Ref=0;
	m_PosV.m_Used=false;
		m_PosV.m_LTO=0.;
		m_PosV.m_LTT=0.;
		m_PosV.m_RBO=100.;
		m_PosV.m_RBT=100.;
		m_PosV.m_Ref=2;
	m_FillH.m_Used=false;
		m_FillH.m_LTO=0.;
		m_FillH.m_LTT=0.;
		m_FillH.m_RBO=100.;
		m_FillH.m_RBT=100.;
		m_FillH.m_Ref=1;
	m_FillV.m_Used=false;
		m_FillV.m_LTO=0.;
		m_FillV.m_LTT=0.;
		m_FillV.m_RBO=100.;
		m_FillV.m_RBT=100.;
		m_FillV.m_Ref=0;
	m_Visible.m_Used=false;
		m_Visible.m_Visble=true;
	m_Blink.m_Used=false;
		m_Blink.m_BlinkSpeed=1;
		m_Blink.m_ColorBrush=0;
		m_Blink.m_ColorPen=0;
		m_Blink.m_ColorText=0;
		m_Blink.m_VisibleAtOn=true;
	m_Direction.m_Used=false;
		m_Direction.m_XBias=0.;
		m_Direction.m_YBias=0.;
		m_Direction.m_MinClockO=0.;
		m_Direction.m_MinClockT=0.;
		m_Direction.m_MaxClockO=100.;
		m_Direction.m_MaxClockT=360.;
	m_Enable.m_Used=false;
		m_Enable.m_Enable=true;
}

CDnyAttr::~CDnyAttr()
{
}

void CDnyAttr::Save(MSXML::IXMLDOMDocumentPtr pDoc, MSXML::IXMLDOMElementPtr pDynamic)
{
	CString strTemp;
	_variant_t val;

	MSXML::IXMLDOMElementPtr pSubAttr;
	MSXML::IXMLDOMElementPtr pItem;

	//blink
	if(m_Blink.m_Used){
		pSubAttr=pDoc->createElement(L"blink");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_Blink.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_Blink.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//visible at on
		val.vt=VT_BOOL;
		if(m_Blink.m_VisibleAtOn) val.boolVal=-1;
		else val.boolVal=FALSE;
		pSubAttr->setAttribute(L"v",val);
		//blink speed
		val.vt=VT_I4;
		val.lVal=m_Blink.m_BlinkSpeed;
		pSubAttr->setAttribute(L"s",val);
		//pen color
		val.vt=VT_UI4;
		val.ulVal=m_Blink.m_ColorPen;
		pSubAttr->setAttribute(L"cp",val);
		//brush color
		val.vt=VT_UI4;
		val.ulVal=m_Blink.m_ColorBrush;
		pSubAttr->setAttribute(L"cb",val);
		//text color
		val.vt=VT_UI4;
		val.ulVal=m_Blink.m_ColorText;
		pSubAttr->setAttribute(L"ct",val);
	}

	//button_a
	if(m_ButtonA.m_Used){
		pSubAttr=pDoc->createElement(L"button_a");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ButtonA.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ButtonA.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//mode
		val.vt=VT_I4;
		val.lVal=m_ButtonA.m_Fuction;
		pSubAttr->setAttribute(L"m",val);
		//down limit
		val.vt=VT_R4;
		val.fltVal=m_ButtonA.m_DownLimit;
		pSubAttr->setAttribute(L"dl",val);
		//up limit
		val.vt=VT_R4;
		val.fltVal=m_ButtonA.m_UpLimit;
		pSubAttr->setAttribute(L"ul",val);
	   //delta
		val.vt=VT_R4;
		val.fltVal=m_ButtonA.m_Delta;
		pSubAttr->setAttribute(L"d",val);
	}

	//button_d
	if(m_ButtonD.m_Used){
		pSubAttr=pDoc->createElement(L"button_d");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ButtonD.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ButtonD.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//mode
		val.vt=VT_I4;
		val.lVal=m_ButtonD.m_Action;
		pSubAttr->setAttribute(L"m",val);
	}

	//button_wnd
	if(m_ButtonWnd.m_Used){
		pSubAttr=pDoc->createElement(L"button_wnd");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ButtonWnd.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//window name
		val.vt=VT_BSTR;
		val.bstrVal=m_ButtonWnd.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"wnd",val);
		//mode
		val.vt=VT_I4;
		val.lVal=m_ButtonWnd.m_Mode;
		pSubAttr->setAttribute(L"m",val);
	}

	//color_brush_a
	if(m_ColorBrushA.m_Used){
		pSubAttr=pDoc->createElement(L"color_brush_a");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorBrushA.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorBrushA.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//split1
		val.vt=VT_R4;
		val.fltVal=m_ColorBrushA.m_Split1;
		pSubAttr->setAttribute(L"s1",val);
		//split2
		val.vt=VT_R4;
		val.fltVal=m_ColorBrushA.m_Split2;
		pSubAttr->setAttribute(L"s2",val);
		//split3
		val.vt=VT_R4;
		val.fltVal=m_ColorBrushA.m_Split3;
		pSubAttr->setAttribute(L"s3",val);
		//split4
		val.vt=VT_R4;
		val.fltVal=m_ColorBrushA.m_Split4;
		pSubAttr->setAttribute(L"s4",val);
		//color0
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushA.m_Color0;
		pSubAttr->setAttribute(L"c0",val);
		//color1
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushA.m_Color1;
		pSubAttr->setAttribute(L"c1",val);
		//color2
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushA.m_Color2;
		pSubAttr->setAttribute(L"c2",val);
		//color3
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushA.m_Color3;
		pSubAttr->setAttribute(L"c3",val);
		//color4
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushA.m_Color4;
		pSubAttr->setAttribute(L"c4",val);
	}

	//color_brush_aa
	if(m_ColorBrushAA.m_Used){
		pSubAttr=pDoc->createElement(L"color_brush_aa");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorBrushAA.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorBrushAA.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//color h2
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushAA.m_ColorH2;
		pSubAttr->setAttribute(L"ch2",val);
		//color h1
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushAA.m_ColorH1;
		pSubAttr->setAttribute(L"ch1",val);
		//color normal
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushAA.m_ColorN;
		pSubAttr->setAttribute(L"cn",val);
		//color l1
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushAA.m_ColorL1;
		pSubAttr->setAttribute(L"cl1",val);
		//color l2
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushAA.m_ColorL2;
		pSubAttr->setAttribute(L"cl2",val);
		//color rate
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushAA.m_ColorR;
		pSubAttr->setAttribute(L"cr",val);
	}

	//color_brush_d
	if(m_ColorBrushD.m_Used){
		pSubAttr=pDoc->createElement(L"color_brush_d");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorBrushD.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorBrushD.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//color on
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushD.m_ColorOn;
		pSubAttr->setAttribute(L"cn",val);
		//color off
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushD.m_ColorOff;
		pSubAttr->setAttribute(L"cf",val);
	}

	//color_brush_da
	if(m_ColorBrushDA.m_Used){
		pSubAttr=pDoc->createElement(L"color_bursh_da");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorBrushDA.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorBrushDA.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//color on
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushDA.m_ColorNomal;
		pSubAttr->setAttribute(L"cn",val);
		//color off
		val.vt=VT_UI4;
		val.ulVal=m_ColorBrushDA.m_ColorAlarm;
		pSubAttr->setAttribute(L"ca",val);
	}

	//color_pen_a
	if(m_ColorPenA.m_Used){
		pSubAttr=pDoc->createElement(L"color_pen_a");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorPenA.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorPenA.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//split1
		val.vt=VT_R4;
		val.fltVal=m_ColorPenA.m_Split1;
		pSubAttr->setAttribute(L"s1",val);
		//split2
		val.vt=VT_R4;
		val.fltVal=m_ColorPenA.m_Split2;
		pSubAttr->setAttribute(L"s2",val);
		//split3
		val.vt=VT_R4;
		val.fltVal=m_ColorPenA.m_Split3;
		pSubAttr->setAttribute(L"s3",val);
		//split4
		val.vt=VT_R4;
		val.fltVal=m_ColorPenA.m_Split4;
		pSubAttr->setAttribute(L"s4",val);
		//color0
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenA.m_Color0;
		pSubAttr->setAttribute(L"c0",val);
		//color1
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenA.m_Color1;
		pSubAttr->setAttribute(L"c1",val);
		//color2
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenA.m_Color2;
		pSubAttr->setAttribute(L"c2",val);
		//color3
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenA.m_Color3;
		pSubAttr->setAttribute(L"c3",val);
		//color4
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenA.m_Color4;
		pSubAttr->setAttribute(L"c4",val);
	}

	//color_pen_aa
	if(m_ColorPenAA.m_Used){
		pSubAttr=pDoc->createElement(L"color_pen_aa");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorPenAA.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorPenAA.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//color h2
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenAA.m_ColorH2;
		pSubAttr->setAttribute(L"ch2",val);
		//color h1
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenAA.m_ColorH1;
		pSubAttr->setAttribute(L"ch1",val);
		//color normal
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenAA.m_ColorN;
		pSubAttr->setAttribute(L"cn",val);
		//color l1
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenAA.m_ColorL1;
		pSubAttr->setAttribute(L"cl1",val);
		//color l2
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenAA.m_ColorL2;
		pSubAttr->setAttribute(L"cl2",val);
		//color rate
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenAA.m_ColorR;
		pSubAttr->setAttribute(L"cr",val);
	}

	//color_pen_d
	if(m_ColorPenD.m_Used){
		pSubAttr=pDoc->createElement(L"color_pen_d");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorPenD.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorPenD.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//color on
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenD.m_ColorOn;
		pSubAttr->setAttribute(L"cn",val);
		//color off
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenD.m_ColorOff;
		pSubAttr->setAttribute(L"cf",val);
	}

	//color_pen_da
	if(m_ColorPenDA.m_Used){
		pSubAttr=pDoc->createElement(L"color_pen_da");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorPenDA.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorPenDA.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//color on
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenDA.m_ColorNomal;
		pSubAttr->setAttribute(L"cn",val);
		//color off
		val.vt=VT_UI4;
		val.ulVal=m_ColorPenDA.m_ColorAlarm;
		pSubAttr->setAttribute(L"ca",val);
	}

	//color_text_a
	if(m_ColorTextA.m_Used){
		pSubAttr=pDoc->createElement(L"color_text_a");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorTextA.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorTextA.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//split1
		val.vt=VT_R4;
		val.fltVal=m_ColorTextA.m_Split1;
		pSubAttr->setAttribute(L"s1",val);
		//split2
		val.vt=VT_R4;
		val.fltVal=m_ColorTextA.m_Split2;
		pSubAttr->setAttribute(L"s2",val);
		//split3
		val.vt=VT_R4;
		val.fltVal=m_ColorTextA.m_Split3;
		pSubAttr->setAttribute(L"s3",val);
		//split4
		val.vt=VT_R4;
		val.fltVal=m_ColorTextA.m_Split4;
		pSubAttr->setAttribute(L"s4",val);
		//color0
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextA.m_Color0;
		pSubAttr->setAttribute(L"c0",val);
		//color1
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextA.m_Color1;
		pSubAttr->setAttribute(L"c1",val);
		//color2
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextA.m_Color2;
		pSubAttr->setAttribute(L"c2",val);
		//color3
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextA.m_Color3;
		pSubAttr->setAttribute(L"c3",val);
		//color4
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextA.m_Color4;
		pSubAttr->setAttribute(L"c4",val);
	}

	//color_text_aa
	if(m_ColorTextAA.m_Used){
		pSubAttr=pDoc->createElement(L"color_text_aa");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorTextAA.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorTextAA.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//color h2
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextAA.m_ColorH2;
		pSubAttr->setAttribute(L"ch2",val);
		//color h1
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextAA.m_ColorH1;
		pSubAttr->setAttribute(L"ch1",val);
		//color normal
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextAA.m_ColorN;
		pSubAttr->setAttribute(L"cn",val);
		//color l1
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextAA.m_ColorL1;
		pSubAttr->setAttribute(L"cl1",val);
		//color l2
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextAA.m_ColorL2;
		pSubAttr->setAttribute(L"cl2",val);
		//color rate
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextAA.m_ColorR;
		pSubAttr->setAttribute(L"cr",val);
	}

	//color_text_d
	if(m_ColorTextD.m_Used){
		pSubAttr=pDoc->createElement(L"color_text_d");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorTextD.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorTextD.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//color on
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextD.m_ColorOn;
		pSubAttr->setAttribute(L"cn",val);
		//color off
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextD.m_ColorOff;
		pSubAttr->setAttribute(L"cf",val);
	}

	//color_text_da
	if(m_ColorTextDA.m_Used){
		pSubAttr=pDoc->createElement(L"color_text_da");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorTextDA.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ColorTextDA.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//color on
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextDA.m_ColorNomal;
		pSubAttr->setAttribute(L"cn",val);
		//color off
		val.vt=VT_UI4;
		val.ulVal=m_ColorTextDA.m_ColorAlarm;
		pSubAttr->setAttribute(L"ca",val);
	}

	//direction
	if(m_Direction.m_Used){
		pSubAttr=pDoc->createElement(L"direction");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_Direction.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_Direction.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//max clock o
		val.vt=VT_R4;
		val.fltVal=m_Direction.m_MaxClockO;
		pSubAttr->setAttribute(L"xo",val);
		//max clock t
		val.vt=VT_R4;
		val.fltVal=m_Direction.m_MaxClockT;
		pSubAttr->setAttribute(L"xt",val);
		//min clock o
		val.vt=VT_R4;
		val.fltVal=m_Direction.m_MinClockO;
		pSubAttr->setAttribute(L"no",val);
		//min clock t
		val.vt=VT_R4;
		val.fltVal=m_Direction.m_MinClockT;
		pSubAttr->setAttribute(L"nt",val);
		//x bias
		val.vt=VT_R4;
		val.fltVal=m_Direction.m_XBias;
		pSubAttr->setAttribute(L"xb",val);
		//y bias
		val.vt=VT_R4;
		val.fltVal=m_Direction.m_YBias;
		pSubAttr->setAttribute(L"yb",val);
	}

	//enable
	if(m_Enable.m_Used){
		pSubAttr=pDoc->createElement(L"enable");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_Enable.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_Enable.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//enable
		val.vt=VT_BOOL;
		if(m_Enable.m_Enable) val.boolVal=-1;
		else val.boolVal=FALSE;
		pSubAttr->setAttribute(L"e",val);
	}

	//fill_h
	if(m_FillH.m_Used){
		pSubAttr=pDoc->createElement(L"fill_h");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_FillH.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_FillH.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//reference
		val.vt=VT_I4;
		val.lVal=m_FillH.m_Ref;
		pSubAttr->setAttribute(L"r",val);
		//left top o
		val.vt=VT_R4;
		val.fltVal=m_FillH.m_LTO;
		pSubAttr->setAttribute(L"lto",val);
		//left top t
		val.vt=VT_R4;
		val.fltVal=m_FillH.m_LTT;
		pSubAttr->setAttribute(L"ltt",val);
		//right bottom o
		val.vt=VT_R4;
		val.fltVal=m_FillH.m_RBO;
		pSubAttr->setAttribute(L"rbo",val);
		//right bottom t
		val.vt=VT_R4;
		val.fltVal=m_FillH.m_RBT;
		pSubAttr->setAttribute(L"rbt",val);
	}

	//fill_v
	if(m_FillV.m_Used){
		pSubAttr=pDoc->createElement(L"fill_v");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_FillV.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_FillV.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//reference
		val.vt=VT_I4;
		val.lVal=m_FillV.m_Ref;
		pSubAttr->setAttribute(L"r",val);
		//left top o
		val.vt=VT_R4;
		val.fltVal=m_FillV.m_LTO;
		pSubAttr->setAttribute(L"lto",val);
		//left top t
		val.vt=VT_R4;
		val.fltVal=m_FillV.m_LTT;
		pSubAttr->setAttribute(L"ltt",val);
		//right bottom o
		val.vt=VT_R4;
		val.fltVal=m_FillV.m_RBO;
		pSubAttr->setAttribute(L"rbo",val);
		//right bottom t
		val.vt=VT_R4;
		val.fltVal=m_FillV.m_RBT;
		pSubAttr->setAttribute(L"rbt",val);
	}

	//input_a
	if(m_InputA.m_Used){
		pSubAttr=pDoc->createElement(L"input_a");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_InputA.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_InputA.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//window prompt
		val.vt=VT_BSTR;
		val.bstrVal=m_InputA.m_WndPrm.AllocSysString();
		pSubAttr->setAttribute(L"wp",val);
		//max value
		val.vt=VT_R4;
		val.fltVal=m_InputA.m_MaxVal;
		pSubAttr->setAttribute(L"xv",val);
		//min value
		val.vt=VT_R4;
		val.fltVal=m_InputA.m_MinVal;
		pSubAttr->setAttribute(L"nv",val);
	}

	//input_d
	if(m_InputD.m_Used){
		pSubAttr=pDoc->createElement(L"input_d");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_InputD.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_InputD.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//window prompt
		val.vt=VT_BSTR;
		val.bstrVal=m_InputD.m_WndPrm.AllocSysString();
		pSubAttr->setAttribute(L"wp",val);
		//set prompt
		val.vt=VT_BSTR;
		val.bstrVal=m_InputD.m_SetPrm.AllocSysString();
		pSubAttr->setAttribute(L"sp",val);
		//reset prompt
		val.vt=VT_BSTR;
		val.bstrVal=m_InputD.m_ResetPrm.AllocSysString();
		pSubAttr->setAttribute(L"rp",val);
		//on message
		val.vt=VT_BSTR;
		val.bstrVal=m_InputD.m_OnMsg.AllocSysString();
		pSubAttr->setAttribute(L"nm",val);
		//off message
		val.vt=VT_BSTR;
		val.bstrVal=m_InputD.m_OffMsg.AllocSysString();
		pSubAttr->setAttribute(L"fm",val);
	}

	//pos_h
	if(m_PosH.m_Used){
		pSubAttr=pDoc->createElement(L"pos_h");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_PosH.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_PosH.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//reference
		val.vt=VT_I4;
		val.lVal=m_PosH.m_Ref;
		pSubAttr->setAttribute(L"r",val);
		//left top o
		val.vt=VT_R4;
		val.fltVal=m_PosH.m_LTO;
		pSubAttr->setAttribute(L"lto",val);
		//left top t
		val.vt=VT_R4;
		val.fltVal=m_PosH.m_LTT;
		pSubAttr->setAttribute(L"ltt",val);
		//right bottom o
		val.vt=VT_R4;
		val.fltVal=m_PosH.m_RBO;
		pSubAttr->setAttribute(L"rbo",val);
		//right bottom t
		val.vt=VT_R4;
		val.fltVal=m_PosH.m_RBT;
		pSubAttr->setAttribute(L"rbt",val);
	}

	//pos_v
	if(m_PosV.m_Used){
		pSubAttr=pDoc->createElement(L"pos_v");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_PosV.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_PosV.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//reference
		val.vt=VT_I4;
		val.lVal=m_PosV.m_Ref;
		pSubAttr->setAttribute(L"r",val);
		//left top o
		val.vt=VT_R4;
		val.fltVal=m_PosV.m_LTO;
		pSubAttr->setAttribute(L"lto",val);
		//left top t
		val.vt=VT_R4;
		val.fltVal=m_PosV.m_LTT;
		pSubAttr->setAttribute(L"ltt",val);
		//right bottom o
		val.vt=VT_R4;
		val.fltVal=m_PosV.m_RBO;
		pSubAttr->setAttribute(L"rbo",val);
		//right bottom t
		val.vt=VT_R4;
		val.fltVal=m_PosV.m_RBT;
		pSubAttr->setAttribute(L"rbt",val);
	}

	//size_h
	if(m_SizeH.m_Used){
		pSubAttr=pDoc->createElement(L"size_h");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_SizeH.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_SizeH.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//reference
		val.vt=VT_I4;
		val.lVal=m_SizeH.m_Ref;
		pSubAttr->setAttribute(L"r",val);
		//left top o
		val.vt=VT_R4;
		val.fltVal=m_SizeH.m_LTO;
		pSubAttr->setAttribute(L"lto",val);
		//left top t
		val.vt=VT_R4;
		val.fltVal=m_SizeH.m_LTT;
		pSubAttr->setAttribute(L"ltt",val);
		//right bottom o
		val.vt=VT_R4;
		val.fltVal=m_SizeH.m_RBO;
		pSubAttr->setAttribute(L"rbo",val);
		//right bottom t
		val.vt=VT_R4;
		val.fltVal=m_SizeH.m_RBT;
		pSubAttr->setAttribute(L"rbt",val);
	}

	//size_v
	if(m_SizeV.m_Used){
		pSubAttr=pDoc->createElement(L"size_v");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_SizeV.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_SizeV.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//reference
		val.vt=VT_I4;
		val.lVal=m_SizeV.m_Ref;
		pSubAttr->setAttribute(L"r",val);
		//left top o
		val.vt=VT_R4;
		val.fltVal=m_SizeV.m_LTO;
		pSubAttr->setAttribute(L"lto",val);
		//left top t
		val.vt=VT_R4;
		val.fltVal=m_SizeV.m_LTT;
		pSubAttr->setAttribute(L"ltt",val);
		//right bottom o
		val.vt=VT_R4;
		val.fltVal=m_SizeV.m_RBO;
		pSubAttr->setAttribute(L"rbo",val);
		//right bottom t
		val.vt=VT_R4;
		val.fltVal=m_SizeV.m_RBT;
		pSubAttr->setAttribute(L"rbt",val);
	}

	//slide_h
	if(m_SlideH.m_Used){
		pSubAttr=pDoc->createElement(L"slide_h");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_SlideH.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_SlideH.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//reference
		val.vt=VT_I4;
		val.lVal=m_SlideH.m_Ref;
		pSubAttr->setAttribute(L"r",val);
		//left top o
		val.vt=VT_R4;
		val.fltVal=m_SlideH.m_LTO;
		pSubAttr->setAttribute(L"lto",val);
		//left top t
		val.vt=VT_R4;
		val.fltVal=m_SlideH.m_LTT;
		pSubAttr->setAttribute(L"ltt",val);
		//right bottom o
		val.vt=VT_R4;
		val.fltVal=m_SlideH.m_RBO;
		pSubAttr->setAttribute(L"rbo",val);
		//right bottom t
		val.vt=VT_R4;
		val.fltVal=m_SlideH.m_RBT;
		pSubAttr->setAttribute(L"rbt",val);
	}

	//slide_v
	if(m_SlideV.m_Used){
		pSubAttr=pDoc->createElement(L"slide_v");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_SlideV.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_SlideV.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//reference
		val.vt=VT_I4;
		val.lVal=m_SlideV.m_Ref;
		pSubAttr->setAttribute(L"r",val);
		//left top o
		val.vt=VT_R4;
		val.fltVal=m_SlideV.m_LTO;
		pSubAttr->setAttribute(L"lto",val);
		//left top t
		val.vt=VT_R4;
		val.fltVal=m_SlideV.m_LTT;
		pSubAttr->setAttribute(L"ltt",val);
		//right bottom o
		val.vt=VT_R4;
		val.fltVal=m_SlideV.m_RBO;
		pSubAttr->setAttribute(L"rbo",val);
		//right bottom t
		val.vt=VT_R4;
		val.fltVal=m_SlideV.m_RBT;
		pSubAttr->setAttribute(L"rbt",val);
	}

	//value_a
	if(m_ValueA.m_Used){
		pSubAttr=pDoc->createElement(L"value_a");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ValueA.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ValueA.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//format
		val.vt=VT_BSTR;
		val.bstrVal=m_ValueA.m_Format.AllocSysString();
		pSubAttr->setAttribute(L"f",val);
	}

	//value_d
	if(m_ValueD.m_Used){
		pSubAttr=pDoc->createElement(L"value_d");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_ValueD.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ValueD.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//on message
		val.vt=VT_BSTR;
		val.bstrVal=m_ValueD.m_OnMsg.AllocSysString();
		pSubAttr->setAttribute(L"nm",val);
		//off message
		val.vt=VT_BSTR;
		val.bstrVal=m_ValueD.m_OffMsg.AllocSysString();
		pSubAttr->setAttribute(L"fm",val);
	}

	//visible
	if(m_Visible.m_Used){
		pSubAttr=pDoc->createElement(L"visible");
		pDynamic->appendChild(pSubAttr);
		//tag lable for cell
		val.vt=VT_BSTR;
		val.bstrVal=m_Visible.m_TagLable.AllocSysString();
		pSubAttr->setAttribute(L"lab",val);
		//tagname
		val.vt=VT_BSTR;
		val.bstrVal=m_ValueD.m_TagName.AllocSysString();
		pSubAttr->setAttribute(L"tag",val);
		//visible
		val.vt=VT_BOOL;
		if(m_Visible.m_Visble) val.boolVal=-1;
		else val.boolVal=FALSE;
		pSubAttr->setAttribute(L"v",val);
	}
}

void CDnyAttr::Load(MSXML::IXMLDOMElementPtr pDynamic)
{
	HRESULT hr;
	MSXML::IXMLDOMElementPtr pSubAttr;
	MSXML::IXMLDOMNodePtr pNode;
	_variant_t val;
	
	//rank
	hr=pDynamic->raw_selectSingleNode(L"./rank",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		val=pSubAttr->getAttribute(L"r");
		val.ChangeType(VT_I4);
		m_rank=val.lVal;
	}

	//blink
	hr=pDynamic->raw_selectSingleNode(L"./blink",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_Blink.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_Blink.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_Blink.m_TagName=val.bstrVal;
		//visible at on
		val=pSubAttr->getAttribute(L"v");
		val.ChangeType(VT_BOOL);
		if(val.boolVal) m_Blink.m_VisibleAtOn=true;
		else m_Blink.m_VisibleAtOn=false;
		//blink speed
		val=pSubAttr->getAttribute(L"s");
		val.ChangeType(VT_I4);
		m_Blink.m_BlinkSpeed=val.lVal;
		//pen color
		val=pSubAttr->getAttribute(L"cp");
		val.ChangeType(VT_UI4);
		m_Blink.m_ColorPen=val.ulVal;
		//brush color
		val=pSubAttr->getAttribute(L"cb");
		val.ChangeType(VT_UI4);
		m_Blink.m_ColorBrush=val.ulVal;
		//text color
		val=pSubAttr->getAttribute(L"ct");
		val.ChangeType(VT_UI4);
		m_Blink.m_ColorText=val.ulVal;
	}

	//button_a
	hr=pDynamic->raw_selectSingleNode(L"./button_a",&pNode);
	if(!(pNode==NULL)){
		m_push=BUTTON_A;
		pSubAttr=pNode;
		m_ButtonA.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ButtonA.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ButtonA.m_TagName=val.bstrVal;
		//mode
		val=pSubAttr->getAttribute(L"m");
		val.ChangeType(VT_I4);
		m_ButtonA.m_Fuction=val.lVal;
		//down limit
		val=pSubAttr->getAttribute(L"dl");
		val.ChangeType(VT_R4);
		m_ButtonA.m_DownLimit=val.fltVal;
		//up limit
		val=pSubAttr->getAttribute(L"ul");
		val.ChangeType(VT_R4);
		m_ButtonA.m_UpLimit=val.fltVal;
	   //delta
		val=pSubAttr->getAttribute(L"d");
		val.ChangeType(VT_R4);
		m_ButtonA.m_Delta=val.fltVal;
	}

	//button_d
	hr=pDynamic->raw_selectSingleNode(L"./button_d",&pNode);
	if(!(pNode==NULL)){
		m_push=BUTTON_D;
		pSubAttr=pNode;
		m_ButtonD.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ButtonD.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ButtonD.m_TagName=val.bstrVal;
		//mode
		val=pSubAttr->getAttribute(L"m");
		val.ChangeType(VT_I4);
		m_ButtonD.m_Action=val.lVal;
		// pulse width
		val = pSubAttr->getAttribute(L"w");
		if(val.vt != VT_EMPTY && val.vt != VT_NULL){
			val.ChangeType(VT_I4);
			m_ButtonD.m_pulseWidth = val.lVal;
		}
	}

	//button_wnd
	hr=pDynamic->raw_selectSingleNode(L"./button_wnd",&pNode);
	if(!(pNode==NULL)){
		m_push=BUTTON_WND;
		pSubAttr=pNode;
		m_ButtonWnd.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ButtonWnd.m_TagLable=val.bstrVal;
		//window name
		val=pSubAttr->getAttribute(L"wnd");
		m_ButtonWnd.m_TagName=val.bstrVal;
		//mode
		val=pSubAttr->getAttribute(L"m");
		val.ChangeType(VT_I4);
		m_ButtonWnd.m_Mode=val.lVal;
	}

	//button close popup
	hr=pDynamic->raw_selectSingleNode(L"./button_cls_pop",&pNode);
	if(!(pNode==NULL)){
		m_ClsPop=TRUE;
	}

	//color_brush_a
	hr=pDynamic->raw_selectSingleNode(L"./color_brush_a",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_ColorBrushA.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ColorBrushA.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ColorBrushA.m_TagName=val.bstrVal;
		//split1
		val=pSubAttr->getAttribute(L"s1");
		val.ChangeType(VT_R4);
		m_ColorBrushA.m_Split1=val.fltVal;
		//split2
		val=pSubAttr->getAttribute(L"s2");
		val.ChangeType(VT_R4);
		m_ColorBrushA.m_Split2=val.fltVal;
		//split3
		val=pSubAttr->getAttribute(L"s3");
		val.ChangeType(VT_R4);
		m_ColorBrushA.m_Split3=val.fltVal;
		//split4
		val=pSubAttr->getAttribute(L"s4");
		val.ChangeType(VT_R4);
		m_ColorBrushA.m_Split4=val.fltVal;
		//color0
		val=pSubAttr->getAttribute(L"c0");
		val.ChangeType(VT_UI4);
		m_ColorBrushA.m_Color0=val.ulVal;
		//color1
		val=pSubAttr->getAttribute(L"c1");
		val.ChangeType(VT_UI4);
		m_ColorBrushA.m_Color1=val.ulVal;
		//color2
		val=pSubAttr->getAttribute(L"c2");
		val.ChangeType(VT_UI4);
		m_ColorBrushA.m_Color2=val.ulVal;
		//color3
		val=pSubAttr->getAttribute(L"c3");
		val.ChangeType(VT_UI4);
		m_ColorBrushA.m_Color3=val.ulVal;
		//color4
		val=pSubAttr->getAttribute(L"c4");
		val.ChangeType(VT_UI4);
		m_ColorBrushA.m_Color4=val.ulVal;
	}

	//color_brush_aa
	hr=pDynamic->raw_selectSingleNode(L"./color_brush_aa",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_ColorBrushAA.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ColorBrushAA.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ColorBrushAA.m_TagName=val.bstrVal;
		//color h2
		val=pSubAttr->getAttribute(L"ch2");
		val.ChangeType(VT_UI4);
		m_ColorBrushAA.m_ColorH2=val.ulVal;
		//color h1
		val=pSubAttr->getAttribute(L"ch1");
		val.ChangeType(VT_UI4);
		m_ColorBrushAA.m_ColorH1=val.ulVal;
		//color normal
		val=pSubAttr->getAttribute(L"cn");
		val.ChangeType(VT_UI4);
		m_ColorBrushAA.m_ColorN=val.ulVal;
		//color l1
		val=pSubAttr->getAttribute(L"cl1");
		val.ChangeType(VT_UI4);
		m_ColorBrushAA.m_ColorL1=val.ulVal;
		//color l2
		val=pSubAttr->getAttribute(L"cl2");
		val.ChangeType(VT_UI4);
		m_ColorBrushAA.m_ColorL2=val.ulVal;
		//color rate
		val=pSubAttr->getAttribute(L"cr");
		val.ChangeType(VT_UI4);
		m_ColorBrushAA.m_ColorR=val.ulVal;
	}

	//color_brush_d
	hr=pDynamic->raw_selectSingleNode(L"./color_brush_d",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_ColorBrushD.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ColorBrushD.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ColorBrushD.m_TagName=val.bstrVal;
		//color on
		val=pSubAttr->getAttribute(L"cn");
		val.ChangeType(VT_UI4);
		m_ColorBrushD.m_ColorOn=val.ulVal;
		//color off
		val=pSubAttr->getAttribute(L"cf");
		val.ChangeType(VT_UI4);
		m_ColorBrushD.m_ColorOff=val.ulVal;
	}

	//color_brush_da
	hr=pDynamic->raw_selectSingleNode(L"./color_brush_da",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_ColorBrushDA.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ColorBrushDA.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ColorBrushDA.m_TagName=val.bstrVal;
		//color on
		val=pSubAttr->getAttribute(L"cn");
		val.ChangeType(VT_UI4);
		m_ColorBrushDA.m_ColorNomal=val.ulVal;
		//color off
		val=pSubAttr->getAttribute(L"ca");
		val.ChangeType(VT_UI4);
		m_ColorBrushDA.m_ColorAlarm=val.ulVal;
	}

	//color_pen_a
	hr=pDynamic->raw_selectSingleNode(L"./color_pen_a",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_ColorPenA.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ColorPenA.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ColorPenA.m_TagName=val.bstrVal;
		//split1
		val=pSubAttr->getAttribute(L"s1");
		val.ChangeType(VT_R4);
		m_ColorPenA.m_Split1=val.fltVal;
		//split2
		val=pSubAttr->getAttribute(L"s2");
		val.ChangeType(VT_R4);
		m_ColorPenA.m_Split2=val.fltVal;
		//split3
		val=pSubAttr->getAttribute(L"s3");
		val.ChangeType(VT_R4);
		m_ColorPenA.m_Split3=val.fltVal;
		//split4
		val=pSubAttr->getAttribute(L"s4");
		val.ChangeType(VT_R4);
		m_ColorPenA.m_Split4=val.fltVal;
		//color0
		val=pSubAttr->getAttribute(L"c0");
		val.ChangeType(VT_UI4);
		m_ColorPenA.m_Color0=val.ulVal;
		//color1
		val=pSubAttr->getAttribute(L"c1");
		val.ChangeType(VT_UI4);
		m_ColorPenA.m_Color1=val.ulVal;
		//color2
		val=pSubAttr->getAttribute(L"c2");
		val.ChangeType(VT_UI4);
		m_ColorPenA.m_Color2=val.ulVal;
		//color3
		val=pSubAttr->getAttribute(L"c3");
		val.ChangeType(VT_UI4);
		m_ColorPenA.m_Color3=val.ulVal;
		//color4
		val=pSubAttr->getAttribute(L"c4");
		val.ChangeType(VT_UI4);
		m_ColorPenA.m_Color4=val.ulVal;
	}

	//color_pen_aa
	hr=pDynamic->raw_selectSingleNode(L"./color_pen_aa",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_ColorPenAA.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ColorPenAA.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ColorPenAA.m_TagName=val.bstrVal;
		//color h2
		val=pSubAttr->getAttribute(L"ch2");
		val.ChangeType(VT_UI4);
		m_ColorPenAA.m_ColorH2=val.ulVal;
		//color h1
		val=pSubAttr->getAttribute(L"ch1");
		val.ChangeType(VT_UI4);
		m_ColorPenAA.m_ColorH1=val.ulVal;
		//color normal
		val=pSubAttr->getAttribute(L"cn");
		val.ChangeType(VT_UI4);
		m_ColorPenAA.m_ColorN=val.ulVal;
		//color l1
		val=pSubAttr->getAttribute(L"cl1");
		val.ChangeType(VT_UI4);
		m_ColorPenAA.m_ColorL1=val.ulVal;
		//color l2
		val=pSubAttr->getAttribute(L"cl2");
		val.ChangeType(VT_UI4);
		m_ColorPenAA.m_ColorL2=val.ulVal;
		//color rate
		val=pSubAttr->getAttribute(L"cr");
		val.ChangeType(VT_UI4);
		m_ColorPenAA.m_ColorR=val.ulVal;
	}

	//color_pen_d
	hr=pDynamic->raw_selectSingleNode(L"./color_pen_d",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_ColorPenD.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ColorPenD.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ColorPenD.m_TagName=val.bstrVal;
		//color on
		val=pSubAttr->getAttribute(L"cn");
		val.ChangeType(VT_UI4);
		m_ColorPenD.m_ColorOn=val.ulVal;
		//color off
		val=pSubAttr->getAttribute(L"cf");
		val.ChangeType(VT_UI4);
		m_ColorPenD.m_ColorOff=val.ulVal;
	}

	//color_pen_da
	hr=pDynamic->raw_selectSingleNode(L"./color_pen_da",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_ColorPenDA.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ColorPenDA.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ColorPenDA.m_TagName=val.bstrVal;
		//color on
		val=pSubAttr->getAttribute(L"cn");
		val.ChangeType(VT_UI4);
		m_ColorPenDA.m_ColorNomal=val.ulVal;
		//color off
		val=pSubAttr->getAttribute(L"ca");
		val.ChangeType(VT_UI4);
		m_ColorPenDA.m_ColorAlarm=val.ulVal;
	}

	//color_text_a
	hr=pDynamic->raw_selectSingleNode(L"./color_text_a",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_ColorTextA.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ColorTextA.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ColorTextA.m_TagName=val.bstrVal;
		//split1
		val=pSubAttr->getAttribute(L"s1");
		val.ChangeType(VT_R4);
		m_ColorTextA.m_Split1=val.fltVal;
		//split2
		val=pSubAttr->getAttribute(L"s2");
		val.ChangeType(VT_R4);
		m_ColorTextA.m_Split2=val.fltVal;
		//split3
		val=pSubAttr->getAttribute(L"s3");
		val.ChangeType(VT_R4);
		m_ColorTextA.m_Split3=val.fltVal;
		//split4
		val=pSubAttr->getAttribute(L"s4");
		val.ChangeType(VT_R4);
		m_ColorTextA.m_Split4=val.fltVal;
		//color0
		val=pSubAttr->getAttribute(L"c0");
		val.ChangeType(VT_UI4);
		m_ColorTextA.m_Color0=val.ulVal;
		//color1
		val=pSubAttr->getAttribute(L"c1");
		val.ChangeType(VT_UI4);
		m_ColorTextA.m_Color1=val.ulVal;
		//color2
		val=pSubAttr->getAttribute(L"c2");
		val.ChangeType(VT_UI4);
		m_ColorTextA.m_Color2=val.ulVal;
		//color3
		val=pSubAttr->getAttribute(L"c3");
		val.ChangeType(VT_UI4);
		m_ColorTextA.m_Color3=val.ulVal;
		//color4
		val=pSubAttr->getAttribute(L"c4");
		val.ChangeType(VT_UI4);
		m_ColorTextA.m_Color4=val.ulVal;
	}

	//color_text_aa
	hr=pDynamic->raw_selectSingleNode(L"./color_text_aa",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_ColorTextAA.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ColorTextAA.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ColorTextAA.m_TagName=val.bstrVal;
		//color h2
		val=pSubAttr->getAttribute(L"ch2");
		val.ChangeType(VT_UI4);
		m_ColorTextAA.m_ColorH2=val.ulVal;
		//color h1
		val=pSubAttr->getAttribute(L"ch1");
		val.ChangeType(VT_UI4);
		m_ColorTextAA.m_ColorH1=val.ulVal;
		//color normal
		val=pSubAttr->getAttribute(L"cn");
		val.ChangeType(VT_UI4);
		m_ColorTextAA.m_ColorN=val.ulVal;
		//color l1
		val=pSubAttr->getAttribute(L"cl1");
		val.ChangeType(VT_UI4);
		m_ColorTextAA.m_ColorL1=val.ulVal;
		//color l2
		val=pSubAttr->getAttribute(L"cl2");
		val.ChangeType(VT_UI4);
		m_ColorTextAA.m_ColorL2=val.ulVal;
		//color rate
		val=pSubAttr->getAttribute(L"cr");
		val.ChangeType(VT_UI4);
		m_ColorTextAA.m_ColorR=val.ulVal;
	}

	//color_text_d
	hr=pDynamic->raw_selectSingleNode(L"./color_text_d",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_ColorTextD.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ColorTextD.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ColorTextD.m_TagName=val.bstrVal;
		//color on
		val=pSubAttr->getAttribute(L"cn");
		val.ChangeType(VT_UI4);
		m_ColorTextD.m_ColorOn=val.ulVal;
		//color off
		val=pSubAttr->getAttribute(L"cf");
		val.ChangeType(VT_UI4);
		m_ColorTextD.m_ColorOff=val.ulVal;
	}

	//color_text_da
	hr=pDynamic->raw_selectSingleNode(L"./color_text_da",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_ColorTextDA.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ColorTextDA.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ColorTextDA.m_TagName=val.bstrVal;
		//color on
		val=pSubAttr->getAttribute(L"cn");
		val.ChangeType(VT_UI4);
		m_ColorTextDA.m_ColorNomal=val.ulVal;
		//color off
		val=pSubAttr->getAttribute(L"ca");
		val.ChangeType(VT_UI4);
		m_ColorTextDA.m_ColorAlarm=val.ulVal;
	}

	//direction
	hr=pDynamic->raw_selectSingleNode(L"./direction",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_Direction.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_Direction.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_Direction.m_TagName=val.bstrVal;
		//max clock o
		val=pSubAttr->getAttribute(L"xo");
		val.ChangeType(VT_R4);
		m_Direction.m_MaxClockO=val.fltVal;
		//max clock t
		val=pSubAttr->getAttribute(L"xt");
		val.ChangeType(VT_R4);
		m_Direction.m_MaxClockT=val.fltVal;
		//min clock o
		val=pSubAttr->getAttribute(L"no");
		val.ChangeType(VT_R4);
		m_Direction.m_MinClockO=val.fltVal;
		//min clock t
		val=pSubAttr->getAttribute(L"nt");
		val.ChangeType(VT_R4);
		m_Direction.m_MinClockT=val.fltVal;
		//x bias
		val=pSubAttr->getAttribute(L"xb");
		val.ChangeType(VT_R4);
		m_Direction.m_XBias=val.fltVal;
		//y bias
		val=pSubAttr->getAttribute(L"yb");
		val.ChangeType(VT_R4);
		m_Direction.m_YBias=val.fltVal;
	}

	//enable
	hr=pDynamic->raw_selectSingleNode(L"./enable",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_Enable.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_Enable.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_Enable.m_TagName=val.bstrVal;
		//enable
		val=pSubAttr->getAttribute(L"e");		 
		val.ChangeType(VT_BOOL);
		if(val.boolVal) m_Enable.m_Enable=true;
		else m_Enable.m_Enable=false;
	}

	//fill_h
	hr=pDynamic->raw_selectSingleNode(L"./fill_h",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_FillH.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_FillH.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_FillH.m_TagName=val.bstrVal;
		//reference
		val=pSubAttr->getAttribute(L"r");
		val.ChangeType(VT_I4);
		m_FillH.m_Ref=val.lVal;
		//left top o
		val=pSubAttr->getAttribute(L"lto");
		val.ChangeType(VT_R4);
		m_FillH.m_LTO=val.fltVal;
		//left top t
		val=pSubAttr->getAttribute(L"ltt");
		val.ChangeType(VT_R4);
		m_FillH.m_LTT=val.fltVal;
		//right bottom o
		val=pSubAttr->getAttribute(L"rbo");
		val.ChangeType(VT_R4);
		m_FillH.m_RBO=val.fltVal;
		//right bottom t
		val=pSubAttr->getAttribute(L"rbt");
		val.ChangeType(VT_R4);
		m_FillH.m_RBT=val.fltVal;
	}

	//fill_v
	hr=pDynamic->raw_selectSingleNode(L"./fill_v",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_FillV.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_FillV.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_FillV.m_TagName=val.bstrVal;
		//reference
		val=pSubAttr->getAttribute(L"r");
		val.ChangeType(VT_I4);
		m_FillV.m_Ref=val.lVal;
		//left top o
		val=pSubAttr->getAttribute(L"lto");
		val.ChangeType(VT_R4);
		m_FillV.m_LTO=val.fltVal;
		//left top t
		val=pSubAttr->getAttribute(L"ltt");
		val.ChangeType(VT_R4);
		m_FillV.m_LTT=val.fltVal;
		//right bottom o
		val=pSubAttr->getAttribute(L"rbo");
		val.ChangeType(VT_R4);
		m_FillV.m_RBO=val.fltVal;
		//right bottom t
		val=pSubAttr->getAttribute(L"rbt");
		val.ChangeType(VT_R4);
		m_FillV.m_RBT=val.fltVal;
	}

	//input_a
	hr=pDynamic->raw_selectSingleNode(L"./input_a",&pNode);
	if(!(pNode==NULL)){
		m_push=INPUT_A;
		pSubAttr=pNode;
		m_InputA.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_InputA.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_InputA.m_TagName=val.bstrVal;
		//window prompt
		val=pSubAttr->getAttribute(L"wp");
		m_InputA.m_WndPrm=val.bstrVal;
		//max value
		val=pSubAttr->getAttribute(L"xv");
		val.ChangeType(VT_R4);
		m_InputA.m_MaxVal=val.fltVal;
		//min value
		val=pSubAttr->getAttribute(L"nv");
		val.ChangeType(VT_R4);
		m_InputA.m_MinVal=val.fltVal;
	}

	//input_d
	hr=pDynamic->raw_selectSingleNode(L"./input_d",&pNode);
	if(!(pNode==NULL)){
		m_push=INPUT_D;
		pSubAttr=pNode;
		m_InputD.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_InputD.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_InputD.m_TagName=val.bstrVal;
		//window prompt
		val=pSubAttr->getAttribute(L"wp");
		m_InputD.m_WndPrm=val.bstrVal;
		//set prompt
		val=pSubAttr->getAttribute(L"sp");
		m_InputD.m_SetPrm=val.bstrVal;
		//reset prompt
		val=pSubAttr->getAttribute(L"rp");
		m_InputD.m_ResetPrm=val.bstrVal;
		//on message
		val=pSubAttr->getAttribute(L"nm");
		m_InputD.m_OnMsg=val.bstrVal;
		//off message
		val=pSubAttr->getAttribute(L"fm");
		m_InputD.m_OffMsg=val.bstrVal;
	}

	//pos_h
	hr=pDynamic->raw_selectSingleNode(L"./pos_h",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_PosH.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_PosH.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_PosH.m_TagName=val.bstrVal;
		//reference
		val=pSubAttr->getAttribute(L"r");
		val.ChangeType(VT_I4);
		m_PosH.m_Ref=val.lVal;
		//left top o
		val=pSubAttr->getAttribute(L"lto");
		val.ChangeType(VT_R4);
		m_PosH.m_LTO=val.fltVal;
		//left top t
		val=pSubAttr->getAttribute(L"ltt");
		val.ChangeType(VT_R4);
		m_PosH.m_LTT=val.fltVal;
		//right bottom o
		val=pSubAttr->getAttribute(L"rbo");
		val.ChangeType(VT_R4);
		m_PosH.m_RBO=val.fltVal;
		//right bottom t
		val=pSubAttr->getAttribute(L"rbt");
		val.ChangeType(VT_R4);
		m_PosH.m_RBT=val.fltVal;
	}

	//pos_v
	hr=pDynamic->raw_selectSingleNode(L"./pos_v",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_PosV.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_PosV.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_PosV.m_TagName=val.bstrVal;
		//reference
		val=pSubAttr->getAttribute(L"r");
		val.ChangeType(VT_I4);
		m_PosV.m_Ref=val.lVal;
		//left top o
		val=pSubAttr->getAttribute(L"lto");
		val.ChangeType(VT_R4);
		m_PosV.m_LTO=val.fltVal;
		//left top t
		val=pSubAttr->getAttribute(L"ltt");
		val.ChangeType(VT_R4);
		m_PosV.m_LTT=val.fltVal;
		//right bottom o
		val=pSubAttr->getAttribute(L"rbo");
		val.ChangeType(VT_R4);
		m_PosV.m_RBO=val.fltVal;
		//right bottom t
		val=pSubAttr->getAttribute(L"rbt");
		val.ChangeType(VT_R4);
		m_PosV.m_RBT=val.fltVal;
	}

	//size_h
	hr=pDynamic->raw_selectSingleNode(L"./size_h",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_SizeH.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_SizeH.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_SizeH.m_TagName=val.bstrVal;
		//reference
		val=pSubAttr->getAttribute(L"r");
		val.ChangeType(VT_I4);
		m_SizeH.m_Ref=val.lVal;
		//left top o
		val=pSubAttr->getAttribute(L"lto");
		val.ChangeType(VT_R4);
		m_SizeH.m_LTO=val.fltVal;
		//left top t
		val=pSubAttr->getAttribute(L"ltt");
		val.ChangeType(VT_R4);
		m_SizeH.m_LTT=val.fltVal;
		//right bottom o
		val=pSubAttr->getAttribute(L"rbo");
		val.ChangeType(VT_R4);
		m_SizeH.m_RBO=val.fltVal;
		//right bottom t
		val=pSubAttr->getAttribute(L"rbt");
		val.ChangeType(VT_R4);
		m_SizeH.m_RBT=val.fltVal;
	}

	//size_v
	hr=pDynamic->raw_selectSingleNode(L"./size_v",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_SizeV.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_SizeV.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_SizeV.m_TagName=val.bstrVal;
		//reference
		val=pSubAttr->getAttribute(L"r");
		val.ChangeType(VT_I4);
		m_SizeV.m_Ref=val.lVal;
		//left top o
		val=pSubAttr->getAttribute(L"lto");
		val.ChangeType(VT_R4);
		m_SizeV.m_LTO=val.fltVal;
		//left top t
		val=pSubAttr->getAttribute(L"ltt");
		val.ChangeType(VT_R4);
		m_SizeV.m_LTT=val.fltVal;
		//right bottom o
		val=pSubAttr->getAttribute(L"rbo");
		val.ChangeType(VT_R4);
		m_SizeV.m_RBO=val.fltVal;
		//right bottom t
		val=pSubAttr->getAttribute(L"rbt");
		val.ChangeType(VT_R4);
		m_SizeV.m_RBT=val.fltVal;
	}

	//slide_h
	hr=pDynamic->raw_selectSingleNode(L"./slide_h",&pNode);
	if(!(pNode==NULL)){
		m_push=SLIDE_H;
		pSubAttr=pNode;
		m_SlideH.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_SlideH.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_SlideH.m_TagName=val.bstrVal;
		//reference
		val=pSubAttr->getAttribute(L"r");
		val.ChangeType(VT_I4);
		m_SlideH.m_Ref=val.lVal;
		//left top o
		val=pSubAttr->getAttribute(L"lto");
		val.ChangeType(VT_R4);
		m_SlideH.m_LTO=val.fltVal;
		//left top t
		val=pSubAttr->getAttribute(L"ltt");
		val.ChangeType(VT_R4);
		m_SlideH.m_LTT=val.fltVal;
		//right bottom o
		val=pSubAttr->getAttribute(L"rbo");
		val.ChangeType(VT_R4);
		m_SlideH.m_RBO=val.fltVal;
		//right bottom t
		val=pSubAttr->getAttribute(L"rbt");
		val.ChangeType(VT_R4);
		m_SlideH.m_RBT=val.fltVal;
	}

	//slide_v
	hr=pDynamic->raw_selectSingleNode(L"./slide_v",&pNode);
	if(!(pNode==NULL)){
		m_push=SLIDE_V;
		pSubAttr=pNode;
		m_SlideV.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_SlideV.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_SlideV.m_TagName=val.bstrVal;
		//reference
		val=pSubAttr->getAttribute(L"r");
		val.ChangeType(VT_I4);
		m_SlideV.m_Ref=val.lVal;
		//left top o
		val=pSubAttr->getAttribute(L"lto");
		val.ChangeType(VT_R4);
		m_SlideV.m_LTO=val.fltVal;
		//left top t
		val=pSubAttr->getAttribute(L"ltt");
		val.ChangeType(VT_R4);
		m_SlideV.m_LTT=val.fltVal;
		//right bottom o
		val=pSubAttr->getAttribute(L"rbo");
		val.ChangeType(VT_R4);
		m_SlideV.m_RBO=val.fltVal;
		//right bottom t
		val=pSubAttr->getAttribute(L"rbt");
		val.ChangeType(VT_R4);
		m_SlideV.m_RBT=val.fltVal;
	}

	//value_a
	hr=pDynamic->raw_selectSingleNode(L"./value_a",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_ValueA.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ValueA.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ValueA.m_TagName=val.bstrVal;
		//format
		val=pSubAttr->getAttribute(L"f");
		m_ValueA.m_Format=val.bstrVal;
	}

	//value_d
	hr=pDynamic->raw_selectSingleNode(L"./value_d",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_ValueD.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_ValueD.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_ValueD.m_TagName=val.bstrVal;
		//on message
		val=pSubAttr->getAttribute(L"nm");
		m_ValueD.m_OnMsg=val.bstrVal;
		//off message
		val=pSubAttr->getAttribute(L"fm");
		m_ValueD.m_OffMsg=val.bstrVal;
	}

	//visible
	hr=pDynamic->raw_selectSingleNode(L"./visible",&pNode);
	if(!(pNode==NULL)){
		pSubAttr=pNode;
		m_Visible.m_Used=true;
		//tag lable
		val=pSubAttr->getAttribute(L"lab");
		m_Visible.m_TagLable=val.bstrVal;
		//tagname
		val=pSubAttr->getAttribute(L"tag");
		m_Visible.m_TagName=val.bstrVal;
		//visible
		val=pSubAttr->getAttribute(L"v");
		val.ChangeType(VT_BOOL);
		if(val.boolVal) m_Visible.m_Visble=true;
		else m_Visible.m_Visble=false;
	}
}

void CDnyAttr::TagToLab()
{
	CDny * pDny;
	//blink
	pDny=(CDny*)(&m_Blink);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//button a
	pDny=(CDny*)(&m_ButtonA);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//button d
	pDny=(CDny*)(&m_ButtonD);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//button wnd
	pDny=(CDny*)(&m_ButtonWnd);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//color brush a
	pDny=(CDny*)(&m_ColorBrushA);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//color brush aa
	pDny=(CDny*)(&m_ColorBrushAA);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//color brush d
	pDny=(CDny*)(&m_ColorBrushD);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//color brush da
	pDny=(CDny*)(&m_ColorBrushDA);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//color pen a
	pDny=(CDny*)(&m_ColorPenA);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//color pen aa
	pDny=(CDny*)(&m_ColorPenAA);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//color pen d
	pDny=(CDny*)(&m_ColorPenD);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//color pen da
	pDny=(CDny*)(&m_ColorPenDA);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//color text a
	pDny=(CDny*)(&m_ColorTextA);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//color text aa
	pDny=(CDny*)(&m_ColorTextAA);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//color text d
	pDny=(CDny*)(&m_ColorTextD);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//color text da
	pDny=(CDny*)(&m_ColorTextDA);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//direction
	pDny=(CDny*)(&m_Direction);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//enable
	pDny=(CDny*)(&m_Enable);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//fill h
	pDny=(CDny*)(&m_FillH);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//fill v
	pDny=(CDny*)(&m_FillV);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//input a
	pDny=(CDny*)(&m_InputA);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//input d
	pDny=(CDny*)(&m_InputD);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//pos h
	pDny=(CDny*)(&m_PosH);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//pos v
	pDny=(CDny*)(&m_PosV);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//size h
	pDny=(CDny*)(&m_SizeH);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//size v
	pDny=(CDny*)(&m_SizeV);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//slide h
	pDny=(CDny*)(&m_SlideH);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//slide v
	pDny=(CDny*)(&m_SlideV);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//value a
	pDny=(CDny*)(&m_ValueA);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//value d
	pDny=(CDny*)(&m_ValueD);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
	//visible
	pDny=(CDny*)(&m_Visible);
	pDny->m_TagLable=pDny->m_TagName;
	pDny->m_TagName.Empty();
}	
