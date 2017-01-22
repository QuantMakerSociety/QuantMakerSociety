#pragma once
/**
 * @file
 */
#include "qms_marketDepth.h"
#include "qms_group.h"
#include "qms_account.h"
#include "qms_deal.h"
#include "qms_order.h"
#include "qms_limit.h"
#include "qms_deposit.h"
#include "qms_pluginInfo.h"

/**
* @defgroup msg Messages
* @{
*/

/**
* Messages id.
*/
enum eQMSMsg
{
	/**
	* Unknown message.
	*/
	gbxam_unknown,

	/**
	* Listener added.
	*/
	gbxam_listenerAdded,

	/**
	* Listener removed.
	*/
	gbxam_listenerRemoved,

	/**
	* Information message.
	*/
	gbxam_info,

	/**
	* Client connected to server.
	*/
	gbxam_connected,

	/**
	* Client disconected from server.
	*/
	gbxam_disconnected,

	/**
	* Quote.
	*/
	gbxam_quote,

	/**
	* Group changed.
	*/
	gbxam_groupChanged,

	/**
	* Account information changed.
	*/
	gbxam_accountChanged,

	/**
	* Order was filled.
	*/
	gbxam_dealFilled,

	/**
	* Order was placed.
	*/
	gbxam_orderPlaced,

	/**
	* Limit was placed.
	*/
	gbxam_limitPlaced,

	/**
	* Limit was changed.
	*/
	gbxam_limitChanged,

	/**
	* Limit was canceled.
	*/
	gbxam_limitCancelled,

	/**
	* Order failed.
	*/
	gbxam_orderFailed,

	/**
	* Limit failed.
	*/
	gbxam_limitFailed,

	/**
	* Deposit or retirement.
	*/
	gbxam_deposit,

	/**
	* Request plugin information.
	*/
	gbxam_getPluginInfo,

	/**
	* Configuration was changed.
	*/
	gbxam_configChanged,

	/**
	 * Command.
	 */
	gbxam_command,

	/**
	 * Configuration per account changed.
	 */
	gbxam_acctCfgChanged,

	/**
	 * An item changed.
	 */
	gbxam_itemChanged,

};

/**
* Message.
*/
struct qms_msg
{
	/**
	* Id of message.
	*/
	uint8_t msg; // 0

	/**
	* Message flags.
	* This will depend on the message.
	*/
	uint8_t flg; // 0 + 1 = 1

	/**
	* Message len.
	*/
	uint16_t len; // 1 + 1 = 2
};                // 4 bytes

/**
* Information string.
*/
struct qms_msgInfo : public qms_msg
{
	enum
	{
		ID = gbxam_info, /// < Id of message.
	};

	/**
	* flag will be set to one of this values.
	*/
	enum eInfoFlag
	{
		UNKNOWN_MESSAGE = 0, ///< Unknown message type.
		INFO_MESSAGE    = 1, ///< Information message.
		WARNING_MESSAGE = 2, ///< Warning message.
		ERROR_MESSAGE   = 3, ///< Error message.
	};

	/**
	* NULL terminated string.
	* Its length is msg.len - sizeof(qms_msgInfo)
	*/
	char message[1]; // 4

	/**
	* Creates message inside a buffer.
	* @param buff Pointer to buffer.
	* @param buffLen Length of buffer.
	* @param mode Mode of Message.
	* @param txt Text for message.
	* @return Pointer to message on success. 0 on failure.
	*/
	static qms_msgInfo*
		Create(void* buff, size_t buffLen, uint8_t mode, const char* txt);
}; // 4 + 1 + strlen(message)

/**
* The listener has connected.
*/
struct qms_msgConnected : public qms_msg
{
	enum
	{
		ID = gbxam_connected, ///< Id of message.
	};

	/**
	* Constructor.
	*/
	qms_msgConnected()
	{
		msg = ID;
		flg = 0;
		len = sizeof(qms_msgConnected);
	}
}; // 4

/**
* Disconnected from server.
*/
struct qms_msgDisconnected : public qms_msg
{
	enum
	{
		ID = gbxam_disconnected, ///< Id of message.
	};

	/**
	* Constructor.
	*/
	qms_msgDisconnected()
	{
		msg = ID;
		flg = 0;
		len = sizeof(qms_msgDisconnected);
	}
}; // 4

/**
* Called when a symbol has changed.
*/
struct qms_msgQuote : public qms_msg
{
	enum
	{
		ID = gbxam_quote, ///< Id of message.
	};

	/**
	* Id of symbol.
	*/
	uint16_t idSymbol; // 0 + 4 = 4

	/**
	* Id of bank,
	*/
	uint16_t idBank; // 4 + 2 = 6

	/**
	* Points to the market depth that changed.
	*/
	const qms_marketDepth* md; // 6 + 2 = 8

