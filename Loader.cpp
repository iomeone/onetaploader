#include "Config.h"
#include "WndProc.h"
#include "Guard.h"
#include <fstream>
#include <string>
#include "LoadLibrary.h"
#include "scam.h"
#include "Fonts.h"
#include "background.h"
// DirectX
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib") 
#pragma warning(disable:4996)

#define BUTTON_WEI 169
#define BUTTON_HEI 29

#using <System.dll>

ImFont*  globalFont = nullptr;
ImFont*  forTabs = nullptr;
ImFont*  bigFont = nullptr;

void spaic()
{
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::SameLine(57.f);
}

ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);
void ToClipboard(const char* text)
{
	if (OpenClipboard(0))
	{
		EmptyClipboard();
		char* clip_data = (char*)(GlobalAlloc(GMEM_FIXED, MAX_PATH));
		lstrcpy(clip_data, text);
		SetClipboardData(CF_TEXT, (HANDLE)(clip_data));
		LCID* lcid = (DWORD*)(GlobalAlloc(GMEM_FIXED, sizeof(DWORD)));
		*lcid = MAKELCID(MAKELANGID(LANG_RUSSIAN, SUBLANG_NEUTRAL), SORT_DEFAULT);
		SetClipboardData(CF_LOCALE, (HANDLE)(lcid));
		CloseClipboard();
	}
}

