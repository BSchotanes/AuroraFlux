#include "ExporterIncludes.h"

bool tVertex::operator==(tVertex _tV)
{
	return this->m_fX == _tV.m_fX && this->m_fY == _tV.m_fY && this->m_fZ == _tV.m_fZ && this->m_fNormX == _tV.m_fNormX && this->m_fNormY == _tV.m_fNormY && this->m_fNormZ == _tV.m_fNormZ && this->m_fU == _tV.m_fU && this->m_fV == _tV.m_fV && this->m_fBNormX == _tV.m_fBNormX && this->m_fBNormY == _tV.m_fBNormY && this->m_fBNormZ == _tV.m_fBNormZ && this->m_fTX == _tV.m_fTX && this->m_fTY == _tV.m_fTY && this->m_fTZ == _tV.m_fTZ;
}