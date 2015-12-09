#ifndef _TERMCAPS_H_
# define _TERMCAPS_H_

/**
 * Text styles definitions.
 */
#define BOLD(s)		"\e[1m" s "\e[21m"
#define UNDERLINED(s)	"\e[4m" s "\e[24m"

/**
 * Color helpers.
 */
#define COLOR_BEGIN(color, s)	color s
#define COLOR_END		"\e[0m"

/**
 * Color definitions.
 */
#define RED		"\e[31m"
#define GREEN		"\e[32m"
#define YELLOW		"\e[33m"
#define BLUE		"\e[34m"
#define MAGENTA		"\e[35m"
#define CYAN		"\e[36m"
#define LIGHT_BLUE	"\e[94m"
#define LIGHT_MAGENTA	"\e[95m"
#define LIGHT_CYAN	"\e[96m"

#endif