static bool meme = CheckLicense();
static bool RevengerLicense = CheckLicense(); 
static bool SpectroLicense = CheckLicenseSpectro();
static int listbox_item_current = 0;
static std::string days = GetUserDayCount();



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	// им€ будущего класса
	LPCTSTR lpzClass = NAME;
	HRESULT hr;

	// регистраци€ класса
	if (!RegMyWindowClass(hInstance, lpzClass))
		return 1;

	// вычисление координат центра экрана
	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect); // разрешение экрана
	int x = screen_rect.right / 2 - 150;
	int y = screen_rect.bottom / 2 - 75;

	// создание диалогового окна
	HWND hWnd = CreateWindow(lpzClass, NAME, WS_POPUP, x, y, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	// если окно не создано, описатель будет равен 0
	if (!hWnd) return 2;

	// Initialize Direct3D
	LPDIRECT3D9 pD3D;
	if ((pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		UnregisterClass(lpzClass, hInstance);
	}

	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // Present with vsync
	//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Present without vsync, maximum unthrottled framerate

	//Ўрифт меню
	ImGuiIO& io = ImGui::GetIO();

	globalFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\tahoma.ttf", 14.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	forTabs = io.Fonts->AddFontFromMemoryCompressedTTF(MyFont_compressed_data2, MyFont_compressed_size2, 25.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	bigFont = io.Fonts->AddFontFromMemoryCompressedTTF(MyFont_compressed_data2, MyFont_compressed_size2, 17.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());

	if (pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
	{

		pD3D->Release();
		UnregisterClass(lpzClass, hInstance);
		return 0;

	}

	// Setup ImGui binding
	ImGui_ImplDX9_Init(hWnd, g_pd3dDevice);

	ImGuiStyle& style = ImGui::GetStyle();
	
	style.FramePadding = ImVec2(4, 2);
	style.ItemSpacing = ImVec2(6, 2);
	style.ItemInnerSpacing = ImVec2(6, 4);
	style.WindowRounding = 1.0f;
	style.WindowTitleAlign = ImVec2(0.5, 0.5);
	//style.Alpha = 0.1f;
	style.FrameRounding = 1.0f;
	style.IndentSpacing = 6.0f;
	style.ItemInnerSpacing = ImVec2(2, 4);
	style.ColumnsMinSpacing = 50.0f;
	style.GrabMinSize = 14.0f;
	style.GrabRounding = 16.0f;
	style.ScrollbarSize = 1.0f;
	style.ScrollbarRounding = 1.0f;
	
	static int hue = 140;
	static float col_main_sat = 180.f / 255.f;
	static float col_main_val = 161.f / 255.f;
	static float col_area_sat = 124.f / 255.f;
	static float col_area_val = 100.f / 255.f;
	static float col_back_sat = 59.f / 255.f;
	static float col_back_val = 40.f / 255.f;

	ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
	ImVec4 col_main = ImColor::HSV(hue / 255.f, col_main_sat, col_main_val);
	ImVec4 col_back = ImColor::HSV(hue / 255.f, col_back_sat, col_back_val);
	ImVec4 col_area = ImColor::HSV(hue / 255.f, col_area_sat, col_area_val);

	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.11f, 0.00f, 0.18f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	bool show_test_window = true;
	bool show_another_window = false;
	ImVec4 clear_col = ImColor(0, 0, 0, 255);
	


	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);

		ImGui_ImplDX9_NewFrame();

		IDirect3DTexture9 *background = nullptr;

		if (background == nullptr) {
			D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &exdsadasdp, sizeof(exdsadasdp), WINDOW_WIDTH, WINDOW_HEIGHT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &background);
		}

		DWORD dwFlag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar;

		static bool open = true;

		if (!open)
			ExitProcess(0);

		ImGui::PushFont(forTabs);
		ImGui::Begin(NAME_LOADER, &open, ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT), 0.9f, dwFlag);
		{
			ImGui::GetWindowDrawList()->AddImage(background, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + WINDOW_WIDTH, ImGui::GetWindowPos().y + WINDOW_HEIGHT), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));
		
			ImGui::PushFont(globalFont);
			if (SpectroLicense)
			{
				//ImGui::Text("Days Left: ", days.c_str());
				//ImGui::NewLine();
				spaic();
				if (ImGui::Button("Load Hack", ImVec2(BUTTON_WEI, BUTTON_HEI)))
				{
					{
						LPCTSTR Url = _T("http://webpanel.ru/cheat.dll"), File = _T("C://Windows//System32//theengine32.dll");
						hr = URLDownloadToFile(0, Url, File, 0, 0);
						switch (hr)
						{
						case S_OK:
							cout << "Successful download\n";
							break;
						case E_OUTOFMEMORY:
							cout << "Out of memory error\n";
							break;
						case INET_E_DOWNLOAD_FAILURE:
							cout << "Cannot access server data\n";
							break;
						default:
							cout << "Unknown error\n";
							break;
						}
						Sleep(7000);
						string proccessname;
						string dllname;
						proccessname = "csgo.exe";
						dllname = "dll1.dll";
						LoadHack->loadlibrarymain(proccessname.c_str(), dllname.c_str());
						exit(0);
					}
				}
				ImGui::NewLine();
				ImGui::SameLine(57.f);
				if (ImGui::Button("Exit", ImVec2(BUTTON_WEI, BUTTON_HEI)))
				{
					exit(0);
				}
			}
			else
			{
				spaic();
				if (ImGui::Button("Copy HWID", ImVec2(BUTTON_WEI, BUTTON_HEI)))
					ToClipboard(GetSerial().c_str());
				ImGui::NewLine();
				ImGui::SameLine(57.f);
				if (ImGui::Button("Exit", ImVec2(BUTTON_WEI, BUTTON_HEI)))
				{
					exit(0);
				}
			}
		}
		ImGui::End();



		// Rendering
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

		//D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*255.0f), (int)(clear_color.y*255.0f), (int)(clear_color.z*255.0f), (int)(clear_color.w*255.0f));
		//g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			g_pd3dDevice->EndScene();
		}
		g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	}

	ImGui_ImplDX9_Shutdown();
	if (g_pd3dDevice) g_pd3dDevice->Release();
	if (pD3D) pD3D->Release();
	UnregisterClass(NAME, hInstance);

	return 0;
}

////////////////////////////////////////////////////////////////////////// 
// функци€ регистрации класса окон

ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName)
{
	WNDCLASS wcWindowClass = { 0 };
	// адрес ф-ции обработки сообщений
	wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
	// стиль окна
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	// дискриптор экземпл€ра приложени€
	wcWindowClass.hInstance = hInst;
	// название класса
	wcWindowClass.lpszClassName = lpzClassName;
	// загрузка курсора
	wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	// загрузка цвета окон
	wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
	return RegisterClass(&wcWindowClass); // регистраци€ класса
}

