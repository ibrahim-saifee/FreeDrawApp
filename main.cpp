#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <stdio.h>
#include <sstream>

#define BITMAPLIMIT 10
#define POINTLIMIT 2000

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

struct COORDINATE
{int x;int y;BOOL flag;int PS;};
COORDINATE pt[POINTLIMIT];

struct BITMAPS
{HBITMAP hBitmap; int xBitmap; int yBitmap; int widthBitmap; int heightBitmap; int hSize; int vSize;};
BITMAPS bitmaps[BITMAPLIMIT];

int bDraw;


HANDLE hSerial;

HWND hwndParent,hwndChild;

HINSTANCE hinstance;

int iPenSelect;

int count;

COLORREF clr;

HBRUSH hBackgroundBrush;

int min(int a, int b)
{if(a>=b)return b;
else return a;
}

int max(int a, int b)
{if(a>=b)return a;
else return b;
}

int mod(int a)
{if(a<0)a=-a;
return a;
}

#include"child.h"



int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
                 /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
hinstance=hThisInstance;
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS|CS_VREDRAW|CS_HREDRAW;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (hinstance, MAKEINTRESOURCE(102));
    wincl.hIconSm = LoadIcon (hinstance, MAKEINTRESOURCE(102));
    wincl.hCursor = LoadCursor (hThisInstance, MAKEINTRESOURCE(100));
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    hBackgroundBrush=CreateSolidBrush(RGB(1,1,255));
    wincl.hbrBackground = hBackgroundBrush;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    
    /* The class is registered, let's create the program*/
    hwndParent = CreateWindowEx (
           WS_EX_LAYERED|WS_EX_TOOLWINDOW,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Projector App",       /* Title Text */
           WS_POPUP|WS_CLIPCHILDREN,//|WS_CAPTION|WS_SYSMENU, /* default window */
           -10,       /* Windows decides the position */
           -15,       /* where the window ends up on the screen */
           1500,                 /* The programs width */
           900,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwndParent,1);
    UpdateWindow(hwndParent);
    SetLayeredWindowAttributes(hwndParent,RGB(1,1,255),0,1);
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (1)
    {
        if(!GetMessage (&messages, NULL, 0, 0))break;
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
      /*  if(GetKeyState(VK_LBUTTON)<=0)SendMessage(hwnd,WM_LBUTTONDOWN,NULL,NULL);
        else SendMessage(hwnd,WM_LBUTTONUP,NULL,NULL);
        SendMessage(hwnd,WM_MOUSEMOVE,NULL,NULL);*/
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
static BOOL draw,hold,horSize,verSize,FroRar;
POINT cpt;
HDC hdc,hdcmem,hdctemp,hdcReal;
HBITMAP hBufferBitmap;
PAINTSTRUCT ps;
static int cxSource,cySource;
static HPEN hPen[7],hSelectPen;
static int xDiff,yDiff;
int ex,ey;
int i,j;
static int x1Select,y1Select,x2Select,y2Select;
RECT rc;
static int iBitmapCount,iBitmapSelect;
POINT tpt1,tpt2;

    switch (message)                  /* handle the messages */
    {

        case WM_CREATE:
             //SetTimer(hwnd,1,5000,NULL);
             hPen[0]=CreatePen(PS_SOLID,3,RGB(255,0,0));
             hPen[1]=CreatePen(PS_SOLID,3,RGB(0,255,0));
             hPen[2]=CreatePen(PS_SOLID,3,RGB(0,0,255));
             hPen[3]=CreatePen(PS_SOLID,3,RGB(255,255,0));
             hPen[4]=CreatePen(PS_SOLID,3,RGB(0,0,0));
             hPen[5]=CreatePen(PS_SOLID,3,RGB(226,78,189));
             hPen[6]=CreatePen(PS_SOLID,3,clr);
             hSelectPen=CreatePen(PS_DASH,1,RGB(0,0,0));
             CreateChild();
             MoveWindow(hwndChild,1100,500,250,250,TRUE);
             //ShowWindow(hwnd,SW_MAXIMIZE);
             break;
      /*  case WM_TIMER:
             if(SeeThrough)
             {
             SetWindowLong(hwnd,GWL_EXSTYLE,WS_EX_TOPMOST|WS_EX_LAYERED);
             SetLayeredWindowAttributes(hwnd,RGB(1,1,255),0,1);
             SeeThrough=0;
             }
             break;*/
        case WM_RBUTTONDOWN:
             ex=LOWORD(lParam);ey=HIWORD(lParam);
             //for(i=0;i<iBitmapCount;i++)
             for(i=iBitmapCount-1;i>=0;i--)
            {
            if((ex>bitmaps[i].xBitmap && ex<bitmaps[i].xBitmap+bitmaps[i].hSize) && (ey>bitmaps[i].yBitmap && ey<bitmaps[i].yBitmap+bitmaps[i].vSize))
              {DeleteObject(bitmaps[i].hBitmap);
               for(j=i;j<iBitmapCount;j++)
               {bitmaps[j].hBitmap=bitmaps[j+1].hBitmap;
                bitmaps[j].xBitmap=bitmaps[j+1].xBitmap;bitmaps[j].yBitmap=bitmaps[j+1].yBitmap;
                bitmaps[j].widthBitmap=bitmaps[j+1].widthBitmap;bitmaps[j].heightBitmap=bitmaps[j+1].heightBitmap;
                bitmaps[j].hSize=bitmaps[j+1].hSize; bitmaps[j].vSize=bitmaps[j+1].vSize;
               }iBitmapCount--;break;
              }
            }
            InvalidateRect(hwnd,NULL,TRUE);
            break;
        case WM_LBUTTONDOWN:
            SetCapture(hwnd);
            ex=LOWORD(lParam);ey=HIWORD(lParam);
            
            if(bDraw==3)
            {hdc=GetDC(NULL);clr=GetPixel(hdc,ex-5,ey-17);
             DeleteObject(hPen[6]);hPen[6]=CreatePen(PS_SOLID,3,clr);
             ReleaseDC(NULL,hdc);
             InvalidateRect(hwndChild,NULL,TRUE);
             return 0;
            }
            
            if(bDraw==2)
            for(i=iBitmapCount-1;i>=0;i--)
            {
             if(ex>bitmaps[i].xBitmap && ex<bitmaps[i].xBitmap+bitmaps[i].hSize)
             {
              if(ey>bitmaps[i].yBitmap-5 && ey<bitmaps[i].yBitmap+5)
              {verSize=1;FroRar=0;SetCursor(LoadCursor(hinstance,MAKEINTRESOURCE(106)));iBitmapSelect=i;return 0;}
              if(ey>bitmaps[i].yBitmap+bitmaps[i].vSize-5 && ey<bitmaps[i].yBitmap+bitmaps[i].vSize+5)
              {verSize=1;FroRar=1;SetCursor(LoadCursor(hinstance,MAKEINTRESOURCE(106)));iBitmapSelect=i;return 0;}
             }
             
             if(ey>bitmaps[i].yBitmap && ey<bitmaps[i].yBitmap+bitmaps[i].vSize)
             {
              if(ex>bitmaps[i].xBitmap-5 && ex<bitmaps[i].xBitmap+5)
              {horSize=1;FroRar=0;SetCursor(LoadCursor(hinstance,MAKEINTRESOURCE(105)));iBitmapSelect=i; return 0;}
              if(ex>bitmaps[i].xBitmap+bitmaps[i].hSize-5 && ex<bitmaps[i].xBitmap+bitmaps[i].hSize+5)
              {horSize=1;FroRar=1;SetCursor(LoadCursor(hinstance,MAKEINTRESOURCE(105)));iBitmapSelect=i; return 0;}
             }
             
             if((ex>bitmaps[i].xBitmap && ex<bitmaps[i].xBitmap+bitmaps[i].hSize) && (ey>bitmaps[i].yBitmap && ey<bitmaps[i].yBitmap+bitmaps[i].vSize))
             {hold=1;SetCursor(LoadCursor(hinstance,MAKEINTRESOURCE(104)));
              xDiff=ex-bitmaps[i].xBitmap; yDiff=ey-bitmaps[i].yBitmap;
              iBitmapSelect=i;return 0;
             }
            }
            
            if((ex>min(x1Select,x2Select) && ex<max(x1Select,x2Select)) && (ey>min(y1Select,y2Select) && ey<max(y1Select,y2Select)))
            {hold=1;
            hdctemp=GetDC(NULL);hdc=GetDC(hwnd);
            hdcmem=CreateCompatibleDC(hdc);
            bitmaps[iBitmapCount].widthBitmap=max(x1Select,x2Select)-min(x1Select,x2Select)-4;
            bitmaps[iBitmapCount].heightBitmap=max(y1Select,y2Select)-min(y1Select,y2Select)-4;
            bitmaps[iBitmapCount].hSize=bitmaps[iBitmapCount].widthBitmap;
            bitmaps[iBitmapCount].vSize=bitmaps[iBitmapCount].heightBitmap;
            bitmaps[iBitmapCount].hBitmap=CreateCompatibleBitmap(hdc,bitmaps[iBitmapCount].widthBitmap,bitmaps[iBitmapCount].heightBitmap);
            SelectObject(hdcmem,bitmaps[iBitmapCount].hBitmap);
            bitmaps[iBitmapCount].xBitmap=min(x1Select,x2Select)+2;
            bitmaps[iBitmapCount].yBitmap=min(y1Select,y2Select)+2;
            xDiff=ex-bitmaps[iBitmapCount].xBitmap; yDiff=ey-bitmaps[iBitmapCount].yBitmap;
            tpt1.x=min(x1Select,x2Select)+2;tpt1.y=min(y1Select,y2Select)+2;
            tpt2.x=max(x1Select,x2Select)-2;tpt2.y=max(y1Select,y2Select)-2;
            ClientToScreen(hwnd,&tpt1);ClientToScreen(hwnd,&tpt2);
            //ex=ex-bitmaps[iBitmapCount].xBitmap;
            //ey=ey-bitmaps[iBitmapCount].yBitmap;
            BitBlt(hdcmem,0,0,tpt2.x,tpt2.y,hdctemp,tpt1.x,tpt1.y,SRCCOPY);
            ReleaseDC(NULL,hdctemp);ReleaseDC(hwnd,hdc);
            DeleteDC(hdcmem);
            iBitmapSelect=iBitmapCount;
            iBitmapCount++;  if(iBitmapCount>BITMAPLIMIT-1)iBitmapCount=0;
            InvalidateRect(hwnd,NULL,TRUE);
            x1Select=LOWORD(lParam);y1Select=HIWORD(lParam);
            x2Select=x1Select;y2Select=y1Select;
            }
            else
            {
            x1Select=LOWORD(lParam);y1Select=HIWORD(lParam);
            x2Select=x1Select;y2Select=y1Select;
            InvalidateRect(hwnd,NULL,TRUE);
            if(draw==0){
                        if(bDraw==1)
                        {
                        //GetCursorPos(&cpt);
                        pt[count].x=LOWORD(lParam);
                        pt[count].y=HIWORD(lParam);
                        pt[count].flag=1;
                        count++;
                        }
                        if(bDraw==0)
                        {
                           ex=LOWORD(lParam);
                           ey=HIWORD(lParam);
                           rc.left=ex-15;rc.right=ex+15;
                           rc.top=ey-15;rc.bottom=ey+15;
                           for(i=0;i<count-1;i++)
                           {if((pt[i].x>(ex-15) && pt[i].x<(ex+15)) && (pt[i].y>(ey-15) && pt[i].y<(ey+15)))
                              {pt[i].flag=0;
                              for(j=i;j<count-1;j++)
                              {pt[j].x=pt[j+1].x;pt[j].y=pt[j+1].y;pt[j].flag=pt[j+1].flag;pt[j].PS=pt[j+1].PS;}
                              pt[j].flag=0;count--;
                               InvalidateRect(hwnd,&rc,TRUE);
                              }
                           }
                        }
                        if(bDraw==2)
                        {
                          x1Select=LOWORD(lParam);
                          y1Select=HIWORD(lParam);
                        }
                        draw=1;
                        }
            }
            break;

        case WM_MOUSEMOVE:
          if(draw==1)
          {   
             if(bDraw==1)
             {
                         pt[count].x=LOWORD(lParam);
                         pt[count].y=HIWORD(lParam);
                         pt[count].flag=1;
                         pt[count].PS=iPenSelect;
                         count++;
                         InvalidateRect(hwnd,NULL,FALSE);
             }
             if(bDraw==0)
             {
                ex=LOWORD(lParam);
                ey=HIWORD(lParam);
                rc.left=ex-15;rc.right=ex+15;
                rc.top=ey-15;rc.bottom=ey+15;
                for(int i=0;i<count-1;i++)
                {if((pt[i].x>(ex-15) && pt[i].x<(ex+15)) && (pt[i].y>(ey-15) && pt[i].y<(ey+15)))
                  {pt[i].flag=0; 
                  InvalidateRect(hwnd,&rc,TRUE);
                  }
                }  
             }
             if(bDraw==2)
             {
               x2Select=LOWORD(lParam);
               y2Select=HIWORD(lParam); 
               rc.left=min(x1Select,x2Select)-30;rc.right=max(x1Select,x2Select)+30;
               rc.top=min(y1Select,y2Select)-30;rc.bottom=max(y1Select,y2Select)+30;
               InvalidateRect(hwnd,&rc,TRUE);
             }
          }
          else
          {if(bDraw==2)
            {ex=LOWORD(lParam);ey=HIWORD(lParam);
             if((ex>min(x1Select,x2Select) && ex<max(x1Select,x2Select)) && (ey>min(y1Select,y2Select) && ey<max(y1Select,y2Select)))
             SetCursor(LoadCursor(hinstance,MAKEINTRESOURCE(104)));
             for(i=0;i<iBitmapCount;i++)
             {
             if((ex>bitmaps[i].xBitmap && ex<bitmaps[i].xBitmap+bitmaps[i].hSize) && (ey>bitmaps[i].yBitmap && ey<bitmaps[i].yBitmap+bitmaps[i].vSize))
             SetCursor(LoadCursor(hinstance,MAKEINTRESOURCE(104)));
             if((ex>bitmaps[i].xBitmap && ex<bitmaps[i].xBitmap+bitmaps[i].hSize) && ((ey>bitmaps[i].yBitmap-5 && ey<bitmaps[i].yBitmap+5) || (ey>bitmaps[i].yBitmap+bitmaps[i].vSize-5 && ey<bitmaps[i].yBitmap+bitmaps[i].vSize+5)))
             SetCursor(LoadCursor(hinstance,MAKEINTRESOURCE(106)));
             if((ey>bitmaps[i].yBitmap && ey<bitmaps[i].yBitmap+bitmaps[i].vSize) && ((ex>bitmaps[i].xBitmap-5 && ex<bitmaps[i].xBitmap+5) || (ex>bitmaps[i].xBitmap+bitmaps[i].hSize-5 && ex<bitmaps[i].xBitmap+bitmaps[i].hSize+5)))
             SetCursor(LoadCursor(hinstance,MAKEINTRESOURCE(105)));
             }
            }
          }
          if(hold==1)
          {rc.left=bitmaps[iBitmapSelect].xBitmap-2;rc.right=bitmaps[iBitmapSelect].xBitmap+bitmaps[iBitmapSelect].hSize+2;
           rc.top=bitmaps[iBitmapSelect].yBitmap-2;rc.bottom=bitmaps[iBitmapSelect].yBitmap+bitmaps[iBitmapSelect].vSize+2;
            
            bitmaps[iBitmapSelect].xBitmap=LOWORD(lParam)-xDiff;
            bitmaps[iBitmapSelect].yBitmap=HIWORD(lParam)-yDiff;
            InvalidateRect(hwnd,&rc,TRUE);
          }
          
          if(horSize==1)
          {rc.left=bitmaps[iBitmapSelect].xBitmap-2;rc.right=bitmaps[iBitmapSelect].xBitmap+bitmaps[iBitmapSelect].hSize+2;
           rc.top=bitmaps[iBitmapSelect].yBitmap-2;rc.bottom=bitmaps[iBitmapSelect].yBitmap+bitmaps[iBitmapSelect].vSize+2;
           if(FroRar==1)
           bitmaps[iBitmapSelect].hSize=LOWORD(lParam)-bitmaps[iBitmapSelect].xBitmap;
           else
             {bitmaps[iBitmapSelect].hSize=bitmaps[iBitmapSelect].xBitmap+bitmaps[iBitmapSelect].hSize-LOWORD(lParam);
              bitmaps[iBitmapSelect].xBitmap=LOWORD(lParam);}
           InvalidateRect(hwnd,&rc,TRUE);
          }
          
          if(verSize==1)
          {
           rc.left=bitmaps[iBitmapSelect].xBitmap-2;rc.right=bitmaps[iBitmapSelect].xBitmap+bitmaps[iBitmapSelect].hSize+2;
           rc.top=bitmaps[iBitmapSelect].yBitmap-2;rc.bottom=bitmaps[iBitmapSelect].yBitmap+bitmaps[iBitmapSelect].vSize+2;
           if(FroRar==1)
           bitmaps[iBitmapSelect].vSize=HIWORD(lParam)-bitmaps[iBitmapSelect].yBitmap;
           else
             {bitmaps[iBitmapSelect].vSize=bitmaps[iBitmapSelect].yBitmap+bitmaps[iBitmapSelect].vSize-HIWORD(lParam);
              bitmaps[iBitmapSelect].yBitmap=HIWORD(lParam);}
           InvalidateRect(hwnd,&rc,TRUE);
          }
             break;
        case WM_LBUTTONUP:
             pt[count].flag=0;
             count++;
             draw=0;
             hold=0;
             verSize=0;horSize=0;
            ReleaseCapture();
             break;
        case WM_ERASEBKGND:
             break;
        case WM_PAINT:
               GetClientRect(hwnd,&rc);
               hdcReal=BeginPaint(hwnd,&ps);
               hdc=CreateCompatibleDC(hdcReal);
               hBufferBitmap=CreateCompatibleBitmap(hdcReal,rc.right,rc.bottom);
               SelectObject(hdc,hBufferBitmap);
               FillRect(hdc,&rc,hBackgroundBrush);
               hdcmem=CreateCompatibleDC(hdc);
              if(bDraw==2)
              {SelectObject(hdc,hSelectPen);
              SelectObject(hdc,GetStockObject(NULL_BRUSH));
              Rectangle(hdc,x1Select,y1Select,x2Select,y2Select);
              }
              SetStretchBltMode(hdc,COLORONCOLOR);
             for(i=0;i<iBitmapCount;i++)
             {
               SelectObject(hdcmem,bitmaps[i].hBitmap);
               //BitBlt(hdc,bitmaps[i].xBitmap,bitmaps[i].yBitmap,bitmaps[i].hSize,bitmaps[i].vSize,hdcmem,0,0,SRCCOPY);
               StretchBlt(hdc,bitmaps[i].xBitmap,bitmaps[i].yBitmap,bitmaps[i].hSize,bitmaps[i].vSize,hdcmem,0,0,bitmaps[i].widthBitmap,bitmaps[i].heightBitmap,SRCCOPY);
               SelectObject(hdc,GetStockObject(NULL_BRUSH));
               SelectObject(hdc,hSelectPen);
               Rectangle(hdc,bitmaps[i].xBitmap,bitmaps[i].yBitmap,bitmaps[i].xBitmap+bitmaps[i].hSize,bitmaps[i].yBitmap+bitmaps[i].vSize);
             } 
             for(i=0;i<count-1;i++)
             {
             SelectObject(hdc,hPen[pt[i].PS]);
             if(pt[i].flag && pt[i+1].flag)
             {
             MoveToEx(hdc,pt[i].x,pt[i].y,0);
             LineTo(hdc,pt[i+1].x,pt[i+1].y);
             }
             }
             DeleteDC(hdcmem);
             BitBlt(hdcReal,0,0,rc.right,rc.bottom,hdc,0,0,SRCCOPY);
             DeleteDC(hdc);
             DeleteObject(hBufferBitmap);
             EndPaint(hwnd,&ps);
             break;
        case WM_DESTROY:
             for(int temp=0;temp<7;temp++)
             DeleteObject(hPen[temp]);
             DeleteDC(hdcmem);
             DeleteObject(hSelectPen);
             DeleteObject(hBackgroundBrush);
            //KillTimer(hwnd,1);
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}



