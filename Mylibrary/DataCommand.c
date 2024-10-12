#include "DataCommand.h"
#include "StateCheck.h"
#include "Other.h"
#include "ResetData.h"
#include "receivedata.h"
#include "checksum.h"
void DataCommand(uint8_t *dataCommand)
{


	for(int l = 0; l < 5; l++)
	{

		if(dataCommand[0] == 0x97 && dataCommand[l + 1] == 0x00)
		{
			clear_flag = 1;
			Reset_data(data_rx);
			working_signal = 0;
			
		}
	}
	 if(dataCommand[0] == 0x98)
		{
			success_failed_signal = 1;
			package_display(data_rx, 6);
		}
				 else if (dataCommand[0] == 0x99)
				{
					
					if(working_signal)
					{
							
							Other(data_rx);
							StateCheck(data_rx);
					}
				}
}
void package_display(uint8_t *data_7Block, uint8_t length)
{
			
				if(Data_enough_Sent)
				{
								
								Other(data_7Block);
								StateCheck(data_7Block);								
								Data_enough_Sent = 0;
}
}