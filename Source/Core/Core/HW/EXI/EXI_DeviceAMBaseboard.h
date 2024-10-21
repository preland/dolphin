//copyrihgt dolhpin 20xx
//

#include "Core/HW/EXI/EXI_Device.h"
class CEXIAMBaseboard : public ExpansionInterface::IEXIDevice
{
  public:
    CEXIAMBaseboard();

    virtual void SetCS(int _iCS);
    virtual bool IsPresent();
    virtual bool IsInterruptSet();

  private:
    virtual void TransferByte(u8& _uByte);
    int m_position;
    bool m_have_irq;
    unsigned char m_command[4];
};

