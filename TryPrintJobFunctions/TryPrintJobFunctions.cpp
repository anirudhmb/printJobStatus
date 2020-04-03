// TryPrintJobFunctions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>

using namespace std;

int main()
{
	wchar_t szBuffer[] = L"  ";
	DWORD cchBuffer;
	LPTSTR pszBuffer = szBuffer;
	LPDWORD pcchBuffer = &cchBuffer;
	cout << "hello"<< endl;
	GetDefaultPrinter(pszBuffer, pcchBuffer);
	wcout << "pszBuf " << pszBuffer << endl;
	wcout << "pcchBuf " << *pcchBuffer << endl;
	wcout << "szBuf " << szBuffer << endl;

	DWORD dwNeeded, dwReturned;
	PRINTER_INFO_4* pinfo4;
	int test = EnumPrinters(PRINTER_ENUM_NAME, NULL, 4, NULL, 0, &dwNeeded, &dwReturned);
	cout << test << endl;
	cout << dwNeeded << endl;
	pinfo4 = (PRINTER_INFO_4*)malloc(dwNeeded);
	test = EnumPrinters(PRINTER_ENUM_NAME, NULL, 4, (PBYTE)pinfo4, dwNeeded, &dwNeeded, &dwReturned);

	HDC hdc;

	for (DWORD i = 0; i < (dwNeeded / sizeof(PRINTER_INFO_4)); i++, pinfo4++) {
		wcout << pinfo4->pPrinterName << endl;
		LPWSTR printerName = pinfo4->pPrinterName;
		//if (wcscmp(printerName, L"Microsoft Print to PDF")==0) {
			hdc = CreateDC(NULL, printerName, NULL, NULL);

			HANDLE hPrinter;
			DWORD pJobCbNeeded, pJobCbReturned;
			JOB_INFO_1* pJob;

			OpenPrinter(printerName, &hPrinter, NULL);
			EnumJobs(hPrinter, 0, 10, 1, NULL, 0, &pJobCbNeeded, &pJobCbReturned);
			pJob = (JOB_INFO_1*)malloc(pJobCbNeeded);
			EnumJobs(hPrinter, 0, 10, 1, (PBYTE)pJob, pJobCbNeeded, &pJobCbNeeded, &pJobCbReturned);
			for (DWORD j = 0; j < pJobCbReturned; j++, pJob++) {
				wcout << "jobid " << pJob->JobId << endl;
				wcout << "jobdoc " << pJob->pDocument << endl;
				cout << "======================" << endl;
			}
			ClosePrinter(printerName);
		//}
	}
	return 1;
}