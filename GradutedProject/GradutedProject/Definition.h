namespace MyNamespace
{
	enum NEIGHBOR_FUNCTION
	{
		DISCRETE = 1,
		GAUS,
		MEXICANHAT,
		FRENCHHAT,
	};

	enum TOPOLOGY
	{
		RECTANGLE = 1,
		HEXAGONAL,
	};
};

typedef struct SDH{
	DWORD  dwIndex;		// index of neuron in Network
	double dDistance;	// Distance from this neuron to a specify pattern

	SDH(DWORD _dwIndex, double _dDistance)
	{
		dwIndex = _dwIndex;
		dDistance = _dDistance;
	}

	BOOL operator<(const SDH &other)
	{
		return (dDistance < other.dDistance);
	}

	void operator=(const SDH& otherSDH)
	{
		dwIndex = otherSDH.dwIndex;
		dDistance = otherSDH.dDistance;
	}
}SDH, *PSDH;