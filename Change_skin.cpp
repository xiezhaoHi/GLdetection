#include "stdafx.h"
#include "Change_skin.h"
#include "resource.h"
#include "GLdetectionDlg.h"

static int controlID[] = { IDC_STATIC_SO2,IDC_STATIC_NO,IDC_STATIC_CO,IDC_STATIC_NO2
,IDC_STATIC_O2,IDC_STATIC_CO2,-1 };
static int controlIDT[] = { IDC_STATIC_SO2_T,IDC_STATIC_NO_T,IDC_STATIC_CO_T,IDC_STATIC_NO2_T
,IDC_STATIC_O2_T,IDC_STATIC_CO2_T,-1 };
static int staticID[] = { IDC_STATIC_CK,IDC_STATIC_BTL,IDC_STATIC_SJW,IDC_STATIC_JYW
,IDC_STATIC_TZW,-1 };
static int staticIDT[] = { -1,IDC_STATIC_BTL_TEXT,IDC_STATIC_SJW_TEXT,IDC_STATIC_JYW_TEXT
,IDC_STATIC_TZW_TEXT,-1 };

Change_skin* Change_skin::m_single = NULL;
Change_skin::Change_skin()
{
	m_fontBigBox.CreatePointFont(165, _T("΢���ź�"));;
	m_fontSmallBox.CreatePointFont(140, _T("΢���ź�"));;
}
Change_skin* Change_skin::GetInstance(CGLdetectionDlg* const& dlg)
{
	if (nullptr == m_single)
	{
		m_single = new Change_skin;
		m_single->InitBrush(); //��ʼ�� ��ˢ
	}
	if (nullptr != m_single)
	{
		m_single->m_dlg = dlg;
	}
	return m_single;
}

Change_skin::~Change_skin()
{
	if (m_single)
	{
		delete m_single;
		m_single = NULL;
	}
	for (int index = 0; index < brush_max; ++index)
		DeleteObject(m_brush[index]);
}