	/**
	* Constructor,
	* @param ids Id of symbol.
	* @param idb Id of bank,
	* @param md_ Market depth.
	*/
	qms_msgQuote(uint16_t ids = 0, uint16_t idb = 0, qms_marketDepth* md_ = 0)
	{
		msg      = ID;
		flg      = 0;
		len      = sizeof(qms_msgQuote);
		idSymbol = ids;
		idBank   = idb;
		md       = md_;
	}
}; // 8 + 8 = 16

/**
* Group was changed.
*/
struct qms_msgGroupChanged : public qms_msg
{
	enum
	{
		ID = gbxam_groupChanged, ///< Id of message.
	};

	/**
	* Id of group.
	*/
	uint32_t idGroup; // 0 + 4 = 4

	/**
	* Points to group.
	*/
	qms_group* group; // 4 + 4 = 8

	/**
	* Constructor.
	* @param idg Id of group.
	* @param pg Pointer to group. 0 if it was removed.
	*/
	qms_msgGroupChanged(uint32_t idg = 0, qms_group* pg = 0)
	{
		msg     = ID;
		flg     = 0;
		len     = sizeof(qms_msgGroupChanged);
		idGroup = idg;
		group   = pg;
	}
}; // 8 + 8 = 16

/**
* Account was changed.
*/
struct qms_msgAccountChanged : public qms_msg
{
	enum
	{
		ID = gbxam_accountChanged, ///< Id of message.
	};

	/**
	* Id of account.
	*/
	uint32_t idAccount; // 4

	/**
	* Points to account changed.
	*/
	qms_account* account; // 4 + 4 = 8

	/**
	* Constructor.
	* @param ida Id of Account.
	* @param pa Pointer to account information. 0 if it was removed.
	*/
	qms_msgAccountChanged(uint32_t ida, qms_account* pa)
	{
		msg       = ID;
		flg       = 0;
		len       = sizeof(qms_msgAccountChanged);
		idAccount = ida;
		account   = pa;
	}
}; // 8 + 8 = 16

/**
* A deal was filled.
*/
struct qms_msgDealFilled : public qms_msg
{
	enum
	{
		ID = gbxam_dealFilled, ///< Id of message.
	};

	/**
	* Id of deal.
	*/
	uint32_t idDeal; // 4

	/**
	* Information on deal.
	*/
	qms_deal deal; // 4 + 4 = 8

	/**
	* Constructor.
	*/
	qms_msgDealFilled()
	{
		msg = ID;
		flg = 0;
		len = sizeof(qms_msgDealFilled);
	}
}; // 8 + 256 = 264

/**
* A market order was placed.
*/
struct qms_msgOrderPlaced : public qms_msg
{
	enum
	{
		ID = gbxam_orderPlaced, ///< Id of message.
	};

	/**
	* Id of order.
	*/
	uint32_t idOrder; // 4

	/**
	* Points to order.
	*/
	const qms_order* order; // 8

	/**
	* Constructor,
	* @param ido Id of order.
	* @param po Pointer to order.
	*/
	qms_msgOrderPlaced(uint32_t ido = 0, qms_order* po = 0)
	{
		msg     = ID;
		flg     = 0;
		len     = sizeof(qms_msgOrderPlaced);
		idOrder = ido;
		order   = po;
	}
};

/**
* A limit was placed.
*/
struct qms_msgLimitPlaced : public qms_msg
{
	enum
	{
		ID = gbxam_limitPlaced, ///< Id of message.
	};

	/**
	* Id of limit.
	*/
	uint32_t idLimit;

	/**
	* Points to limit.
	*/
	const qms_limit* limit; // 8

	/**
	* Constructor.
	* @param idl Id of limit.
	* @param pl Pointer to limit.
	*/
	qms_msgLimitPlaced(uint32_t idl = 0, qms_limit* pl = 0)
	{
		msg     = ID;
		flg     = 0;
		len     = sizeof(qms_msgLimitPlaced);
		idLimit = idl;
		limit   = pl;
	}
}; // 16

/**
* A limit was changed.
*/
struct qms_msgLimitChanged : public qms_msg
{
	enum
	{
		ID = gbxam_limitChanged, ///< Id of message.
	};

	/**
	* Id of limit.
	*/
	uint32_t idLimit; // 4

	/**
	* Points to limit.
	*/
	const qms_limit* limit; // 8
};                          // 16

/**
* A limit was cancelled
*/
struct qms_msgLimitCancelled : public qms_msg
{
	enum
	{
		ID = gbxam_limitCancelled, ///< Id of message.
	};

	/**
	* Id of limit.
	*/
	uint32_t idLimit; // 4

	/**
	* Points to limit that was cancelled.
	*/
	const qms_limit* limit; // 8
};                          // 16

/**
* Failed to place order.
*/
struct qms_msgOrderFailed : public qms_msg
{
	enum
	{
		ID = gbxam_orderFailed, ///< Id of message.
	};

	/**
	* Id of order.
	*/
	uint32_t idOrder; // 4

