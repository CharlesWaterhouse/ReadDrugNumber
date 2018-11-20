

#include <windows.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <allheaders.h> // leptonica main header for image io
#include <baseapi.h> // tesseract main header
//#include "tpmvscattestation.h"

#include <waterhouse_function.h>
#include <waterhouse_function_for_opencv.h>
#include "test_for_opencv_win32API_h1.h"

using namespace cv;
using namespace std;

void MyInitializeWndclassex(WNDCLASSEX & input_WNDCLASSEX, HINSTANCE & input_hInstance);
LRESULT CALLBACK WindowsProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void MyCreateAllWindows(HINSTANCE & hInstance);
void RunMyProcessFunction(const string & input_text);

void ShowImageIntoWindowRatio(Mat & be_show_mat, const string output_window_name);


static TCHAR my_window_class_1_name[] = _T("DesktopApp");        // The name of main window class .
static TCHAR MainWindowsTitle[] = _T("Win32_and_Opencv_APP");   // The string that appears in the application's title bar.
HWND hwnd_myclass_main_window;
HWND hwnd_edit_for_choose_path_file;
HWND hwnd_button_for_choose_path_file;
HWND hwnd_button_for_read_path_file;
HWND hwnd_myclass_display_window_1;
string gs_opencv_display_windows_names = "test_display";
int g_display_window_width = 500, g_display_window_height = 600;
HWND hwnd_edit_for_output_1;
HWND hwnd_edit_for_input_1;
HWND hwnd_button_for_input_1;
HWND hwnd_button_for_cut_image;

string target_file_path_name;
Mat my_image;
Mat cut_img;
tesseract::TessBaseAPI * api = new tesseract::TessBaseAPI();

//
//int main() {
//	const char * read_char = "C:\\Users\\t6.tiff";
//	my_image = imread("C:\\Users\\t6.tiff", 0);
//	cout << my_image.empty() << endl;
//	string choose_file;
//
//	//GetOpeningFilePathName(choose_file, "3");
//
//	Mat read_img = imread(choose_file, 0);
//
//
//	system("pause");
//}




int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{

	



	WNDCLASSEX windwo_class_ex_o1;
	MyInitializeWndclassex(windwo_class_ex_o1, hInstance);
	MyCreateAllWindows(hInstance);


	if (api->Init(NULL, "chi_tra")) {
		cout << "line39" << endl;
		fprintf(stderr, "Could not initialize tesseract.\n");
		system("pause");
		exit(1);
	}


	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hwnd_myclass_main_window, nCmdShow);
	UpdateWindow(hwnd_myclass_main_window);


	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;

}




void MyInitializeWndclassex(WNDCLASSEX & input_WNDCLASSEX, HINSTANCE & input_hInstance) {
	input_WNDCLASSEX.cbSize = sizeof(WNDCLASSEX);
	input_WNDCLASSEX.style = CS_HREDRAW | CS_VREDRAW;
	input_WNDCLASSEX.lpfnWndProc = WindowsProcess;
	input_WNDCLASSEX.cbClsExtra = 0;
	input_WNDCLASSEX.cbWndExtra = 0;
	input_WNDCLASSEX.hInstance = input_hInstance;
	input_WNDCLASSEX.hIcon = LoadIcon(input_hInstance, IDI_APPLICATION);
	input_WNDCLASSEX.hCursor = LoadCursor(NULL, IDC_ARROW);
	input_WNDCLASSEX.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	input_WNDCLASSEX.lpszMenuName = NULL;
	input_WNDCLASSEX.lpszClassName = my_window_class_1_name;
	input_WNDCLASSEX.hIconSm = LoadIcon(input_WNDCLASSEX.hInstance, IDI_APPLICATION);

	// register the main windows
	if (!RegisterClassEx(&input_WNDCLASSEX)){
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Windows Desktop Guided Tour"), NULL);
		exit(EXIT_SUCCESS);
	}
}


