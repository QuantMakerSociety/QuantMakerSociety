#pragma once

/**
 * Dual moving average plugin.
 * @param svr Server interface.
 * @param msg Message received.
 * @param bd Bars interface. 
 */
void DualMAPlugin(QMSServer &svr, int msg, const qms_barData &bd, const void * pExtra);
