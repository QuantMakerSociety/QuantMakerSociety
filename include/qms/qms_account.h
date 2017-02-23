#pragma once
/**
 * @file
 */
/**
* Account.
*/
struct qms_account
{
	/**
	 * Id of account.
	 */
	uint32_t id; // 0 + 4 = 4

	/**
	 * Id of group.
	 */
	uint32_t idGroup; // 4 + 4 = 8

	/**
	* Account flags.
	*/
	uint32_t flags; // 8 + 4 = 12

	/**
	* Name of account.
	*/
	char name[36]; // 12 + 36 = 48

	/**
	* Current account balance.
	*/
	double balance; // 48 + 8 = 56

	/**
	* Current account equity.
	*/
	double equity; // 56 + 8 = 64

	/**
	* Leverage of account.
	*/
	double leverage; // 64 + 8 = 72

	/**
	* Maximum margin.
	*/
	double marginRatio; // 72 + 8 = 80

	/**
	* Margin in use.
	*/
	double marginInUse; // 80 + 8 = 88

	/**
	* Constructor.
	*/
	qms_account()
	{
		id          = 0;
		idGroup     = 0;
		flags       = 0;
		name[0]     = 0;
		balance     = 0;
		equity      = 0;
		leverage    = 1.0;
		marginRatio = 1.0;
		marginInUse = 0;
	}
}; // 88

/**
* Array of accounts.
*/
typedef qms_tarray< qms_account > qms_accountArray;

