// TryPrintJobFunctions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>

using namespace std;

int main()
{
	//Testing code
	wchar_t szBuffer[] = L"  ";
	DWORD cchBuffer;
	LPTSTR pszBuffer = szBuffer;
	LPDWORD pcchBuffer = &cchBuffer;
	cout << "hello"<< endl;
	GetDefaultPrinter(pszBuffer, pcchBuffer);
	wcout << "pszBuf " << pszBuffer << endl;
	wcout << "pcchBuf " << *pcchBuffer << endl;
	wcout << "szBuf " << szBuffer << endl;


	//This is the CODE!!!

	//Get Printers using EnumPrinters
	DWORD dwNeeded, dwReturned;
	PRINTER_INFO_4* pinfo4;
	int test = EnumPrinters(PRINTER_ENUM_NAME, NULL, 4, NULL, 0, &dwNeeded, &dwReturned);
	cout << test << endl;
	cout << dwNeeded << endl;
	pinfo4 = (PRINTER_INFO_4*)malloc(dwNeeded);
	test = EnumPrinters(PRINTER_ENUM_NAME, NULL, 4, (PBYTE)pinfo4, dwNeeded, &dwNeeded, &dwReturned);

	HDC hdc;

	//Iterate through printers
	for (DWORD i = 0; i < dwReturned; i++, pinfo4++) {
		wcout << pinfo4->pPrinterName << endl;
		LPWSTR printerName = pinfo4->pPrinterName;
		if (wcscmp(printerName, L"HP63EB96 (HP Deskjet 3540 series)")==0) {
			//create device context for printer specified
			hdc = CreateDC(NULL, printerName, NULL, NULL);

			HANDLE hPrinter;
			DWORD pJobCbNeeded, pJobCbReturned;
			JOB_INFO_1* pJob;

			OpenPrinter(printerName, &hPrinter, NULL);

			//get jobs in queue for the printer
			EnumJobs(hPrinter, 0, 10, 1, NULL, 0, &pJobCbNeeded, &pJobCbReturned);
			pJob = (JOB_INFO_1*)malloc(pJobCbNeeded);

			EnumJobs(hPrinter, 0, 10, 1, (PBYTE)pJob, pJobCbNeeded, &pJobCbNeeded, &pJobCbReturned);

			//Iterate through queued jobs for specified printer
			for (DWORD j = 0; j < pJobCbReturned; j++, pJob++) {
				wcout << "jobid " << pJob->JobId << endl;
				wcout << "jobdoc " << pJob->pDocument << endl;
			}
			ClosePrinter(printerName);
		}
	}
	return 1;
}