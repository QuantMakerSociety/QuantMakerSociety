#pragma once
/**
 * @file
 */

/**
 * Exception.
 */
class qms_exception
{
private:
	const char* m_fname; ///< File name where the exception took place.
	const char* m_msg;   ///< Message for exception.
	int         m_line;  ///< Line for exception.

	/**
	 * Default constructor. Private on purpose.
	 */
	qms_exception() : m_fname(0), m_msg(0), m_line(0)
	{
	}

public:
	/**
	 * Constructor.
	 * @param fname Path where the exception took place.
	 * @param line Line where the exception took place.
	 * @param msg Exception message.
	 * @return
	 */
	explicit qms_exception(const char* fname, int line, const char* msg)
	    : m_fname(fname), m_line(line), m_msg(msg)
	{
	}
	/**
	 * Copy constructor.
	 * @param x What to copy.
	 * @return
	 */
	explicit qms_exception(const qms_exception& x)
	    : m_fname(x.m_fname), m_msg(x.m_msg), m_line(x.m_line)
	{
	}

	/**
	 * Copy operator.
	 * @param x What to copy.
	 * @return *this
	 */
	qms_exception& operator=(const qms_exception& x)
	{
		if (this != &x) {
			m_fname = x.m_fname;
			m_msg   = x.m_msg;
			m_line  = x.m_line;
		}
		return *this;
	}

	/**
	 * Get Path to file.
	 * @return m_fname
	 */
	const char* GetPath() const
	{
		return m_fname;
	}

	/**
	 * Get Message.
	 * @return m_msg
	 */
	const char* GetMessage() const
	{
		return m_msg;
	}

	/**
	 * Get Line
	 * @return m_line
	 */
	int GetLine() const
	{
		return m_line;
	}
};

/**
 * @def QMS_EXCEPTION
 * Macro to create a qms_exception with the current file and line.
 *
 * @param x Exception message.
 */
#define QMS_EXCEPTION(x) qms_exception(__FILE__, __LINE__, x)
