#pragma once

#if !defined(ASSERT) && defined(ATLASSERT)
#define ASSERT ATLASSERT
#endif


template<typename T>
class Matrix
{
public:

	Matrix(const int NumRows = 0, const int NumCols = 1, T* pMem = NULL)
	{
		m_pMem = NULL;
		m_Owner = FALSE;

		Create(NumRows, NumCols, pMem);
	}

	virtual ~Matrix()
	{
		Destroy();
	}

	BOOL Create(const int NumRows, const int NumCols = 1, T* pMem = NULL)
	{
		if (m_pMem && m_Owner) delete[] m_pMem;

		m_NumCols = 0;
		m_NumRows = 0;
		m_pMem = NULL;
		m_Owner = FALSE;

		ASSERT(NumRows >= 0 && NumCols >= 0);

		if (NumCols <= 0 || NumRows <= 0) return FALSE;

		int Size = NumRows * NumCols;

		try
		{
			m_pMem = new T[Size];
		}
		catch (...)
		{
			m_pMem = NULL;
			return FALSE;
		}

		ASSERT(m_pMem);

		m_NumRows = NumRows;
		m_NumCols = NumCols;

		if (pMem) memcpy(m_pMem, pMem, Size * sizeof(T));

		m_Owner = TRUE;
		return TRUE;
	}

	void Destroy()
	{
		m_NumRows = 0;
		m_NumCols = 0;
		if (m_pMem)
		{
			if (m_Owner) delete[] m_pMem;
			m_pMem = NULL;
		}
		m_Owner = FALSE;
	}

	void Attach(const int NumRows, const int NumCols, T* pMem)
	{
		if (m_pMem && m_Owner) delete[] m_pMem;
		m_NumRows = NumRows;
		m_NumCols = NumCols;
		m_pMem = pMem;
		m_Owner = FALSE;
	}

	void CopyMem(const T* pMem)
	{
		ASSERT(m_pMem && pMem);
		memcpy(m_pMem, pMem, m_NumCols * m_NumRows * sizeof(T));
	}

	int GetNumRows() const
	{
		return m_NumRows;
	}

	int GetNumCols() const
	{
		return m_NumCols;
	}

	int GetSize() const
	{
		return m_NumRows * m_NumCols;
	}

	T* GetMem() const
	{
		return m_pMem;
	}

	const Matrix<T>& operator=(const Matrix<T>& M)
	{
		Create(M.m_NumRows, M.m_NumCols, M.m_pMem);
		return *this;
	}

	BOOL operator==(const Matrix<T>& M) const
	{
		if (m_NumRows != M.m_NumRows || m_NumCols != M.m_NumCols) return FALSE;
		return memcmp(m_pMem, M.m_pMem, m_NumRows * m_NumCols * sizeof(T)) == 0;
	}

	T& operator()(const int Row, const int Col)
	{
		ASSERT(Col >= 0 && Col < m_NumCols && Row >= 0 && Row < m_NumRows);
		return m_pMem[Row * m_NumCols + Col];
	}

	T operator()(const int Row, const int Col) const
	{
		ASSERT(Col >= 0 && Col < m_NumCols && Row >= 0 && Row < m_NumRows);
		return m_pMem[Row * m_NumCols + Col];
	}

	T& operator[](const int i)
	{
		ASSERT(i >= 0 && i < m_NumRows * m_NumCols);
		return m_pMem[i];
	}

	T operator[](const int i) const
	{
		ASSERT(i >= 0 && i < m_NumRows * m_NumCols);
		return m_pMem[i];
	}

	operator T* () const
	{
		return m_pMem;
	}

	void Fill(const T& Value)
	{
		for (int i = 0; i < m_NumRows * m_NumCols; i++) m_pMem[i] = Value;
	}

	int GetMin(T& Value) const
	{
		int Size = m_NumRows * m_NumCols;
		if (Size == 0) return -1;
		int k = 0;

		Value = m_pMem[0];

		for (int i = 1; i < Size; i++)
		{
			if (Value > m_pMem[i])
			{
				Value = m_pMem[i];
				k = i;
			}
		}
		return k;
	}

	int GetMax(T& Value) const
	{
		int Size = m_NumRows * m_NumCols;
		if (Size == 0) return -1;
		int k = 0;

		Value = m_pMem[0];

		for (int i = 1; i < Size; i++)
		{
			if (Value < m_pMem[i])
			{
				Value = m_pMem[i];
				k = i;
			}
		}
		return k;
	}

private:

	int m_NumRows;
	int m_NumCols;
	T* m_pMem;
	BOOL m_Owner;

};

//***************************************************************************************

void Transpose(const Matrix<real_t> &M, Matrix<real_t> &MT);
BOOL Multiply(const Matrix<real_t> &M1, const Matrix<real_t> &M2, Matrix<real_t> &M);
BOOL Invert(const Matrix<real_t> &M, Matrix<real_t> &MI);
BOOL LSM(const Matrix<real_t> &A, Matrix<real_t> &X, const Matrix<real_t> &F);
BOOL LES(const Matrix<real_t> &A, Matrix<real_t> &X, const Matrix<real_t> &F);
void Angles(const Matrix<real_t> &A, Matrix<real_t> &B);

void Zoom(const Matrix<real_t> &A, Matrix<real_t> &B, const real_t k);
void Normalize(Matrix<real_t> &A, const real_t Min, const real_t Max);