LRESULT CALLBACK WindowsProcess(
	HWND hWnd,  //A handle to the window.
	UINT message,  //For lists of the system-provided messages, https://docs.microsoft.com/en-us/windows/desktop/winmsg/about-messages-and-message-queues#system_defined
	WPARAM wParam,
	LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	

	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
		if (HWND(lParam) == hwnd_button_for_input_1){
			string edit_for_input_1_appear = GetStringFromEdit(hwnd_edit_for_input_1);
			
			RunMyProcessFunction(edit_for_input_1_appear);
			//SendMessage(test_child_output_handle_1, WM_SETTEXT, 1, (LPARAM)test_child_output_handle_1_text.c_str());
			
			InvalidateRect(hWnd, NULL, TRUE);
		}
		if (HWND(lParam) == hwnd_button_for_choose_path_file) {
			target_file_path_name = GetOpeningFilePathNameByWindows();
			SendMessage(hwnd_edit_for_choose_path_file, WM_SETTEXT, 1, (LPARAM)target_file_path_name.c_str());
			SendMessage(hwnd_edit_for_output_1, EM_REPLACESEL, 1, (LPARAM)(target_file_path_name+"\r\n").c_str());
		}
		if (HWND(lParam) == hwnd_button_for_read_path_file) {
			my_image=imread(target_file_path_name);
			if (my_image.empty()) { SendMessage(hwnd_edit_for_output_1, EM_REPLACESEL, 1, (LPARAM)"reading fail\r\n"); break; }
			
			SendMessage(hwnd_edit_for_output_1, EM_REPLACESEL, 1, (LPARAM)"read file success\r\n");
			
			ShowImageIntoWindowRatio(my_image, gs_opencv_display_windows_names);
			::MoveWindow(hwnd_myclass_display_window_1, 20, 20, g_display_window_width, g_display_window_height, 0);
			SendMessage(hwnd_edit_for_output_1, EM_REPLACESEL, 1, (LPARAM)"show file success\r\n");
			
		}
		if (HWND(lParam) == hwnd_button_for_cut_image) {
			if (my_image.empty()) { SendMessage(hwnd_edit_for_output_1, EM_REPLACESEL, 1, (LPARAM)"reading fail\r\n"); break; }

			int  x = 100, w = 500, y = 950, h = 100;

			//Mat cut_img = my_image[y:y + h, x : x + w];
			cut_img = my_image(Rect(x, y, w, h));

			ShowImageIntoWindowRatio(cut_img, gs_opencv_display_windows_names);
			::MoveWindow(hwnd_myclass_display_window_1, 20, 20, g_display_window_width, g_display_window_height, 0);
			SendMessage(hwnd_edit_for_output_1, EM_REPLACESEL, 1, (LPARAM)"cut_image file success\r\n");

		}
		return 0;
	case WM_PAINT:
	
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}









