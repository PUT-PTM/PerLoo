
typedef enum eError
{
    ERROR_NONE         = 0,
    BUS_BUSY           = 1,
    ERROR_NOT_PRESENT  = 2,
    ERROR_ACK_TOO_LONG = 3,
    ERROR_SYNC_TIMEOUT = 4,
    ERROR_DATA_TIMEOUT = 5,
    ERROR_CHECKSUM     = 6,
    ERROR_NO_PATIENCE  = 7,
} eError;

extern void DHT_Init(void);
extern int DHT_readData(void);
extern int przeliczTemp();
extern void wait_ms(volatile int ms);
