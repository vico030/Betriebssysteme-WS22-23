#include "dbgu.h"

#define DBGU 0xFFFFF200

#define DBGU_CR 0x0000 // Control Register Write-only -
#define DBGU_MR 0x0004 // Mode Register Read/Write 0x0
#define DBGU_IER 0x0008 // Interrupt Enable Register Write-only –
#define DBGU_IDR 0x000C // Interrupt Disable Register Write-only –
#define DBGU_IMR 0x0010 // Interrupt Mask Register Read-only 0x0
#define DBGU_SR 0x0014 // Status Register Read-only –
#define DBGU_RHR 0x0018 // Receive Holding Register Read-only 0x0
#define DBGU_THR 0x001C // Transmit Holding Register Write-only –
#define DBGU_BRGR 0x0020 // Baud Rate Generator Register Read/Write 0x0