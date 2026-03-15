#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdint.h>

#define DEV "/dev/disp"
#define DISP_LAYER_SET_CONFIG2 0x49

int main(int argc, char **argv) {
    int fd;
    uint8_t buf[208];   // exact 208-byte buffer
    int i;

    // optional: clear buffer
    memset(buf, 0, sizeof(buf));

    // Example: set first byte to brightness value from arg1
    if (argc > 1) {
        int val = atoi(argv[1]);
        if (val < 0) val = 0;
        if (val > 255) val = 255;
        buf[0] = (uint8_t)val;
    }

    fd = open(DEV, O_RDWR);
    if (fd < 0) {
        perror("open /dev/disp");
        return 1;
    }

    if (ioctl(fd, DISP_LAYER_SET_CONFIG2, buf) < 0) {
        perror("ioctl DISP_LAYER_SET_CONFIG2");
        close(fd);
        return 1;
    }

    close(fd);

    printf("ioctl success, buffer first 16 bytes:\n");
    for (i = 0; i < 16; i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");
    return 0;
}