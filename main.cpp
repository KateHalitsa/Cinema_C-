#include <iomanip>
#include <windows.h>
#include <conio.h>
#include <string>
#include <format>
#include <vector>
#include <filesystem>
#include <algorithm>
#include "tinyxml2.h"
#include"Header.h"
using namespace std;
using namespace tinyxml2;

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	Database db;
	db.Load();
	int count ;
	count = db.Users.size();
	if (count == 0)
	{
		db.addFirstUser();
	}
	if (AskRegistration(&db))
	{

		if (!db.login())
	{
		return 0;
	}
	}
	else
	{
		SuggestRegistration(&db);
	}
	if (db.GetloggedInUser()->ReturnWorker() == 1)
	{
		db.GetWorker()->GetAdminDependedActivity(&db);
	}
	else {
		db.GetViewer()->GetAdminIndependedActivity(&db);
	}
	
	return 0;
}