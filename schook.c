#include <windows.h>

/* The hook itself when materialised. */
HHOOK hhk;
/* The module for ourself, which we get from the DLL entry point. */
HMODULE hmod;

/* If a key event is for an extended keycode (0x100_0000, bit 24) */
#define EXTENDED_KEY (1 << 24)

__declspec(dllexport) LRESULT __stdcall KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	UINT newMsg;
	HWND hWnd;
	BYTE pc_keycode;
	UINT oldLParam;
	char scan_code;
	/* Negative codes must be passed to the next handler. */
	if (nCode < 0) {
		return CallNextHookEx(hhk, nCode, wParam, lParam);
	}
	/* We'll mutate the argument, but keep the old one around for once. */
	oldLParam = lParam;
	/* Extract the keycode for conversion. */
	scan_code = (char)((int)(lParam << 8) >> 0x18);
	/* Take a PC98 scancode and turn it into a PC one. */
	switch (scan_code) {
	/*
	 * There are cases we can be clever because the IBM and NEC PCs
	 * have the same character clusters just shifted over some.
	 */
	case 0: /* Escape */
	case 1: /* 1  */
	case 2: /* 2 */
	case 3: /* 3 */
	case 4: /* 4 */
	case 5: /* 5 */
	case 6: /* 6 */
	case 7: /* 7 */
	case 8: /* 8 */
	case 9: /* 9 */
	case 0xa: /* 0 */
	case 0xb: /* -/_ */
	case 0xc: /* +/= */
	case 0x1d: /* A */
	case 0x1e: /* S */
	case 0x1f: /* D */ 
	case 0x20: /* F */
	case 0x21: /* G */
	case 0x22: /* H */
	case 0x23: /* J */
	case 0x24: /* K */
	case 0x25: /* L */
	case 0x26: /* ; */
	  pc_keycode = scan_code + 1;
	  break;
	case 0xd: /* Yen (Backslash) */
	  pc_keycode = 0x2b;
	  break;
	/* We were missing this in the original DLL */
	case 0x1a: /* Tilde */
	  pc_keycode = 0x29;
	  break;
	case 0x1b: /* Right Bracket */
	  pc_keycode = 0x1a;
	  break;
	case 0x28: /* Left Bracket */
	  pc_keycode = 0x1b;
	  break;
	case 0x29: /* Z */
	case 0x2a: /* X */
	case 0x2b: /* C */
	case 0x2c: /* V */
	case 0x2d: /* B */
	case 0x2e: /* N */
	case 0x2f: /* M */
	case 0x30: /* , */
	case 0x31: /* . */
	case 0x32: /* / */
	  pc_keycode = scan_code + 3;
	  break;
	case 0x33: /* R0 */
	  pc_keycode = 0x73;
	  break;
	case 0x34: /* Spacebar */
	  pc_keycode = 0x39;
	  break;
	case 0x35: /* XFER */
	  pc_keycode = 0x79;
	  break;
	case 0x36: /* Roll Up */
	  pc_keycode = 0x51;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x37: /* Roll Down */
	  pc_keycode = 0x49;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x38: /* Insert */
	  pc_keycode = 0x52;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x39: /* Delete */
	  pc_keycode = 0x53;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x3a: /* Up */
	  pc_keycode = 0x48;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x3b: /* Left */
	  pc_keycode = 0x4b;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x3c: /* Right */
	  pc_keycode = 0x4d;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x3d: /* Down */
	  pc_keycode = 0x50;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x3e: /* Home */
	  pc_keycode = 0x47;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x3f: /* Help (End) */
	  pc_keycode = 0x4f;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x40: /* NumPad - */
	  pc_keycode = 0x4a;
	  break;
	case 0x41: /* NumPad / */
	  pc_keycode = 0x35;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x42: /* NumPad 7 */
	case 0x43: /* NumPad 8 */
	case 0x44: /* NumPad 9 */
	case 0x46: /* NumPad 4 */
	case 0x47: /* NumPad 5 */
	case 0x48: /* NumPad 6 */
	case 0x49: /* NumPad + */
	case 0x4a: /* NumPad 1 */
	case 0x4b: /* NumPad 2 */
	case 0x4c: /* NumPad 3 */
	  pc_keycode = scan_code + 5;
	  break;
	case 0x45: /* NumPad * */
	  pc_keycode = 0x37;
	  break;
	case 0x4e: /* NumPad 0 */
	  pc_keycode = 0x52;
	  break;
	case 0x50: /* NumPad . */
	  pc_keycode = 0x53;
	  break;
	case 0x52: /* F11 */
	  pc_keycode = 0x57;
	  break;
	case 0x53: /* F12 */
	  pc_keycode = 0x58;
	  break;
	case 0x5b: /* NumLock? */
	  pc_keycode = 0x45;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x5c: /* NumPad Enter? */
	  pc_keycode = 0x1c;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x5d: /* ScrollLock? */
	  pc_keycode = 0x46;
	  break;
	case 0x5f: /* Tilde (XXX: Problematic, see case 1A) */
	  pc_keycode = 0x29;
	  break;
	case 0x60: /* Stop (NumLock) */
	  pc_keycode = 0x45;
	  break;
	case 0x61: /* Copy (PrintScreen) */
	  pc_keycode = 0x37;
	  break;
	case 0x62: /* F1 */
	case 0x63: /* F2 */
	case 0x64: /* F3 */
	case 0x65: /* F4 */
	case 0x66: /* F5 */
	case 0x67: /* F6 */
	case 0x68: /* F7 */
	case 0x69: /* F8 */
	case 0x6a: /* F9 */
	case 0x6b: /* F10 */
	  pc_keycode = scan_code - 0x27;
	  break;
	case 0x70: /* Left Shift */
	  pc_keycode = 0x2a;
	  break;
	case 0x71: /* Caps Lock */
	  pc_keycode = 0x3a;
	  break;
	case 0x72: /* Kana */
	  pc_keycode = 0x70;
	  break;
	case 0x73: /* GRP */
	  pc_keycode = 0x38;
	  break;
	case 0x74: /* Left Control */
	  pc_keycode = 0x1d;
	  break;
	case 0x75: /* Right Control? */
	  pc_keycode = 0x1d;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x76: /* Right Alt? */
	  pc_keycode = 0x38;
	  lParam |= EXTENDED_KEY;
	  break;
	case 0x77: /* Left Windows */
	case 0x78: /* Right Windows */
	case 0x79: /* Application */
		pc_keycode = scan_code + 0x1c;
		break;
	case 0x7d: /* Right Shift */
		pc_keycode = 0x36;
		break;
	/* Some cases (like Q through P or Tab) are actually identical. */
	default:
		pc_keycode = (byte)scan_code;
		break;
	}
	/* Replace lParam with the new code */
	lParam = (UINT)pc_keycode << 0x10 | lParam & 0xff00ffff;
	/*
	 * If we're getting KEYUP/KEYDOWN. Top bit of LPARAM is transition
	 * state, which is 0 for KEYDOWN (pressed) or 1 for KEYUP (release).
	 */
	newMsg = oldLParam & 0x80000000 ? WM_KEYUP : WM_KEYDOWN;
	/* Get the sender and post the converted message back to them. */
	hWnd = GetFocus();
	PostMessageA(hWnd, newMsg, wParam, lParam);
	/* Tell Windows we've handled this and not to pass it back. */
	return 1;
}

__declspec(dllexport) BOOL __stdcall SetHookProc(BOOL disable)
{
	if (!disable)
		return UnhookWindowsHookEx(hhk);
	hhk = SetWindowsHookExA(WH_KEYBOARD, KeyboardProc, hmod, 0);
	return hhk != 0;
}

__declspec(dllexport) BOOL __stdcall DllEntryPoint(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	hmod = hinstDLL;
	return 1;
}
