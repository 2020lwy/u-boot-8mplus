#include <common.h>
#include <command.h>
#include <fs.h>
#include <usb.h>
#include <stdlib.h>

#define FILE_PATH "uboot_file"
#define BUFFER_SIZE 512
#if 1
int do_read_uboot_file(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
    #if 0
    printf("%x\n",cmdtp);
    printf("%s\n",argv);
    printf("%d\n",flag);
    printf("%d\n",argc);
    printf("test here -_-.\n");
    
    char buffer[BUFFER_SIZE + 1];
    loff_t file_size, actread;
    int ret;
    // 启动USB子系统
    //usb_stop();
   // usb_start(); //没有这个 
    // 设置U盘为当前设备和文件系统，根据实际情况修改设备号和分区号
    if (fs_set_blk_dev("usb", "0:4", FS_TYPE_ANY)) {
        printf("Error setting block device.\n");
        return 1;
    }

    // 获取文件大小
    if (fs_size(FILE_PATH, &file_size) < 0) {
        printf("Error getting size of %s\n", FILE_PATH);
        return 1;
    }

    // 确保不尝试读取超出文件大小的内容
    if (file_size > BUFFER_SIZE) {
        printf("File is larger than buffer. Adjust BUFFER_SIZE.\n");
        return 1;
    }


    // 读取文件，选择一个合适的内存地址，避免与其他内存区域冲突
    actread = 0;
    //ret = fs_read(FILE_PATH, (ulong)0x60000000, 0, 0, &actread); // 修改这一行
   // ret = fs_read(FILE_PATH, (ulong)buffer, 0, file_size, &actread);
   //ret = fs_read(FILE_PATH, (ulong)0x60000000, 0, file_size, &actread);
    ret = fs_read(FILE_PATH, (ulong)0x7ef000, 0, file_size, &actread);
    if (ret) {
        printf("fs_read returned error code: %d\n", ret);
    }
    if (actread != file_size) {
        printf("Expected to read %lld bytes but read %lld bytes\n", file_size, actread);
    }
    if (ret || actread != file_size) {
        printf("Error reading %s\n", FILE_PATH);
        return 1;
    }

    // 确保字符串以null结尾
    buffer[actread] = '\0';

    // 解析文件内容
    char *ptr_x = strstr(buffer, "lcd_x=");
    char *ptr_y = strstr(buffer, "lcd_y=");
    if (ptr_x && ptr_y) {
        int lcd_x = simple_strtoul(ptr_x + 6, NULL, 10);
        int lcd_y = simple_strtoul(ptr_y + 6, NULL, 10);
        printf("lcd_x = %d, lcd_y = %d\n", lcd_x, lcd_y);
    } else {
        printf("Failed to parse the file\n");
    }
    #endif
    return 0;
}
#endif

#if 0
U_BOOT_CMD(
    read_uboot_file,    // command name
    1,                  // maxargs
    1,                  // repeatable
    do_read_uboot_file, // command function
    "Read and parse the uboot_file from USB",  // description
    "This command reads the uboot_file from USB and parses lcd_x and lcd_y values"  // usage
);
#endif

