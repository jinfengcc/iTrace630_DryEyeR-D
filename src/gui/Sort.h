//***************************************************************************************

#pragma once

//***************************************************************************************

template<typename T>

void sort_asc(T* m, const int n, int k = INT_MAX)
{
	k = __min(k, n - 1);

	for (int i = 0; i < k; i++)
	{
		for (int j = n - 1; j > i; j--)
		{
			if (m[j - 1] > m[j]) Swap(m[j], m[j - 1]);
		}
	}
}

//***************************************************************************************

template<typename T>

void sort_des(T* m, const int n, int k = INT_MAX)
{
	k = __min(k, n - 1);

	for (int i = 0; i < k; i++)
	{
		for (int j = n - 1; j > i; j--)
		{
			if (m[j - 1] < m[j]) Swap(m[j], m[j - 1]);
		}
	}
}

//***************************************************************************************