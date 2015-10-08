#include "windows.h"
#include "tchar.h"
#include "EnableGroupboxControls.h"

void EnableGroupboxControls(HWND hWnd, BOOL bEnable)
{
	if (::IsWindow(hWnd))
	{
		// get class name
		TCHAR szClassName[MAX_PATH];
		szClassName[0] = _T('\0');
		::GetClassName(hWnd, szClassName, sizeof(szClassName)/sizeof(TCHAR)-2);

		// get window style
		LONG lStyle = ::GetWindowLong(hWnd, GWL_STYLE);

		if ((_tcsicmp(szClassName, _T("Button")) == 0) &&
			((lStyle & BS_GROUPBOX) == BS_GROUPBOX))
		{
			// this is a groupbox

			RECT rectGroupbox;
			::GetWindowRect(hWnd, &rectGroupbox);

			// get first child control

			HWND hWndParent = ::GetParent(hWnd);
			HWND hWndChild = ::GetWindow(hWndParent, GW_CHILD);

			while (hWndChild)
			{
				RECT rectChild;
				::GetWindowRect(hWndChild, &rectChild);

				// check if child rect is entirely contained within groupbox
				if ((rectChild.left > rectGroupbox.left) &&
					(rectChild.right < rectGroupbox.right) &&
					(rectChild.top > rectGroupbox.top) &&
					(rectChild.bottom < rectGroupbox.bottom))
				{
					//TRACE(_T("found child window 0x%X\n"), hWndChild);
					::EnableWindow(hWndChild, bEnable);
				}

				// get next child control
				hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT);
			}
		}
	}
}
