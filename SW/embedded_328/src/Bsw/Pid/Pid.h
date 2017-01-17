#include <Std_Types.h>

/*typedef struct
{
  int Ta;                      // Abtastzeit in ms
  int I;                      // Integralanteil
  int Kp;                      // Verst�rkung
  int D;                      // Differenzieller Anteil
  int e;                      // Regelabweichung
  float esum;                    // Summe der Regelabweichungen
  int ealt;                    // Regelabweichung zum ZP z-1
  int y;

}PID_Einstellung;                  // Struktur PID_Einstellungen erzeugen

PID_Einstellung pid;
*/

uint16 reg_adc_to_deg(uint16 value);
/*
void PID_Init();//PID_Einstellung *PID);
void PID_Cyclic(int, int, PID_Einstellung *);
*/
