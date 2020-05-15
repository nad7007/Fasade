#include "stdafx.h"
#include "panel.h"
stColor PanelColours[] = {
	{255, 255, 0},
	{255, 0, 255},
	{0, 255, 255},

	{128, 0, 0},
	{0, 128, 128},
	{0, 0, 128},

	{128, 128, 0},
	{128, 0, 128},
	{0, 128, 128},

	{255, 128, 0},
	{255, 0, 128},
	{128, 255, 0},
	{0, 255, 128},
	{0, 128, 255},
	{128, 0, 250},

	{255, 128, 255},
	{255, 255, 128},
	{128, 255, 255},
	{255, 255, 128},
	{255, 128, 255},
	{128, 255, 250},

	{255, 128, 128},
	{255, 128, 128},
	{128, 255, 128},
	{128, 255, 128},
	{128, 128, 255},
	{128, 128, 250},

	{128, 128, 128},
};

static int sCurrentColour = 0;

int cPDFSize::m_sMaxB = 800;
int cPDFSize::m_sMinB = 400;

int cLDSize::m_sMaxB = 600;
int cLDSize::m_sMinB = 300;
int cLDSize::m_sMaxC = 340;
int cLDSize::m_sMinC = 250;

IfacePanel* IfacePanel::Create(PanelType type, double dX, double dY, double dH, double dL)
{
	IfacePanel *pNew = nullptr;

	switch (type) {
	case PDF:
		pNew = new cPDFPanel(dX, dY, dH, dL);
		break;
	case LD :
		pNew = new cLDPanel( dX, dY, dH,dL);
		break;
	default:
		{
		ASSERT(false);
		}
	}
	return pNew;
}
