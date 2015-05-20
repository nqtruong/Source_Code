#pragma once
#include <vector>


using namespace std;
class CNeuron
{
// Attributes of class CNeuron
public:
	vector<double>		m_vWeights;
	vector<CString>		m_vLabels;
	DWORD				m_dwXCoor, m_dwYCoor;
	double				m_dXCoor, m_dYCoor;		// Real Coordinate depend on topology
	DWORD				m_dwSDHPoint;

	CObject				*m_pNNetwork;

// Methods of class CNeuron
public:
	CNeuron(void);
	~CNeuron(void);
	CNeuron(DWORD dwXCoor, DWORD dwYCoor, CObject *pNNetwork);
	CNeuron( DWORD dwXCoor, DWORD dwYCoor, CObject *pNNetwork, vector<double> weightVector);
	
	double		Factor();

	void		ComputeRealCoordinate();
	void		InitWeigths();
	void		InitWeigths(vector<double> weightVector);

	void		ResetListLabels();
	void		Clear();

	double		UpdateWeight();

	void		AddPoint( DWORD dwPoint );
};

