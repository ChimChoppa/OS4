#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <ctime>

using namespace std;


int main()
{

	const int PageCount = 19, PageSize = 4096;

	srand(GetCurrentProcessId());

	HANDLE WriteSem[PageCount];
	HANDLE ReadSem[PageCount];
	for (int i = 0; i < PageCount; i++) {
		WriteSem[i] = OpenSemaphoreA(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, false, ("WSemaphore" + to_string(i)).c_str());
		ReadSem[i] = OpenSemaphoreA(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, false, ("RSemaphore" + to_string(i)).c_str());

	}

	HANDLE LogMutex = OpenMutexA(MUTEX_MODIFY_STATE | SYNCHRONIZE, false, "LogMutex");
	ofstream Log;
	Log.open("C:\\Lab4\\log.txt", fstream::out | fstream::app);

	HANDLE Mapping = OpenFileMappingA(GENERIC_READ, false, "map_file");
	void* OwnMap = MapViewOfFile(Mapping, FILE_MAP_READ, 0, 0, PageSize * PageCount);
	VirtualLock(OwnMap, PageSize * PageCount);

	while (1) {
		char msg[PageSize];

		WaitForSingleObject(LogMutex, INFINITE);
		Log << "Type: ReadProcess " << "Time: " << GetTickCount() << " ID: " << GetCurrentProcessId() << " Operation: start of waiting" << endl;
		ReleaseMutex(LogMutex);

		DWORD index = WaitForMultipleObjects(PageCount, ReadSem, false, INFINITE);

		WaitForSingleObject(LogMutex, INFINITE);
		Log << "Type: ReadProcess " << "Time: " << GetTickCount() << " ID: " << GetCurrentProcessId() << " Operation: start to read" << "Page: " << index << endl;
		ReleaseMutex(LogMutex);

		CopyMemory(msg, (LPVOID)((int)OwnMap + (index * PageSize)), PageSize);
		Sleep(500 + (rand() % 1000));

		WaitForSingleObject(LogMutex, INFINITE);
		Log << "Type: ReadProcess " << "Time: " << GetTickCount() << " ID: " << GetCurrentProcessId() << " Operation: end of read" << "Page: " << index << endl;
		ReleaseMutex(LogMutex);

		ReleaseSemaphore(WriteSem[index], 1, NULL);

	}
	system("pause");
	return 0;
}
