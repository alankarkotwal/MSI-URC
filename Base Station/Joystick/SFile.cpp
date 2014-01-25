
/*
Serial I/O class using the Win32 file funcions for compatibility with all
versions of Windows (including CE)
*/

#include "SFile.h"

#include <stdio.h>
#include <tchar.h>
#include <assert.h>



//----------------------------------------------------------------------------

// Format specification used in setting com properties
static TCHAR MODESETTINGS[]= _T("baud=%d parity=N data=8 stop=1  ");


// Format specification used in opening a port
static TCHAR COMMNAME[] =
#ifndef _WIN32_WCE
    _T("COM%d");
#else
    _T("COM%d:");
#endif

//----------------------------------------------------------------------------

// Utility class for closing handle in destructor
class AutoHandleCloser
{
    HANDLE m_h;
public:
    AutoHandleCloser(HANDLE h) : m_h(h) {}
    ~AutoHandleCloser()
    {
        if ((m_h != NULL) && (m_h != INVALID_HANDLE_VALUE))
            CloseHandle(m_h);
    }//destructor
    operator HANDLE() {return m_h;}
};//AutoHandleCloser

//----------------------------------------------------------------------------

#ifdef _WIN32_WCE

// Not supported in CE so I implement it here
BOOL BuildCommDCB(TCHAR *settings, DCB *dcb)
{
    TCHAR *p = _tcstok(settings, _T(" ="));
    while (p != NULL)
    {
        if (_tcscmp(_T("baud"), p) == 0)
        {
            p = _tcstok(NULL, _T(" ="));
            dcb->BaudRate = _ttoi(p);;
        }//if
        else if (_tcscmp(_T("parity"), p) == 0)
        {
            p = _tcstok(NULL, _T(" ="));
            if (_tcscmp(p, _T("N")) == 0)
            {
                dcb->fParity = FALSE;
                dcb->Parity = NOPARITY;
            }//if
            else if (_tcscmp(p, _T("E")) == 0)
            {
                dcb->fParity = TRUE;
                dcb->Parity = EVENPARITY;
            }//if
            else if (_tcscmp(p, _T("O")) == 0)
            {
                dcb->fParity = TRUE;
                dcb->Parity = ODDPARITY;
            }//if
            else
                assert(0);
        }//if
        else if (_tcscmp(_T("data"), p) == 0)
        {
            p = _tcstok(NULL, _T(" ="));
            dcb->ByteSize = _ttoi(p);;
        }//if
        else if (_tcscmp(_T("stop"), p) == 0)
        {
            p = _tcstok(NULL, _T(" ="));
            int stop = _ttoi(p);

            // assume one stop bit for now
            dcb->StopBits = ONESTOPBIT;
        }//if
        else
            assert(0);

        p = _tcstok(NULL, _T(" ="));
    }//while

    return TRUE;
}//BuildCommDCB

#endif

//----------------------------------------------------------------------------

void SFile::Close ()
{
    if (hComm != INVALID_HANDLE_VALUE)
	{
        CloseHandle(hComm);
		hComm = INVALID_HANDLE_VALUE;
		nComm = ReadTimeOut = ReadIter = nBaud = 0;
	}//if
}//Close

//----------------------------------------------------------------------------

