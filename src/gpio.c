// file: gpio.c
// gcc -shared -o libgpio.so -fPIC gpio.c
// libgpio.so = libgpio shared object
// -fPIC = feature Position Independent Code

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// gpio->gpfsel0 bit offsets
#define GPIO_GPFSEL0_RSVDA                      30
#define GPIO_GPFSEL0_FSEL9                      27
#define GPIO_GPFSEL0_FSEL8                      24
#define GPIO_GPFSEL0_FSEL7                      21
#define GPIO_GPFSEL0_FSEL6                      18
#define GPIO_GPFSEL0_FSEL5                      15
#define GPIO_GPFSEL0_FSEL4                      12
#define GPIO_GPFSEL0_FSEL3                      9
#define GPIO_GPFSEL0_FSEL2                      6
#define GPIO_GPFSEL0_FSEL1                      3
#define GPIO_GPFSEL0_FSEL0                      0

// gpio->gpfsel1 bit offsets
#define GPIO_GPFSEL1_RSVDA                      30
#define GPIO_GPFSEL1_FSEL19                     27
#define GPIO_GPFSEL1_FSEL18                     24
#define GPIO_GPFSEL1_FSEL17                     21
#define GPIO_GPFSEL1_FSEL16                     18
#define GPIO_GPFSEL1_FSEL15                     15
#define GPIO_GPFSEL1_FSEL14                     12
#define GPIO_GPFSEL1_FSEL13                     9
#define GPIO_GPFSEL1_FSEL12                     6
#define GPIO_GPFSEL1_FSEL11                     3
#define GPIO_GPFSEL1_FSEL10                     0

// gpio->gpset0 bit offsets
#define GPIO_GPSET0_SETGPIO31                   31
#define GPIO_GPSET0_SETGPIO30                   30
#define GPIO_GPSET0_SETGPIO29                   29
#define GPIO_GPSET0_SETGPIO28                   28
#define GPIO_GPSET0_SETGPIO27                   27
#define GPIO_GPSET0_SETGPIO26                   26
#define GPIO_GPSET0_SETGPIO25                   25
#define GPIO_GPSET0_SETGPIO24                   24
#define GPIO_GPSET0_SETGPIO23                   23
#define GPIO_GPSET0_SETGPIO22                   22
#define GPIO_GPSET0_SETGPIO21                   21
#define GPIO_GPSET0_SETGPIO20                   20
#define GPIO_GPSET0_SETGPIO19                   19
#define GPIO_GPSET0_SETGPIO18                   18
#define GPIO_GPSET0_SETGPIO17                   17
#define GPIO_GPSET0_SETGPIO16                   16
#define GPIO_GPSET0_SETGPIO15                   15
#define GPIO_GPSET0_SETGPIO14                   14
#define GPIO_GPSET0_SETGPIO13                   13
#define GPIO_GPSET0_SETGPIO12                   12
#define GPIO_GPSET0_SETGPIO11                   11
#define GPIO_GPSET0_SETGPIO10                   10
#define GPIO_GPSET0_SETGPIO9                    9
#define GPIO_GPSET0_SETGPIO8                    8
#define GPIO_GPSET0_SETGPIO7                    7
#define GPIO_GPSET0_SETGPIO6                    6
#define GPIO_GPSET0_SETGPIO5                    5
#define GPIO_GPSET0_SETGPIO4                    4
#define GPIO_GPSET0_SETGPIO3                    3
#define GPIO_GPSET0_SETGPIO2                    2
#define GPIO_GPSET0_SETGPIO1                    1
#define GPIO_GPSET0_SETGPIO0                    0

// gpio->gpclr0 bit offsets
#define GPIO_GPCLR0_CLEARGPIO31                 31
#define GPIO_GPCLR0_CLEARGPIO30                 30
#define GPIO_GPCLR0_CLEARGPIO29                 29
#define GPIO_GPCLR0_CLEARGPIO28                 28
#define GPIO_GPCLR0_CLEARGPIO27                 27
#define GPIO_GPCLR0_CLEARGPIO26                 26
#define GPIO_GPCLR0_CLEARGPIO25                 25
#define GPIO_GPCLR0_CLEARGPIO24                 24
#define GPIO_GPCLR0_CLEARGPIO23                 23
#define GPIO_GPCLR0_CLEARGPIO22                 22
#define GPIO_GPCLR0_CLEARGPIO21                 21
#define GPIO_GPCLR0_CLEARGPIO20                 20
#define GPIO_GPCLR0_CLEARGPIO19                 19
#define GPIO_GPCLR0_CLEARGPIO18                 18
#define GPIO_GPCLR0_CLEARGPIO17                 17
#define GPIO_GPCLR0_CLEARGPIO16                 16
#define GPIO_GPCLR0_CLEARGPIO15                 15
#define GPIO_GPCLR0_CLEARGPIO14                 14
#define GPIO_GPCLR0_CLEARGPIO13                 13
#define GPIO_GPCLR0_CLEARGPIO12                 12
#define GPIO_GPCLR0_CLEARGPIO11                 11
#define GPIO_GPCLR0_CLEARGPIO10                 10
#define GPIO_GPCLR0_CLEARGPIO9                  9
#define GPIO_GPCLR0_CLEARGPIO8                  8
#define GPIO_GPCLR0_CLEARGPIO7                  7
#define GPIO_GPCLR0_CLEARGPIO6                  6
#define GPIO_GPCLR0_CLEARGPIO5                  5
#define GPIO_GPCLR0_CLEARGPIO4                  4
#define GPIO_GPCLR0_CLEARGPIO3                  3
#define GPIO_GPCLR0_CLEARGPIO2                  2
#define GPIO_GPCLR0_CLEARGPIO1                  1
#define GPIO_GPCLR0_CLEARGPIO0                  0

typedef struct
{
    uint32_t    gpfsel0;
    uint32_t    gpfsel1;
    uint32_t    gpfsel2;
    uint32_t    gpfsel3;
    uint32_t    gpfsel4;
    uint32_t    gpfsel5;
    uint32_t    reserved1;
    uint32_t    gpset0;
    uint32_t    gpset1;
    uint32_t    reserved2;
    uint32_t    gpclr0;
    uint32_t    gpclr1;
} __attribute__((packed, aligned(4))) gpio_t;

#define PAGE_SIZE   0x1000

volatile gpio_t *gpio;

// gpio14 = rbpi3b+_conn_40_pin_08
void gpio_init(void) {
    int fd;

    // open physical memory filedescriptor
    fd = open("/dev/mem", (O_RDWR | O_SYNC));

    // map a page of memory (4 KB) to gpio at offset 0x3F200000 which is where BCM2837B0 GPIO registers start
    gpio = (gpio_t *) mmap(0, PAGE_SIZE, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, 0x3F200000);

    close(fd);

    // gpio14 is input
    gpio->gpfsel1 &= ~(7 << GPIO_GPFSEL1_FSEL14);

    // gpio14 is output
    gpio->gpfsel1 |= (1 << GPIO_GPFSEL1_FSEL14);
}

void gpio_set(uint32_t pin, uint32_t value) {
    // set gpio14 to 1 or 0
    if (value == 1) {
        gpio->gpset0 = (1 << pin);
    }
    else {
        gpio->gpclr0 = (1 << pin);
    }
}
