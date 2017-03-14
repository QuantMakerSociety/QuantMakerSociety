#pragma once
#include <stdio.h>

extern FILE * g_log;

/**
* Starts log.
*/
void StartLog();

/**
* Stops log.
*/
void CloseLog();

/**
* Prints to the log.
* @param fmt printf like format.
*/
void Log(const char *fmt, ...);
