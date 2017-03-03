#pragma once

/**
 * Sees all the trades that where received.
 * @param svr Server,
 * @param msg Message with informaiton on the deal being filled.
 */
void OnDealFilled(QMSServer &svr, const qms_msgDealFilled &msg);
