/*
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/poll.h>
#include <linux/timer.h>

#include "kernel_module.h"

#define DEVICE_NAME "kernel_module"
#define CLASS_NAME  "kernel"
#define BUFFER_SIZE 128

static int major;
static struct class* kernel_class = NULL;
static struct device* kernel_device = NULL;
static int param1;
static int param2;

// Timer
static struct timer_list my_timer;

// Sayaç
static int count = 0;

// Poll bekleme kuyruğu
static DECLARE_WAIT_QUEUE_HEAD(my_wait_queue);
static int data_ready = 0; // Veri hazır mı?

// Cihaz dosyası işlemleri için fonksiyonlar
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset);
static long dev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);
static void timer_callback(unsigned long data); // Timer geri çağırma fonksiyonu
static unsigned int dev_poll(struct file *filep, poll_table *wait);

// Cihaz dosyası işlemleri yapısı
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = dev_read,
    .unlocked_ioctl = dev_ioctl,
    .poll = dev_poll,
};

// Modül yüklendiğinde çalışacak fonksiyon
static int __init kernel_init(void) {
    printk(KERN_INFO "kernel: Initializing\n");

    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "kernel: Failed to register a major number\n");
        return major;
    }
    printk(KERN_INFO "kernel: Registered with major number %d\n", major);

    kernel_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(kernel_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(kernel_class);
    }

    kernel_device = device_create(kernel_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(kernel_device)) {
        class_destroy(kernel_class);
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(kernel_device);
    }

    // Timer'ı başlat
    setup_timer(&my_timer, timer_callback, 0);
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(5000)); // 5 saniye

    printk(KERN_INFO "kernel: Device created\n");
    return 0;
}

// Modül kapatıldığında çalışacak fonksiyon
static void __exit kernel_exit(void) {
    del_timer_sync(&my_timer); // Timer'ı sil
    device_destroy(kernel_class, MKDEV(major, 0));
    class_unregister(kernel_class);
    class_destroy(kernel_class);
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "kernel: Exiting\n");
}

// Timer geri çağırma fonksiyonu
static void timer_callback(unsigned long data) {
    count++; // Sayacı artır
    data_ready = 1; // Veri hazır olduğunu belirt
    printk(KERN_INFO "kernel: %d - Current Params - Param1: %d, Param2: %d\n", count, param1, param2);
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(5000)); // Timer'ı yeniden ayarla
    wake_up_interruptible(&my_wait_queue); // Poll için bekleyenleri uyandır
}

// Cihaz dosyasından okunduğunda çağrılır
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    char message[BUFFER_SIZE];

    if (!data_ready) { // Veri hazır değilse, kullanıcıyı beklet
        return -EAGAIN; // Bekleme hatası
    }

    snprintf(message, sizeof(message), "Param1: %d, Param2: %d\n", param1, param2);
    
    if (copy_to_user(buffer, message, strlen(message)) != 0) {
        return -EFAULT;
    }

    data_ready = 0; // Veriyi okuduktan sonra veri hazır durumu sıfırlanır
    return strlen(message);
}

// IOCTL işlemleri
static long dev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
    struct ioctl_params params;
    
    switch (cmd) {
        case IOCTL_SET_PARAMS:
            if (copy_from_user(&params, (struct ioctl_params*)arg, sizeof(struct ioctl_params))) {
                return -EFAULT;
            }
            param1 = params.param1;
            param2 = params.param2;
            printk(KERN_INFO "kernel: IOCTL Set Params: Param1 = %d, Param2 = %d\n", param1, param2);
            break;

        default:
            return -EINVAL;
    }
    return 0;
}

// Poll işlemi
static unsigned int dev_poll(struct file *filep, poll_table *wait) {
    poll_wait(filep, &my_wait_queue, wait); // Bekleme kuyruğunu ekle

    // Veri hazırsa okuma için okunanları belirt
    return data_ready ? POLLIN | POLLRDNORM : 0; 
}

module_init(kernel_init);
module_exit(kernel_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ELA");
MODULE_DESCRIPTION("A simple Linux char driver with ioctl support for multiple params and a polling mechanism");
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/poll.h>
#include <linux/timer.h>

#include "kernel_module.h"

#define DEVICE_NAME "kernel_module"
#define CLASS_NAME  "kernel"
#define BUFFER_SIZE 128

static int major;
static struct class* kernel_class = NULL;
static struct device* kernel_device = NULL;
static int param1;
static int param2;

// Timer
static struct timer_list my_timer;

// Sayaç
static int count = 0;

// Poll bekleme kuyruğu
static DECLARE_WAIT_QUEUE_HEAD(my_wait_queue);
static int data_ready = 0; // Veri hazır mı?

// Cihaz dosyası işlemleri için fonksiyonlar
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset);
static long dev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);
static void timer_callback(unsigned long data); // Timer geri çağırma fonksiyonu
static unsigned int dev_poll(struct file *filep, poll_table *wait);

// Cihaz dosyası işlemleri yapısı
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = dev_read,
    .unlocked_ioctl = dev_ioctl,
    .poll = dev_poll,
};

// Modül yüklendiğinde çalışacak fonksiyon
static int __init kernel_init(void) {
    printk(KERN_INFO "kernel: Initializing\n");

    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "kernel: Failed to register a major number\n");
        return major;
    }
    printk(KERN_INFO "kernel: Registered with major number %d\n", major);

    kernel_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(kernel_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(kernel_class);
    }

    kernel_device = device_create(kernel_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(kernel_device)) {
        class_destroy(kernel_class);
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(kernel_device);
    }

    // Timer'ı başlat
    init_timer(&my_timer);
    my_timer.function = timer_callback;
    my_timer.data = 0; // Kullanıcı verisi
    my_timer.expires = jiffies + msecs_to_jiffies(5000); // 5 saniye
    add_timer(&my_timer); // Timer'ı ekle

    printk(KERN_INFO "kernel: Device created\n");
    return 0;
}

// Modül kapatıldığında çalışacak fonksiyon
static void __exit kernel_exit(void) {
    del_timer_sync(&my_timer); // Timer'ı sil
    device_destroy(kernel_class, MKDEV(major, 0));
    class_unregister(kernel_class);
    class_destroy(kernel_class);
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "kernel: Exiting\n");
}

// Timer geri çağırma fonksiyonu
static void timer_callback(unsigned long data) {
    printk(KERN_INFO "kernel: Timer callback triggered\n"); // Timer çalıştığında yaz
    count++; // Sayacı artır
    data_ready = 1; // Veri hazır olduğunu belirt
    printk(KERN_INFO "kernel: %d - Current Params - Param1: %d, Param2: %d\n", count, param1, param2);
    my_timer.expires = jiffies + msecs_to_jiffies(5000); // Timer'ı yeniden ayarla
    add_timer(&my_timer); // Timer'ı yeniden ekle
    wake_up_interruptible(&my_wait_queue); // Poll için bekleyenleri uyandır
}

// Cihaz dosyasından okunduğunda çağrılır
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    char message[BUFFER_SIZE];

    if (!data_ready) { // Veri hazır değilse, kullanıcıyı beklet
        return -EAGAIN; // Bekleme hatası
    }

    snprintf(message, sizeof(message), "Param1: %d, Param2: %d\n", param1, param2);
    
    if (copy_to_user(buffer, message, strlen(message)) != 0) {
        return -EFAULT;
    }

    data_ready = 0; // Veriyi okuduktan sonra veri hazır durumu sıfırlanır
    return strlen(message);
}

// IOCTL işlemleri
static long dev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
    struct ioctl_params params;
    
    switch (cmd) {
        case IOCTL_SET_PARAMS:
            if (copy_from_user(&params, (struct ioctl_params*)arg, sizeof(struct ioctl_params))) {
                return -EFAULT;
            }
            param1 = params.param1;
            param2 = params.param2;
            printk(KERN_INFO "kernel: IOCTL Set Params: Param1 = %d, Param2 = %d\n", param1, param2);
            break;

        default:
            return -EINVAL;
    }
    return 0;
}

// Poll işlemi
static unsigned int dev_poll(struct file *filep, poll_table *wait) {
    poll_wait(filep, &my_wait_queue, wait); // Bekleme kuyruğunu ekle

    // Veri hazırsa okuma için okunanları belirt
    return data_ready ? POLLIN | POLLRDNORM : 0; 
}

module_init(kernel_init);
module_exit(kernel_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ELA");
MODULE_DESCRIPTION("A simple Linux char driver with ioctl support for multiple params and a polling mechanism");
