#pragma once
#include <vector>
#include "afx.h"
#include "Neuron.h"


using namespace std;

class CNeuronNetwork :	public CObject
{
// Attributes of class CNeuronNetwork
public:
	vector<CNeuron>			m_vNetwork;
	vector<vector<double>>	m_vSampleDatas;
	vector<CString>			m_vLabels;
	vector<INT>				m_vLocationOfLabel;		// sort by order of sample data ID
	vector<double>			m_vErrors;
	double					m_dAlpha, m_dAlphaMAX, m_dAlphaMIN;
	double					m_dSigma, m_dSigma0;
	double					m_dCurrentError;
	INT						m_iFunction;
	INT						m_iCurrentPattern;
	DWORD					m_dwTopo;
	DWORD					m_dwIteration, m_dwCurrentIteration;
	DWORD					m_dwDimensionOfInputLayer, m_dwDimensionOfOutputLayer;
	CNeuron					m_CurrentWinner;


	CWnd					*m_Dialog;
	CString					m_sDataFilePath;
	CString					m_sWeightFilePath;
	CString					m_sSavedFilePath;
// Methods of class CNeuronNetwork
public:
	CNeuronNetwork(void);
	~CNeuronNetwork(void);

	void operator=(const CNeuronNetwork& otherNetwork){
		m_vNetwork = otherNetwork.m_vNetwork;
		m_vSampleDatas = otherNetwork.m_vSampleDatas;
		m_vLabels = otherNetwork.m_vLabels;
		m_vLocationOfLabel = otherNetwork.m_vLocationOfLabel;		
		m_vErrors = otherNetwork.m_vErrors;
		m_dAlpha = otherNetwork.m_dAlpha;
		m_dAlphaMAX = otherNetwork.m_dAlphaMAX;
		m_dAlphaMIN = otherNetwork.m_dAlphaMIN;
		m_dSigma = otherNetwork.m_dSigma;
		m_dSigma0 = otherNetwork.m_dSigma0;
		m_dCurrentError = otherNetwork.m_dCurrentError;
		m_iFunction = otherNetwork.m_iFunction;
		m_iCurrentPattern = otherNetwork.m_iCurrentPattern;
		m_dwTopo = otherNetwork.m_dwTopo;
		m_dwIteration = otherNetwork.m_dwIteration;
		m_dwCurrentIteration = otherNetwork.m_dwCurrentIteration;
		m_dwDimensionOfInputLayer = otherNetwork.m_dwDimensionOfInputLayer;
		m_dwDimensionOfOutputLayer = otherNetwork.m_dwDimensionOfOutputLayer;
		m_CurrentWinner = otherNetwork.m_CurrentWinner;


		m_Dialog = otherNetwork.m_Dialog;
		m_sDataFilePath = otherNetwork.m_sDataFilePath;
		m_sWeightFilePath = otherNetwork.m_sWeightFilePath;
		m_sSavedFilePath = otherNetwork.m_sSavedFilePath;
	}

	void		Init(CWnd *dialog = NULL);

	void		SetDialog(CWnd *dialog);
	
	void		ClearSDHPoint();
	void		ClearAll();
	
	CNeuron		GetNeuron(DWORD dwIndex);
	CNeuron		GetNeuron(DWORD dwRowIndex, DWORD dwCollIndex);

	double		ComputeDistance(vector<double> vPattern, CNeuron neuron);
	double		ComputeDistance(vector<double> v1, vector<double> v2);

	CNeuron		FindWinner(DWORD dwIndex);
	CNeuron		FindWinner(vector<double> vPattern);

	// return Error
	double		LearnOnePattern();
	double		LearnOnePatternEx();

	// return Error
	double		LearnOneEpoch();
	void		UpdateParam();

	void		TrainingDataSet();
	void		ExecuteSDH(DWORD dwSparam);
};