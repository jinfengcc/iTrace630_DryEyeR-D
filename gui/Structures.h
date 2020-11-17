//***************************************************************************************

#pragma once

//***************************************************************************************

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

template<typename T>
class ListNode
{
public:
	ListNode<T>* m_pPrvListNode;
	ListNode<T>* m_pNxtListNode;
	T m_Data;
};

//***************************************************************************************

template<typename T>
class List
{
public:

	List()
	{
		m_pHeadListNode = NULL;
		m_pTailListNode = NULL;
		m_Size = 0;
		m_pCurListNode = NULL;
	}

	virtual ~List()
	{
		Destroy();
	}

	int GetSize() const
	{
		return m_Size;
	}

	void Destroy()
	{
		ListNode<T>* pNode = m_pHeadListNode;

		while (pNode)
		{
			ListNode<T>* pNextNode = pNode->m_pNxtListNode;
			delete pNode;
			pNode = pNextNode;
		}

		m_pHeadListNode = NULL;
		m_pTailListNode = NULL;
		m_Size = 0;
		m_pCurListNode = NULL;
	}

	T* Prepend(T* pData = NULL)
	{
		ListNode<T>* pNewNode = new ListNode<T>;
		pNewNode->m_pPrvListNode = NULL;
		pNewNode->m_pNxtListNode = m_pHeadListNode;

		if (pData) pNewNode->m_Data = *pData;

		if (!m_pHeadListNode)
		{
			m_pTailListNode = pNewNode;
		}
		else
		{
			m_pHeadListNode->m_pPrvListNode = pNewNode;
		}

		m_pHeadListNode = pNewNode;
		m_Size++;
		return &pNewNode->m_Data;
	}

	T* Append(const T* pData = NULL)
	{
		ListNode<T>* pNewNode = new ListNode<T>;
		pNewNode->m_pPrvListNode = m_pTailListNode;
		pNewNode->m_pNxtListNode = NULL;

		if (pData) pNewNode->m_Data = *pData;

		if (!m_pHeadListNode)
		{
			m_pHeadListNode = pNewNode;
		}
		else
		{
			m_pTailListNode->m_pNxtListNode = pNewNode;
		}

		m_pTailListNode = pNewNode;
		m_Size++;
		return &pNewNode->m_Data;
	}

	T* Append(const T& Data)
	{
		return Append(&Data);
	}

	T* Insert(const int i, T* pData = NULL)
	{
		if (i <= 0) return Prepend(pData);

		if (i >= m_Size) return Append(pData);

		ListNode<T>* pNode = m_pHeadListNode;

		for (int k = 0; k < i; k++) pNode = pNode->m_pNxtListNode;

		ListNode<T>* pNewNode = new ListNode<T>;

		pNewNode->m_pPrvListNode = pNode->m_pPrvListNode;
		pNewNode->m_pNxtListNode = pNode;

		if (pData) pNewNode->m_Data = *pData;

		pNode->m_pPrvListNode->m_pNxtListNode = pNewNode;
		pNode->m_pPrvListNode = pNewNode;

		m_Size++;

		return &pNewNode->m_Data;
	}

	void Remove(const int i)
	{
		if (i < 0 || i >= m_Size) return;

		ListNode<T>* pNode = m_pHeadListNode;

		for (int k = 0; k < i; k++) pNode = pNode->m_pNxtListNode;

		if (pNode == m_pCurListNode) m_pCurListNode = NULL;

		ListNode<T>* pPrevNode = pNode->m_pPrvListNode;
		ListNode<T>* pNextNode = pNode->m_pNxtListNode;

		delete pNode;

		if (pPrevNode)
		{
			pPrevNode->m_pNxtListNode = pNextNode;
		}
		else
		{
			m_pHeadListNode = pNextNode;
		}

		if (pNextNode)
		{
			pNextNode->m_pPrvListNode = pPrevNode;
		}
		else
		{
			m_pTailListNode = pPrevNode;
		}

		m_Size--;
	}

	T* Get(const int i)
	{
		if (i < 0 || i >= m_Size) return NULL;

		ListNode<T>* pNode = NULL;

		pNode = m_pHeadListNode;

		for (int k = 0; k < i; k++)
		{
			pNode = pNode->m_pNxtListNode;
		}

		if (pNode) return &pNode->m_Data;

		return NULL;
	}

	T* MoveFirst()
	{
		m_pCurListNode = m_pHeadListNode;
		if (m_pCurListNode == NULL) return NULL;
		return &m_pCurListNode->m_Data;
	}

	T* MoveLast()
	{
		m_pCurListNode = m_pTailListNode;

		if (m_pCurListNode == NULL) return NULL;

		return &m_pCurListNode->m_Data;
	}

	T* MoveNext()
	{
		if (m_pCurListNode == NULL) return NULL;

		m_pCurListNode = m_pCurListNode->m_pNxtListNode;

		if (m_pCurListNode == NULL) return NULL;

		return &m_pCurListNode->m_Data;
	}

	T* MovePrev()
	{
		if (m_pCurListNode == NULL) return NULL;

		m_pCurListNode = m_pCurListNode->m_pPrvListNode;

		if (m_pCurListNode == NULL) return NULL;

		return &m_pCurListNode->m_Data;
	}

	BOOL Find(const T& Data)
	{
		ListNode<T>* pNode = m_pHeadListNode;

		while (pNode)
		{
			if (pNode->m_Data == Data) return TRUE;
			pNode = pNode->m_pNxtListNode;
		}

		return FALSE;
	}

	void operator=(const List<T>& L)
	{
		Destroy();

		ListNode<T>* pNode = L.m_pHeadListNode;

		while (pNode)
		{
			Append(&pNode->m_Data);
			pNode = pNode->m_pNxtListNode;
		}
	}

	ListNode<T>* GetCurListNode()
	{
		return m_pCurListNode;
	}

	void SetCurListNode(ListNode<T>* pCurListNode)
	{
		m_pCurListNode = pCurListNode;
	}

private:

	ListNode<T>* m_pHeadListNode;
	ListNode<T>* m_pTailListNode;

	int m_Size;

	ListNode<T>* m_pCurListNode;

};

//***************************************************************************************

void Transpose(const Matrix<real_t>& M, Matrix<real_t>& MT);
BOOL Multiply(const Matrix<real_t>& M1, const Matrix<real_t>& M2, Matrix<real_t>& M);
BOOL Invert(const Matrix<real_t>& M, Matrix<real_t>& MI);
BOOL LSM(const Matrix<real_t>& A, Matrix<real_t>& X, const Matrix<real_t>& F);
BOOL LES(const Matrix<real_t>& A, Matrix<real_t>& X, const Matrix<real_t>& F);
void Angles(const Matrix<real_t>& A, Matrix<real_t>& B);

void Zoom(const Matrix<real_t>& A, Matrix<real_t>& B, const real_t k);
void Normalize(Matrix<real_t>& A, const real_t Min, const real_t Max);

//***************************************************************************************
