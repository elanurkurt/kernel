#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

struct ioctl_params {
    int param1;
    int param2;
};

#define IOCTL_SET_PARAMS _IOW('a', 'b', struct ioctl_params*)

int main() {
    int fd;
    struct ioctl_params params;

    // Parametreleri ayarla
    params.param1 = 111;
    params.param2 = 222;

    // Cihaz dosyasını aç
    fd = open("/dev/kernel_module", O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    while (1) {
        // IOCTL ile parametreleri ayarla
        if (ioctl(fd, IOCTL_SET_PARAMS, &params) == -1) {
            perror("Failed to set parameters");
            close(fd);
            return 1;
        }

        printf("Parameters sent: Param1 = %d, Param2 = %d\n", params.param1, params.param2);
        sleep(5); // 5 saniye bekle
    }

    // Cihaz dosyasını kapat
    close(fd);
    return 0;
}
