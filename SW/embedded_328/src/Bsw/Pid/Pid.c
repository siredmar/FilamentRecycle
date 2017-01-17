#include <inttypes.h>
#include "Pid.h"

uint16 reg_adco_deg(uint16 value)
{
 uint16 degree = 0;
 static const float temp1=23;
  static const float adc1=275;  //ADC-Wert=215 @ 150°C

  static const float temp2=300;
  static const float adc2=172;  //ADC-Wert=152 @ 240°C


  float m=((temp1-temp2)/(adc1-adc2));
  degree = ((uint16) ((m* (float)value) + (-m*adc1) + temp1));
  return degree;
}

/*void main(void)
{
  PID_Einstellung Regler1;
  PID_Init(&Regler1);

  int Ausgang, w,x;

  while(1)
  {
    PID_Cyclic(x,w,&Regler1);       // Parameter f�r Regler1 �bergeben
  }
}
*/

/*
void PID_Init()//PID_Einstellungen *pid)	// In der Init m�ssen die Reglereinstellungen gemacht werden
{
  pid.y=0;
  pid.Ta=100;		 // Abtastzeit in ms
  pid.I=1;		// Integralanteil
  pid.D=0;		// Differenzieller Anteil
  pid.Kp=1;		  // Verst�rkung
  o pid.esum=0;		// Summe der Regelabweichungen
  pid.e=0;                   // Regelabweichung
  pid.ealt=0;                  // Regelabweichung zum ZP z-1
}


void PID_Cyclic (int x, int w, PID_Einstellung* PID)
{
  PID->e = w - x;                                   // aktuelle Regelabweichung bestimmen
  if ((PID->y < 1023)&&(PID->y > 0))                      // bei �bersteuertem stellglied Integration einfrieren
   {                                           // (Anti-Windup)
        PID->esum = PID->esum + PID->e;                               // Summe der Regelabweichung aktualisieren
    }
  PID->y = (PID->Kp*PID->e)+(PID->I*PID->Ta*PID->esum)+(PID->D*((PID->e-PID->ealt))/PID->Ta);    // Reglergleichung
  PID->ealt = PID->e;                                     // Regelabweichung f�r n�chste Abtastung merken
  if (PID->y > 1023)                                 // Stellgr��e auf 0..1023 begrenzen (10 bit PWM)
   {
      PID->y = 1023;
    }
  if (PID->y < 1)
    {
      PID->y = 0;
    }
                                    // Stellgr��e zur�ckgeben
}
*/
