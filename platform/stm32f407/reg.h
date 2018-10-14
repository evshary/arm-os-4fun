#ifndef __REG_H_
#define __REG_H_

#include <cmsis.h>
/* I don't know why CMSIS doesn't define this, so I define it by myself. */
#define SCB_ICSR_PENDSVSET                  ((uint32_t)0x10000000)             /*!< Set pending pendSV bit */

#endif
