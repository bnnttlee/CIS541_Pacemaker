// identify yourself specifically	
const char TALKING_TO[] PROGMEM = "\rYou are talking to the ";
const char WHO_DEMO[] PROGMEM = "'Real Time Clock' demo.\r";	

// bad command
const char BAD_COMMAND1[] PROGMEM = "\rYou sent: '";
const char BAD_COMMAND2[] PROGMEM = "' - I don't understand.\r";

const char ENTER[] PROGMEM = "Enter ";
const char TEXT_GET[] PROGMEM = "'get' to get the time and date.\r";	
const char TEXT_SEC[] PROGMEM = "'secXX' to set the second";	
const char TEXT_MIN[] PROGMEM = "'minXX' to set the minute";	
const char TEXT_HOUR[] PROGMEM = "'hourXX' to set the hour";
const char TEXT_TOXX[] PROGMEM = " to XX.\r";

const char ERROR_NUMBER[] PROGMEM = "\rERROR - number must be less than ";
const char ERROR_60[] PROGMEM = " 60.\r";	
const char ERROR_12[] PROGMEM = " 12.\r";

const char THE_TIME_IS[] PROGMEM = "The time is: ";
