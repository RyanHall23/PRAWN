#pragma once

#include <atlstr.h>
#include <string>

// Translator Class
//
// Responsible for converting Virtual Key codes to alphanumeric codes
// And truncates RID Device Name to only PID & VID string
class CTranslator
{
public:
	CTranslator();		// Constructor
	~CTranslator();		// Destructor
	unsigned short VKeyToNKey(const unsigned short VKey);	// Function for translating
	CString TruncateRegistration(std::string devName);

	const unsigned short nKey_Return = 10;			// Key "Enter/Return" on the keyboard | End of string marker key
	const unsigned short nKey_NotRegistered = 99;	// Mark if unknown marked key is entered
	const unsigned short nKey_DownFlag = 1;

private:
	// Virtual keys for comparison
	const unsigned short vKey_0 = 0x30;			// Key "0" on the keyboard
	const unsigned short vKey_1 = 0x31;			// Key "1" on the keyboard
	const unsigned short vKey_2 = 0x32;			// Key "2" on the keyboard
	const unsigned short vKey_3 = 0x33;			// Key "3" on the keyboard
	const unsigned short vKey_4 = 0x34;			// Key "4" on the keyboard
	const unsigned short vKey_5 = 0x35;			// Key "5" on the keyboard
	const unsigned short vKey_6 = 0x36;			// Key "6" on the keyboard
	const unsigned short vKey_7 = 0x37;			// Key "7" on the keyboard
	const unsigned short vKey_8 = 0x38;			// Key "8" on the keyboard
	const unsigned short vKey_9 = 0x39;			// Key "9" on the keyboard
	const unsigned short vKey_Return = 0x0D;	// Key "Enter/Return" on the keyboard | End of string marker key


	// Numeric Keys for returning
	const unsigned short nKey_0 = 0;			// Key "0" on the keyboard
	const unsigned short nKey_1 = 1;			// Key "1" on the keyboard
	const unsigned short nKey_2 = 2;			// Key "2" on the keyboard
	const unsigned short nKey_3 = 3;			// Key "3" on the keyboard
	const unsigned short nKey_4 = 4;			// Key "4" on the keyboard
	const unsigned short nKey_5 = 5;			// Key "5" on the keyboard
	const unsigned short nKey_6 = 6;			// Key "6" on the keyboard
	const unsigned short nKey_7 = 7;			// Key "7" on the keyboard
	const unsigned short nKey_8 = 8;			// Key "8" on the keyboard
	const unsigned short nKey_9 = 9;			// Key "9" on the keyboard

};