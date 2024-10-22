//something something dolphin project copyright


//includes when needed

#include "Common/Logging/Log.h"
#include "Core/HW/EXI/EXI_Device.h"
#include "Core/HW/EXI/EXI_DeviceAMBaseboard.h"

CEXIAMBaseboard::CEXIAMBaseboard(Core::System& system) : ExpansionInterface::IEXIDevice(system){}//, m_position(0), m_have_irq(false) {}//m_position(0), m_have_irq(false) {}

void CEXIAMBaseboard::SetCS(int cs)
{
  ERROR_LOG_FMT(SP1, "AM-BB ChipSelect={}", cs);
  if (cs)
    m_position = 0;
}
bool IsPresent() {
  return true;
}
void CEXIAMBaseboard::TransferByte(u8& _byte)
{
  	/* original comment:
	ID:
		00 00 xx xx xx xx
		xx xx 06 04 10 00
	CMD:
		01 00 00 b3 xx
		xx xx xx xx 04
	exi_lanctl_write:
		ff 02 01 63 xx
		xx xx xx xx 04
	exi_imr_read:
		86 00 00 f5 xx xx xx
		xx xx xx xx 04 rr rr
	exi_imr_write:
		87 80 5c 17 xx
		xx xx xx xx 04
	exi_isr_read:
		82 .. .. .. xx xx xx
		xx xx xx xx 04 rr rr
		3 byte command, 1 byte checksum
	*/
  ERROR_LOG_FMT(SP1, "AM-BB > {}", _byte);

  if(m_position < 4)
  {
    unsigned int checksum = (m_command[0] << 24) | (m_command[1] << 16) | (m_command[2] << 8);
    unsigned int bit = 0x80000000UL;
    unsigned int check = 0x8D800000UL;
    while (bit >= 0x100)
    {
      if (checksum & bit)
        checksum ^= check;
      check >>= 1;
      bit >>= 1;
    }
    if (m_command[3] != (checksum & 0xFF))
      ERROR_LOG_FMT(SP1, "AM-BB cs: {}, w: {}", m_command[3], checksum & 0xFF);
  }
  else
  {
    if (m_position == 4)
    {
      _byte = 4;
      ERROR_LOG_FMT(SP1, "AM-BB COMMAND: {%02x} {%02x} {%02x}", m_command[0], m_command[1], m_command[2]);
      if ((m_command[0] == 0xFF) && (m_command[1] == 0) && (m_command[2] == 0))
        m_have_irq = true;
      else if (m_command[0] == 0x82)
        m_have_irq = false;
    }
    else if (m_position > 4)
    {
      switch (m_command[0])
      {
        case 0xFF: // lan
          _byte = 0xFF;
          break;
        case 0x86: // imr
          _byte = 0x00;
          break;
        case 0x82: //isr
          _byte = m_have_irq ? 0xFF : 0;
          break;
        default:
          //_dbg_assert_msg_(SP1, 0, "Unknown AM-BB cmd"); // preland note: idk what function this
                                                           // should be replaced with
          break;
      }
    }
  }
}
bool CEXIAMBaseboard::IsInterruptSet()
{
  if (m_have_irq)
    ERROR_LOG_FMT(SP1, "AM-BB IRQ");
  return m_have_irq;
}
