#include <common.h>
#include <command.h>
#include <fs.h>
#include <usb.h>
#include <stdlib.h>

#define UBOOT_FILE_PATH     "IMAGE_UPGRADE/flash.bin"
#define KERNEL_FILE_PATH    "IMAGE_UPGRADE/Image"
#define DTB_FILE_PATH       "IMAGE_UPGRADE/imx8mp-evk.dtb"
#define ROOTFS_FILE_PATH    "IMAGE_UPGRADE/rootfs.ext2"

// 函数：将十进制数转换为十六进制字符串
void decimalToHex(int decimal, char *hexString) 
{
    int i = 0;
    int remainder;
    char hexDigits[] = "0123456789ABCDEF";

    // 处理特殊情况
    if (decimal == 0) {
        hexString[i++] = '0';
        hexString[i] = '\0';
        return;
    }

    // 转换过程
    while (decimal != 0) {
        remainder = decimal % 16;
        hexString[i++] = hexDigits[remainder];
        decimal /= 16;
    }

    // 反转字符串
    hexString[i] = '\0';
    for (int j = 0; j < i / 2; j++) {
        char temp = hexString[j];
        hexString[j] = hexString[i - j - 1];
        hexString[i - j - 1] = temp;
    }
}

#if 1
int do_image_upgrade(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{

    // 启动USB子系统
    char _cmd[32];
    sprintf(_cmd, "usb start");
	run_command(_cmd, 0);

    // 设置U盘为当前设备和文件系统，根据实际情况修改设备号和分区号
    if (fs_set_blk_dev("usb", "0:1", FS_TYPE_ANY)) {
        printf("Error setting block device.\n");
        return 1;
    }

    //判断升级文件是否存在

    if(fs_exists(KERNEL_FILE_PATH))
    {
        printf("kernel upgrage file exit\n");
    }
    else
    {
        printf("can not find kernel upgrage file\n");
        //return 1;
    }

    //每次访问之前都要设置一下这个，不然无法识别对应的文件，至于为什么，需要看一下
    fs_set_blk_dev("usb", "0:1", FS_TYPE_ANY);

    if(fs_exists(UBOOT_FILE_PATH))
    {
        printf("uboot upgrage file exit\n");
    }
    else
    {
        printf("can not find uboot upgrage file\n");
        //return 1;
    }

    
    fs_set_blk_dev("usb", "0:1", FS_TYPE_ANY);
    if(fs_exists(DTB_FILE_PATH))
    {
        printf("dtb upgrage file exit\n");
    }
    else
    {
        printf("can not find dtb upgrage file\n");
        //return 1;
    }

    fs_set_blk_dev("usb", "0:1", FS_TYPE_ANY);
    if(fs_exists(ROOTFS_FILE_PATH))
    {
        printf("rootfs upgrage file exit\n");
    }
    else
    {
        printf("can not find rootfs upgrage file\n");
        //return 1;
    }

    loff_t uboot_size   = 0;
    loff_t kernel_size  = 0;
    loff_t dtb_size     = 0;
    loff_t rootfs_size  = 0;
    fs_set_blk_dev("usb", "0:1", FS_TYPE_ANY);
    if (fs_size(UBOOT_FILE_PATH, &uboot_size) < 0) {
        printf("Error getting size of %s\n", UBOOT_FILE_PATH);
        return 1;
    }
    else
    {
        printf("uboot size :%lld\n",uboot_size);
    }

    fs_set_blk_dev("usb", "0:1", FS_TYPE_ANY);
    if (fs_size(KERNEL_FILE_PATH, &kernel_size) < 0) {
        printf("Error getting size of %s\n", KERNEL_FILE_PATH);
        return 1;
    }
    else
    {
        printf("kernel size :%lld\n",kernel_size);
    }

    fs_set_blk_dev("usb", "0:1", FS_TYPE_ANY);
    if (fs_size(DTB_FILE_PATH, &dtb_size) < 0) {
        printf("Error getting size of %s\n", DTB_FILE_PATH);
        return 1;
    }
    else
    {
        printf("dtb  size :%lld\n",dtb_size);
    }

    fs_set_blk_dev("usb", "0:1", FS_TYPE_ANY);
    if (fs_size(ROOTFS_FILE_PATH, &rootfs_size) < 0) {
        printf("Error getting size of %s\n", ROOTFS_FILE_PATH);
        return 1;
    }
    else
    {
        printf("rootfs size :%lld\n",rootfs_size);
    }

    //将各个文件大小转换成block的大小，再转换成16进制数
    int uboot_size_block    = uboot_size/512 + 1;
    //int kernel_size_block   = kernel_size/512 + 1;
    //int dtb_size_block      = dtb_size/512 + 1;
    int rootfs_size_block   = rootfs_size/512 + 1;

    char hex_uboot[20];             // 足够大的缓冲区来存储十六进制字符串
    char hex_kernel[20];            // 足够大的缓冲区来存储十六进制字符串
    char hex_dtb[20];               // 足够大的缓冲区来存储十六进制字符串
    char hex_rootfs[20];            // 足够大的缓冲区来存储十六进制字符串
    decimalToHex(uboot_size_block, hex_uboot);
    decimalToHex(kernel_size, hex_kernel);
    decimalToHex(dtb_size, hex_dtb);
    decimalToHex(rootfs_size_block, hex_rootfs);

    printf("hex_uboot: %s\n", hex_uboot);
    printf("hex_kernel: %s\n", hex_kernel);
    printf("hex_dtb: %s\n", hex_dtb);
    printf("hex_rootfs: %s\n", hex_rootfs);
    
    
    char uboot_upgrade_cmd[128];
    sprintf(uboot_upgrade_cmd, "fatload usb 0 40400000 IMAGE_UPGRADE/flash.bin");
	run_command(uboot_upgrade_cmd, 0);
    memset(uboot_upgrade_cmd,128,0);
    sprintf(uboot_upgrade_cmd, "mmc dev 1 0"); //后续如果是要升级到EMMC，需要更改一下这个位置
    run_command(uboot_upgrade_cmd, 0);
    memset(uboot_upgrade_cmd,128,0);
    sprintf(uboot_upgrade_cmd, "mmc write 40400000 40 %s",hex_uboot);
    printf("uboot_upgrade_cmd: %s\n", uboot_upgrade_cmd);
    run_command(uboot_upgrade_cmd, 0);


    char kernel_upgrade_cmd[128];
    //int kernel_fload_addr = 0x40400000+uboot_size+4096;   //多给4KB的偏差
    int kernel_fload_addr = 0x40400000;  
    char kernel_offset[16];
    decimalToHex(kernel_fload_addr, kernel_offset);
    printf("kernel fatload offset: %s\n", kernel_offset);
    sprintf(kernel_upgrade_cmd, "fatload usb 0 %s IMAGE_UPGRADE/Image",kernel_offset);
    run_command(kernel_upgrade_cmd, 0);
    memset(kernel_upgrade_cmd,128,0);
    sprintf(kernel_upgrade_cmd, "fatwrite mmc 1:1  %s Image %s",kernel_offset,hex_kernel);
    printf("kernel_upgrade_cmd: %s\n", kernel_upgrade_cmd);
    run_command(kernel_upgrade_cmd, 0);

    char dtb_upgrade_cmd[128];
    //int dtb_fload_addr = 0x40400000+uboot_size+4096+kernel_size+4096;   //多给4KB的偏差
    int dtb_fload_addr = 0x40400000;   
    char dtb_offset[16];
    decimalToHex(dtb_fload_addr, dtb_offset);
    printf("dtb fatload offset: %s\n", dtb_offset);
    sprintf(dtb_upgrade_cmd, "fatload usb 0 %s IMAGE_UPGRADE/imx8mp-evk.dtb",dtb_offset);
    run_command(dtb_upgrade_cmd, 0);
    memset(dtb_upgrade_cmd,128,0);
    sprintf(dtb_upgrade_cmd, "fatwrite mmc 1:1  %s imx8mp-evk.dtb %s",dtb_offset,hex_dtb);
    printf("dtb_upgrade_cmd: %s\n", dtb_upgrade_cmd);
    run_command(dtb_upgrade_cmd, 0);

    char rootfs_upgrade_cmd[128];
    //int rootfs_fload_addr = 0x40400000+uboot_size+4096+kernel_size+4096+dtb_size+4096;   //多给4KB的偏差
    int rootfs_fload_addr = 0x40400000; 
    char rootfs_offset[16];
    decimalToHex(rootfs_fload_addr, rootfs_offset);
    printf("rootfs fatload offset: %s\n", rootfs_offset);
    sprintf(rootfs_upgrade_cmd, "fatload usb 0 %s IMAGE_UPGRADE/rootfs.ext2",rootfs_offset);
    run_command(rootfs_upgrade_cmd, 0);
    memset(rootfs_upgrade_cmd,128,0);
    sprintf(rootfs_upgrade_cmd, " mmc dev 1 2");            //如果是EMMC需要改
    run_command(rootfs_upgrade_cmd, 0);
    memset(rootfs_upgrade_cmd,128,0);
    sprintf(rootfs_upgrade_cmd, " mmc write %s 24000 %s",rootfs_offset,hex_rootfs);  //24000与分区表有关，实际看自己的分区表多大
    run_command(rootfs_upgrade_cmd, 0);
    usb_stop();
    return 0;
}
#endif

#if 1
U_BOOT_CMD(
    image_upgrade,    // command name
    1,                  // maxargs
    1,                  // repeatable
    do_image_upgrade, // command function
    "auto upgrage image if find usb and upgrade file",  // description
    "image_upgrage"  // usage
);
#endif

