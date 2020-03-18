#pragma once
#include"panelTypeEnum.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cPDFSize//In MM by specification
{
	const static int m_sMaxB = 800;
	const static int m_sMinB = 400;
	
public:
	cPDFSize()
	{}

	static int GetMinLength() {
		return m_sMinB;
	}
	static int GetMaxLength() {
		return m_sMaxB;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cLDSize//In MM by specification
{
	const static int m_sMaxB = 600;
	const static int m_sMinB = 300;
	const static int m_sMaxC = 340;
	const static int m_sMinC = 250;

public:
	cLDSize() {}

	static int GetMinLongLength(int iPanelWidth)  {
		return m_sMinB - iPanelWidth;
	}
	static int GetMaxLongLength(int iPanelWidth)  {
		return m_sMaxB - iPanelWidth;
	}
	static int GetMinShortLength(int iPanelWidth)  {
		return m_sMinC - iPanelWidth;
	}
	static int GetMaxShortLength(int iPanelWidth)  {
		return m_sMaxC - iPanelWidth;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IfacePanel
{
public: //IfacePanel interface

	virtual	PanelType		GetType() const = 0;
	virtual void				GetStartPoint(double& dX, double&dY) const = 0;
	virtual double			GetStartPointX() const = 0;
	virtual double			GetEndPointX() const = 0;
	virtual	double			GetHeight() const = 0;
	virtual	double			GetLength() const = 0;
	virtual	void				SetHeight(float fHeight) = 0;
	virtual	void				SetLength(float fLength) = 0;
	virtual void				GetColor(unsigned char& Reg, unsigned char& Green, unsigned char& Blue) const = 0;
	virtual void				SetColor(unsigned char Reg, unsigned char Green, unsigned char Blue) = 0;
	virtual bool				Compare(const IfacePanel* pOther) const = 0;
	virtual bool				Compare(PanelType type, float fHeight, float fLength) const = 0;
	virtual HoleStatus	GetHoleSatus() const = 0;
	virtual IfacePanel* Copy() const = 0;
	static IfacePanel*	Create(PanelType typePanel, double dStartX, double dStartY, double dH, double dL);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cPanel : public IfacePanel
{
protected://Constructors & destructor

	cPanel(PanelType t): m_Type(t), m_fHeight(0), m_fLength(0){}
	cPanel(PanelType t, double dX, double dY, double dH, double dL)
		: m_Type(t)
		, m_fHeight(dH)
		, m_fLength(dL)
		, m_dStartX(dX)
		, m_dStartY(dY)
	{}
	virtual ~cPanel() {}

public: //IfacePanel interface

	virtual	PanelType		GetType() const
	{
		return m_Type;
	}

	double			GetStartPointX() const override {
		return m_dStartX;
	}

	double			GetEndPointX() const override {
		return (m_dStartX + m_fLength);
	}

	void				GetStartPoint(double&dX, double &dY) const	override {
		dX = m_dStartX;
		dY = m_dStartY;
	}

	double			GetHeight() const override {
		return m_fHeight;
	}
	void			SetHeight(float fHeight) override {
		m_fHeight = fHeight;
	}
	
	double			GetLength() const override {
		return m_fLength;
	}
	void			SetLength( float fLength) override {
		m_fLength = fLength;
	}

	void	GetColor(unsigned char& red, unsigned char& green, unsigned char& blue) const override {
		red = m_Color.red; green = m_Color.green; blue =  m_Color.blue;
	}
	void			SetColor(unsigned char red, unsigned char green, unsigned char blue) override {
		m_Color.red = red;  m_Color.green = green; m_Color.blue = blue;
	}

	bool			Compare(const IfacePanel* pOther) const override {
		return GetType() == pOther->GetType()
			&& m_fHeight == pOther->GetHeight()
			&& fabs(m_fLength - pOther->GetLength()) < 1.0;
	}
	bool			Compare(PanelType type, float fHeight, float fLength) const override {
		return GetType() == type
			&& m_fHeight == fHeight
			&& fabs(m_fLength - fLength) < 1.0;
	}

protected:

	PanelType	m_Type;
	double		m_dStartX;
	double		m_dStartY;
	double		m_fHeight;
	double		m_fLength;
	stColor		m_Color;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cPDFPanel : public cPanel
{
	cPDFSize		m_Size;
	HoleStatus	m_HoleStatus;

public://Constructors & destructor
	cPDFPanel() : cPanel(PDF)
	{
		SetColor(255, 200, 0);
	}
	cPDFPanel(double dX, double dY, double dH, double dL) : cPanel(PDF, dX, dY, dH, dL)
	{
		SetColor(255, 200, 0);
	}

	IfacePanel* Copy() const override
	{
		try {
			return new cPDFPanel(*this);
		}
		catch (...)
		{
			ASSERT(false);
			return nullptr;
		}
	}

	HoleStatus	GetHoleSatus() const override {
		return m_HoleStatus;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cLDPanel : public cPanel
{
		static cLDSize	m_Size;

public://Constructors & destructor

	cLDPanel() : cPanel(LD) {}
	cLDPanel(double dX, double dY, double dH, double dL) : cPanel(LD, dX, dY, dH, dL) {}
	virtual HoleStatus	GetHoleSatus() const override {
		return NoHole;
	}

	IfacePanel* Copy() const override
	{
		try {
			return new cLDPanel(*this);
		}
		catch (...)
		{
			ASSERT(false);
			return nullptr;
		}
	}

};

