#include <windows.h>  
#include <stdlib.h> // random
#include <string>
#include <dsound.h>
#include <tchar.h>  

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void Win32UpdateWindows(HWND hWnd);
HINSTANCE hInst;
static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");
bool isRunning = true;
int xoffset = 0;
int yoffset = 0;
IDirectSoundBuffer *secondaryBuffer;
#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);


void initDirectSound(HWND window, int samplesPerSouncd, int bufferSize) {
	HMODULE DSoundLibrary = LoadLibrary("dsound.dll");
	if (DSoundLibrary) {
		direct_sound_create *DirectSoundCreate = (direct_sound_create*)
			GetProcAddress(DSoundLibrary, "DirectSoundCreate");
		IDirectSound *DirectSound;
		if (DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &DirectSound, 0))) {
			WAVEFORMATEX waveFormat = {};
			waveFormat.cbSize = sizeof(WAVEFORMATEX);
			waveFormat.wFormatTag = WAVE_FORMAT_PCM;
			waveFormat.nChannels = 2;
			waveFormat.nSamplesPerSec = samplesPerSouncd;
			waveFormat.nBlockAlign = 4;
			waveFormat.wBitsPerSample = 16;
			waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec*waveFormat.nBlockAlign;
			waveFormat.cbSize = 0;
			if (SUCCEEDED(DirectSound->SetCooperativeLevel(window, DSSCL_PRIORITY))) {
				DSBUFFERDESC BufferDescription = {};
				BufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;
				BufferDescription.dwSize = sizeof(DSBUFFERDESC);

				IDirectSoundBuffer *PrimaryBuffer;
				if (SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription, &PrimaryBuffer, 0))) {

					if (SUCCEEDED(PrimaryBuffer->SetFormat(&waveFormat))) {
						OutputDebugString("primary buffer format was set.\n");
					}
					else {
						// TODO log
					}
				}
				else {
					//TODO log
				}
			}
			else {
				//TODO paddy log
			}

			DSBUFFERDESC SecondBufferDescription = {};
			SecondBufferDescription.dwSize = sizeof(DSBUFFERDESC);
			SecondBufferDescription.dwFlags = 0;
			SecondBufferDescription.dwFlags = DSBCAPS_GETCURRENTPOSITION2;
			SecondBufferDescription.dwBufferBytes = bufferSize;
			SecondBufferDescription.lpwfxFormat = &waveFormat;

			//IDirectSoundBuffer *secondaryBuffer;
			if (SUCCEEDED(DirectSound->CreateSoundBuffer(&SecondBufferDescription, &secondaryBuffer, 0))) {
				//if (SUCCEEDED(secondaryBuffer->SetFormat(&waveFormat))) {
				OutputDebugString("second buffer format was set.\n");
				//}
			}
		}
	}
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance; // Store instance handle in our global variable  

					   // The parameters to CreateWindow explained:  
					   // szWindowClass: the name of the application  
					   // szTitle: the text that appears in the title bar  
					   // WS_OVERLAPPEDWINDOW: the type of window to create  
					   // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)  
					   // 500, 100: initial size (width, length)  
					   // NULL: the parent of this window  
					   // NULL: this application dows not have a menu bar  
					   // hInstance: the first parameter from WinMain  
					   // NULL: not used in this application  
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:  
	// hWnd: the value returned from CreateWindow  
	// nCmdShow: the fourth parameter from WinMain  
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	initDirectSound(hWnd, 48000, 48000 * sizeof(int) * 2);
	secondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
	UINT32 runningSampleIndex = 0;
	int squareWaveCounter = 0;
	int hz = 256;
	int samplesPerSecond = 48000;
	int squareWavePeriod = samplesPerSecond / hz;
	int halfSquareWavePeriod = squareWavePeriod / 2;
	int BytesPerSample = sizeof(INT16) * 2;
	int secondaryBufferSize = samplesPerSecond*BytesPerSample;

	// Main message loop:  
	MSG msg;
	while (isRunning) {
		while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				isRunning = false;
			}

			switch (msg.message) {
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYDOWN:
			case WM_KEYUP: {
				int VKCode = (int)msg.wParam;

				// NOTE: Since we are comparing WasDown to IsDown,
				// we MUST use == or != to convert these bit tests to actual
				// 0 or 1 values.
				bool WasDown = ((msg.lParam & (1 << 30)) != 0);
				bool IsDown = ((msg.lParam & (1 << 31)) == 0);
				if (WasDown != IsDown) {
					{
						WPARAM wParam = msg.wParam;
						if (wParam == 'A')
						{
							xoffset -= 5;
						}
						else if (wParam == 'D')
						{
							xoffset += 5;
						}
						else if (wParam == 'W')
						{
							yoffset += 5;
						}
						else if (wParam == 'S')
						{
							yoffset -= 5;
						}
					}
				}
			}
						   break;
			default: {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			}
		}

		void *region1;
		DWORD regionSize1;
		void *region2;
		DWORD regionSize2;

		DWORD playCursor;
		DWORD writeCursor;


		if (SUCCEEDED(secondaryBuffer->GetCurrentPosition(&playCursor, &writeCursor))) {

			DWORD byteToLock = runningSampleIndex & BytesPerSample % secondaryBufferSize;
			DWORD byteToWrite;
			if (byteToLock > playCursor) {
				byteToWrite = (secondaryBufferSize - byteToLock);
				byteToWrite += playCursor;
			}
			else {
				byteToWrite = playCursor - byteToLock;
			}
			//DWORD writePointer = ;
			if (SUCCEEDED(secondaryBuffer->Lock(
				byteToLock,
				byteToWrite,
				&region1, &regionSize1,
				&region2, &regionSize2,
				0))) {
				int i = 0;
				INT16 *sampleOut = (INT16*)region1;
				DWORD region1SampleCount = regionSize1 / BytesPerSample;
				DWORD region2SampleCount = regionSize2 / BytesPerSample;
				for (DWORD sampleIndex = 0; sampleIndex < region1SampleCount; ++sampleIndex) {
					
					INT16 SampleValue = ((runningSampleIndex / halfSquareWavePeriod) % 2) ? 16000 : -16000;
					*sampleOut++ = SampleValue;
					*sampleOut++ = SampleValue;
					runningSampleIndex++;
				}
				sampleOut = (INT16*)region2;
				for (DWORD sampleIndex = 0; sampleIndex < region2SampleCount; ++sampleIndex) {
					
					INT16 SampleValue = ((runningSampleIndex / halfSquareWavePeriod) % 2) ? 16000 : -16000;
					*sampleOut++ = SampleValue;
					*sampleOut++ = SampleValue;
					runningSampleIndex++;
				}
				secondaryBuffer->Unlock(region1, regionSize1, region2, regionSize2);
			}
		}
	}

	return (int)msg.wParam;
}

