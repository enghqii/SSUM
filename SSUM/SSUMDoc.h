
// SSUMDoc.h : CSSUMDoc Ŭ������ �������̽�
//


#pragma once

#include "afxcoll.h"

class CSSUMDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CSSUMDoc();
	DECLARE_DYNCREATE(CSSUMDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CSSUMDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	CObArray m_Chatroom;//ä�ù� ����Ʈ ����
	CObArray m_Contents;//ä�ù濡 �� �� ä�ù��� ��ȭ���� ����
};
