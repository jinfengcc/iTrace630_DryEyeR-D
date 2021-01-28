#pragma once

template <typename T>
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


