/*
 *  arch/arm/mach-rtk119x/include/mach/memory.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

#define RTK_FLAG_NONCACHED      (1U << 0)
#define RTK_FLAG_SCPUACC        (1U << 1)
#define RTK_FLAG_ACPUACC        (1U << 2)
#define RTK_FLAG_HWIPACC        (1U << 3)
#define RTK_FLAG_VE_SPEC        (1U << 4)
#define RTK_FLAG_SECURE_AUDIO   (1U << 5)
#define RTK_FLAG_DEAULT         (/*RTK_FLAG_NONCACHED | */RTK_FLAG_SCPUACC | RTK_FLAG_ACPUACC | RTK_FLAG_HWIPACC)

#if 0
#define PLAT_FRAME_BUFFER_SIZE_720P         (0x00C00000) // 12MB
#define PLAT_FRAME_BUFFER_SIZE_1080P        (0x01800000) // 24MB
#define PLAT_FRAME_BUFFER_SIZE	PLAT_FRAME_BUFFER_SIZE_720P
#define VE_ION_SIZE			    (1024*1024*169)
#define ION_AUDIO_HEAP_SIZE		(1024*1024*1)
#define ION_MEDIA_HEAP_SIZE		(1024*1024*39 + VE_ION_SIZE + PLAT_FRAME_BUFFER_SIZE)
#endif


#define PLAT_PHYS_OFFSET        (0x00000000)
#define PLAT_MEM_SIZE           (512*1024*1024)

/* 0x00000000 ~ 0x0001efff */ // (X) ALL
#define SYS_BOOTCODE_MEMBASE    (PLAT_PHYS_OFFSET)
#define SYS_BOOTCODE_MEMSIZE    (0x00030000)
/* 0x0001f000 ~ 0x0001ffff */
#define RPC_COMM_PHYS           (0x0001F000)
#define RPC_COMM_SIZE           (0x00001000)
/* 0x00030000 ~ 0x000fffff */
#define RESERVED_832KB_PHYS     (0x00030000)
#define RESERVED_832KB_SIZE     (0x000D0000)
/* 0x01ffe000 ~ 0x02001fff */
#define RPC_RINGBUF_PHYS        (0x01ffe000)
#define RPC_RINGBUF_SIZE        (0x00004000)
/* 0x02200000 ~ 0x025fffff */
#ifdef CONFIG_RTK_VMX
#define ROOTFS_NORMAL_START     (0x3F000000)
#define ROOTFS_NORMAL_SIZE      (0x12C00000) //300MB
#else
#define ROOTFS_NORMAL_START     (0x02200000)
#define ROOTFS_NORMAL_SIZE      (0x00400000) //4MB
#endif
#define ROOTFS_NORMAL_END       (ROOTFS_NORMAL_START + ROOTFS_NORMAL_SIZE)
/* 0x02200000 ~ 0x02dfffff */
#define ROOTFS_RESCUE_START     (0x02200000)
#define ROOTFS_RESCUE_SIZE      (0x00C00000) //12MB
#ifdef CONFIG_RTK_VMX
#define ROOTFS_RESCUE_END       (ROOTFS_RESCUE_START + ROOTFS_RESCUE_SIZE)
#else
#define ROOTFS_RESCUE_END       (ROOTFS_NORMAL_START + ROOTFS_RESCUE_SIZE)
#endif
/* 0x02600000 ~ 0x02bfffff */
#define MEM_SLOT_PHYS_1         (0x02600000)
#define MEM_SLOT_SIZE_1         (0x00c00000) // Max : 12M
#define MEM_SLOT_FLAG_1         (RTK_FLAG_SCPUACC | RTK_FLAG_ACPUACC | RTK_FLAG_HWIPACC)
/* 0x03200000 ~ 0x0f8fffff */
#define MEM_SLOT_PHYS_0         (0x03200000)
#define MEM_SLOT_SIZE_0         (0x0c700000) // Max : 199M
#define MEM_SLOT_FLAG_0         (RTK_FLAG_SCPUACC | RTK_FLAG_ACPUACC | RTK_FLAG_HWIPACC)
/* 0x0f900000 ~ 0x0fdfffff */
#define ACPU_FIREWARE_PHYS      (0x0f900000)
#define ACPU_FIREWARE_SIZE      (0x00500000)
/* 0x10000000 ~ 0x10013fff */ // (X) ALL
#define ACPU_IDMEM_PHYS         (0x10000000)
#define ACPU_IDMEM_SIZE         (0x00014000)
/* Kernel will resrved memory for TEE OS if kernel config is for DRM */
#define TEE_OS_PHYS     		(0x10100000)
#define TEE_OS_SIZE     		(0x04100000)
/* 0x14200000 ~ 0x1e7fffff */
#define MEM_SLOT_PHYS_2 		(0x14200000)
#define MEM_SLOT_SIZE_2			(0x0a600000) // Max : 166M
#define MEM_SLOT_FLAG_2 		(RTK_FLAG_SCPUACC | RTK_FLAG_ACPUACC | \
									RTK_FLAG_HWIPACC)
