#define BAUDRATE 9600
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

#define MOTOR_FORWARD PC1
#define MOTOR_REVERSE PC0

#define SPEED_UP_OUT PD3
#define SPEED_DOWN_OUT PD2

#define MOTOR_SPEED_0 PB5
#define MOTOR_SPEED_1 PB4
#define MOTOR_SPEED_2 PB3
#define MOTOR_SPEED_3 PB2
#define MOTOR_SPEED_4 PB1