#pragma once

class HookProcedure
{
public:
	HookProcedure();
	~HookProcedure();

public:
	BOOL SetHook();
	BOOL UnHook();

private:
	HHOOK hHook_Msg;

	static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
};

