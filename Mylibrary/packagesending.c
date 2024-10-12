#include "packagesending.h"
#include "choice_selection.h"
#include "receivedata2.h"
#include "string.h"
void package_sending(uint8_t *data2)  // UART2
{
				
								if(data2[4]== 0x65){
									th0_Sent = 1;
                 choice_selection_data_tx(Th0, data_rx2);
								}
                else if(data2[4]== 0x66 && th0_Sent){
                    choice_selection_data_tx(Th1, data_rx2);
                }
                else if(data2[4] == 0x67 && th0_Sent){
                    choice_selection_data_tx(Th2, data_rx2);
                }
                else if(data2[4] == 0x68){
                    choice_selection_data_tx(Th3, data_rx2);
                }
				
}