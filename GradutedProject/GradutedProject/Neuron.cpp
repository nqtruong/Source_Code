#include "StdAfx.h"
#include "Neuron.h"
#include "NeuronNetwork.h"
#include "Definition.h"
#include <math.h>

CNeuron::CNeuron(void)
{
}

CNeuron::CNeuron( DWORD dwXCoor, DWORD dwYCoor, CObject *pNNetwork )
{
	m_dwXCoor = dwXCoor;
	m_dwYCoor = dwYCoor;
	m_pNNetwork = pNNetwork;

	ComputeRealCoordinate();

	InitWeigths();
}

CNeuron::CNeuron( DWORD dwXCoor, DWORD dwYCoor, CObject *pNNetwork, vector<double> weightVector)
{
	m_dwXCoor = dwXCoor;
	m_dwYCoor = dwYCoor;
	m_pNNetwork = pNNetwork;
	
	ComputeRealCoordinate();

	m_vWeights = weightVector;
}

CNeuron::~CNeuron(void)
{
}

double CNeuron::Factor()
{
	CNeuron	winnerNeuron = ((CNeuronNetwork*)m_pNNetwork)->m_CurrentWinner;
	double	sigma = ((CNeuronNetwork*)m_pNNetwork)->m_dSigma;
	INT		iFunction = ((CNeuronNetwork*)m_pNNetwork)->m_iFunction;
	double	result = 0;
	double	distance = 0;

	if (sigma <= 0 || distance > sigma)
		return 0.0;

	switch (iFunction)
	{
	case MyNamespace::DISCRETE:
		{
			distance = fabs((double)(m_dXCoor - winnerNeuron.m_dXCoor)) + fabs((double)(m_dYCoor - winnerNeuron.m_dYCoor));
			switch ((int)distance)
			{
			case 0:
				result = 1;
				break;
			case 1:
				result = 0.5f;
				break;
			case 2:
				result = 0.25f;
				break;
			case 3:
				result = 0.125f;
				break;
			}
			break;
		}
	case MyNamespace::GAUS:
		{
			distance = sqrt(pow((double)(m_dXCoor - winnerNeuron.m_dXCoor), 2) + pow((double)(m_dYCoor - winnerNeuron.m_dYCoor), 2));
			result = exp(-(distance * distance) / (2.0*pow(sigma, 2)));
			break;
		}
	case MyNamespace::MEXICANHAT:
		{
			distance = sqrt(pow((double)(m_dXCoor - winnerNeuron.m_dXCoor), 2) + pow((double)(m_dYCoor - winnerNeuron.m_dYCoor), 2));
			result = exp(-(distance * distance) / pow(sigma, 2)) * (1 - (2.0 / pow(sigma, 2)) * (distance * distance));                        
			break;
		}
	case MyNamespace::FRENCHHAT:
		{
			int a = 2;
			distance = fabs((double)(m_dXCoor - winnerNeuron.m_dXCoor)) + fabs((double)(m_dYCoor - winnerNeuron.m_dYCoor));
			if (distance <= a) result = 1.0;
			else
				if (distance < a && distance <= 3 * a) result = -1.0 / 3.0;
				else
					if (distance > 3 * a) result = 0.0;
			break;
		}
	}
	return result;
}

void CNeuron::InitWeigths()
{
	DWORD	dwDimension = ((CNeuronNetwork*)m_pNNetwork)->m_dwDimensionOfInputLayer;

	for(DWORD i = 0; i < dwDimension; i++){
		long temp = (rand() * rand()) %331;
		long t = (m_dwXCoor*17 + m_dwYCoor*11);
		double x = sin((double)((temp+t)%7));
		x = sqrt(fabs(x+1))*1000.0;
		x -= (int)x;
// 		if(x <= 0.01) x += 0.6115;
// 		else if(x > 0.01 && x <= 0.2309) x = 0.02;
// 		else if(x > 0.23 && x <= 0.6112) x+= 0.2315;
// 		else if(x > 0.6) x = 0.513;
		m_vWeights.push_back(x);
	}
}

void CNeuron::ComputeRealCoordinate()
{
	if (m_pNNetwork)
	{
			if (((CNeuronNetwork*)m_pNNetwork)->m_dwTopo == MyNamespace::RECTANGLE)
		{
			/* Rectangle
				___ ___ ___ ___
			   |0;0|0;1|0;2|0;3|
			   |___|___|___|___|
			   |1;0|1;1|1;2|1;3|
			   |___|___|___|___|
			*/

			m_dXCoor = m_dwXCoor;
			m_dYCoor = m_dwYCoor;
		}
		else
		{
				/* Hexagonal
					 / \ / \ / \ / \	   _______Y
		    		|0;0|0;1|0;2|0;3|	  |
				   / \ / \ / \ / \ /	  |
				  |1;0|1;1|1;2|1;3|       |
				   \ / \ / \ / \ /		  X
				*/
			if (m_dwXCoor%2 == 0)
			{
				m_dYCoor = (double)m_dwYCoor + 0.5;
			}
			else
				m_dYCoor = (double)m_dwYCoor;

			m_dXCoor = (double)m_dwXCoor*sqrt(3.0)/2.0;
		}
	}
}

void CNeuron::InitWeigths(vector<double> weightVector)
{
	m_vWeights = weightVector;
}

void CNeuron::ResetListLabels()
{
	m_vLabels.clear();
}

void CNeuron::Clear()
{
	m_dwXCoor = 0;
	m_dwYCoor = 0;
	m_dwSDHPoint = 0;
	m_pNNetwork = NULL;
	m_vLabels.clear();
	m_vWeights.clear();
}

double CNeuron::UpdateWeight()
{
	INT				iCurrentPattern = ((CNeuronNetwork*)m_pNNetwork)->m_iCurrentPattern;
	vector<double>	vPattern = ((CNeuronNetwork*)m_pNNetwork)->m_vSampleDatas[iCurrentPattern];
	double			dAlpha = ((CNeuronNetwork*)m_pNNetwork)->m_dAlpha;
	DWORD			dwDimension = ((CNeuronNetwork*)m_pNNetwork)->m_dwDimensionOfInputLayer;

	double			avgDelta = 0.0;
	double			modificationValue =0.0;
	double			dFactor = Factor();

	if(dFactor == 0.0)
		return 0.0;
	
	for (DWORD i = 0; i < dwDimension; i++)
	{
		modificationValue = dAlpha * dFactor * (vPattern[i] - m_vWeights[i]);
		m_vWeights[i] += modificationValue;
		avgDelta += modificationValue*modificationValue;			// Lay tri tuyet doi hay k ?????
	}
	
	return sqrt(avgDelta) / (double)dwDimension;
}

void CNeuron::AddPoint( DWORD dwPoint )
{
	m_dwSDHPoint += dwPoint;
}
