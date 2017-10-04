#ifndef CRENDERTARGET_H_
#define CRENDERTARGET_H_
#include "../StdAfx.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			if (p) { p->Release(); p = NULL; }
#endif

class CRenderTarget
{
private:
	//	D3DDevice
	IDirect3DDevice9*		m_pD3DDevice;
	// Texture to Render to
	IDirect3DTexture9*		m_pTextureTarget;
	//	Surface of the render texture to use
	IDirect3DSurface9*		m_pRenderTarget;
	//	The Surface that was valid before activating this one
	IDirect3DSurface9*		m_pPreviousRenderTarget;

	//	Surface Index this render target is using
	long			m_lTargetNumber;

public:
	CRenderTarget();
	~CRenderTarget();
	//
	//	This function initializes the render target
	//	unsigned int width - the width of the render target
	//	unsigned int height - the height of the render target
	//	D3DFORMAT _format - the pixel format of the render target
	//	
	void InitRenderTarget(unsigned int _width, unsigned _height,IDirect3DDevice9* _D3DDevice, D3DFORMAT _format = D3DFMT_A2B10G10R10);
	//
	//	Sets this as the current render target
	//	_ulTargetToActivate - Index of the surface this render target should replace
	// 
	void ActiveTarget(long _lTargetToActivate);

	//
	//	Reverts the render target to its previous target
	// 
	void RevertTarget();
	//
	//	Returns the TextureTarget
	// 
	IDirect3DBaseTexture9* GetTexture() {return m_pTextureTarget;}
};
#endif