//void* buffer;
void *buffer;

void AllocateBuffer(int width, int height) {
	if (buffer) {
		VirtualFree(buffer, 0, MEM_RELEASE);
	}
	int size = width * height * sizeof(RGBQUAD);
	buffer = VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE);
}
//RGBQUAD *alo = new RGBQUAD[100];

void Win32UpdateWindows(HWND hWnd) {
	HDC hdc = GetDC(hWnd);
	RECT rect;
	GetClientRect(hWnd, &rect);
	int clientHeight = rect.bottom - rect.top;
	int clientWidth = rect.right - rect.left;
	//buffer = new RGBQUAD[clientHeight * clientWidth];

	BITMAPINFO bitMapInfo = {};
	bitMapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitMapInfo.bmiHeader.biWidth = clientWidth;
	bitMapInfo.bmiHeader.biHeight = clientHeight;
	bitMapInfo.bmiHeader.biPlanes = 1;
	bitMapInfo.bmiHeader.biBitCount = 32;
	bitMapInfo.bmiHeader.biCompression = BI_RGB;
	bitMapInfo.bmiHeader.biSizeImage = 0;
	int length = clientHeight * clientWidth;
	RGBQUAD* p = (RGBQUAD*)buffer;

	for (int i = 0; i < clientHeight; i++) {
		for (int j = 0; j < clientWidth; j++) {
			p->rgbRed = j + xoffset;
			p->rgbGreen = i + yoffset;
			p->rgbBlue = 0;
			p++;
		}
	}

	/*StretchDIBits(
	hdc,
	0, 0, clientWidth, clientHeight,
	0, 0, clientWidth, clientHeight,
	buffer,
	&bitMapInfo,
	DIB_RGB_COLORS,
	SRCCOPY
	);*/
	ReleaseDC(hWnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, World!");
	int clientHeight = 0;
	int clientWidth = 0;
	switch (message)
	{
	case WM_CLOSE:
	{
		// TODO: Handle this with a message to the user?
		isRunning = false;
	} break;

	case WM_SIZE:
		RECT rect;
		GetClientRect(hWnd, &rect);
		clientHeight = rect.bottom - rect.top;
		clientWidth = rect.right - rect.left;
		AllocateBuffer(clientWidth, clientHeight);
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		//DeleteObject(hBitMap);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		isRunning = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}
