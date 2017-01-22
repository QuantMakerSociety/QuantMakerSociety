/**
 * @file
 */
#pragma once
#ifndef _WINDOWS_
#error Windows.h must be included before this header.
#endif

/**
 * Critical section.
 */
class qms_cs
{
private:
	CRITICAL_SECTION m_cs; ///< Mutex between threads of the same process.

	/**
	 * Assignment operator. Private an unimplemented on purpose.
	 * @param	rhs	Not used.
	 * @return	Not used.
	 */
	qms_cs& operator=(const qms_cs& rhs);

	/**
	 * Copy constructor. Private an unimplemented on purpose.
	 * @param	rhs	Not used.
	 */
	qms_cs(const qms_cs& rhs);

public:
	/**
	 * Lock.
	 */
	class Lock
	{
	private:
		qms_cs& m_cs; ///< Reference to critical section.

		/**
		 * Assignment operator. @warning Private and unimplemented on purpose.
		 * @param rhs Not used.
		 * @return Not used.
		 */
		Lock& operator=(const Lock& rhs);

		/**
		 * Copy constructor. @warning Private an unimplemented on purpose.
		 * @param	rhs	Not used.
		 */
		Lock(const Lock& rhs);

	public:
		/**
		 * Constructor.
		 *
		 * @param [in,out]	cs	The create struct.
		 */
		explicit Lock(qms_cs& cs) : m_cs(cs)
		{
			m_cs.Acquire();
		}

		/**
		 * Destructor.
		 */
		~Lock()
		{
			m_cs.Release();
		}
	};

	/**
	 * Default constructor.
	 */
	qms_cs()
	{
		::InitializeCriticalSection(&m_cs);
	}

	/**
	 * Destructor.
	 */
	~qms_cs()
	{
		::DeleteCriticalSection(&m_cs);
	}

	/**
	 * Acquires this object.
	 */
	void Acquire()
	{
		::EnterCriticalSection(&m_cs);
	}

	/**
	 * Releases this object.
	 */
	void Release()
	{
		::LeaveCriticalSection(&m_cs);
	}
};
