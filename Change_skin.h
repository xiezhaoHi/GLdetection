#pragma once
/*static �ؼ�ID*/

//����ģʽ
class CGLdetectionDlg;
class Change_skin
{
public:
	~Change_skin();
	HBRUSH& GetBrush(int const& index);
	HBRUSH& GetWhiteBrush();
	static Change_skin* GetInstance(CGLdetectionDlg* const&);
	void InitFace(void);
private:
	HWND m_hWnd;

	/*static ����*/
	CFont m_fontBigBox;
	CFont m_fontSmallBox;
	CGLdetectionDlg* m_dlg;
	HBRUSH m_brush[brush_max];
	HBRUSH m_white_brush;

	
	bool InitBrush();
	static Change_skin* m_single;
	
	Change_skin();
};

