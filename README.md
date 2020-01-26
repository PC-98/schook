# schook replacement

This is a replacement for the `SCHOOK.DLL` that ships with [a program][schook]
that converts PC-98 keycode scancodes into IBM PC ones on Windows. The reason
why this program exists are for Win32 programs that assume raw IBM PC scancodes
in their keyboard handlers; these are usually games.

The reason why this exists is because it doesn't support the `~` key on the
PC-98, which means you can't for example, use the console in Half-Life. This
is a rebuilt DLL that acts as a drop-in replacement, enabling use of `~`.

This has been tested with Visual C++ 2.0 and verified to work.

[schook]: http://121ware.com/psp/PA121/121ETC2/CRM/s/WEBLIB_NECS_DID.PRODUCT_ID.FieldFormula.IScript_VDown_Details?modId=88
