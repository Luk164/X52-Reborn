#pragma once
#include "resource.h"
#include "DirectOutputImpl.h"

///
/// CX56GroupDlg
///
/// Handles the CX56GroupDlg Test Dialog
///
class CX56GroupDlg : public CDialogImpl<CX56GroupDlg>
{
public:
	CX56GroupDlg(CDirectOutput& directoutput, void* hDevice);

	enum { IDD = IDD_X56_GROUP };

	BEGIN_MSG_MAP(CX56GroupDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	VOID OnSoftButtonChanged(DWORD dwButtons);

private:
	CDirectOutput&	m_directoutput;
	void*			m_device;

	void DisplayErrorMessage(LPCTSTR tszMsg, HRESULT hr);
	static void WINAPI SoftButtonChangeCB(void* hDevice, DWORD dwButtons, void* pCtxt);
};