void MyCreateAllWindows(HINSTANCE & hInstance) {
	// MyCreate_handle_myclass_main_window
	if (1) {
		hwnd_myclass_main_window = CreateWindow(
			my_window_class_1_name,       //the name of the application
			MainWindowsTitle,      //the name appeared on the title in this window
			WS_OVERLAPPEDWINDOW, //The style of the window being created. This parameter can be a combination of the window style values. https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-styles
			CW_USEDEFAULT, CW_USEDEFAULT, // the position of windows 
			1200, 700,   //the size of the window being created
			NULL,     //the parent of this window
			NULL,     //this application does not have a menu bar
			hInstance, //the first parameter from WinMain
			NULL       //not used in this application
		);
	}
	// MyCreate_hwnd_edit_for_choosing_path_file
	if (1) {
		string edit_for_choose_path_file_appear = " nuchoose file ";
		hwnd_edit_for_choose_path_file = CreateWindow(
			_T("EDIT"),
			_T(edit_for_choose_path_file_appear.c_str()),
			WS_CHILD | WS_BORDER | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE,
			20, 0, 400, 20,
			hwnd_myclass_main_window, NULL, hInstance, NULL
		);
		SendMessage(hwnd_edit_for_choose_path_file, ES_READONLY, 0, 0);
	}
	// MyCreate_hwnd_button_for_choose_path_file
	if (1) {
		string button_for_choose_path_file_appear = "open";
		hwnd_button_for_choose_path_file = CreateWindow(
			_T("BUTTON"),
			_T(button_for_choose_path_file_appear.c_str()),
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			420, 0, 40, 20,
			hwnd_myclass_main_window, NULL, hInstance, NULL
		);
		SendMessage(hwnd_button_for_choose_path_file, ES_READONLY, 0, 0);
	}
	// MyCreate_hwnd_button_for_read_path_file
	if (1) {
		string button_for_read_path_file_appear = "read";
		hwnd_button_for_read_path_file = CreateWindow(
			_T("BUTTON"),
			_T(button_for_read_path_file_appear.c_str()),
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			460, 0, 40, 20,
			hwnd_myclass_main_window, NULL, hInstance, NULL
		);
		SendMessage(hwnd_button_for_read_path_file, ES_READONLY, 0, 0);
	}
	// MyCreat_hwnd_myclass_display_window_1
	if (0) {
		string myclass_display_window_1_appear = "tdwh1t: **** ";
		hwnd_myclass_display_window_1 = CreateWindow(
			my_window_class_1_name,
			_T(myclass_display_window_1_appear.c_str()),
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			20, 20, g_display_window_width, g_display_window_height,
			hwnd_myclass_main_window, NULL, hInstance, NULL
		);
		SendMessage(hwnd_myclass_display_window_1, ES_READONLY, 0, 0);
	}
	else if (1) {
		namedWindow(gs_opencv_display_windows_names, WINDOW_AUTOSIZE);
		hwnd_myclass_display_window_1 = ::FindWindow(NULL, gs_opencv_display_windows_names.c_str());
		if (hwnd_myclass_display_window_1 == 0) { SendMessage(hwnd_edit_for_output_1, WM_SETTEXT, 1, (LPARAM)"find window fail\r\n"); return; }
		if (SetParent(hwnd_myclass_display_window_1, hwnd_myclass_main_window) == 0) { SendMessage(hwnd_edit_for_output_1, WM_SETTEXT, 1, (LPARAM)"SetParent fail\r\n"); return; }
		//::SetWindowPos(opencv_window_handle, HWND_TOPMOST, 0, 0, g_display_window_width, g_display_window_height, SWP_NOSIZE | SWP_NOMOVE);
		SetWindowLong(hwnd_myclass_display_window_1, GWL_STYLE, WS_BORDER | WS_VISIBLE);
		::MoveWindow(hwnd_myclass_display_window_1, 20, 20, g_display_window_width, g_display_window_height, 0);
	}
	// MyCreat_hwnd_edit_for_output_1
	if (1) {
		string edit_for_output_1_appear = "initialize display window success\r\n";
		hwnd_edit_for_output_1 = CreateWindow(
			_T("EDIT"),
			_T(edit_for_output_1_appear.c_str()),
			WS_CHILD | WS_BORDER | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE,
			540, 540, 500, 100,
			hwnd_myclass_main_window, NULL, hInstance, NULL
		);
		SendMessage(hwnd_edit_for_output_1, ES_READONLY, 0, 0);
	}
	// MyCreat_hwnd_edit_for_input_1
	if (1) {
		string edit_for_input_1_appear = "tcih1t: **** ";
		hwnd_edit_for_input_1 = CreateWindow(
			_T("EDIT"),
			_T(edit_for_input_1_appear.c_str()),
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			540, 0, 500, 100,
			hwnd_myclass_main_window, NULL, hInstance, NULL
		);
		SendMessage(hwnd_edit_for_input_1, ES_READONLY, 0, 0);
	}
	// MyCreat_hwnd_button_for_input_1
	if (1) {
		string button_for_input_1_appear = "tcib1t: **** ";
		hwnd_button_for_input_1 = CreateWindow(
			_T("BUTTON"),
			_T(button_for_input_1_appear.c_str()),
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			1060, 0, 100, 100,
			hwnd_myclass_main_window, NULL, hInstance, NULL
		);
		SendMessage(hwnd_button_for_input_1, ES_READONLY, 0, 0);
	}
	//hwnd_button_for_cut_image
	if (1) {
		string button_for_cut_image_appear = "cut_image ";
		hwnd_button_for_cut_image = CreateWindow(
			_T("BUTTON"),
			_T(button_for_cut_image_appear.c_str()),
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			1060, 100, 100, 100,
			hwnd_myclass_main_window, NULL, hInstance, NULL
		);
		SendMessage(hwnd_button_for_cut_image, ES_READONLY, 0, 0);
	
	}

	


	//check hundle_main_window
	if (!hwnd_myclass_main_window) {
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Windows Desktop Guided Tour"), NULL);
		exit(0);
	}
}