	/**
	* Points to order.
	*/
	const qms_order* order; // 8

	/**
	* Reason it failed.
	*/
	const char* reason; // 16
};

/**
* Failed to place limit
*/
struct qms_msgLimitFailed : public qms_msg
{
	enum
	{
		ID = gbxam_limitFailed, ///< Id of message.
	};

	/**
	* Id of limit
	*/
	uint32_t idLimit; // 4

	/**
	* Points to limit that was cancelled.
	*/
	const qms_limit* limit; // 8

	/**
	* Reason it failed.
	*/
	const char* reason; // 16
};                      // 24

/**
* A deposit or a retirement was made.
*/
struct qms_msgDeposit : public qms_msg
{
	enum
	{
		ID = gbxam_deposit, ///< Id of message.
	};

	/**
	* Deposit or retirement.
	*/
	qms_deposit deposit; // 8

	/**
	* Constructor
	*/
	qms_msgDeposit()
	{
		msg = ID;
		flg = 0;
		len = sizeof(qms_msgDeposit);
	}
}; // 16

/**
* Listener was added
* This is the first message the listener will receive.
*/
struct qms_msgAdded : public qms_msg
{
	enum
	{
		ID = gbxam_listenerAdded, ///< Id of message.
	};

	/**
	* Set by the addon to
	*/
	void* ptr;

	/**
	* Constructor.
	*/
	qms_msgAdded(void* p = 0)
	{
		msg = ID;
		flg = 0;
		len = sizeof(qms_msgAdded);
		ptr = p;
	}
}; // 4

/**
* Listener was removed.
* This is the last message the listener will receive.
*/
struct qms_msgRemoved : public qms_msg
{
	enum
	{
		ID = gbxam_listenerRemoved, ///< Id of message.
	};

	/**
	* Constructor.
	*/
	qms_msgRemoved()
	{
		msg = ID;
		flg = 0;
		len = sizeof(qms_msgRemoved);
	}

}; // 4

/**
* Request information on plugin.
*/
struct qms_getPluginInfo : public qms_msg
{
	enum
	{
		/**
		* Id of message.
		*/
		ID = gbxam_getPluginInfo
	};

	/**
	* Padding so next item is aligned to 8 bytes
	*/
	uint32_t pad;

	/**
	* Information must be filled by plugin.
	*/
	qms_pluginInfo* pInfo;

	/**
	* Constructor.
	* @param pi Pointer to information.
	*/
	qms_getPluginInfo(qms_pluginInfo* pi = 0)
	{
		msg   = ID;
		flg   = 0;
		len   = sizeof(qms_getPluginInfo);
		pad   = 0;
		pInfo = pi;
	}
};

/**
* Configuration for plugin was changed.
*/
struct qms_configChanged : public qms_msg
{
	enum
	{
		/**
		* Id of message.
		*/
		ID = gbxam_configChanged,
	};

	/**
	* Padding so next item is aligned to 8 bytes
	*/
	uint32_t pad;

	/**
	* Configuration.
	*/
	const qms_itemArray* pCfg;

	/**
	* Constructor.
	* @param pc = Pointer to configuration items.
	*/
	qms_configChanged(qms_itemArray* pc = 0)
	{
		msg  = ID;
		flg  = 0;
		len  = sizeof(qms_configChanged);
		pad  = 0;
		pCfg = pc;
	}
};

/**
 * Command.
 */
struct qms_command : public qms_msg
{
	enum
	{
		/**
		 * Id of message.
		 */
		ID = gbxam_command,
	};

	/**
	 * Command.
	 */
	int32_t cmd;

	/**
	 * Constructor.
	 * @param cmd_ Command.
	 */
	qms_command(int cmd_)
	{
		msg = ID;
		flg = 0;
		len = sizeof(qms_command);
		cmd = cmd_;
	}
};

/**
 * Called when the configuration of an account or symbol changed.
 */
struct qms_acctCfgChanged : public qms_msg
{
	enum
	{
		/**
		 * Id of message.
		 */
		ID = gbxam_acctCfgChanged,
	};

	/**
	 * Id of account. 0 = none.
	 */
	int32_t idAcct;

	/**
	 * Pointer to main node. Of config.
	 */
	const qms_item* pItem;
};

/**
 * Called when an item of a configuration has being changed
 */
struct qms_itemChanged : public qms_msg
{
	enum
	{
		/**
		 * Id of message.
		 */
		ID = gbxam_itemChanged,
	};

	/**
	 * Not used.
	 */
	int32_t pad;

	/**
	 * Pointer to item. 
	 */
	const qms_item* pItem;

	/**
	 * Constructor.
	 * @param pItem_ Pointer to item.
	 */
	qms_itemChanged(const qms_item* pItem_)
	{
		msg   = ID;
		flg   = 0;
		len   = sizeof(qms_itemChanged);
		pad   = 0;
		pItem = pItem_;
	}
};

/**
* @}
*/
