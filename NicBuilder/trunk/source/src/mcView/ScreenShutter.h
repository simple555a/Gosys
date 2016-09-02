#pragma once

//CScreenShutter
//屏幕截图

class CScreenShutter 
{

public:
	CScreenShutter();
	~CScreenShutter();
	//保存m_pBackBitmap中储存的位图数据到磁盘文件中
	//BOOL SaveBitmapToFile(LPCWSTR lpFileName);
	BOOL SaveBitmapToFile(CString FileName);
	// 截图屏幕指定区域
	// 结果保存在成员变量m_pBackBitmap中
    CScreenShutter* getScreenBitmap(POINT pt1, POINT pt2);
	// 截图屏幕完整区域（全屏）
	// 结果保存在成员变量m_pBackBitmap中
	CScreenShutter* getScreenBitmap(bool flag);

public:
	CBitmap* m_pBitmap; //存储背景图片

private:
	CDC ScrDC; //显示器	
};