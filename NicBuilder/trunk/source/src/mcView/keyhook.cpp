#include "stdafx.h"
extern HHOOK hKeyboardLL;
extern BOOL gbNoSwitch;

LRESULT CALLBACK LowLevelKeyboardProc (INT nCode, WPARAM wParam, LPARAM lParam)
{
#ifdef NDEBUG
    // By returning a non-zero value from the hook procedure, the
    // message does not get passed to the target window
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
    BOOL bControlKeyDown = 0;

	if(gbNoSwitch){
		switch (nCode)
		{
			case HC_ACTION:
				// Check to see if the CTRL key is pressed
				bControlKeyDown = GetAsyncKeyState (VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1);

				// Disable CTRL+ESC
				if (pkbhs->vkCode == VK_ESCAPE && bControlKeyDown)
					return 1;

				// Disable ALT+TAB
				if (pkbhs->vkCode == VK_TAB && pkbhs->flags & LLKHF_ALTDOWN)
					return 1;

				// Disable ALT+ESC
				if (pkbhs->vkCode == VK_ESCAPE && pkbhs->flags & LLKHF_ALTDOWN)
					return 1;

				// Disable so many shit keys
				if(VK_LWIN==pkbhs->vkCode || VK_RWIN==pkbhs->vkCode)
					return 1;

				break;

			default:
				break;
		}
	}
#endif
    return CallNextHookEx (hKeyboardLL, nCode, wParam, lParam);
} 
