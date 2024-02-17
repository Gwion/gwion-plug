#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

#define N 48

#define GWWS_HANDLE(ex) do{handle(shred,"WS2812b"ex);return;}while(0)

typedef struct gwws {
  m_int nled;
  uint8_t *buf;
  int fd;
  int sz;
} gwws;

#define GWWS(o) (struct gwws*)((o)->data)

#define GWWS_IOCTL(ws, mode, data) ioctl(ws->fd, mode, data)

static MFUN(ws2812b_setall);
static MFUN(ws2812b_new) {
  *(M_Object*)RETURN = o;
  gwws *ws = GWWS(o);
  char *name = STRING(*(M_Object*)MEM(SZ_INT));
  int mode = *(m_int*)MEM(SZ_INT * 2);
  int freq = *(m_int*)MEM(SZ_INT * 3);
  int lsb = 0;
  int wordsz = 8;

  if((ws->fd = open(name, O_RDWR)) < 0)
    GWWS_HANDLE("Open");

  if(GWWS_IOCTL(ws, SPI_IOC_WR_MODE, &mode)            < 0 ||
     GWWS_IOCTL(ws, SPI_IOC_RD_MODE, &mode)            < 0 ||
     GWWS_IOCTL(ws, SPI_IOC_WR_MAX_SPEED_HZ, &freq)    < 0 ||
     GWWS_IOCTL(ws, SPI_IOC_RD_MAX_SPEED_HZ, &freq)    < 0 ||
     GWWS_IOCTL(ws, SPI_IOC_WR_LSB_FIRST, &lsb)        < 0 ||
     GWWS_IOCTL(ws, SPI_IOC_RD_LSB_FIRST, &lsb)        < 0 ||
     GWWS_IOCTL(ws, SPI_IOC_WR_BITS_PER_WORD, &wordsz) < 0 ||
     GWWS_IOCTL(ws, SPI_IOC_RD_BITS_PER_WORD, &wordsz))
       GWWS_HANDLE("Init");

  const int nled = *(m_int*)MEM(SZ_INT * 4);
  if(nled <= 0) GWWS_HANDLE("LedNumber");
  ws->nled = nled;
  ws->buf = mp_calloc2(shred->info->mp, nled * N);
//  ws2812b_setall(shred, 0, RETURN);
}

static DTOR(ws2812b_dtor) {
  gwws *ws = GWWS(o);
  if(ws->fd < 0) return;
  close(ws->fd);
  if(ws->buf)
    mp_free2(shred->info->mp, ws->nled * N, ws->buf);
}

