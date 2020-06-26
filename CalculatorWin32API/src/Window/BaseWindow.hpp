#pragma once

#include "pch.h"


namespace Calculator
{

	template<class DERIVED_TYPE>
	class BaseWindow
	{
	private:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			DERIVED_TYPE* pThis = NULL;

			if (uMsg == WM_NCCREATE)
			{
				CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
				pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

				pThis->m_hwnd = hwnd;
			}
			else
			{
				pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			}
			if (pThis)
			{
				return pThis->HandleMessage(uMsg, wParam, lParam);
			}
			else
			{
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
		}

	public:
		BaseWindow()
			:m_hwnd(NULL), wc{} {}

		BOOL CreateMainWindow(
			PCWSTR lpWindowName,
			DWORD dwStyle,
			DWORD dwExStyle = 0,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int nWidth = CW_USEDEFAULT,
			int nHeight = CW_USEDEFAULT,
			HWND hWndParent = 0,
			HMENU hMenu = 0)
		{
			wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
			wc.hInstance = GetModuleHandle(NULL);
			wc.lpszClassName = ClassName();

			RegisterClass(&wc);
			m_hwnd = CreateWindowEx(dwExStyle, ClassName(), lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this);

			return (m_hwnd ? TRUE : FALSE);
		}

		HWND GetMainWindow() const { return m_hwnd; }

	protected:
		HWND m_hwnd;
		WNDCLASS wc;

		virtual PCWSTR ClassName() const = 0;
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	};

}