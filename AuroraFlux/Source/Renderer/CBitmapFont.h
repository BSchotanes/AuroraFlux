#ifndef CBITMAPFONT_H
#define CBITMAPFONT_H
#include "../StdAfx.h"

struct CBitmapFont 
{
	//int	m_nCharWidth;
	//int m_nCharHeight;
	//int	m_nNumCols;
	//
	int	m_nImageID;
	//char m_cFirstChar; 	//	first char on sheet

	//RECT CellAlgorithnm(int id);
	int m_nImageWidth;
	int m_nImageHeight;
	int m_nCellWidth;
	int m_nCellHeight;
	char m_cFirstChar;
	char m_cWidths[256];

	int m_nCharactersPerLine;
public:
	CBitmapFont(const char* _fileName,int nImageID);
	
};

#endif