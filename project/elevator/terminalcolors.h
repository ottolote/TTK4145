// DEFINING ESCAPE CHARACTERS FOR ANSI COLORS
// JOINED BY COMPILER, NOT PREPROCESSOR


// NO COLOR
#define TCOLOR_NC           "\033[0m"

// COLORS
#define TCOLOR_BLACK        "\033[0;30m"     
#define TCOLOR_RED          "\033[0;31m"     
#define TCOLOR_GREEN        "\033[0;32m"     
#define TCOLOR_ORANGE       "\033[0;33m"     
#define TCOLOR_BLUE         "\033[0;34m"     
#define TCOLOR_PURPLE       "\033[0;35m"     
#define TCOLOR_CYEN         "\033[0;36m"     
#define TCOLOR_LIGHTGRAY    "\033[0;37m"     
#define TCOLOR_DARKGRAY     "\033[1;30m"
#define TCOLOR_LIGHTRED     "\033[1;31m"
#define TCOLOR_LIGHTGREEN   "\033[1;32m"
#define TCOLOR_YELLOW       "\033[1;33m"
#define TCOLOR_LIGHTBLUE    "\033[1;34m"
#define TCOLOR_LIGHTPURPLE  "\033[1;35m"
#define TCOLOR_LIGHTYAN     "\033[1;36m"
#define TCOLOR_WHITE        "\033[1;37m"

#define ERROR "[" TCOLOR_RED "ERROR" TCOLOR_NC "] : "