/* 0x1e800000 ~ 0x1effffff */
#define MEM_SLOT_PHYS_3 		(0x1e800000)
#define MEM_SLOT_SIZE_3	 		(0x00800000) // Max : 8M
#define MEM_SLOT_FLAG_3 		(RTK_FLAG_SCPUACC | RTK_FLAG_ACPUACC | \
									RTK_FLAG_HWIPACC)
/* 0x32800000 ~ 0x3effffff */
#define MEM_SLOT_PHYS_4 		(0x32b00000)
#define MEM_SLOT_SIZE_4 		(0x12c00000)
#define MEM_SLOT_FLAG_4 (RTK_FLAG_SCPUACC | RTK_FLAG_HWIPACC)
#define MEM_SLOT_FLAG_4         (RTK_FLAG_SCPUACC | RTK_FLAG_HWIPACC)
/* 0x1fc00000 ~ 0x1fc00fff */ // (X) ALL
#define ACPU_BOOTCODE_PHYS      (0x1FC00000)
#define ACPU_BOOTCODE_SIZE      (0x00001000)
/* 0x32800000 ~ 0x3effffff */
#define MEM_SLOT_SIZE_4         (0x12c00000)
#define MEM_SLOT_FLAG_4         (RTK_FLAG_SCPUACC | RTK_FLAG_HWIPACC)

/* 0x80000000 ~ 0x80007fff */
#define PLAT_SECURE_PHYS        (0x80000000)
#define PLAT_SECURE_SIZE        (0x00008000)
/* 0x88100000 ~ 0x88107fff */
#define PLAT_NOR_PHYS           (0x88100000)
#define PLAT_NOR_SIZE           (0x00008000)
/* 0x98000000 ~ 0x981fffff */
#define RBUS_BASE_PHYS          (0x98000000)
#define RBUS_BASE_SIZE          (0x00200000)

#define RBUS_BASE_VIRT          (0xFE000000)
//#define RPC_COMM_VIRT           (RBUS_BASE_VIRT+RBUS_BASE_SIZE)
//#define RPC_RINGBUF_VIRT        (0xFC7F8000+0x00004000)

#define ROOTFS_BIST_START     (0x30000000)
#define ROOTFS_BIST_SIZE      (0x00C00000) //12MB
#define ROOTFS_BIST_END       (ROOTFS_BIST_START + ROOTFS_BIST_SIZE)

#define HW_LIMITATION_PHYS      (0x3FFFF000)
#define HW_LIMITATION_SIZE      (0x00001000) //4KB
#define HW_LIMITATION_START     (HW_LIMITATION_PHYS)
#define HW_LIMITATION_END       (HW_LIMITATION_START + HW_LIMITATION_SIZE)

#define HW_LIMITATION_3GB_PHYS  (0x7FFFF000) //for 3.0-GB case
#define HW_LIMITATION_3GB_SIZE  (0x00001000) //4KB
#define HW_LIMITATION_3GB_START (HW_LIMITATION_3GB_PHYS)
#define HW_LIMITATION_3GB_END   (HW_LIMITATION_3GB_START + HW_LIMITATION_3GB_SIZE)

#define HW_SECURE_RAM_PHYS      (0x80000000)
#define HW_SECURE_RAM_SIZE      (0x00008000) //32KB
#define HW_SECURE_RAM_START     (HW_SECURE_RAM_PHYS)
#define HW_SECURE_RAM_END       (HW_SECURE_RAM_START + HW_SECURE_RAM_SIZE)

