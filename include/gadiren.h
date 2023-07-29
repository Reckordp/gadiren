// Game yang nantinya mengganti gintkel.exe di startup

#define GADIREN_DIRECTORY "Gadiren"
#define GADIREN_DATA "sarang.bin"
#define WM_UBAHPOINT WM_USER + 0x00
#define WM_UBAHJUMLAH WM_USER + 0x01
#define SEHARIAN 86400
#define JUMLAH_BIBIT 2

struct pot {
  int id;
  time_t awal;
};

typedef struct
{
  char lokasi[4096];
  int point;
  time_t terakhir;
  int persediaan[JUMLAH_BIBIT];
  struct pot tumbuh[4096];
} gadiren;

struct bibit {
  char nama[256];
  int harga;
  time_t lama;
  int panen;
};

struct bibit toko[JUMLAH_BIBIT] = {
  { 
    .nama = "Jagung", 
    .harga = 20, 
    .lama = SEHARIAN, 
    .panen = 1 
  }, 
  { 
    .nama = "Mangga", 
    .harga = 80, 
    .lama = SEHARIAN * 4, 
    .panen = 5 
  }
};