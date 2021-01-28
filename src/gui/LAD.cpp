//***************************************************************************************

#include "StdAfx.h"
#include "Data.h"
#include "FT.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

CLAD::CLAD()
{
	memset(m_Signal, 0, sizeof(m_Signal));
	m_Baseline = 0.0;
	m_Max = 0;
	m_MaxPos = 0;
	m_CentroidPos = 0.0;
	m_Area = 0;
	m_Bad = FALSE;
}

//***************************************************************************************

void CLAD::Process()
{
	m_Baseline = 0.0;
	m_Max = 0;
	m_MaxPos = 0;
	m_CentroidPos = 0.0;
	m_Area = 0;
	m_Bad = TRUE;

	int il = 20;
	int ir = 511 - il;

	// baseline
	int k = 0;

	for (int i = il; i < il + 50; i++)
	{
		m_Baseline += m_Signal[i];
		k++;
	}

	for (int i = ir; i > ir - 50; i--)
	{
		m_Baseline += m_Signal[i];
		k++;
	}

	m_Baseline /= k;

	// maximum
	for (int i = il; i <= ir; i++)
	{
		if (m_Max < m_Signal[i])
		{
			m_Max = m_Signal[i];
			m_MaxPos = i;
		}

		// area under the curve above baseline
		real_t d = m_Signal[i] - m_Baseline;
		if (d > 0.0) m_Area += d;
	}

	// filtering
	real_t PrcSignal[512];
	memset(PrcSignal, 0, sizeof(PrcSignal));

	for (int i = il; i <= ir; i++)
	{
		int s = 0;
		int k = 0;
		int m = 30;
		int a = __max(0, i - m);
		int b = __min(511, i + m);

		for (int j = a; j <= b; j++)
		{
			int q = m + 1 - abs(j - i);
			s += q * m_Signal[j];
			k += q;
		}

		PrcSignal[i] = (real_t)s / k;
	}

	// global maximum
	real_t vmax = 0.0;
	int imax = 255;

	for (int i = il; i <= ir; i++)
	{
		if (vmax < PrcSignal[i])
		{
			vmax = PrcSignal[i];
			imax = i;
		}
	}

	if (vmax - m_Baseline < 40.0) return;

	// 90% centroid
	real_t vthr90 = m_Baseline + 0.9 * (vmax - m_Baseline);
	real_t vs = 0.0;
	real_t s = 0.0;

	for (int i = imax - 1; i >= il; i--)
	{
		real_t d = PrcSignal[i] - vthr90;
		if (d < 0.0) break;
		vs += i * d;
		s += d;
	}

	for (int i = imax + 1; i <= ir; i++)
	{
		real_t d = PrcSignal[i] - vthr90;
		if (d < 0.0) break;
		vs += i * d;
		s += d;
	}

	if (s == 0.0) return;

	m_CentroidPos = vs / s;

	// validation

	// signal above the 50% threshold
	real_t vthr50 = m_Baseline + 0.5 * (vmax - m_Baseline);
	int ithrl50 = imax; // на всякий

	for (int i = imax - 1; i >= il; i--)
	{
		if (PrcSignal[i] < vthr50)
		{
			ithrl50 = i + 1;
			break;
		}
	}

	int ithrr50 = imax;

	for (int i = imax + 1; i <= ir; i++)
	{
		if (PrcSignal[i] < vthr50)
		{
			ithrr50 = i - 1;
			break;
		}
	}

	// there is another spike above the 50% threshold
	for (int i = ithrl50 - 1; i >= il; i--)
	{
		if (PrcSignal[i] > vthr50) return;
	}

	for (int i = ithrr50 + 1; i <= ir; i++)
	{
		if (PrcSignal[i] > vthr50) return;
	}

	// local maxima
	real_t local_vmin = PrcSignal[imax];

	for (int i = imax - 1; i >= ithrl50 + 1; i--)
	{
		if (PrcSignal[i - 1] > PrcSignal[i] && PrcSignal[i] < PrcSignal[i + 1])
		{
			local_vmin = PrcSignal[i];
		}
		else if (PrcSignal[i - 1] < PrcSignal[i] && PrcSignal[i] > PrcSignal[i + 1]) {
			if (PrcSignal[i] - local_vmin > 0.05 * (vmax - m_Baseline)) return;
		}
	}

	local_vmin = PrcSignal[imax];

	for (int i = imax + 1; i <= ithrr50 + 1; i++)
	{
		if (PrcSignal[i - 1] > PrcSignal[i] && PrcSignal[i] < PrcSignal[i + 1])
		{
			local_vmin = PrcSignal[i];
		}
		else if (PrcSignal[i - 1] < PrcSignal[i] && PrcSignal[i] > PrcSignal[i + 1])
		{
			if (PrcSignal[i] - local_vmin > 0.05 * (vmax - m_Baseline)) return;
		}
	}

	m_Bad = FALSE;
}

//***************************************************************************************
