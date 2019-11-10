#include "FreePrintf.h"

static void SerialPutChar(char c)
{
    Serial.write(c);
}

FreePrintf fp(SerialPutChar);

void setup()
{
    Serial.begin(115200);
    const char *ptr = "Hello world!";
    char *np = 0;
    int i = 5;
    unsigned int bs = sizeof(int) * 8;
    int mi;

    mi = (1 << (bs - 1)) + 1;
    fp.printf("%s\r\n", ptr);
    fp.printf("printf test\r\n");
    fp.printf("%s is null pointer\r\n", np);
    fp.printf("%d = 5\r\n", i);
    fp.printf("%d = - max int\r\n", mi);
    fp.printf("char %c = 'a'\r\n", 'a');
    fp.printf("hex %x = ff\r\n", 0xff);
    fp.printf("hex %02x = 00\r\n", 0);
    fp.printf("signed %d = unsigned %u = hex %x\r\n", -3, -3, -3);
    fp.printf("%d %s(s)%", 0, "message");
    fp.printf("\r\n");
    fp.printf("%d %s(s) with %%\r\n", 0, "message");
    fp.printf("justif: \"%-10s\"\r\n", "left");
    fp.printf("justif: \"%10s\"\r\n", "right");
    fp.printf(" 3: %04d zero padded\r\n", 3);
    fp.printf(" 3: %-4d left justif.\r\n", 3);
    fp.printf(" 3: %4d right justif.\r\n", 3);
    fp.printf("-3: %04d zero padded\r\n", -3);
    fp.printf("-3: %-4d left justif.\r\n", -3);
    fp.printf("-3: %4d right justif.\r\n", -3);
}

void loop()
{
}
