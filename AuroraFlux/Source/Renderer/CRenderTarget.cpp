#include "CRenderTarget.h"

CRenderTarget::CRenderTarget()
{
	m_pTextureTarget = NULL;
	m_pRenderTarget = NULL;
	m_pPreviousRenderTarget = NULL;
	m_pD3DDevice = NULL;
	m_lTargetNumber = -1;
}

CRenderTarget::~CRenderTarget()
{
	SAFE_RELEASE(m_pTextureTarget);
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pPreviousRenderTarget);
	m_pD3DDevice = NULL;
}

//
//	This function initializes the render target
//	unsigned int width - the width of the render target
//	unsigned int length - the length of the render target
//	D3DFORMAT - the pixel format of the render target
//	
void CRenderTarget::InitRenderTarget(unsigned int _width, unsigned _height,IDirect3DDevice9* _D3DDevice, D3DFORMAT _format)
{
	m_pD3DDevice= _D3DDevice;
	D3DXCreateTexture(m_pD3DDevice,_width,_height,1,D3DUSAGE_RENDERTARGET,_format,D3DPOOL_DEFAULT,&m_pTextureTarget);
	m_pTextureTarget->GetSurfaceLevel(0,&m_pRenderTarget);
}

//
//	Sets this as the current render target
//	_ulTargetToActivate - Index of the surface this render target should replace
// 
void CRenderTarget::ActiveTarget(long _ulTargetToActivate)
{
	m_lTargetNumber = _ulTargetToActivate;
	m_pD3DDevice->GetRenderTarget(m_lTargetNumber, &m_pPreviousRenderTarget);
	m_pD3DDevice->SetRenderTarget(m_lTargetNumber, m_pRenderTarget);
}

//
//	Reverts the render target to its previous target
// 
void CRenderTarget::RevertTarget()
{
	m_pD3DDevice->SetRenderTarget(m_lTargetNumber,m_pPreviousRenderTarget);
	m_lTargetNumber = -1;
}