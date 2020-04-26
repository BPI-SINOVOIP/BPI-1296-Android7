/*
 * Copyright (c) 2015 Travis Geiselbrecht
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

/* up to 30 GB of ram */
#define MEMORY_BASE_PHYS     (MEMBASE)
#define MEMORY_BASE_VIRT     (MEMBASE)
#define MEMORY_SIZE (MEMSIZE) //1G

/* map all of 0-1GB into kernel space in one shot */
#define PERIPHERAL_BASE_PHYS (0x98000000UL)
#define PERIPHERAL_BASE_SIZE (0x00200000UL)
#define PERIPHERAL_BASE_VIRT (PERIPHERAL_BASE_PHYS)

/* map nor flash 64MByte */
#define NOR_FLASH_BASE_PHYS (0x88100000UL)
#define NOR_FLASH_BASE_SIZE (0x04000000UL)
#define NOR_FLASH_BASE_VIRT (NOR_FLASH_BASE_PHYS)

/* individual peripherals in this mapping */
#define CPUPRIV_BASE_PHYS   (0xff000000UL)
#define CPUPRIV_BASE_VIRT   (CPUPRIV_BASE_PHYS)
#define CPUPRIV_SIZE        (0x00020000)

/* map vmx fw table addr */
#define VMX_FW_TABLE_BASE_PHYS	(0x54000000UL)
#define VMX_FW_TABLE_BASE_VIRT	(VMX_FW_TABLE_BASE_PHYS)
#define VMX_FW_TABLE_SIZE	(0x01000000)

/* interrupts */
#define ARM_GENERIC_TIMER_VIRTUAL_INT 27
#define ARM_GENERIC_TIMER_PHYSICAL_INT 30

#define MAX_INT 576 // Get from GICD_TYPER[4:0]

/* uart */
#define UART1_BASE		(MEMORY_BASE_VIRT + 0x98007800)
#define UART1_CLOCK_FREQ	27000000
