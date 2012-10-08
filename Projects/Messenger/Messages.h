// Messages.h

// identify yourself specifically	
const char TALKING_TO[] PROGMEM = "\r\rYou are talking to the \0";
const char WHO_DEMO[] PROGMEM = "'Messenger' demo.\r\r\0";	

// bad command
const char BAD_COMMAND1[] PROGMEM = "\rYou sent: '\0";
const char BAD_COMMAND2[] PROGMEM = "' - I don't understand.\r\0";

const char ENTER[] PROGMEM = "Enter a ";	
const char FOR[] PROGMEM = " for error message: ";

const char ERROR_YOUFOOBARED[] PROGMEM = "You fouled up beyond repair.\r\0";
const char ERROR_SNAFUED[] PROGMEM = "Situation normal, all fouled up.\r\0";
const char ERROR_STOPTHEMADNESS[] PROGMEM = "Stop the madness!\r\0";
const char ERROR_WHERE[] PROGMEM = "Where did you learn to program?\r\0";
const char ERROR_RTFM[] PROGMEM = "Read the freaking manual!\r\0";

const char *ERROR_TBL[]    = { ERROR_YOUFOOBARED, ERROR_SNAFUED, ERROR_STOPTHEMADNESS, ERROR_WHERE, ERROR_RTFM };
