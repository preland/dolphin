//something copyright you get the picture
//

#include "Core/HW/SI/SI_Device.h"

class CSIDevice_AMBaseboard : public SerialInterface::ISIDevice {
  private:
    enum EBufferCommands
    {
      CMD_RESET = 0x00,
      CMD_GCAM = 0x70,
    }; //todo: move this into parent class enum
  public:
    //constructor
    CSIDevice_AMBaseboard(Core::System& system, SerialInterface::SIDevices device_type, int device_number);

    //run the SI Buffer
    virtual int RunBuffer(u8* _pBuffer, int _iLength);

    //return true on new data
    virtual bool GetData(u32& _Hi, u32& _Low);

    //send a command directly
    virtual void SendCommand(u32 _Cmd, u8 _Poll);
};
