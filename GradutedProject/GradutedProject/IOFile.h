#pragma once
#include <vector>

using namespace std;

class CIOFile
{
public:
	CIOFile(void);
	~CIOFile(void);

	static CString GetContentFromFile( LPCTSTR szFile ,UINT uCodePage=CP_UTF8);
	static void	SaveContentToFile( LPCTSTR szFile,LPCTSTR szContent ,UINT uCodePage);

	//////////////////////////////////////////////////////////////////////////
	static BOOL	ReadDataFile(LPCTSTR pszFilePath,vector<vector<double>> &patternsMatrix, vector<CString> &labelVector);
	static void	WriteDataFile(LPCTSTR pszFilePath);

	//////////////////////////////////////////////////////////////////////////
	static BOOL	ReadWeigthFile(LPCTSTR pszFilePath, DWORD &dwDimension, vector<vector<double>> &weigthMatrix);
	static void	WriteWeightFile(LPCTSTR pszFilePath, vector<vector<double>> &weigthMatrix);
	
};