BOOL SFile::Open(UINT port, DWORD baud)
{
    if(IsOpen()){
        if((nComm==port)&&(nBaud==baud)){
            return true;
        }
        else
            Close();
    }
    //create the comm name
    TCHAR comm_name[64];
    _stprintf(comm_name, COMMNAME, port);

    //open the port
    hComm = CreateFile(comm_name,
                       GENERIC_READ | GENERIC_WRITE,
                       0, 0,
                       OPEN_EXISTING,
                       #ifdef _WIN32_WCE
                          0,
                       #else
                          FILE_FLAG_OVERLAPPED,
                       #endif
                       0);

    //check for error opening the port
    if (hComm == INVALID_HANDLE_VALUE)
        return FALSE;

	nComm = port;

    //set the comm properties
    DCB dcb = {0};
    dcb.DCBlength = sizeof(DCB);

    if (!GetCommState(hComm, &dcb))
    {
        Close();
        return FALSE;
    }//if

    TCHAR mode[64];
    _stprintf(mode, MODESETTINGS, baud);

    if (!BuildCommDCB(mode, &dcb))
    {
        Close();
        return FALSE;
    }//if

    //keep track of current baud
    nBaud = baud;

    //set some other default values that we're gonna use
	dcb.fOutxCtsFlow = FALSE; //don't monitor CTS line
    dcb.fOutxDsrFlow = FALSE; //don't monitor DSR line
    dcb.fDtrControl = DTR_CONTROL_DISABLE; //disable DTR
    dcb.fRtsControl = RTS_CONTROL_DISABLE; //disable RTS

    if (!SetCommState(hComm, &dcb))
    {
        Close();
        return FALSE;
    }//if

    //purge the I/O buffers and any events on the comm port
    if (!PurgeComm())
    {
        Close();
        return FALSE;
    }//if

    //disable all comm timeouts, put SFile in complete control
    COMMTIMEOUTS commtimeouts;
    #ifndef _WIN32_WCE
        commtimeouts.ReadIntervalTimeout         = 0;
        commtimeouts.ReadTotalTimeoutMultiplier  = 0;
        commtimeouts.ReadTotalTimeoutConstant    = 0;
    #else
        commtimeouts.ReadIntervalTimeout         = MAXDWORD;
        commtimeouts.ReadTotalTimeoutMultiplier  = MAXDWORD;
        commtimeouts.ReadTotalTimeoutConstant    = ReadTimeOut*ReadIter;
    #endif
    commtimeouts.WriteTotalTimeoutMultiplier = 0;
    commtimeouts.WriteTotalTimeoutConstant   = 0;

    if (!SetCommTimeouts(hComm, &commtimeouts))
    {
        Close();
        return FALSE;
    }//if

    return TRUE;
}//Open

//----------------------------------------------------------------------------

BOOL SFile::Read (LPVOID lpvBuff, DWORD dwSize, DWORD &dwRead)
{
    if (hComm == INVALID_HANDLE_VALUE)
        return FALSE;

#ifndef _WIN32_WCE //CE can't do overlapped I/O damnit!

    //create overlapped structures
    OVERLAPPED osReader = {0};
    osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    AutoHandleCloser ahc(osReader.hEvent);

    //issue read operation
    if (!ReadFile(hComm, lpvBuff, dwSize, &dwRead, &osReader))
    {
        if (GetLastError() != ERROR_IO_PENDING)
        {
            return FALSE;
        }//if
    }//if
    else
        return TRUE; //read completed immediately


    //read up to dwSize bytes in READ_TIMEOUT ms
    UINT iters = 0;
    while (1)
    {
        DWORD dwResult = WaitForSingleObject(osReader.hEvent, ReadTimeOut);
        switch (dwResult)
        {
            case WAIT_OBJECT_0: //Read completed
            {
                if (!GetOverlappedResult(hComm, &osReader, &dwRead, FALSE))
                {
                    //Error in communications
                    return FALSE;
                }//if

                //Read completed succesfully
                return TRUE;
            }//case

            case WAIT_TIMEOUT: //Read timed out
            {
                if (++iters > ReadIter)
                {
                    CancelIo(hComm);
                    return FALSE;
                }//if

                continue;
            }//case

            default: //error
            {
                CancelIo(hComm);
                return FALSE;
            }//default
        }//switch
    }//while

#else
    //Windows CE version of Read - CE does not support overlapped file I/O
    if (!ReadFile(hComm, lpvBuff, dwSize, &dwRead, NULL))
        return FALSE;
#endif

    return TRUE;
}//Read

//----------------------------------------------------------------------------

BOOL SFile::Write(LPCVOID lpvBuff, DWORD dwSize, DWORD &dwWritten)
{
    if (hComm == INVALID_HANDLE_VALUE)
        return FALSE;

#ifndef _WIN32_WCE //CE can't do overlapped I/O damnit!

    //create overlapped structures
    OVERLAPPED osWrite = {0};
    osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    AutoHandleCloser ahc(osWrite.hEvent);

    //issue write operation
    if (!WriteFile(hComm, lpvBuff, dwSize, &dwWritten, &osWrite))
    {
        if (GetLastError() != ERROR_IO_PENDING)
        {
            return FALSE;
        }//if

        //write is pending
        DWORD dwResult = WaitForSingleObject(osWrite.hEvent, INFINITE);
        switch (dwResult)
        {
            case WAIT_OBJECT_0: //Write completed
            {
                if (!GetOverlappedResult(hComm, &osWrite, &dwWritten, FALSE))
                {
                    return FALSE;
                }//if

                //write completed succesfully
                return TRUE;
            }//case

            default:
            {
                CancelIo(hComm);
                return FALSE;
            }//default
        }//switch
    }//if

#else
    //Windows CE version of Write - CE does not support overlapped file I/O
    if (!WriteFile(hComm, lpvBuff, dwSize, &dwWritten, NULL))
        return FALSE;
#endif

    return TRUE;
}//Write

