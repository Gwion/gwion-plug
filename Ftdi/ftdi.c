#include "ftd2xx.h"
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "plug.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "array.h"
#include "gwi.h"

static CTOR(ftdi_ctor) { /*code here */ }

static DTOR(ftdi_dtor) { /*code here */ }

GWION_IMPORT(Ftdi) {
  DECL_B(const Type, t_ftdi, = gwi_class_ini(gwi, "Ftdi", "Object"));
  gwi_class_xtor(gwi, ftdi_ctor, ftdi_dtor);

  gwidoc(gwi, "status vallue for api calls");
  GWI_B(gwi_enum_ini(gwi, "Status"));
  GWI_B(gwi_enum_add(gwi, "OK", FT_OK));
  GWI_B(gwi_enum_add(gwi, "INVALID_HANDLE", FT_INVALID_HANDLE));
  GWI_B(gwi_enum_add(gwi, "DEVICE_NOT_FOUND", FT_DEVICE_NOT_FOUND));
  GWI_B(gwi_enum_add(gwi, "DEVICE_NOT_OPENED", FT_DEVICE_NOT_OPENED));
  GWI_B(gwi_enum_add(gwi, "IO_ERROR", FT_IO_ERROR));
  GWI_B(gwi_enum_add(gwi, "INSUFFICIENT_RESOURCES", FT_INSUFFICIENT_RESOURCES));
  GWI_B(gwi_enum_add(gwi, "INVALID_PARAMETER", FT_INVALID_PARAMETER));
  GWI_B(gwi_enum_add(gwi, "INVALID_BAUD_RATE", FT_INVALID_BAUD_RATE));
  GWI_B(gwi_enum_add(gwi, "DEVICE_NOT_OPENED_FOR_ERASE", FT_DEVICE_NOT_OPENED_FOR_ERASE));
  GWI_B(gwi_enum_add(gwi, "DEVICE_NOT_OPENED_FOR_WRITE", FT_DEVICE_NOT_OPENED_FOR_WRITE));
  GWI_B(gwi_enum_add(gwi, "FAILED_TO_WRITE_DEVICE", FT_FAILED_TO_WRITE_DEVICE));
  GWI_B(gwi_enum_add(gwi, "EEPROM_READ_FAILED", FT_EEPROM_READ_FAILED));
  GWI_B(gwi_enum_add(gwi, "EEPROM_WRITE_FAILED", FT_EEPROM_WRITE_FAILED));
  GWI_B(gwi_enum_add(gwi, "EEPROM_ERASE_FAILED", FT_EEPROM_ERASE_FAILED));
  GWI_B(gwi_enum_add(gwi, "EEPROM_NOT_PRESENT", FT_EEPROM_NOT_PRESENT));
  GWI_B(gwi_enum_add(gwi, "EEPROM_NOT_PROGRAMMED", FT_EEPROM_NOT_PROGRAMMED));
  GWI_B(gwi_enum_add(gwi, "INVALID_ARGS", FT_INVALID_ARGS));
  GWI_B(gwi_enum_add(gwi, "NOT_SUPPORTED", FT_NOT_SUPPORTED));
  GWI_B(gwi_enum_add(gwi, "OTHER_ERROR", FT_OTHER_ERROR));
  GWI_B(gwi_enum_add(gwi, "DEVICE_LIST_NOT_READY", FT_DEVICE_LIST_NOT_READY));
  GWI_B(gwi_enum_end(gwi));

  gwidoc(gwi, "flags for openEx");
  GWI_B(gwi_enum_ini(gwi, "OPEN"));
  GWI_B(gwi_enum_add(gwi, "BY_SERIAL_NUMBER", FT_OPEN_BY_SERIAL_NUMBER));
  GWI_B(gwi_enum_add(gwi, "BY_DESCRIPTION", FT_OPEN_BY_DESCRIPTION));
  GWI_B(gwi_enum_add(gwi, "BY_LOCATION", FT_OPEN_BY_LOCATION));
  GWI_B(gwi_enum_add(gwi, "MASK", FT_OPEN_MASK));
  GWI_B(gwi_enum_end(gwi));

  gwidoc(gwi, "flags for listDevices");
  GWI_B(gwi_enum_ini(gwi, "LIST"));
  GWI_B(gwi_enum_add(gwi, "NUMBER_ONLY", FT_LIST_NUMBER_ONLY));
  GWI_B(gwi_enum_add(gwi, "INDEX", FT_LIST_BY_INDEX));
  GWI_B(gwi_enum_add(gwi, "ALL", FT_LIST_ALL));
  GWI_B(gwi_enum_end(gwi));

  gwidoc(gwi, "common baud rates");
  GWI_B(gwi_enum_ini(gwi, "RATE"));
  GWI_B(gwi_enum_add(gwi, "BAUD_300", FT_BAUD_300));
  GWI_B(gwi_enum_add(gwi, "BAUD_600", FT_BAUD_600));
  GWI_B(gwi_enum_add(gwi, "BAUD_1200", FT_BAUD_1200));
  GWI_B(gwi_enum_add(gwi, "BAUD_2400", FT_BAUD_2400));
  GWI_B(gwi_enum_add(gwi, "BAUD_4800", FT_BAUD_4800));
  GWI_B(gwi_enum_add(gwi, "BAUD_9600", FT_BAUD_9600));
  GWI_B(gwi_enum_add(gwi, "BAUD_14400", FT_BAUD_14400));
  GWI_B(gwi_enum_add(gwi, "BAUD_19200", FT_BAUD_19200));
  GWI_B(gwi_enum_add(gwi, "BAUD_38400", FT_BAUD_38400));
  GWI_B(gwi_enum_add(gwi, "BAUD_57600", FT_BAUD_57600));
  GWI_B(gwi_enum_add(gwi, "BAUD_115200", FT_BAUD_115200));
  GWI_B(gwi_enum_add(gwi, "BAUD_230400", FT_BAUD_230400));
  GWI_B(gwi_enum_add(gwi, "BAUD_460800", FT_BAUD_460800));
  GWI_B(gwi_enum_add(gwi, "BAUD_921600", FT_BAUD_921600));
  GWI_B(gwi_enum_end(gwi));

  gwidoc(gwi, "words lenghts");
  GWI_B(gwi_enum_ini(gwi, "WORD_LENGHT"));
  GWI_B(gwi_enum_add(gwi, "BITS_8", FT_BITS_8));
  GWI_B(gwi_enum_add(gwi, "BITS_7", FT_BITS_7));
  GWI_B(gwi_enum_end(gwi));

  gwidoc(gwi, "stop bits");
  GWI_B(gwi_enum_ini(gwi, "STOP_BITS"));
  GWI_B(gwi_enum_add(gwi, "BITS_1", FT_STOP_BITS_1));
  GWI_B(gwi_enum_add(gwi, "BITS_2", FT_STOP_BITS_2));
  GWI_B(gwi_enum_end(gwi));

  gwidoc(gwi, "parity");
  GWI_B(gwi_enum_ini(gwi, "PARITY"));
  GWI_B(gwi_enum_add(gwi, "NONE", FT_PARITY_NONE));
  GWI_B(gwi_enum_add(gwi, "ODD", FT_PARITY_ODD));
  GWI_B(gwi_enum_add(gwi, "EVEN", FT_PARITY_EVEN));
  GWI_B(gwi_enum_add(gwi, "MARK", FT_PARITY_MARK));
  GWI_B(gwi_enum_add(gwi, "SPACE", FT_PARITY_SPACE));
  GWI_B(gwi_enum_end(gwi));

  gwidoc(gwi, "flow");
  GWI_B(gwi_enum_ini(gwi, "FLOW"));
  GWI_B(gwi_enum_add(gwi, "NONE", FT_FLOW_NONE));
  GWI_B(gwi_enum_add(gwi, "RTS_CTS", FT_FLOW_RTS_CTS));
  GWI_B(gwi_enum_add(gwi, "DTR_DSR", FT_FLOW_DTR_DSR));
  GWI_B(gwi_enum_add(gwi, "XON_XOFF", FT_FLOW_XON_XOFF));
  GWI_B(gwi_enum_end(gwi));

  gwidoc(gwi, "purge rx and tx buffers");
  GWI_B(gwi_enum_ini(gwi, "PURGE"));
  GWI_B(gwi_enum_add(gwi, "RX", FT_PURGE_RX));
  GWI_B(gwi_enum_add(gwi, "TX", FT_PURGE_TX));
  GWI_B(gwi_enum_end(gwi));

  GWI_B(gwi_class_end(gwi));
  return true;
}
