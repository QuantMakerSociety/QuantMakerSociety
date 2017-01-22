#pragma once
/**
 * Constants.
 */
/**
 * Constants.
 */
enum eQMSConstants
{
    /**
    * Maximum market depth.
    */
    QMS_MAX_MD = 40,

    /**
    * Maximum number of symbols.
    */
    QMS_MAX_SYMBOLS = 256,

    /**
    * Maximum number of banks.
    */
    QMS_MAX_BANKS = 64,
};

/**
 * Error codes.
 */
enum eQMSError
{
    /**
     * No error.
     */
    eQMSError_ok = 0,

    /**
     * Generic error.
     */
    eQMSError_fail = -1,

    /**
     * Invalid parameters.
     */
    eQMSError_invalidParams = -2,
};

/**
 * Connection status.
 */
enum eQMSConnectionStatus
{
    /**
     * Disconnected.
     * Does not want to login.
     */
    eQMSConnectionStatus_disconnected,

    /**
     * Connecting.
     * Not yet connected.
     */
    eQMSConnectionStatus_connecting,

    /**
     * Connected.
     * Not yet started
     */
    eQMSConnectionStatus_connected,

    /**
     * Logged in.
     */
    eQMSConnectionStatus_loggedIn,

    /**
     * Connected.
     * Got currencies.
     */
    eQMSConnectionStatus_gotCurrencies,

    /**
     * Connected.
     * Got Symbols.
     */
    eQMSConnectionStatus_gotSymbols,

    /**
     * Connected.
     * Got Banks.
     */
    eQMSConnectionStatus_gotBanks,

    /**
     * Connected.
     * Got Groups.
     */
    eQMSConnectionStatus_gotGroups,

    /**
     * Connected.
     * Got accounts.
     */
    eQMSConnectionStatus_gotAccounts,

    /**
     * Connected.
     * Got deposits.
     */
    eQMSConnectionStatus_gotDeposits,

    /**
     * Connected.
     * Got deals.
     */
    eQMSConnectionStatus_gotDeals,

    /**
     * Connected.
     * Got limits.
     */
    eQMSConnectionStatus_gotLimits,

    /**
    * Connected.
    * Started quotes.
    */
    eQMSConnectionStatus_startedQuotes,

    /**
    * Connected.
    * Main state.
    */
    eQMSConnectionStatus_running,

    /**
    * Disconnecting.
    */
    eQMSConnectionStatus_disconnecting,

    /**
    * Exiting.
    */
    eQMSConnectionStatus_exiting,
};