/*�����ʼ��*/
void Change_skin::InitFace(void)
{
	if (nullptr == m_dlg)
	{
		return;
	}

	if (nullptr == m_dlg)
	{
		return;
	}


	static unsigned int uLeftLogoW = 237, uLeftLogoH = 140
		, uLogoW = 150, uLogoH = 90, uLeftSplitW = 2, uLeftSplitH = 960
		, uStaticW = 70, uStaticH = 30 //��ǩ ���
		, uTitleH = 64 //�����
		, uSpaceV = 41, uSpaceH = 30; //���� ������ ��static ��ֱ ˮƽ ���
	CRect rectShow, rectTemp;
	static unsigned int pic_top = 20; //��������ͼƬ��λ����Ϣ
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_LEFT))->ShowWindow(SW_HIDE);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_SHOW))->ShowWindow(SW_HIDE);


	/*button ��ʼ���*/
	m_dlg->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);

	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_LEFT_SPLIT))->SetWindowPos(NULL
		, uLeftLogoW, 0, uLeftSplitW, uLeftSplitH
		, SWP_SHOWWINDOW);

	/*logo*/
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_LEFT))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_LOG))->SetWindowPos(NULL
		,  (uLeftLogoW - uLogoW) / 2,  90
		, uLogoW, uLogoH, SWP_SHOWWINDOW);

	//���ڱ�ǩ
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_LOG))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_CK))->SetWindowPos(NULL
		, rectTemp.left , rectTemp.bottom+ uLogoH
		, uStaticW, uStaticH, SWP_SHOWWINDOW);


	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_CK))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_COMBO_COM))->SetWindowPos(NULL
		, rectTemp.right+ uSpaceH, rectTemp.top
		, uStaticW, uStaticH, SWP_SHOWWINDOW);

	//������
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_CK))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_BTL))->SetWindowPos(NULL
		, rectTemp.left, rectTemp.bottom+ uSpaceV
		, uStaticW, uStaticH, SWP_SHOWWINDOW);


	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_BTL))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_BTL_TEXT))->SetWindowPos(NULL
		, rectTemp.right + uSpaceH, rectTemp.top
		, uStaticW, uStaticH, SWP_SHOWWINDOW);

	//����λ
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_BTL))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_SJW))->SetWindowPos(NULL
		, rectTemp.left, rectTemp.bottom + uSpaceV
		, uStaticW, uStaticH, SWP_SHOWWINDOW);

	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_SJW))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_SJW_TEXT))->SetWindowPos(NULL
		, rectTemp.right + uSpaceH, rectTemp.top
		, uStaticW, uStaticH, SWP_SHOWWINDOW);

	//У��λ
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_SJW))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_JYW))->SetWindowPos(NULL
		, rectTemp.left, rectTemp.bottom + uSpaceV
		, uStaticW, uStaticH, SWP_SHOWWINDOW);

	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_JYW))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_JYW_TEXT))->SetWindowPos(NULL
		, rectTemp.right + uSpaceH, rectTemp.top
		, uStaticW, uStaticH, SWP_SHOWWINDOW);

	//ֹͣλ
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_JYW))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_TZW))->SetWindowPos(NULL
		, rectTemp.left, rectTemp.bottom + uSpaceV
		, uStaticW, uStaticH, SWP_SHOWWINDOW);

	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_TZW))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_TZW_TEXT))->SetWindowPos(NULL
		, rectTemp.right + uSpaceH, rectTemp.top
		, uStaticW, uStaticH, SWP_SHOWWINDOW);


	//��ʼ��ť
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_TZW))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_BUTTON_START))->SetWindowPos(NULL
		, rectTemp.left-23, rectTemp.bottom+200
		, 190, 48, SWP_SHOWWINDOW);




	//group box ����
	unsigned int uWndW = 1280, uWndH = 1024 //���ڴ�С
		, uRightLeftW = 14, uRightTopW = 64 //�ұ� ����߿ؼ� ���� �� ����ؼ� ����
		, uRightCenterW = 953 // �м�ָ��� ����
		, uBoxW = 441, uBoxH = 194 //Сbox �� ��
		, uRightBoxW = 1012, uRightH = 852 //���box �� ��
		, uBoxSpace = 25, uRightBoxSpace = (uRightBoxW - uBoxW * 2 - uBoxSpace) / 2 //����box ��� �� Сbox���� ��box ����
		, uBoxSpaceV = 122;//Сbox ��ֱ �����box 
		
	
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_LEFT_SPLIT))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_RIGHT))->SetWindowPos(NULL
		, rectTemp.left+uRightLeftW, rectTemp.top+ uRightTopW
		, uRightBoxW, uRightH, SWP_SHOWWINDOW);
	
	//�м� �ָ���
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_RIGHT))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_RIGHT_CENTER))->SetWindowPos(NULL
		, rectTemp.left + 30
		, rectTemp.top + uBoxSpaceV*2 + uBoxH
		, uRightCenterW,2,  SWP_SHOWWINDOW);

	//������group box
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_RIGHT))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_ONE))->SetWindowPos(NULL
		, rectTemp.left + uRightBoxSpace, rectTemp.top + uBoxSpaceV
		, uBoxW
		, uBoxH, SWP_SHOWWINDOW);


	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_ONE))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_TWO))->SetWindowPos(NULL
		, rectTemp.right + uBoxSpace, rectTemp.top
		, rectTemp.Width()
		, rectTemp.Height() , SWP_SHOWWINDOW);

	//������group box
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_ONE))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);

	CRect centerRect;
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_RIGHT_CENTER))->GetWindowRect(centerRect);
	m_dlg->ScreenToClient(&centerRect);

	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_THREE))->SetWindowPos(NULL
		, rectTemp.left , centerRect.bottom + uBoxSpaceV
		, rectTemp.Width()
		, rectTemp.Height() , SWP_SHOWWINDOW);


	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_THREE))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_FOUR))->SetWindowPos(NULL
		, rectTemp.right + uBoxSpace, rectTemp.top
		, rectTemp.Width()
		, rectTemp.Height(), SWP_SHOWWINDOW);


	// ��ʾ�ؼ� IDC_STATIC_NO, IDC_STATIC_CO, IDC_STATIC_NO2, IDC_STATIC_O2
	unsigned int uShowTextW = 240, uShowTextH = 86, //��ʾ �ؼ� �� ��
		uStaticTextW = 90, uStaticTextH = 43 //��λ ���
		, uStaticSpaceV = 60
		, uStaticSpaceH = 60;

	//��ʾ �ؼ�
	//NO
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_ONE))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_ONE))->SetWindowText(_T("  һ������(NO)  "));
	
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_NO))->SetWindowPos(NULL
		, rectTemp.left + uStaticSpaceH
		, rectTemp.top + uStaticSpaceV
		, uShowTextW
		, uShowTextH, SWP_SHOWWINDOW);
	//��λ
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_NO))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_NO_T))->SetWindowPos(NULL
		, rectTemp.right
		, rectTemp.bottom - uStaticTextH
		, uStaticTextW
		, uStaticTextH, SWP_SHOWWINDOW);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_NO_T))->GetWindowRect(rectTemp);

	//NOX
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_TWO))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_TWO))->SetWindowText(_T("  ��������(NOX)  "));

	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_NO2))->SetWindowPos(NULL
		, rectTemp.left + uStaticSpaceH
		, rectTemp.top + uStaticSpaceV
		, uShowTextW
		, uShowTextH, SWP_SHOWWINDOW);
	//��λ
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_NO2))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);

	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_NO2_T))->SetWindowPos(NULL
		, rectTemp.right
		, rectTemp.bottom - uStaticTextH
		, uStaticTextW
		, uStaticTextH, SWP_SHOWWINDOW);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_NO2_T))->GetWindowRect(rectTemp);

	//CO
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_THREE))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_THREE))->SetWindowText(_T("  һ����̼(CO)  "));

	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_CO))->SetWindowPos(NULL
		, rectTemp.left + uStaticSpaceH
		, rectTemp.top + uStaticSpaceV
		, uShowTextW
		, uShowTextH, SWP_SHOWWINDOW);
	//��λ
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_CO))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);

	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_CO_T))->SetWindowPos(NULL
		, rectTemp.right
		, rectTemp.bottom - uStaticTextH
		, uStaticTextW
		, uStaticTextH, SWP_SHOWWINDOW);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_CO_T))->GetWindowRect(rectTemp);

	//02
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_FOUR))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_FOUR))->SetWindowText(_T("  ����(O2)  "));

	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_O2))->SetWindowPos(NULL
		, rectTemp.left + uStaticSpaceH
		, rectTemp.top + uStaticSpaceV
		, uShowTextW
		, uShowTextH, SWP_SHOWWINDOW);
	//��λ
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_O2))->GetWindowRect(rectTemp);
	m_dlg->ScreenToClient(&rectTemp);

	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_O2_T))->SetWindowPos(NULL
		, rectTemp.right
		, rectTemp.bottom - uStaticTextH
		, uStaticTextW
		, uStaticTextH, SWP_SHOWWINDOW);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_O2_T))->GetWindowRect(rectTemp);


	//�ұ� group box
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_RIGHT))->SetWindowText(_T("  �������  "));
	
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_RIGHT))->SetFont(&m_fontBigBox);

	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_ONE))->SetFont(&m_fontSmallBox);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_TWO))->SetFont(&m_fontSmallBox);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_THREE))->SetFont(&m_fontSmallBox);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_FOUR))->SetFont(&m_fontSmallBox);
	
	//���� SO2 CO2

	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_SO2))->ShowWindow(SW_HIDE);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_SO2_T))->ShowWindow(SW_HIDE);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_Bit_SO2))->ShowWindow(SW_HIDE);

	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_CO2))->ShowWindow(SW_HIDE);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_CO2_T))->ShowWindow(SW_HIDE);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_Bit_CO2))->ShowWindow(SW_HIDE);


	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_Bit_NO))->ShowWindow(SW_HIDE);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_Bit_CO))->ShowWindow(SW_HIDE);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_Bit_NO2))->ShowWindow(SW_HIDE);
	((CButton*)m_dlg->GetDlgItem(IDC_STATIC_Bit_O2))->ShowWindow(SW_HIDE);


	

}
HBRUSH&  Change_skin::GetBrush(int const& index)
{
	if (index >0 && index < brush_max)
	{
		return m_brush[index];
	}
	
}
HBRUSH& Change_skin::GetWhiteBrush()
{
	return m_white_brush;
}

bool	Change_skin::InitBrush()
{

	return true;
}