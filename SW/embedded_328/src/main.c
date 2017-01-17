#include "Std_Types.h"
#include "EcuM.h"


void Uart0_RxCallback(uint16 data)
{

}

int main()
{
    EcuM_InitSystem();

    while(1)
    {
        EcuM_Handler();

    }
    return 0;
}
