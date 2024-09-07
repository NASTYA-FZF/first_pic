#pragma once
#include <gdiplus.h>

// Drawer

class Drawer : public CStatic
{
	DECLARE_DYNAMIC(Drawer)

public:
	Drawer();
	virtual ~Drawer();

protected:
	DECLARE_MESSAGE_MAP()
public:
	ULONG_PTR token;
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