#define HW_NOR_REMAP_PHYS       (0x88100000)
#define HW_NOR_REMAP_SIZE       (0x02000000) //32MB
#define HW_NOR_REMAP_START      (HW_NOR_REMAP_PHYS)
#define HW_NOR_REMAP_END        (HW_NOR_REMAP_START + HW_NOR_REMAP_SIZE)

#define HW_RBUS_PHYS            (0x98000000)
#define HW_RBUS_SIZE            (0x00200000) //2MB
#define HW_RBUS_START           (HW_RBUS_PHYS)
#define HW_RBUS_END             (HW_RBUS_START + HW_RBUS_SIZE)

#define HW_PCI1_MMAP_PHYS       (0xC0000000)
#define HW_PCI1_MMAP_SIZE       (0x01000000) //16MB
#define HW_PCI1_MMAP_START      (HW_PCI1_MMAP_PHYS)
#define HW_PCI1_MMAP_END        (HW_PCI1_MMAP_START + HW_PCI1_MMAP_SIZE)

#define HW_PCI2_MMAP_PHYS       (0xC1000000)
#define HW_PCI2_MMAP_SIZE       (0x01000000) //16MB
#define HW_PCI2_MMAP_START      (HW_PCI2_MMAP_PHYS)
#define HW_PCI2_MMAP_END        (HW_PCI2_MMAP_START + HW_PCI2_MMAP_SIZE)

#define HW_JTAG_GIC_RSV_PHYS    (0xFF000000)
#define HW_JTAG_GIC_RSV_SIZE    (0x00800000) //8MB
#define HW_JTAG_GIC_RSV_START   (HW_JTAG_GIC_RSV_PHYS)
#define HW_JTAG_GIC_RSV_END     (HW_JTAG_GIC_RSV_START + HW_JTAG_GIC_RSV_SIZE)

#define MEM_SLOT_PHYS_BY(_nr, req_size)    (MEM_SLOT_PHYS_##_nr##)
#define MEM_SLOT_SIZE_BY(_nr, req_size)    ( \
        (req_size  > MEM_SLOT_SIZE_##_nr##) ? MEM_SLOT_SIZE_##_nr## : \
        req_size)
#define MEM_SLOT_FLAG_BY(_nr, req_size)    (MEM_SLOT_FLAG_##_nr##)

#define MEM_SLOT(_nr, _type, req_size) \
    MEM_SLOT_##_type##_BY(_nr, req_size)


/* legacy : 184 M */
#define ION_MEDIA_HEAP_PHYS1    (MEM_SLOT(0, PHYS, 0x0b800000))
#define ION_MEDIA_HEAP_SIZE1    (MEM_SLOT(0, SIZE, 0x0b800000))
#define ION_MEDIA_HEAP_FLAG1    (MEM_SLOT(0, FLAG, 0x0b800000))

/* legacy : 12 M */
#define ION_AUDIO_HEAP_PHYS     (MEM_SLOT(1, PHYS, 0x00c00000))
#define ION_AUDIO_HEAP_SIZE     (MEM_SLOT(1, SIZE, 0x00c00000))
#define ION_AUDIO_HEAP_FLAG     (MEM_SLOT(1, FLAG, 0x00c00000))

/* legacy : 146 M */
#define ION_MEDIA_HEAP_PHYS2    (MEM_SLOT(2, PHYS, 0x09200000))
#define ION_MEDIA_HEAP_SIZE2    (MEM_SLOT(2, SIZE, 0x09200000))
#define ION_MEDIA_HEAP_FLAG2    (MEM_SLOT(2, FLAG, 0x09200000))

/* legacy : 8 M */
#define ION_MEDIA_HEAP_PHYS3    (MEM_SLOT(3, PHYS, 0x00800000))
#define ION_MEDIA_HEAP_SIZE3    (MEM_SLOT(3, SIZE, 0x00800000))
#define ION_MEDIA_HEAP_FLAG3    (MEM_SLOT(3, FLAG, 0x00800000) | RTK_FLAG_VE_SPEC)

/* legacy : 200 M */
#define ION_SECURE_HEAP_PHYS    (MEM_SLOT(4, PHYS, 0x0c800000))
#define ION_SECURE_HEAP_SIZE    (MEM_SLOT(4, SIZE, 0x0c800000))
#define ION_SECURE_HEAP_FLAG    (/*MEM_SLOT(4, FLAG, 0x0c800000)*/RTK_FLAG_HWIPACC)
#endif
