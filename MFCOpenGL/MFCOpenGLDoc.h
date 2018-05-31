
// MFCOpenGLDoc.h : CMFCOpenGLDoc ��Ľӿ�
//


#pragma once
#include <list>
#include <Shape\Graph.h>
typedef std::list<Graph *> GLIST;

class CMFCOpenGLDoc : public CDocument
{
protected: // �������л�����
	CMFCOpenGLDoc();
	DECLARE_DYNCREATE(CMFCOpenGLDoc)

// ����
public:
	GLIST m_graphList;			//�����ѻ��Ƶ�ͼԪ

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CMFCOpenGLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
