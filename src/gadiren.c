#include <windows.h>
#include <commctrl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include <gadiren.h>

#define NAMA_KELAS "Gadiren"

HINSTANCE inst;
HWND papanPoint, papanJumlah[JUMLAH_BIBIT];
gadiren gadiren_ket;

void beliBibit(HWND hwnd, int id) {
  if (gadiren_ket.point < toko[id].harga) return;
  gadiren_ket.persediaan[id] += 1;
  gadiren_ket.point -= toko[id].harga;
  PostMessage(hwnd, WM_UBAHPOINT, 0, 0);
  PostMessage(hwnd, WM_UBAHJUMLAH, id, 0);
}

void ubahPapanPoint() {
  char teks[4096] = "Point ";
  itoa(gadiren_ket.point, teks + 6, 10);
  SetWindowText(papanPoint, teks);
}

void ubahPapanJumlah(int id) {
  char teks[4096] = "x";
  itoa(gadiren_ket.persediaan[id], teks + 1, 10);
  SetWindowText(papanJumlah[id], teks);
}

void buatToko(HWND hwnd) {
  char harga[4096];
  int ypos;
  for (size_t i = 0; i < JUMLAH_BIBIT; i++)
  {
    ypos = 28 * i;
    itoa(toko[i].harga, harga, 10);
    CreateWindow(WC_BUTTON, harga, WS_VISIBLE | WS_CHILD, 8, ypos + 30, 80, 24,
      hwnd, (HMENU)i, inst, NULL);
    CreateWindow(WC_STATIC, toko[i].nama, WS_VISIBLE | WS_CHILD, 100, ypos + 34, 80, 24,
      hwnd, (HMENU)0, inst, NULL);
    papanJumlah[i] = CreateWindow(WC_STATIC, "x", WS_VISIBLE | WS_CHILD, 200, ypos + 34, 80, 24,
      hwnd, (HMENU)0, inst, NULL);
  }
}

LRESULT CALLBACK AppProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch(uMsg) {
    case WM_CREATE:
      papanPoint = CreateWindow(WC_STATIC, "", WS_VISIBLE | WS_CHILD, 120, 10, 80, 25, 
        hwnd, (HMENU)0, inst, NULL);
      buatToko(hwnd);
      return 0;

    case WM_CLOSE:
      DestroyWindow(hwnd);
      return 0;
    
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

    case WM_UBAHPOINT:
      ubahPapanPoint();
      return 0;
    
    case WM_UBAHJUMLAH:
      ubahPapanJumlah(LOWORD(wParam));
      return 0;
    
    case WM_COMMAND:
      beliBibit(hwnd, LOWORD(wParam));
      return 0;
    
    default:
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
}

void dapatPoint() {
  time_t now, midnight, timeleft;
  double diff;
  now = time(NULL);
  diff = difftime(now, gadiren_ket.terakhir);
  if (diff > 0) {
    timeleft = now % 86400;
    midnight = now - timeleft + 86400;
    gadiren_ket.terakhir = midnight;
    gadiren_ket.point += 1000;
    MessageBox(NULL, "Dapat 1000 point!", "Hadiah", MB_OK);
  }
}

void loadData() {
  char *appdata;
  FILE* sarang;

  appdata = getenv("APPDATA");
  memcpy(gadiren_ket.lokasi, appdata, strlen(appdata));
  strcat(strcat(gadiren_ket.lokasi, "\\"), GADIREN_DIRECTORY);
  if (access(gadiren_ket.lokasi, F_OK) == -1) CreateDirectory(gadiren_ket.lokasi, NULL);
  strcat(strcat(gadiren_ket.lokasi, "\\"), GADIREN_DATA);
  if (access(gadiren_ket.lokasi, F_OK) != -1) {
    sarang = fopen(gadiren_ket.lokasi, "rb");
    fread(&gadiren_ket, sizeof(gadiren), 1, sarang);
    fclose(sarang);
  }
}

void saveData() {
  FILE* sarang;
  sarang = fopen(gadiren_ket.lokasi, "wb");
  fwrite(&gadiren_ket, sizeof(gadiren), 1, sarang);
  fclose(sarang);
}

int main(int argc, char const *argv[])
{
  loadData();
  inst = GetModuleHandle(NULL);
  WNDCLASSEX wc;

  ZeroMemory(&wc, sizeof(wc));
  wc.cbSize = sizeof(wc);
  wc.lpfnWndProc = AppProc;
  wc.hInstance = inst;
  wc.lpszClassName = NAMA_KELAS;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(inst, IDI_APPLICATION);
  int width = GetSystemMetrics(SM_CXSCREEN);
  int height = GetSystemMetrics(SM_CYSCREEN);

  RegisterClassEx(&wc);
  HWND mainWindow = CreateWindow(
    NAMA_KELAS, 
    "Gadiren", 
    WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_SIZEBOX), 
    width / 2 - 160, 
    height / 2 - 120, 
    320, 240, 
    NULL, NULL, inst, NULL);
  
  UpdateWindow(mainWindow);
  ShowWindow(mainWindow, SW_SHOWDEFAULT);
  dapatPoint();
  PostMessage(mainWindow, WM_UBAHPOINT, 0, 0);
  for (size_t i = 0; i < JUMLAH_BIBIT; i++)
    PostMessage(mainWindow, WM_UBAHJUMLAH, i, 0);
  

  MSG msg;
  while(GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  saveData();
  return 0;
}