#include "Translator.h"


CTranslator::CTranslator()
{

}

CTranslator::~CTranslator()
{

}

// Recieves a decimal value which is compared against the hex codes for the keys and returns their numeric values
unsigned short CTranslator::VKeyToNKey(const unsigned short VKey)
{
	if (VKey == vKey_0)			// KEY CODE FOR 0
		return nKey_0;

	else if (VKey == vKey_1)	// KEY CODE FOR 1
		return nKey_1;

	else if (VKey == vKey_2)	// KEY CODE FOR 2
		return nKey_2;

	else if (VKey == vKey_3)	// KEY CODE FOR 2
		return nKey_3;

	else if (VKey == vKey_4)	// KEY CODE FOR 3
		return nKey_4;

	else if (VKey == vKey_5)	// KEY CODE FOR 4
		return nKey_5;

	else if (VKey == vKey_6)	// KEY CODE FOR 6
		return nKey_6;

	else if (VKey == vKey_7)	// KEY CODE FOR 7
		return nKey_7;

	else if (VKey == vKey_8)	// KEY CODE FOR 8
		return nKey_8;

	else if (VKey == vKey_9)	// KEY CODE FOR 9
		return nKey_9;

	else if (VKey == vKey_Return)	// KEY CODE FOR RETURN
		return nKey_Return;

	else
		return nKey_NotRegistered;	// KEY CODE FOR UNREGISTERED KEYS
}

CString CTranslator::TruncateRegistration(std::string devName)
{
	if (devName.size() < 19)
	{
		return "Device Name Error \n";
	}

	int iStart = 7;		// Start point of truncating string
	int iEnd = 18;		// End point of truncating string

	std::string strTruncated = devName.substr(iStart, iEnd);	// Truncates device name to following params

	return strTruncated.c_str();	// Return truncated devicename
}