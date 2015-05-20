#include "StdAfx.h"
#include "NeuronNetwork.h"
#include "Definition.h"
#include <math.h>
#include "IOFile.h"
#include "GradutedProjectDlg.h"
#include <list>

CNeuronNetwork::CNeuronNetwork(void)
{
}

void CNeuronNetwork::Init(CWnd *dialog)
{
	if (dialog)
	{
		m_Dialog = dialog;
	}
	
	if (m_Dialog)
	{
		CGradutedProjectDlg *pDialog = (CGradutedProjectDlg*)m_Dialog;

		m_iCurrentPattern = 0;
		m_dwCurrentIteration = 0;
		m_iFunction = MyNamespace::GAUS;
		m_dwIteration = pDialog->m_dwIteration;
		m_dAlphaMAX = pDialog->m_dAlphaMAX;
		m_dAlphaMIN = pDialog->m_dAlphaMIN;
		m_dwDimensionOfOutputLayer = pDialog->m_dwDimensionOfOutputLayer;
		m_sDataFilePath = pDialog->m_sDataFilePath;
		m_sSavedFilePath = pDialog->m_sSavedFilePath;
		m_sWeightFilePath = pDialog->m_sWeightFilePath;
		m_dwTopo	= pDialog->m_Topology;
		m_dSigma0 = (double)m_dwDimensionOfOutputLayer/2.0;
		m_dCurrentError= 0.0;

		if(!CIOFile::ReadDataFile(m_sDataFilePath, m_vSampleDatas, m_vLabels) || m_vSampleDatas.empty())
		{
			AfxMessageBox(L"Structor of Dataset File's wrong");
			ClearAll();
			return;
		}
		m_dwDimensionOfInputLayer = m_vSampleDatas[0].size();

		
		vector<vector<double>>	weightMatrix;
		DWORD					dwDimension;
		BOOL					bInitRandom = FALSE;
		if (!m_sWeightFilePath.IsEmpty())
		{
			if (!CIOFile::ReadWeigthFile(m_sWeightFilePath, dwDimension, weightMatrix) || weightMatrix.empty()
				|| dwDimension != m_dwDimensionOfInputLayer
				|| weightMatrix.size() != (m_dwDimensionOfOutputLayer*m_dwDimensionOfOutputLayer))
			{
				bInitRandom = TRUE;
			}
			if (bInitRandom)
			{
				AfxMessageBox(L"Structor of Weight File's wrong.\nInit weight vector random.");
			}
		}
		else
			bInitRandom = TRUE;

		for(DWORD i = 0; i < m_dwDimensionOfOutputLayer; i++)
		{
			for(DWORD j = 0; j < m_dwDimensionOfOutputLayer; j++)
			{
				CNeuron neuron;
				if (bInitRandom)
					neuron = CNeuron(i, j, this);
				else
					neuron = CNeuron(i, j, this, weightMatrix[i*m_dwDimensionOfOutputLayer + j]);

				m_vNetwork.push_back(neuron);
			}
		}
				
		m_vLocationOfLabel.clear();		
		m_vErrors.clear();
		m_CurrentWinner.Clear();

		m_dAlpha = m_dAlphaMAX;
		m_dSigma = m_dSigma0;
	}
}

CNeuronNetwork::~CNeuronNetwork(void)
{
}

void CNeuronNetwork::SetDialog( CWnd *dialog )
{
	m_Dialog = dialog;
}

void CNeuronNetwork::ClearSDHPoint()
{
	for(UINT i = 0; i < m_vNetwork.size(); i++)
	{
		m_vNetwork[i].m_dwSDHPoint = 0;
	}
}

void CNeuronNetwork::ClearAll()
{
	m_vNetwork.clear();

	// m_vSampleDatas.clear()
	for(UINT i = 0; i < m_vSampleDatas.size(); i++)
	{
		m_vSampleDatas[i].clear();
	}
	m_vSampleDatas.clear();

	m_vLabels.clear();
	m_vLocationOfLabel.clear();		
	m_vErrors.clear();
	m_dAlpha = 0;
	m_dAlphaMAX = 0;
	m_dAlphaMIN = 0;
	m_dSigma = 0;
	m_dSigma0 = 0;
	m_iFunction = 0;
	m_iCurrentPattern = 0;
	m_dwIteration = 0;
	m_dwCurrentIteration = 0;
	m_dwDimensionOfInputLayer = 0;
	m_dwDimensionOfOutputLayer = 0;
	m_CurrentWinner.Clear();
}

