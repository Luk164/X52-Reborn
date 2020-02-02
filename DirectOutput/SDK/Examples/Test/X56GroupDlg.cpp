#include "StdAfx.h"
#include "X56GroupDlg.h"

namespace
{
	static LPCTSTR s_ErrorToString(HRESULT hr)
	{
		switch (hr)
		{
		case S_OK:			return _T("S_OK");
		case E_FAIL:		return _T("E_FAIL");
		case E_HANDLE:		return _T("E_HANDLE");
		case E_INVALIDARG:	return _T("E_INVALIDARG");
		default:			return _T("Unknown");
		}
	}
}
CX56GroupDlg::CX56GroupDlg(CDirectOutput& directoutput, void* hDevice) : m_directoutput(directoutput), m_device(hDevice)
{
}
LRESULT CX56GroupDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Add a page to the device (Page 1)
	// Flag the page to be activated when created
	// NOTE: This will NOT call the OnPageChanged callback
	HRESULT hr = m_directoutput.AddPage(m_device, 1, L"X56 Group Test Page", FLAG_SET_AS_ACTIVE);
	if (FAILED(hr))
	{
		DisplayErrorMessage(_T("DirectOutput_AddPage failed with error "), hr);
		return -1;
	}

	hr = m_directoutput.RegisterSoftButtonCallback(m_device, SoftButtonChangeCB, this);
	if (FAILED(hr))
	{
		DisplayErrorMessage(_T("DirectOutput_RegisterSoftButtonCallback failed with error "), hr);
		return -1;
	}

    OnSoftButtonChanged(0);

	return 0;
}
LRESULT CX56GroupDlg::OnClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Remove page 1
	HRESULT hr = m_directoutput.RemovePage(m_device, 1);
	if (FAILED(hr))
	{
		// flag this failure
		DisplayErrorMessage(_T("DirectOutput_RemovePage failed with error "), hr);
	}

	EndDialog(wID);
	return 0;
}
LRESULT CX56GroupDlg::OnOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UINT nBrightness = GetDlgItemInt(IDC_EDIT_BRIGHTNESS);
	if (nBrightness > 100)
	{
		DisplayErrorMessage(_T("Invalid brightness value: "), E_INVALIDARG);
		return -1;
	}

	BYTE ucRed = (BYTE)GetDlgItemInt(IDC_EDIT_RED);
	BYTE ucGreen = (BYTE)GetDlgItemInt(IDC_EDIT_GREEN);
	BYTE ucBlue = (BYTE)GetDlgItemInt(IDC_EDIT_BLUE);

	HRESULT hr = m_directoutput.SetLed(m_device, 1, 0, nBrightness);
	if (FAILED(hr))
	{
		DisplayErrorMessage(_T("Failed in setting the led brightness: "), hr);
		return -1;
	}

	hr = m_directoutput.SetLed(m_device, 1, 1, RGB(ucRed, ucGreen, ucBlue));
	if (FAILED(hr))
	{
		DisplayErrorMessage(_T("Failed in setting the led color: "), hr);
		return -1;
	}

	return 0;
}
void CX56GroupDlg::OnSoftButtonChanged(DWORD dwButtons)
{
	TCHAR tszBuffer[256] = { 0 };
	_sntprintf_s(tszBuffer, sizeof(tszBuffer) / sizeof(tszBuffer[0]), sizeof(tszBuffer) / sizeof(tszBuffer[0]), _T("0x%08X"), dwButtons);

	SendMessage(GetDlgItem(IDC_SOFTBUTTON), WM_SETTEXT, 0, (LPARAM)tszBuffer);
}
void CX56GroupDlg::DisplayErrorMessage(LPCTSTR tszMsg, HRESULT hr)
{
	TCHAR tszBuffer[1024] = { 0 };
	_sntprintf_s(tszBuffer, sizeof(tszBuffer)/sizeof(tszBuffer[0]), sizeof(tszBuffer)/sizeof(tszBuffer[0]), _T("%s %08X %s\n"), tszMsg, hr, s_ErrorToString(hr));
	MessageBox(tszBuffer, _T("Test.exe - X56GroupDlg"), MB_ICONERROR);
}
/*static*/void WINAPI CX56GroupDlg::SoftButtonChangeCB(void* hDevice, DWORD dwButtons, void* pCtxt)
{
	CX56GroupDlg* pThis = reinterpret_cast<CX56GroupDlg*>(pCtxt);
	if (pThis != NULL)
	{
		pThis->OnSoftButtonChanged(dwButtons);
	}
}

