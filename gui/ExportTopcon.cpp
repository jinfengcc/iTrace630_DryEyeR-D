//***************************************************************************************

#include "StdAfx.h"
#include "Export.h"
#include "Settings.h"
#include "Data.h"
#include "NumDlg.h"

//***************************************************************************************

void ExportTopconCV5000(CWFExam* pWFODExam, CCTExam* pCTODExam, CWFExam* pWFOSExam, CCTExam* pCTOSExam)
{
	real_t PortNumber;
	if (!EnterNumber(PortNumber, 0, "COM port")) return;

	static int counter = -1;

	if (++counter == 100)
	{
		counter = 0;
	}

	char* pBuffer = new char[512];
	memset(pBuffer, 0, 512);

	int k = 0;
	//const char LF = (char)0x0a; // line feed
	const char CR = (char)0x0d; // carriage return
	const char EOT = (char)0x04;

	pBuffer[k++] = '@';
	pBuffer[k++] = CR;

	// product name
	k += sprintf(pBuffer + k, "KR-8000        ");
	// device no
	k += sprintf(pBuffer + k, "00");
	// rom version
	k += sprintf(pBuffer + k, "0.00      ");
	pBuffer[k++] = CR;

	// real id no
	k += sprintf(pBuffer + k, "             ");
	pBuffer[k++] = CR;

	// work id no
	k += sprintf(pBuffer + k, "             ");
	pBuffer[k++] = CR;

	// serial no
	k += sprintf(pBuffer + k, "00%02i", counter);
	pBuffer[k++] = CR;

	pBuffer[k++] = '*';
	pBuffer[k++] = CR;

	// date & time
	SYSTEMTIME Time;
	::GetLocalTime(&Time);
	k += sprintf(pBuffer + k, "%i_%02i_%02i ", Time.wYear, Time.wMonth, Time.wDay);
	k += sprintf(pBuffer + k, "%s %02i:%02i", Time.wHour > 11 ? "PM" : "AM", Time.wHour % 12, Time.wMinute);
	pBuffer[k++] = CR;

	CZernikeSurface Surface;
	real_t SphEq, Sph, Cyl;
	int Axis;

	if (pWFODExam)
	{
		Surface = pWFODExam->m_WfSurface;
		Surface.ChangeRMaxUm(2000.0);
		CWFExam::GetSpheqSphCylAxis(Surface, 12000, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);
		SphEq = RealRound(SphEq, 0.25);
		Sph = RealRound(Sph, 0.25);
		Cyl = RealRound(Cyl, 0.25);

		// sph
		k += sprintf(pBuffer + k, "%s", Sph > 0.0 ? "+" : Sph < 0.0 ? "-" : " ");
		k += sprintf(pBuffer + k, "%5.2f", fabs(Sph));

		// cyl
		k += sprintf(pBuffer + k, "%s", Cyl >= 0.0 ? "+" : Cyl < 0.0 ? "-" : " ");
		k += sprintf(pBuffer + k, "%5.2f", fabs(Cyl));

		// axis
		k += sprintf(pBuffer + k, "%3i", Axis);
		pBuffer[k++] = CR;

		// spheq
		k += sprintf(pBuffer + k, "%s", SphEq >= 0.0 ? "+" : SphEq < 0.0 ? "-" : " ");
		k += sprintf(pBuffer + k, "%5.2f", fabs(SphEq));
		pBuffer[k++] = CR;
	}
	else
	{
		k += sprintf(pBuffer + k, "                       ");
	}

	//
	if (pCTODExam && pCTODExam->m_sk_ok)
	{
		k += sprintf(pBuffer + k, "%5.2f", pCTODExam->m_sk_ok ? 0.001 * pCTODExam->m_sk_fl_um : 0.0);
		k += sprintf(pBuffer + k, "%5.2f", pCTODExam->m_sk_ok ? pCTODExam->m_sk_fl_dp : 0.0);
		k += sprintf(pBuffer + k, "%3i", pCTODExam->m_sk_ok ? pCTODExam->m_sk_fl_dg : 0);
		k += sprintf(pBuffer + k, "%5.2f", pCTODExam->m_sk_ok ? 0.001 * pCTODExam->m_sk_st_um : 0.0);
		k += sprintf(pBuffer + k, "%5.2f", pCTODExam->m_sk_ok ? pCTODExam->m_sk_st_dp : 0.0);
		k += sprintf(pBuffer + k, "%3i", pCTODExam->m_sk_ok ? pCTODExam->m_sk_st_dg : 0);
		k += sprintf(pBuffer + k, "%5.2f", pCTODExam->m_sk_ok ? 0.001 * pCTODExam->m_sk_av_um : 0.0);
		k += sprintf(pBuffer + k, "+");
		k += sprintf(pBuffer + k, "%5.2f", pCTODExam->m_sk_ok ? pCTODExam->m_sk_df_dp : 0.0);
		k += sprintf(pBuffer + k, "%3i", pCTODExam->m_sk_ok ? pCTODExam->m_sk_st_dg : 0);
	}
	else
	{
		k += sprintf(pBuffer + k, "                                        ");
	}
	pBuffer[k++] = CR;

	// 100 spaces
	for (int i = 0; i < 10; i++)
	{
		k += sprintf(pBuffer + k, "          ");
	}
	pBuffer[k++] = CR;

	//
	if (pWFOSExam)
	{
		Surface = pWFOSExam->m_WfSurface;
		Surface.ChangeRMaxUm(2000.0);
		CWFExam::GetSpheqSphCylAxis(Surface, 12000, ::Settings.m_PositiveCylinder, SphEq, Sph, Cyl, Axis);

		SphEq = RealRound(SphEq, 0.25);
		Sph = RealRound(Sph, 0.25);
		Cyl = RealRound(Cyl, 0.25);

		// sph
		k += sprintf(pBuffer + k, "%s", Sph > 0.0 ? "+" : Sph < 0.0 ? "-" : " ");
		k += sprintf(pBuffer + k, "%5.2f", fabs(Sph));

		// cyl
		k += sprintf(pBuffer + k, "%s", Cyl >= 0.0 ? "+" : Cyl < 0.0 ? "-" : " ");
		k += sprintf(pBuffer + k, "%5.2f", fabs(Cyl));

		// axis
		k += sprintf(pBuffer + k, "%3i", Axis);
		pBuffer[k++] = CR;

		// spheq
		k += sprintf(pBuffer + k, "%s", SphEq >= 0.0 ? "+" : SphEq < 0.0 ? "-" : " ");
		k += sprintf(pBuffer + k, "%5.2f", fabs(SphEq));
		pBuffer[k++] = CR;
	}
	else
	{
		k += sprintf(pBuffer + k, "                       ");
	}

	//
	if (pCTOSExam && pCTOSExam->m_sk_ok)
	{
		k += sprintf(pBuffer + k, "%5.2f", pCTOSExam->m_sk_ok ? 0.001 * pCTOSExam->m_sk_fl_um : 0.0);
		k += sprintf(pBuffer + k, "%5.2f", pCTOSExam->m_sk_ok ? pCTOSExam->m_sk_fl_dp : 0.0);
		k += sprintf(pBuffer + k, "%3i", pCTOSExam->m_sk_ok ? pCTOSExam->m_sk_fl_dg : 0);
		k += sprintf(pBuffer + k, "%5.2f", pCTOSExam->m_sk_ok ? 0.001 * pCTOSExam->m_sk_st_um : 0.0);
		k += sprintf(pBuffer + k, "%5.2f", pCTOSExam->m_sk_ok ? pCTOSExam->m_sk_st_dp : 0.0);
		k += sprintf(pBuffer + k, "%3i", pCTOSExam->m_sk_ok ? pCTOSExam->m_sk_st_dg : 0);
		k += sprintf(pBuffer + k, "%5.2f", pCTOSExam->m_sk_ok ? 0.001 * pCTOSExam->m_sk_av_um : 0.0);
		k += sprintf(pBuffer + k, "+");
		k += sprintf(pBuffer + k, "%5.2f", pCTOSExam->m_sk_ok ? pCTOSExam->m_sk_df_dp : 0.0);
		k += sprintf(pBuffer + k, "%3i", pCTOSExam->m_sk_ok ? pCTOSExam->m_sk_st_dg : 0);
	}
	else
	{
		k += sprintf(pBuffer + k, "                                        ");
	}

	pBuffer[k++] = CR;

	// 100 spaces
	for (int i = 0; i < 10; i++)
	{
		k += sprintf(pBuffer + k, "          ");
	}
	pBuffer[k++] = CR;

	// pupillary distance
	k += sprintf(pBuffer + k, "65.0");
	pBuffer[k++] = CR;

	// vertex distance
	k += sprintf(pBuffer + k, "12.00");
	pBuffer[k++] = CR;

	pBuffer[k++] = '*';
	pBuffer[k++] = CR;

	pBuffer[k++] = EOT;

	// k  429

	CString s;

	s.Format(_T("COM%i"), (int)PortNumber);
	HANDLE hComm = ::CreateFile(s, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (hComm == INVALID_HANDLE_VALUE)
	{
		::Error("Failed to open " + s);
		return;
	}

	OVERLAPPED ol;
	memset(&ol, 0, sizeof(OVERLAPPED));
	ol.hEvent = ::CreateEvent(NULL, FALSE, FALSE, _T(""));

	DCB dcb;
	memset(&dcb, 0, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);
	if (!::GetCommState(hComm, &dcb))
	{
		ulong ErrorCode = ::GetLastError();
		s.Format(_T("GetCommState failed. Error code = %i."), ErrorCode);
		::Error(s);
		goto _END;
	}
	dcb.BaudRate = CBR_2400;
	dcb.ByteSize = 8;
	dcb.fParity = FALSE;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;

	if (!::SetCommState(hComm, &dcb))
	{
		ulong ErrorCode = ::GetLastError();
		s.Format(_T("SetCommState failed. Error code = %i."), ErrorCode);
		::Error(s);
		goto _END;
	}

	// timeouts
	COMMTIMEOUTS CommTimeouts;
	if (!::GetCommTimeouts(hComm, &CommTimeouts))
	{
		ulong ErrorCode = ::GetLastError();
		s.Format(_T("GetCommTimeouts failed. Error code = %i."), ErrorCode);
		::Error(s);
		goto _END;
	}
	CommTimeouts.ReadIntervalTimeout = 0xffffffff;
	CommTimeouts.ReadTotalTimeoutMultiplier = 10;
	CommTimeouts.ReadTotalTimeoutConstant = 1000;
	CommTimeouts.WriteTotalTimeoutMultiplier = 10;
	CommTimeouts.WriteTotalTimeoutConstant = 1000;

	if (!::SetCommTimeouts(hComm, &CommTimeouts))
	{
		ulong ErrorCode = ::GetLastError();
		s.Format(_T("SetCommTimeouts failed. Error code = %i."), ErrorCode);
		::Error(s);
		goto _END;
	}

	// buffers sizes
	if (!::SetupComm(hComm, 1024, 1024))
	{
		ulong ErrorCode = ::GetLastError();
		s.Format(_T("SetupComm failed. Error code = %i."), ErrorCode);
		::Error(s);
		goto _END;
	}

	if (!::CancelIo(hComm))
	{
		ulong ErrorCode = ::GetLastError();
		s.Format(_T("CancelIo failed. Error code = %i."), ErrorCode);
		::Error(s);
		goto _END;
	}

	if (!::PurgeComm(hComm, PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_RXABORT))
	{
		ulong ErrorCode = ::GetLastError();
		s.Format(_T("PurgeComm failed. Error code = %i."), ErrorCode);
		::Error(s);
		goto _END;
	}

	DWORD Errors;
	if (!::ClearCommError(hComm, &Errors, NULL))
	{
		ulong ErrorCode = ::GetLastError();
		s.Format(_T("ClearCommError failed. Error code = %i."), ErrorCode);
		::Error(s);
		goto _END;
	}

	if (!::EscapeCommFunction(hComm, SETDTR))
	{
		ulong ErrorCode = ::GetLastError();
		s.Format(_T("EscapeCommFunction failed to SETDTR. Error code = %i."), ErrorCode);
		::Error(s);
		goto _END;
	}

	if (!::EscapeCommFunction(hComm, SETRTS))
	{
		ulong ErrorCode = ::GetLastError();
		s.Format(_T("EscapeCommFunction failed to SETRTS. Error code = %i."), ErrorCode);
		::Error(s);
		goto _END;
	}

	ulong bt = 0;
	if (::WriteFile(hComm, pBuffer, k, &bt, &ol))
	{
		// is not supposed to happen with asynchronous handle
	}
	else
	{
		ulong ErrorCode = ::GetLastError();
		if (ErrorCode == ERROR_IO_PENDING)
		{
			if (::WaitForSingleObject(ol.hEvent, 10000) != WAIT_OBJECT_0)
			{
				::Error("Timed out waiting for data transfer to complete.");
				goto _END;
			}
		}
		else
		{
			s.Format(_T("WriteFile failed. Error code = %i."), ErrorCode);
			::Error(s);
			goto _END;
		}
	}

	pBuffer[k] = 0;
	s.Format(_T("%i bytes transferred:\n%s"), ol.InternalHigh, (const char*)pBuffer);
	::Info(s);

_END:

	if (!::EscapeCommFunction(hComm, CLRDTR))
	{
		ulong ErrorCode = ::GetLastError();
		s.Format(_T("EscapeCommFunction failed to CLRDTR. Error code = %i."), ErrorCode);
		::Error(s);
	}

	if (!::EscapeCommFunction(hComm, CLRRTS))
	{
		ulong ErrorCode = ::GetLastError();
		s.Format(_T("EscapeCommFunction failed to CLRRTS. Error code = %i."), ErrorCode);
		::Error(s);
	}

	::CloseHandle(ol.hEvent);
	::CloseHandle(hComm);

	delete[] pBuffer;
}

//***************************************************************************************