//----------------------------------------------------------------------------

BOOL SFile::ReadByte(BYTE &b)
{
	DWORD nread;
	BOOL res = Read(&b, 1, nread);
	return res && nread == 1;
}//ReadByte

//----------------------------------------------------------------------------

BOOL SFile::WriteByte(BYTE b)
{
	DWORD written;
	BOOL res = Write(&b, 1, written);
	return res && written == 1;
}//WriteByte

//----------------------------------------------------------------------------

BOOL SFile::SetCommMask(DWORD dwEvtMask, BYTE rxc)
{
    if (hComm == INVALID_HANDLE_VALUE)
        return FALSE;

    //set the received event character if the user wants that event
    if (dwEvtMask & EV_RXFLAG)
    {
        DCB dcb;

        //Always retrieve the current state before modifying it
        dcb.DCBlength = sizeof(DCB);
        if (!GetCommState(hComm, &dcb))
        {
            return FALSE;
        }//if

        dcb.EvtChar = rxc;

        if (!SetCommState(hComm, &dcb))
        {
            return FALSE;
        }//if
    }//if

    if (!::SetCommMask(hComm, dwEvtMask))
    {
        return FALSE;
    }//if

    return TRUE;
}//SetCommMask

//----------------------------------------------------------------------------

BOOL SFile::WaitCommEvent(UINT timeout,DWORD *com_ev)
{
    if (hComm == INVALID_HANDLE_VALUE)
        return FALSE;

    DWORD dwRes,        //result from call to WaitForSingleObject()
          dwCommEvent,  //event that cause WaitCommEvent to return;
          dwDummy;

    //Create overlapped structures
    OVERLAPPED osStatus = {0};
    osStatus.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    AutoHandleCloser ahc(osStatus.hEvent);

    if (!::WaitCommEvent(hComm, &dwCommEvent, &osStatus))
    {
        if (::GetLastError() == ERROR_IO_PENDING)
        {
                dwRes = WaitForSingleObject(osStatus.hEvent, timeout);
                if (dwRes == WAIT_OBJECT_0)
                {
                    #ifndef _WIN32_WCE
                        if (!GetOverlappedResult(hComm, &osStatus,
                                                 &dwDummy, FALSE))
                            return FALSE;
                    #endif
                }//if
                else
					return FALSE;
        }//if
        else
            return FALSE;
    }//if

    if (com_ev != NULL)
        *com_ev = dwCommEvent;

    return TRUE;
}//WaitCommEvent

//----------------------------------------------------------------------------

BOOL SFile::SetBaudRate(DWORD baud)
{
    if (hComm == INVALID_HANDLE_VALUE)
        return FALSE;

    DCB dcb;
    dcb.DCBlength = sizeof(DCB);

    //always retrieve the current state before modifying it
    if (!GetCommState(hComm, &dcb))
    {
        return FALSE;
    }//if

    dcb.BaudRate = baud;

    if (!SetCommState(hComm, &dcb))
    {
        return FALSE;
    }//if

    //keep track of current baud
    nBaud = baud;

    return TRUE;
}//SetBaudRate

//----------------------------------------------------------------------------

DWORD SFile::BytesInComm ()
{
    if (hComm == INVALID_HANDLE_VALUE)
        return 0;

    COMSTAT comstat;
    DWORD dwError;

    if (!ClearCommError(hComm, &dwError, &comstat))
    {
        return 0;
    }//if

    return comstat.cbInQue;
}//BytesInComm

//----------------------------------------------------------------------------

BOOL SFile::PurgeComm()
{
    if (hComm == INVALID_HANDLE_VALUE)
        return FALSE;

    DWORD flags = PURGE_TXABORT |
                  PURGE_RXABORT |
                  PURGE_TXCLEAR |
                  PURGE_RXCLEAR;

    if (!::PurgeComm(hComm, flags))
        return FALSE;

    return TRUE;
}//PurgeComm


//----------------------------------------------------------------------------
