/**
 * Store some configuration values
 */

#ifndef CONFIG_H
#define CONFIG_H

#define LED0    D,5     // access via I2C
#define LED1    D,6     // keep alive signal

#define ADDR0   D,2
#define ADDR1   D,3
#define ADDR2   D,4

#define GPIO0   C,3
#define GPIO1   C,1
#define GPIO2   C,2
#define GPIO3   C,0

#define BAUDRATE 9600

#endif /* CONFIG_H */
