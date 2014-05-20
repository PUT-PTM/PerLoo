//#define SERVO_MAX 52
//#define SERVO_MIN 12
#define SERVO_MAX 32
#define SERVO_MIN 15

void servo_relay_init(void);
void init_timer_pwm(void); // wyjscie PWM na pinie PB0
void init_leds_relay(void); //wyjscia sterujace PD5 i PD6
void init_user_button(void);