// rather return it?
static void write_one_frame(uint32_t rgb, uint8_t temp[N], bool even) {
printf("color %x\n", rgb);
  const uint16_t one_code = 0xFFF8U;
  const uint16_t zero_code = 0xE000U;
//  const uint16_t one_code =  0b0000111110000000;
//  const uint16_t zero_code = 0b1110000000000000;

  const uint8_t r = (uint8_t)((rgb >> 16) & 0xFF);
  const uint8_t g = (uint8_t)((rgb >> 8) & 0xFF);
  const uint8_t b = (uint8_t)((rgb >> 0) & 0xFF);
  const uint32_t c = ((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | b;

uint32_t point = 0;

  // is this needed ?
//  memset(temp, , sizeof(uint8_t) * 30);

  for (uint8_t i = 0; i < 24; i++) {
    if (((c >> (23 - i)) & 0x01) != 0) {
//      temp[i*2] = 0b1111111111110000;
      temp[i*2]     = 0b11111111;
      temp[i*2 + 1] = 0b11110000;
    } else {
puts("what");
      temp[i*2]     = 0b11110000;
      temp[i*2 + 1] = 0b00000000;
}
  }
return;
  for (uint8_t i = 0; i < 24; i++) {
    if (((c >> (23 - i)) & 0x01) != 0) {
      for (uint8_t j = 0; j < 16; j ++) {
        const bool code = !!((one_code >> (15 - j)) & 0x01);
        temp[point / 8] |= code << (7 - (point % 8));
        point++;
      }
    } else {
      for (uint8_t j = 0; j < 16; j ++) {
        const bool code = !!((zero_code >> (15 - j)) & 0x01);
        temp[point / 8] |= code << (7 - (point % 8));
        point++;
      }
    }
  }
}

static MFUN(ws2812b_write) {
  gwws *ws = GWWS(o);
  const m_int nled = ws->nled;
  const m_int sz = nled * N;
  struct spi_ioc_transfer k = {
    .tx_buf = (__u64)ws->buf,
    .len = sz,
  };
  if(sz != GWWS_IOCTL(ws, SPI_IOC_MESSAGE(1), &k))
    handle(shred, "WS2812bWrite");
}

static MFUN(ws2812b_set) {
  gwws *ws = GWWS(o);
  const m_int idx = *(m_int*)MEM(SZ_INT);
// check idx
  write_one_frame(*(uint32_t*)MEM(SZ_INT*2), &ws->buf[idx * N], !(idx % 2));
}

static MFUN(ws2812b_setall) {
  gwws *ws = GWWS(o);
//  for(m_int i = ws->nled + 1; --i;)
//    write_one_frame(*(uint32_t*)MEM(SZ_INT), &ws->buf[(i-1) * 36], !(i % 2));
  for(m_int i = 0; i < ws->nled; i++)
    write_one_frame(*(uint32_t*)MEM(SZ_INT), &ws->buf[i * N], !(i % 2));
}

GWION_IMPORT(WS2812b) {
  gwidoc(gwi, "a class to handle ws2812b based led stripes");
  DECL_B(const Type, t_ws2812b, = gwi_class_ini(gwi, "WS2812b", "Object"));
  gwi_class_xtor(gwi, NULL, ws2812b_dtor);

  gwidoc(gwi, "number of leds");
  GWI_B(gwi_item_ini(gwi, "int", "n_leds"));
  GWI_B(gwi_item_end(gwi, ae_flag_const, num, 0));

  t_ws2812b->nspc->offset += sizeof(gwws) - SZ_INT;

  gwidoc(gwi, "spi modes");
  GWI_B(gwi_enum_ini(gwi, "Mode"));
  GWI_B(gwi_enum_add(gwi, "ZERO", SPI_MODE_0));
  GWI_B(gwi_enum_add(gwi, "ONE", SPI_MODE_1));
  GWI_B(gwi_enum_add(gwi, "TWO", SPI_MODE_2));
  GWI_B(gwi_enum_add(gwi, "THREE", SPI_MODE_3));
  GWI_B(gwi_enum_end(gwi));

  gwidoc(gwi, "constructor");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "string", "spi_device"));
  GWI_B(gwi_func_arg(gwi, "WS2812b.Mode", "mode"));
  GWI_B(gwi_func_arg(gwi, "int", "freq"));
  GWI_B(gwi_func_arg(gwi, "int", "n_leds"));
  GWI_B(gwi_func_end(gwi, ws2812b_new, ae_flag_none));


  gwidoc(gwi, "write data");
  GWI_B(gwi_func_ini(gwi, "void", "write"));
  GWI_B(gwi_func_end(gwi, ws2812b_write, ae_flag_none));

  gwidoc(gwi, "set a specifid led");
  GWI_B(gwi_func_ini(gwi, "void", "set"));
  GWI_B(gwi_func_arg(gwi, "int", "led"));
  GWI_B(gwi_func_arg(gwi, "int", "rgb"));
  GWI_B(gwi_func_end(gwi, ws2812b_set, ae_flag_none));

  gwidoc(gwi, "set all leds");
  GWI_B(gwi_func_ini(gwi, "void", "set"));
  GWI_B(gwi_func_arg(gwi, "int", "rgb"));
  GWI_B(gwi_func_end(gwi, ws2812b_setall, ae_flag_none));

  GWI_B(gwi_class_end(gwi));
  return true;
}
