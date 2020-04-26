
Off-line generator
----------------------------
step 1: make a install.img as usual
step 2: if you got new flash_nand_list.h or wanna change another nandflash, goto step 3, otherwise goto step 4
step 3: run the script as below to convert related options for Rconfig && setting.txt (Exist one could be re-used)
        ./prepare_for_offline_gen (in the menu, please choose your nandflash &
save the configuration file as setting.txt
step 4: ./install_a.pc ../../install.img (_path_of_install.img_)
step 5: additional files : fw_tbl.bin , layout.txt would be packed in
install.img especial for mp tool

