#pragma once

//CScreenShutter
//��Ļ��ͼ

class CScreenShutter 
{

public:
	CScreenShutter();
	~CScreenShutter();
	//����m_pBackBitmap�д����λͼ���ݵ������ļ���
	//BOOL SaveBitmapToFile(LPCWSTR lpFileName);
	BOOL SaveBitmapToFile(CString FileName);
	// ��ͼ��Ļָ������
	// ��������ڳ�Ա����m_pBackBitmap��
    CScreenShutter* getScreenBitmap(POINT pt1, POINT pt2);
	// ��ͼ��Ļ��������ȫ����
	// ��������ڳ�Ա����m_pBackBitmap��
	CScreenShutter* getScreenBitmap(bool flag);

public:
	CBitmap* m_pBitmap; //�洢����ͼƬ

private:
	CDC ScrDC; //��ʾ��	
};