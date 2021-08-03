#include "ownport.h"

#define iCD		PC.PIN.B0
#define iAUX	PC.PIN.B1
#define iTUNER	PC.PIN.B2
#define iSpare	PC.PIN.B4
#define iSPKAB	PC.PIN.B5
#define iTUNP	PC.PIN.B6
#define iTUNM	PC.PIN.B7


#define qCD		relOut.BIT.B0 //bity w pamieci
#define qAUX	relOut.BIT.B1
#define qTUNER	relOut.BIT.B2
#define qSpare	relOut.BIT.B3
#define qSpare1	relOut.BIT.B4
#define qSpare2	relOut.BIT.B5
#define qSPKA	relOut.BIT.B6
#define qSPKB	relOut.BIT.B7