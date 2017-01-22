#pragma once
/**
 * @file
 */
#include "qms_item.h"
#include "qms_currency.h"
#include "qms_symbol.h"
#include "qms_bankInfo.h"
#include "qms_exposure.h"
#include "qms_marketDepth.h"

#pragma region QMSServer
struct QMSServer;
struct qms_msg;
struct qms_barData;

/**
 * Listener
 * @param svr Server Instance.
 * @param msg Message received.
 * @param ptr User supplied pointer.
 * @return Depends on message.
 */
typedef int(
	__stdcall* qms_listener)(QMSServer& svr, const qms_msg& msg, void* ptr);

/**
 * Called when the bar data changed.
 * @param svr Server instance.
 * @param msg Message received. @see eBarDataMsg
 * @param bd Bar data.
 */
typedef void(__stdcall* qms_onBarDataChanged)(
	QMSServer&         svr,
	int                msg,
	const qms_barData& bd);

/**
 * Retrieve icon. (Optional)
 */
typedef HICON(__stdcall* qms_getIcon)();

/**
* API Server.
*/
struct QMSServer
{
	/**
	* Virtual Destructor.
	*/
	virtual ~QMSServer()
	{
	}

	/**
	* Get status of connection.
	*
	* @return Status of connection.
	*/
	virtual int GetConnectionStatus() const = 0;

	/**
	 * Places a market order.
	 * @param who Pointer to listener
	 * @param idAccount Id of account.
	 * @param idSymbol Id of symbol
	 * @param stratId If of strategy.
	 * @param volume Volume (Negative for short).
	 * @param price Price.
	 * @param desc Description.
	 * @return 0 on success. -1 on failure.
	 */
	virtual int PlaceMarketOrder(
		qms_listener who,
		int          idAccount,
		int          idSymbol,
		uint64_t     stratId,
		double       volume,
		double       price,
		const char*  desc) = 0;

	/**
	 * Places a limit order.
	 * @param who Pointer to listener
	 * @param idAccount Id of the account.
	 * @param idSymbol Id of the symbol.
	 * @param stratId Strategy id.
	 * @param mode Mode
	 * @param tif Time in force.
	 * @param volume Volume.
	 * @param price Price
	 * @param stopPrice Stop price
	 * @param desc Description
	 * @return 0 on success, -1 on failure.
	 */
	virtual int PlaceLimit(
		qms_listener who,
		int          idAccount,
		int          idSymbol,
		uint64_t     stratId,
		char         mode,
		char         tif,
		double       volume,
		double       price,
		double       stopPrice,
		const char*  desc) = 0;

	virtual int CancelLimit(uint32_t id) = 0;

	virtual int EditLimit(uint32_t id, double newPrice, double newStop) = 0;

	/**
	* Get Unix time in micro seconds.
	* @return Unix time in microseconds.
	*/
	virtual uint64_t USec() const = 0;

	/**
	* Get Array of currencies.
	* @return Array of currencies.
	*/
	virtual const qms_currencyArray* GetCurrencies() const = 0;

	/**
	* Get Array of Symbols
	* @return Array of symbols.
	*/
	virtual const qms_symbolArray* GetSymbols() const = 0;

	/**
	* Get Array of banks,
	* @return Array of banks.
	*/
	virtual const qms_bankInfoArray* GetBanks() const = 0;

	/**
	* Get array of groups.
	* @return Array of groups.
	*/
	virtual const qms_groupArray* GetGroups() const = 0;

	/**
	* Request starting of quotes.
	* @param ltn Pointer to listener. (So that the server knows who is
	*  		  requesting the quotes.
	* @param bStop Set to true to stop the quotes instead of starting them.
	*/
	virtual void StartQuotes(qms_listener ltn, bool bStop = false) = 0;

	/**
	* Given a group id get a group of accounts.
	* @param id Id of group.
	* @return group if found, otherwise NULL
	*/
	virtual const qms_group* GetGroup(uint32_t id) const = 0;

	/**
	 * Given a group id. Get an array of accounts.
	 * @param id Id of group.
	 * @return Array of accounts.
	 */
	virtual const qms_accountArray* GetGroupAccounts(uint32_t id) const = 0;

	/**
	* Given an id get an account.
	* @param id Id of account.
	* @return Account if found. Otherwise NULL;
	*/
	virtual const qms_account* GetAccount(uint32_t id) const = 0;

	/**
	* Given an id, get a deal.
	* @return Deal if found. Otherwise NULL
	*/
	virtual const qms_deal* GetDeal(uint32_t id) const = 0;

	/**
	* Get array of deals of an account.
	* @return pointer to
	*/
	virtual const qms_dealArray* GetAccountDeals(uint32_t id) const = 0;

	/**
	* Get array of account deposits.
	* @return pointer to deposits.
	*/
	virtual const qms_depositArray* GetAccountDeposits(uint32_t id) const = 0;

	/**
	* Get exposure of an account.
	* @return pointer to account exposure.
	*/
	virtual const qms_exposureArray* GetAccountExposure(uint32_t id) const = 0;

	/**
	* Request market depth
	* @param ids Id of symbol.
	* @return Market depth if found. NULL if not.
	*/
	virtual const qms_marketDepth* GetMarketDepth(uint32_t ids) const = 0;

	/**
	 * Register bar data listener.
	 * @param ltn Listener (used to identify what plugin is calling)
	 * @param fun Pointer to listener.
	 * @param baseCfg Base configuration.
	 * @param ptr User defined pointer.
	 * The configuration received will be based on this one.
	 * @return < 0 on error. >= 0 on success.
	 */
	virtual int RegisterBarDataListener(
		qms_listener         ltn,
		qms_onBarDataChanged fun,
		const qms_itemArray& baseCfg,
		void*                ptr) = 0;

	/**
	* Add quant.
	* @param ltn
	* @param ids
	* @param ida
	* @param tf
	*/
	virtual void AddQuant(qms_listener ltn, int ids, int ida, int tf) = 0;
};

#pragma endregion