CNeuron CNeuronNetwork::GetNeuron( DWORD dwIndex )
{
	return m_vNetwork[dwIndex];
}

CNeuron CNeuronNetwork::GetNeuron( DWORD dwRowIndex, DWORD dwCollIndex )
{
	return m_vNetwork[dwRowIndex*m_dwDimensionOfOutputLayer + dwCollIndex];
}

double CNeuronNetwork::ComputeDistance(vector<double> vPattern, CNeuron neuron)
{
	vector<double> weights = neuron.m_vWeights;
	return ComputeDistance(vPattern, weights);
}

double CNeuronNetwork::ComputeDistance(vector<double> v1, vector<double> v2)
{
	DWORD dwSize = v1.size();
	double value = 0.0;

	if (dwSize != v2.size())
	{
		return -1.0;
	}
	
	for(DWORD i = 0; i < dwSize; i++)
		value += pow((v1[i] - v2[i]), 2);

	return sqrt(value);
}

CNeuron CNeuronNetwork::FindWinner( DWORD dwIndex )
{
	vector<double> vPattern = m_vSampleDatas[dwIndex];
	return FindWinner(vPattern);
}

CNeuron CNeuronNetwork::FindWinner( vector<double> vPattern )
{
	DWORD		dwNumberOfNeuron = m_dwDimensionOfOutputLayer* m_dwDimensionOfOutputLayer;
	double		D = 0, minValue;
	CNeuron		winnerNeuron = m_vNetwork[0];
	
	minValue = ComputeDistance(vPattern, winnerNeuron);
	for (DWORD i = 0; i < dwNumberOfNeuron; i++)
	{
			D = ComputeDistance(vPattern, m_vNetwork[i]);
			if (D < minValue)
			{
				minValue = D;
				winnerNeuron = m_vNetwork[i];
			}
	}
	return winnerNeuron;
}

double CNeuronNetwork::LearnOnePattern()
{
	DWORD		dwNumberOfNeuron = m_dwDimensionOfOutputLayer* m_dwDimensionOfOutputLayer;
	DWORD		dwIndex = 0;
	double		dError = 0.0;

	if (m_iCurrentPattern == 0 || m_iCurrentPattern >=  m_vSampleDatas.size())
	{
		for (DWORD i = 0; i < dwNumberOfNeuron; i++)
		{
			m_vNetwork[i].m_vLabels.clear();
		}
		m_dCurrentError = 0.0;
	}
	
	m_CurrentWinner   = FindWinner(m_vSampleDatas[m_iCurrentPattern]);
	dwIndex = m_CurrentWinner.m_dwXCoor*m_dwDimensionOfOutputLayer + m_CurrentWinner.m_dwYCoor;
	m_vNetwork[dwIndex].m_vLabels.push_back(m_vLabels[m_iCurrentPattern]);
	m_vLocationOfLabel.push_back(m_CurrentWinner.m_dwXCoor * m_dwDimensionOfOutputLayer + m_CurrentWinner.m_dwYCoor);

	for (DWORD i = 0; i < dwNumberOfNeuron; i++)
	{
		dError += m_vNetwork[i].UpdateWeight();
	}

	m_iCurrentPattern = ((m_iCurrentPattern + 1)%(m_vSampleDatas.size()));
	return dError / (double)dwNumberOfNeuron;
}

