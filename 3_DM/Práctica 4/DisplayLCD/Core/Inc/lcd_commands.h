#define SFF         0x11
#define SMM         0x1F
#define DISP_ON     0x09
#define DISP_OFF    0x08
#define LDPI        
// #define SRM         // No usaremos este modo
#define SWM         0x60   // Luego incrementa en 1 el contenido del puntero
#define SORM        0x6B   // Luego no hace nada con el puntero
#define SANDM       0x6F   // Luego no hace nada con el puntero
// #define SCML        // No usaremos este modo
#define SCMR        0x72
// #define BRESET      // Según bit que queramos poner a 0
// #define BSET        // Según bit que queramos poner a 1
#define CLCURS      0x7C
// #define WRCURS      // No mostraremos el cursor (bit 7)
#define STOP        0x01
#define DATA        0x00