void RunMyProcessFunction(const string & input_text) {

	imwrite("output.tiff", cut_img);
	//return;

	/*read into tesseract working*/
	Pix * image; 
	image = pixRead("output.tiff");
	api->SetImage(image);

	Boxa* boxes = api->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);
	//printf("Found %d textline image components.\n", boxes->n);
	for (int i = 0; i < boxes->n; i++) {
		BOX* box = boxaGetBox(boxes, i, L_CLONE);
		api->SetRectangle(box->x, box->y, box->w, box->h);
		char* ocrResult = api->GetUTF8Text();
		int conf = api->MeanTextConf();
		/*fprintf(stdout, "Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s",
			i, box->x, box->y, box->w, box->h, conf, ocrResult);*/
		MessageBox(NULL, (LPCTSTR)ocrResult, NULL, MB_OK);
	}
	
	
	
	vector<string> output_vector_string;
	MessageBox(NULL, (LPCTSTR)"end my button", NULL, MB_OK);
}

void ShowImageIntoWindowRatio(cv::Mat & be_show_mat, const string output_window_name) {
	cv::Mat show_resize_mat;
	PutImageIntoWindowRatio(be_show_mat, show_resize_mat, g_display_window_width, g_display_window_height);
	cv::imshow(output_window_name, show_resize_mat);
}


BOOL LoadBitmapFromBMPFile(LPTSTR szFileName, HBITMAP *phBitmap, HPALETTE *phPalette)
{

	BITMAP  bm;

	*phBitmap = NULL;
	*phPalette = NULL;

	// Use LoadImage() to get the image loaded into a DIBSection
	*phBitmap = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (*phBitmap == NULL)
		return FALSE;

	// Get the color depth of the DIBSection
	GetObject(*phBitmap, sizeof(BITMAP), &bm);
	// If the DIBSection is 256 color or less, it has a color table
	if ((bm.bmBitsPixel * bm.bmPlanes) <= 8)
	{
		HDC           hMemDC;
		HBITMAP       hOldBitmap;
		RGBQUAD       rgb[256];
		LPLOGPALETTE  pLogPal;
		WORD          i;

		// Create a memory DC and select the DIBSection into it
		hMemDC = CreateCompatibleDC(NULL);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, *phBitmap);
		// Get the DIBSection's color table
		GetDIBColorTable(hMemDC, 0, 256, rgb);
		// Create a palette from the color tabl
		pLogPal = (LOGPALETTE *)malloc(sizeof(LOGPALETTE) + (256 * sizeof(PALETTEENTRY)));
		pLogPal->palVersion = 0x300;
		pLogPal->palNumEntries = 256;
		for (i = 0; i < 256; i++)
		{
			pLogPal->palPalEntry[i].peRed = rgb[i].rgbRed;
			pLogPal->palPalEntry[i].peGreen = rgb[i].rgbGreen;
			pLogPal->palPalEntry[i].peBlue = rgb[i].rgbBlue;
			pLogPal->palPalEntry[i].peFlags = 0;
		}
		*phPalette = CreatePalette(pLogPal);
		// Clean up
		free(pLogPal);
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}
	else   // It has no color table, so use a halftone palette
	{
		HDC    hRefDC;

		hRefDC = GetDC(NULL);
		*phPalette = CreateHalftonePalette(hRefDC);
		ReleaseDC(NULL, hRefDC);
	}
	return TRUE;

}
