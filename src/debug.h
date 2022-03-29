/*
 * Simple WHIP client
 *
 * Author:  Lorenzo Miniero <lorenzo@meetecho.com>
 * License: GPLv3
 *
 * Logging utilities, copied from the Janus WebRTC Server
 *
 */

#ifndef WHIP_DEBUG_H
#define WHIP_DEBUG_H

#include <inttypes.h>

#include <glib.h>
#include <glib/gprintf.h>

extern int whip_log_level;
extern gboolean whip_log_timestamps;
extern gboolean whip_log_colors;

/* Log colors */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* Log levels */
#define LOG_NONE     (0)
#define LOG_FATAL    (1)
#define LOG_ERR      (2)
#define LOG_WARN     (3)
#define LOG_INFO     (4)
#define LOG_VERB     (5)
#define LOG_HUGE     (6)
#define LOG_DBG      (7)
#define LOG_MAX LOG_DBG

/* Coloured prefixes for errors and warnings logging. */
static const char *whip_log_prefix[] = {
/* no colors */
	"",
	"[FATAL] ",
	"[ERR] ",
	"[WARN] ",
	"",
	"",
	"",
	"",
/* with colors */
	"",
	ANSI_COLOR_MAGENTA"[FATAL]"ANSI_COLOR_RESET" ",
	ANSI_COLOR_RED"[ERR]"ANSI_COLOR_RESET" ",
	ANSI_COLOR_YELLOW"[WARN]"ANSI_COLOR_RESET" ",
	"",
	"",
	"",
	""
};
static const char *whip_name_prefix[] = {
/* no colors */
	"[WHIP] ",
/* with colors */
	ANSI_COLOR_CYAN"[WHIP]"ANSI_COLOR_RESET" "
};

/* Simple wrapper to g_print/printf */
#define WHIP_PRINT g_print
/* Logger based on different levels, which can either be displayed
 * or not according to the configuration of the gateway.
 * The format must be a string literal. */
#define WHIP_LOG(level, format, ...) \
do { \
	if (level > LOG_NONE && level <= LOG_MAX && level <= whip_log_level) { \
		char whip_log_ts[64] = ""; \
		char whip_log_src[128] = ""; \
		if (whip_log_timestamps) { \
			struct tm whiptmresult; \
			time_t whipltime = time(NULL); \
			localtime_r(&whipltime, &whiptmresult); \
			strftime(whip_log_ts, sizeof(whip_log_ts), \
			         "[%a %b %e %T %Y] ", &whiptmresult); \
		} \
		if (level == LOG_FATAL || level == LOG_ERR || level == LOG_DBG) { \
			snprintf(whip_log_src, sizeof(whip_log_src), \
			         "[%s:%s:%d] ", __FILE__, __FUNCTION__, __LINE__); \
		} \
		g_print("%s%s%s" format, \
		        whip_log_ts, \
		        whip_log_prefix[level | ((int)whip_log_colors << 3)], \
		        whip_log_src, \
		        ##__VA_ARGS__); \
	} \
} while (0)

/* Same as above, but with a [WHIP] prefix */
#define WHIP_PREFIX(level, format, ...) \
do { \
	if (level > LOG_NONE && level <= LOG_MAX && level <= whip_log_level) { \
		char whip_log_ts[64] = ""; \
		char whip_log_src[128] = ""; \
		if (whip_log_timestamps) { \
			struct tm whiptmresult; \
			time_t whipltime = time(NULL); \
			localtime_r(&whipltime, &whiptmresult); \
			strftime(whip_log_ts, sizeof(whip_log_ts), \
			         "[%a %b %e %T %Y] ", &whiptmresult); \
		} \
		if (level == LOG_FATAL || level == LOG_ERR || level == LOG_DBG) { \
			snprintf(whip_log_src, sizeof(whip_log_src), \
			         "[%s:%s:%d] ", __FILE__, __FUNCTION__, __LINE__); \
		} \
		g_print("%s%s%s%s" format, \
		        whip_name_prefix[whip_log_colors], \
		        whip_log_ts, \
		        whip_log_prefix[level | ((int)whip_log_colors << 3)], \
		        whip_log_src, \
		        ##__VA_ARGS__); \
	} \
} while (0)

#endif
