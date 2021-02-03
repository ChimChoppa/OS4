#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

void WINAPI ReadCallback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
	cout << "Информация успешно считана!" << endl;
}


int main()
{
	setlocale(LC_ALL, "Rus");

	HANDLE hPipe = INVALID_HANDLE_VALUE;
	OVERLAPPED overlapped = OVERLAPPED();
	BOOL connection = false;

	char flag;

	do {
		system("cls");
		cout << "Введите пункт меню для работы: \n";
		cout << "1 - Подключиться к каналу" << endl;
		cout << "2 - Считать строку" << endl;
		cout << "0 - Выход" << endl;
		cin >> flag;
		system("cls");

		switch (flag) {
		case '1':

			hPipe = CreateFileA("\\\\.\\pipe\\pipename", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
			if (hPipe != INVALID_HANDLE_VALUE)
				cout << "Подключение выполнено!\n";
			else
				cout << GetLastError();

			system("pause");
			break;

		case '2': {
			char* mes = new char[512];
			overlapped = OVERLAPPED();
			if (!ReadFileEx(hPipe, (LPVOID)mes, 512, &overlapped, ReadCallback))
				cout << "Не считанно!\n";
			else
			{
				SleepEx(INFINITE, true);
				cout << "Полученное сообщение: ";
				for (int i = 0; i < strlen(mes); ++i) {
					cout << mes[i];
				}
				cout << endl;
			}

			system("pause");
			break;
		}
		default:
			break;

		case '0':
			system("cls");
			break;
		}

	} while (flag != '0');


	if (hPipe != INVALID_HANDLE_VALUE)
		CloseHandle(hPipe);

	return 0;
}
