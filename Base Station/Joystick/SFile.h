
/*
Serial I/O class using the Win32 file funcions for compatibility with all
versions of Windows (including CE)
*/

#ifndef _SFILE_H
#define _SFILE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

//-----------------------------------------------------------------------------

#ifdef _WIN32_WCE
    BOOL BuildCommDCB (TCHAR *settings,DCB *dcb);
#endif

#define CBR_230400 230400 //they forgot this one

//-----------------------------------------------------------------------------


class SFile
{
    HANDLE hComm;       //handle to com
	int nComm;			//com number (ie 1 for COM1)
    UINT ReadTimeOut,   //milliseconds befor a ReadFile() times out
         ReadIter;      //number of ReadFile() iterations after a time out
    DWORD nBaud;        //current baud rate in bps

public:
    SFile () : hComm(INVALID_HANDLE_VALUE),nComm(0), ReadTimeOut(INFINITE),
                ReadIter(0), nBaud(0) {}
    void setBegining(){
         hComm=INVALID_HANDLE_VALUE;
         nComm=0;
         ReadTimeOut=INFINITE;
         ReadIter =0;
         nBaud =0;
    }

    virtual ~SFile () {Close();}

    //Open a com port at the given speed
    //  port - com port to open, 1=COM1, 2=COM2, etc...
    //  baud - initial baud rate for com in bps
    //Returns FALSE on failure - use GetLastError() for more info
    BOOL Open(UINT port, DWORD baud);

    //Close the com port
    void Close();

    //Is the com port open??
    BOOL IsOpen() {return hComm!=INVALID_HANDLE_VALUE;}

	//Get the com number for currently open com (ie 1 for COM1)
	int CommNumber() {return nComm;}

    //Set the read timeout (ms) and the number of read iterations. Read timeout
    // specifies the timeout time (in ms) for a ReadFile() operation. The
    //read iterations specifies the number of ReadFile() operations to perform
    //consecutively after a read timeout occurs.
    //The default timeout is INFINITE, meaning ReadFile() operations block
    //completely
    //NOTE: For WindowsCE, you must call this function before calling Open()
    void SetReadTimeOut(UINT rto, UINT ri=0) {ReadTimeOut=rto; ReadIter=ri;}

    //Read/Write on the com port
    //Returns FALSE on failure - use GetLastError() for more info
    BOOL Read(LPVOID lpvBuff, DWORD dwSize, DWORD &dwRead);
    BOOL Write(LPCVOID lpvBuff, DWORD dwSize, DWORD &dwWritten);

	//Read/Write bytes on com port
	//Returns FALSE on failure - use GetLastError() for more info
	BOOL ReadByte(BYTE &b);
	BOOL WriteByte(BYTE b);

    //Set the com event mask, you only need to set rxc if the event
    //mask contains EV_RXFLAG
    //Returns FALSE on failure - use GetLastError() for more info
    BOOL SetCommMask(DWORD dwEvtMask, BYTE rxc=0);

    //Wait for the event(s) specified by SetCommMask()
    //  timeout - timeout in ms
    //  com_ev - recieves the event that occured
    //Returns FALSE on failure - use GetLastError() for more info
    BOOL WaitCommEvent(UINT timeout, DWORD *com_ev = NULL);

    //Get a handle to the device
    HANDLE GetHandle() {return hComm;}
    operator HANDLE() {return hComm;}

    //Change the baud rate of an open com port
    BOOL SetBaudRate(DWORD baud);
    DWORD GetBaudRate() {return nBaud;}

    //Return the number of bytes in the RX buffer;
    DWORD BytesInComm();

    //Purge all buffers used in com
    //Returns FALSE on failure - use GetLastError() for more info
    BOOL PurgeComm();



};//SFile

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#endif
