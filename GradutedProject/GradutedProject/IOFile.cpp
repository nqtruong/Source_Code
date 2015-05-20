#include "StdAfx.h"
#include "IOFile.h"

CIOFile::CIOFile(void)
{
}

CIOFile::~CIOFile(void)
{
}

CString CIOFile::GetContentFromFile( LPCTSTR szFile ,UINT uCodePage)
{
	CString strContent;
	FILE* file=  NULL;
	if (_tfopen_s(&file,szFile,_T("rb"))==0 && file!=NULL)
	{
		fseek(file,0,SEEK_END);
		long lLength =  ftell(file);
		char* buff = new char[lLength+2];
		fseek(file,0,SEEK_SET);
		fread(buff,1,lLength,file);
		
		DWORD dwLengthW  = MultiByteToWideChar(uCodePage,0,buff,lLength,NULL,NULL);
		MultiByteToWideChar(uCodePage,0,buff,lLength,strContent.GetBuffer(dwLengthW),dwLengthW);
		strContent.ReleaseBuffer(dwLengthW);
		delete [] buff;
		buff = NULL;
		fclose(file);
		file=NULL;
	}
	return strContent;
}

void CIOFile::SaveContentToFile( LPCTSTR szFile,LPCTSTR szContent ,UINT uCodePage)
{
	CString sContent = szContent;
	FILE* file=  NULL;

	if (_tfopen_s(&file,szFile,_T("wb+"))==0 && file!=NULL)
	{
		long lLength =  0;
		fseek(file,0,SEEK_SET);

		//Repace Homepage.
		lLength = WideCharToMultiByte(uCodePage,0,sContent.GetString(),sContent.GetLength(),NULL,NULL,NULL,NULL);
		char* buff = new char[lLength+2];
		WideCharToMultiByte(uCodePage,0,sContent.GetString(),sContent.GetLength(),buff,lLength,NULL,NULL);
		fwrite(buff,1,lLength,file);
		delete[] buff;
		buff=NULL;

		fclose(file);
		file=NULL;
	}
}

BOOL CIOFile::ReadDataFile(LPCTSTR pszFilePath, vector<vector<double>> &patternsMatrix, vector<CString> &labelVector )
{
	CString sTemp, sContentLine, sToken;
	CString sContentFile = GetContentFromFile(pszFilePath);
	DWORD	dwCountColum = 0, dwNumOfRow= 0, dwNumOfColum = 0;
	INT		iPos1 = 0, iPos2 = 0;

	if (sContentFile.IsEmpty())
	{
		return FALSE;
	}

	sContentLine = sContentFile.Tokenize(L"\r\n", iPos1);
	if (sContentLine.Find(L"\t") > 0)
		sToken = L"\t";
	else
		sToken = L" ";

	
	while((sTemp = sContentLine.Tokenize(sToken, iPos2)) != L"")
		dwNumOfColum++;

	while((sContentLine = sContentFile.Tokenize(L"\r\n", iPos1)) != L"")
	{
		vector<double> vTemp;
		iPos2 = 0;
		dwCountColum = 0;
		while((sTemp = sContentLine.Tokenize(sToken, iPos2))!= L"")
		{
			dwCountColum++;
			if (dwCountColum == 1)
				continue;
			if (dwCountColum < dwNumOfColum)
			{
				double d = _tstof(sTemp);
				vTemp.push_back(d);
			}
			else
			{
				labelVector.push_back(sTemp);
			}
		}
		if (vTemp.size() != (dwNumOfColum - 2))
		{
			//Clear all
			for(UINT i = 0; i < patternsMatrix.size(); i++)
			{
				patternsMatrix[i].clear();
			}
			patternsMatrix.clear();
			labelVector.clear();

			return FALSE;
		}
		patternsMatrix.push_back(vTemp);
		++dwNumOfRow;
	}
	
	if (patternsMatrix.empty() || labelVector.empty())
	{
		return FALSE;
	}

	if ((patternsMatrix.size() != dwNumOfRow) ||(labelVector.size() != dwNumOfRow))
	{
		// Clear all
		for(UINT i = 0; i < patternsMatrix.size(); i++)
		{
			for(UINT j = 0; j < patternsMatrix[j].size(); j++)
				patternsMatrix[j].clear();
		}
		patternsMatrix.clear();
		labelVector.clear();

		return FALSE;
	}

	return TRUE;
}

void CIOFile::WriteDataFile(LPCTSTR pszFilePath)
{

}

BOOL CIOFile::ReadWeigthFile(LPCTSTR pszFilePath, DWORD &dwDimension, vector<vector<double>> &weigthMatrix )
{
	CString sTemp, sContentLine;
	CString sContentFile = GetContentFromFile(pszFilePath);
	INT		iPos1 = 0, iPos2 = 0;

	if (sContentFile.IsEmpty())
	{
		return FALSE;
	}

	sContentLine = sContentFile.Tokenize(L"\r\n", iPos1);
	dwDimension  = (DWORD)_ttoi(sContentLine);

	while((sContentLine = sContentFile.Tokenize(L"\r\n", iPos1)) != L"")
	{
		vector<double> vTemp;
		iPos2 = 0;
		while((sTemp = sContentLine.Tokenize(L"\t", iPos2))!= L"")
		{
			double d = _tstof(sTemp);
			vTemp.push_back(d);
		}
		weigthMatrix.push_back(vTemp);
	}

	if (weigthMatrix.empty())
		return FALSE;

	if (dwDimension != weigthMatrix[0].size())
	{
		//Clear all
		for(UINT i = 0; i < weigthMatrix.size(); i++)
		{
			weigthMatrix[i].clear();
		}
		weigthMatrix.clear();

		return FALSE;
	}

	return TRUE;
}

void CIOFile::WriteWeightFile(LPCTSTR pszFilePath, vector<vector<double>> &weigthMatrix )
{

}

