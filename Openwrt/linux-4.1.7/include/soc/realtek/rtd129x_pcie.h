
void rtk_pcie1_write(u32 addr, u8 size, u32 wval);
u32 rtk_pcie1_read(u32 addr, u8 size);
void rtk_pcie2_write(u32 addr, u8 size, u32 wval);
u32 rtk_pcie2_read(u32 addr, u8 size);
u32 rtk_pcie_mmio_start(void);
u32 rtk_pcie_mmio_end(void);
u32 rtk_pcie_mmio_len(void);
u32 rtk_pcie2_mmio_start(void);
u32 rtk_pcie2_mmio_end(void);
u32 rtk_pcie2_mmio_len(void);
