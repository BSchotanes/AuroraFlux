#include "CBitmapFont.h"

CBitmapFont::CBitmapFont(const char* _fileName,int nImageID)
{
	//m_nCharWidth = nCharWidth;
	//m_nCharHeight = nCharHeight;
	//m_nNumCols = nNumCols;
	m_nImageID = nImageID;
	//m_cFirstChar = cFirstChar;
	// 

	ifstream _ifStream;
	_ifStream.open(_fileName,ios_base::in | ios_base::binary);
	if(_ifStream.is_open())
	{
		_ifStream.read((char*)&m_nImageWidth,sizeof(m_nImageWidth));
		_ifStream.read((char*)&m_nImageHeight,sizeof(m_nImageHeight));
		_ifStream.read((char*)&m_nCellWidth,sizeof(m_nCellWidth));
		_ifStream.read((char*)&m_nCellHeight,sizeof(m_nCellHeight));
		_ifStream.read((char*)&m_cFirstChar,sizeof(m_cFirstChar));
		_ifStream.read((char*)&m_cWidths,sizeof(m_cWidths));
		_ifStream.close();
	}
	m_nCharactersPerLine = m_nImageWidth/m_nCellWidth;

}