double CNeuronNetwork::LearnOnePatternEx()
{
	DWORD		dwNumberOfNeuron = m_dwDimensionOfOutputLayer* m_dwDimensionOfOutputLayer;
	DWORD		dwIndex = 0;
	double		dError = 0.0;

	if (m_iCurrentPattern == 0 || m_iCurrentPattern >=  m_vSampleDatas.size())
	{
		for (DWORD i = 0; i < dwNumberOfNeuron; i++)
		{
			m_vNetwork[i].m_vLabels.clear();
		}
		m_dCurrentError = 0.0;
	}

	m_CurrentWinner   = FindWinner(m_vSampleDatas[m_iCurrentPattern]);
	dwIndex = m_CurrentWinner.m_dwXCoor*m_dwDimensionOfOutputLayer + m_CurrentWinner.m_dwYCoor;
	m_vNetwork[dwIndex].m_vLabels.push_back(m_vLabels[m_iCurrentPattern]);
	m_vLocationOfLabel.push_back(m_CurrentWinner.m_dwXCoor * m_dwDimensionOfOutputLayer + m_CurrentWinner.m_dwYCoor);

	for (DWORD i = 0; i < dwNumberOfNeuron; i++)
	{
		dError += m_vNetwork[i].UpdateWeight();
	}

	m_iCurrentPattern = ((m_iCurrentPattern + 1)%(m_vSampleDatas.size()));
	m_dCurrentError += dError / (double)dwNumberOfNeuron;
	
	if (m_iCurrentPattern == 0)
	{
		++m_dwCurrentIteration;
		UpdateParam();
		m_vErrors.push_back(m_dCurrentError / (double)m_vSampleDatas.size());
		m_dCurrentError = 0.0;
	}
	
	return dError / (double)dwNumberOfNeuron;
}
void CNeuronNetwork::UpdateParam()
{
	m_dSigma = m_dSigma0*exp(-(double)m_dwCurrentIteration * log(m_dSigma0) / (double)(m_dwIteration));
	m_dAlpha = m_dAlphaMAX * exp(-(double)m_dwCurrentIteration / (double)(m_dwIteration));
}

double CNeuronNetwork::LearnOneEpoch()
{
	DWORD	dwNumberOfNeuron = m_dwDimensionOfOutputLayer* m_dwDimensionOfOutputLayer;
	DWORD	dwNumberOfPatterns = m_vSampleDatas.size();
	double	dError = 0.0;
	DWORD	i = m_iCurrentPattern;

	for(i; i < dwNumberOfPatterns; i++)
	{
		dError += LearnOnePattern();
	}

	m_vErrors.push_back(dError / (double)m_vSampleDatas.size());
	m_dCurrentError = 0.0;
	++m_dwCurrentIteration;
	UpdateParam();

	m_Dialog->PostMessage(WM_NOTIFY_TRAINING_PERCENT, 100, TRUE);

	return fabs( (dError + m_dCurrentError) / (double)dwNumberOfPatterns);
}

void CNeuronNetwork::TrainingDataSet()
{
	DWORD		dwNumberOfNeuron = m_dwDimensionOfOutputLayer* m_dwDimensionOfOutputLayer;
	while (m_dwCurrentIteration <= m_dwIteration && m_dAlpha > m_dAlphaMIN)
	{
		double dErrorPerEpoch = LearnOneEpoch();
		m_vErrors.push_back(dErrorPerEpoch);
	}
}

void CNeuronNetwork::ExecuteSDH(DWORD dwSparam)
{
	ClearSDHPoint();

	list<SDH>	lSDHvalue;
	DWORD		dwNumOfPattern = m_vSampleDatas.size();
	DWORD		dwNumOfNeuron  = m_vNetwork.size();
	
	for(DWORD i = 0; i < dwNumOfPattern; i++)
	{
		lSDHvalue.clear();
		for(DWORD j = 0; j < dwNumOfNeuron; j++)
		{
			lSDHvalue.push_back(SDH(j, ComputeDistance(m_vSampleDatas[i], m_vNetwork[j])));
		}
		lSDHvalue.sort();

		DWORD dwSize = lSDHvalue.size();
		DWORD dwIndex = 0;
		for(DWORD t = 0; t < dwSparam; t++)
		{
			SDH sdh = lSDHvalue.back();
			lSDHvalue.pop_back();
			m_vNetwork[sdh.dwIndex].AddPoint(dwSparam - t);
		}
	}
}