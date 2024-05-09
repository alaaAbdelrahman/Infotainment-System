#define RDONLY      0x01
#define WRONLY      0x02
#define RDWR        0x03

struct pcdev_platform_data {

    int permission;
    int value;
    int direction;
    int size;
    char * serial_number;
char user_direction[50];

};