void CreateChild()
{
     WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hinstance;
    wincl.lpszClassName = "Child";
    wincl.lpfnWndProc = ChildProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS|CS_VREDRAW|CS_HREDRAW;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (hinstance, MAKEINTRESOURCE(102));
    wincl.hIconSm = LoadIcon (hinstance, MAKEINTRESOURCE(102));
    wincl.hCursor = LoadCursor (NULL,IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    
    wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    RegisterClassEx (&wincl);
    
    /* The class is registered, let's create the program*/
    hwndChild = CreateWindowEx (
           WS_EX_LAYERED|WS_EX_TOPMOST,                   /* Extended possibilites for variation */
           "Child",         /* Classname */
           "Controls",       /* Title Text */
           WS_OVERLAPPED, /* default window */
           1200,       /* Windows decides the position */
           600,       /* where the window ends up on the screen */
           250,                 /* The programs width */
           250,                 /* and height in pixels */
           NULL,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hinstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwndChild,SW_MAXIMIZE);
    SetLayeredWindowAttributes(hwndChild,RGB(1,1,255),150,2);
}

LRESULT CALLBACK ChildProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static HWND hwndMin,hwndR,hwndG,hwndB,hwndY,hwndBl,hwndP,hwndPen,hwndErase,hwndClear,hwndClose,hwndCapture,hwndTest,hwndTestColor;
RECT rc;
static BOOL OnOff;
LPDRAWITEMSTRUCT pdis;
static HBRUSH hRed,hBlue,hGreen,hBlack,hYellow,hPink,hTestBrush;
static HPEN hFrame,hFrame1,hRedPen,hSelectPen;
static POINT pt[5];

        switch(message)
        {
        case WM_CREATE:
             hwndMin=CreateWindow(TEXT("button"),"OFF",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,0,0,80,20,hwnd,(HMENU)1,hinstance,NULL);
             hwndR=CreateWindow(TEXT("button"),"Red",WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,0,0,80,20,hwnd,(HMENU)2,hinstance,NULL);
             hwndG=CreateWindow(TEXT("button"),"Green",WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,0,0,80,20,hwnd,(HMENU)3,hinstance,NULL);
             hwndB=CreateWindow(TEXT("button"),"Blue",WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,0,0,80,20,hwnd,(HMENU)4,hinstance,NULL);
             hwndY=CreateWindow(TEXT("button"),"Yellow",WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,0,0,80,20,hwnd,(HMENU)5,hinstance,NULL);
             hwndBl=CreateWindow(TEXT("button"),"Black",WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,0,0,80,20,hwnd,(HMENU)6,hinstance,NULL);
             hwndP=CreateWindow(TEXT("button"),"Pink",WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,0,0,80,20,hwnd,(HMENU)10,hinstance,NULL);
             hwndPen=CreateWindow(TEXT("button"),"Pencil",WS_CHILD | WS_VISIBLE | BS_OWNERDRAW|BS_BITMAP,0,0,80,20,hwnd,(HMENU)7,hinstance,NULL);
             hwndErase=CreateWindow(TEXT("button"),"Erase",WS_CHILD | WS_VISIBLE | BS_OWNERDRAW|BS_BITMAP,0,0,80,20,hwnd,(HMENU)8,hinstance,NULL);
             hwndClear=CreateWindow(TEXT("button"),"Clear",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,0,0,80,20,hwnd,(HMENU)9,hinstance,NULL);
             hwndClose=CreateWindow(TEXT("button"),"Close",WS_CHILD | WS_VISIBLE | BS_OWNERDRAW|BS_BITMAP,0,0,80,20,hwnd,(HMENU)11,hinstance,NULL);
             hwndCapture=CreateWindow(TEXT("button"),"Capture",WS_CHILD | WS_VISIBLE | BS_OWNERDRAW|BS_BITMAP,0,0,80,20,hwnd,(HMENU)12,hinstance,NULL);
             hwndTest=CreateWindow(TEXT("button"),"Color",WS_CHILD | WS_VISIBLE | BS_OWNERDRAW|BS_ICON,0,0,80,20,hwnd,(HMENU)13,hinstance,NULL);
             hwndTestColor=CreateWindow(TEXT("button"),"TestColor",WS_CHILD | WS_VISIBLE | BS_OWNERDRAW|BS_BITMAP,0,0,80,20,hwnd,(HMENU)14,hinstance,NULL);
             
             hRed=CreateSolidBrush(RGB(255,0,0));
             hGreen=CreateSolidBrush(RGB(0,255,0));
             hBlue=CreateSolidBrush(RGB(0,0,255));
             hYellow=CreateSolidBrush(RGB(255,255,0));
             hBlack=CreateSolidBrush(RGB(0,0,0));
             hPink=CreateSolidBrush(RGB(226,78,189));
             hFrame=CreatePen(PS_SOLID,4,RGB(0,0,0));
             hFrame1=CreatePen(PS_SOLID,4,RGB(255,255,255));
             hRedPen=CreatePen(PS_SOLID,4,RGB(255,0,0));
             hSelectPen=CreatePen(PS_DOT,1,RGB(0,0,0));
             hTestBrush=CreateSolidBrush(clr);
             
             pt[0].x=20;pt[0].y=5;
             pt[1].x=25;pt[1].y=10;
             pt[2].x=15;pt[2].y=20;
             pt[3].x=7;pt[3].y=23;
             pt[4].x=10;pt[4].y=15;
             bDraw=1;
        break;
        case WM_DRAWITEM:
            pdis=(LPDRAWITEMSTRUCT)lParam;
            switch(pdis->CtlID)
            {case 2:
            FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)hRed);
            break;
            case 3:
            FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)hGreen);
            break;
            case 4:
            FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)hBlue);
            break;
            case 5:
            FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)hYellow);
            break;
            case 6:
            FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)hBlack);
            break;
            case 10:
            FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)hPink);
            break;
            case 11:
            SelectObject(pdis->hDC,hRedPen);
            MoveToEx(pdis->hDC,pdis->rcItem.left,pdis->rcItem.top,NULL);
            LineTo(pdis->hDC,pdis->rcItem.right,pdis->rcItem.bottom);
            MoveToEx(pdis->hDC,pdis->rcItem.right,pdis->rcItem.top,NULL);
            LineTo(pdis->hDC,pdis->rcItem.left,pdis->rcItem.bottom);
            SelectObject(pdis->hDC,hFrame1);
            SelectObject(pdis->hDC,GetStockObject(NULL_BRUSH));
            Rectangle(pdis->hDC,pdis->rcItem.left,pdis->rcItem.top,pdis->rcItem.right,pdis->rcItem.bottom);
            break;
            case 7:
            FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)GetStockObject(GRAY_BRUSH));
            SelectObject(pdis->hDC,GetStockObject(BLACK_BRUSH));
            Polygon(pdis->hDC,pt,5);
            break;
            case 8:
            FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)GetStockObject(GRAY_BRUSH));
            Rectangle(pdis->hDC,pdis->rcItem.left+5,pdis->rcItem.top+5,pdis->rcItem.right-5,pdis->rcItem.bottom-5);
            break;
            case 12:
            FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)GetStockObject(GRAY_BRUSH));
            SelectObject(pdis->hDC,GetStockObject(NULL_BRUSH));
            SelectObject(pdis->hDC,hSelectPen);
            Rectangle(pdis->hDC,pdis->rcItem.left+5,pdis->rcItem.top+5,pdis->rcItem.right-5,pdis->rcItem.bottom-5);
            break;
            case 13:
              //SendMessage(hwndTest,BM_SETIMAGE,(WPARAM)IMAGE_ICON,(LPARAM)LoadIcon(hinstance,MAKEINTRESOURCE(108)));
            break;
            case 14: 
               DeleteObject(hTestBrush);
               hTestBrush=CreateSolidBrush(clr);
               FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)hTestBrush);  
            break;
            }
            if(pdis->itemState & ODS_FOCUS)
            SelectObject(pdis->hDC,hFrame);
            
            if(pdis->itemState & ODS_SELECTED)
            SelectObject(pdis->hDC,hFrame1);
            
            SelectObject(pdis->hDC,GetStockObject(NULL_BRUSH));
            Rectangle(pdis->hDC,pdis->rcItem.left,pdis->rcItem.top,pdis->rcItem.right,pdis->rcItem.bottom);
        break;
        case WM_COMMAND:
          switch(LOWORD(wParam))
             {case 1:
             OnOff=~OnOff;
             if(OnOff){SetWindowText(hwndMin,"ON");SetWindowLong(hwndParent,GWL_EXSTYLE,WS_EX_LAYERED|WS_EX_TRANSPARENT|WS_EX_TOPMOST);}
             else {SetWindowText(hwndMin,"OFF");SetWindowLong(hwndParent,GWL_EXSTYLE,WS_EX_LAYERED);SetFocus(hwndParent);}
             SetLayeredWindowAttributes(hwndParent,RGB(1,1,255),0,1);
             break;
             case 2:
                  iPenSelect=0;
                  SetClassLong(hwndParent,GCL_HCURSOR,(LONG)LoadCursor(hinstance,MAKEINTRESOURCE(100)));
                  bDraw=1;
             break;
             case 3:
                  iPenSelect=1;
                  SetClassLong(hwndParent,GCL_HCURSOR,(LONG)LoadCursor(hinstance,MAKEINTRESOURCE(100)));
                  bDraw=1;
             break;
             case 4:
                  iPenSelect=2;
                  SetClassLong(hwndParent,GCL_HCURSOR,(LONG)LoadCursor(hinstance,MAKEINTRESOURCE(100)));
                  bDraw=1;
             break;
             case 5:
                  iPenSelect=3;
                  SetClassLong(hwndParent,GCL_HCURSOR,(LONG)LoadCursor(hinstance,MAKEINTRESOURCE(100)));
                  bDraw=1;
             break;
             case 6:
                  iPenSelect=4;
                  SetClassLong(hwndParent,GCL_HCURSOR,(LONG)LoadCursor(hinstance,MAKEINTRESOURCE(100)));
                  bDraw=1;
             break;
             case 10:
                 iPenSelect=5;
                  SetClassLong(hwndParent,GCL_HCURSOR,(LONG)LoadCursor(hinstance,MAKEINTRESOURCE(100)));
                  bDraw=1;
             break;
             case 11:
                  SendMessage(hwndChild,WM_CLOSE,NULL,NULL);
             break;
             case 7:
                  SetClassLong(hwndParent,GCL_HCURSOR,(LONG)LoadCursor(hinstance,MAKEINTRESOURCE(100)));
                  bDraw=1;
             break;
             case 8:
                  SetClassLong(hwndParent,GCL_HCURSOR,(LONG)LoadCursor(hinstance,MAKEINTRESOURCE(101)));
                  bDraw=0;
             break;
             case 9:
                  count=0;
                  InvalidateRect(hwndParent,NULL,TRUE);
             break;
             case 12:
                  SetClassLong(hwndParent,GCL_HCURSOR,(LONG)LoadCursor(hinstance,MAKEINTRESOURCE(103)));
                  bDraw=2;
             break;
             case 13:
                  SetClassLong(hwndParent,GCL_HCURSOR,(LONG)LoadCursor(hinstance,MAKEINTRESOURCE(107)));
                  bDraw=3;
             break;
             case 14:
                  iPenSelect=6;
                  SetClassLong(hwndParent,GCL_HCURSOR,(LONG)LoadCursor(hinstance,MAKEINTRESOURCE(100)));
                  bDraw=1;
             break;
             }
             break;  
        case WM_PAINT:
             DefWindowProc(hwnd,message,wParam,lParam);
             GetClientRect(hwnd,&rc);
             MoveWindow(hwndMin,10,180,80,20,TRUE);
             MoveWindow(hwndR,110,10,30,30,TRUE);
             MoveWindow(hwndG,160,10,30,30,TRUE);
             MoveWindow(hwndB,110,50,30,30,TRUE);
             MoveWindow(hwndY,160,50,30,30,TRUE);
             MoveWindow(hwndBl,110,90,30,30,TRUE);
             MoveWindow(hwndP,160,90,30,30,TRUE);
             MoveWindow(hwndPen,45,10,30,30,TRUE);
             MoveWindow(hwndErase,45,50,30,30,TRUE);
             MoveWindow(hwndClear,110,180,80,20,TRUE);
             MoveWindow(hwndClose,45,90,30,30,TRUE);
             MoveWindow(hwndCapture,45,130,30,30,TRUE);
             MoveWindow(hwndTest,110,130,30,30,TRUE);
             MoveWindow(hwndTestColor,160,130,30,30,TRUE);
             break;
        case WM_DESTROY:
             DeleteObject(hRed);DeleteObject(hGreen);
             DeleteObject(hBlue);DeleteObject(hYellow);
             DeleteObject(hBlack);DeleteObject(hFrame);
             DeleteObject(hFrame1);DeleteObject(hRedPen);
             DeleteObject(hTestBrush);
             SendMessage(hwndParent,WM_CLOSE,NULL,NULL);
        default:
                return DefWindowProc(hwnd,message,wParam,lParam);
        }
return 0;
}
