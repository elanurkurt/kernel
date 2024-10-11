/*
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>  // Zamanlama için

struct ioctl_params {
    int param1;
    int param2;
};

#define IOCTL_SET_PARAMS _IOW('a', 'b', struct ioctl_params*)

#define BUFFER_SIZE 128
#define INTERVAL 5 // 5 saniye

int main() {
    int fd;
    struct ioctl_params params;
    char buffer[BUFFER_SIZE];

    // Cihaz dosyasını aç
    fd = open("/dev/kernel_module", O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    // Sonsuz döngü başlat
    while (1) {
        // Parametreleri ayarla
        params.param1 = 111;
        params.param2 = 222;

        // IOCTL ile parametreleri ayarla
        if (ioctl(fd, IOCTL_SET_PARAMS, &params) == -1) {
            perror("Failed to set parameters");
            close(fd);
            return 1;
        }

        printf("Parameters sent: Param1 = %d, Param2 = %d\n", params.param1, params.param2);

        // Cihazdan okuma işlemi
        ssize_t bytesRead = read(fd, buffer, BUFFER_SIZE);
        if (bytesRead < 0) {
            perror("Failed to read from device");
            close(fd);
            return 1;
        }

        // Okunan veriyi yazdır
        printf("Read from device: %s", buffer);

        // 5 saniye bekle
        sleep(INTERVAL);
    }

    // Cihaz dosyasını kapat
    close(fd);
    return 0;
}*/


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
