#include <iostream>
#include <iomanip>
#include <windows.h>
#include <conio.h>
#include <string>
#include <format>
#include <vector>
#include <iterator>
#include <filesystem>
#include <algorithm>
#include "tinyxml2.h"
#include"Header.h"
using namespace std;
using namespace tinyxml2;
extern int globalObjectID = 0;
string name;

extern int GetNewID()
{
	globalObjectID++;
	return globalObjectID;
}

extern string getString(string question)
{
	cout << question;

	//fflush(stdin);
	//getchar();

  string s = "";
 // cin.ignore(1024, '\n');
  cin.clear();
  cin.sync();
  while (s == "") 
  {
	  getline(cin, s, '\n');
  }
//  cin >> s;
  return s;
}

int getInt() {
	int i = -1;
	while (true)
	{   
		 cin.unsetf(ios::skipws);
		
			cin >> i;
		 cin.clear();
		cin.sync();
		if (cin.fail()) // ����� ���������� ��� cin.fail ()                 
		{
			cout << "������������ ����! ����������, ������� ������" << endl;
			cin.clear();
			cin.ignore(10, '\n');
			cin.sync(); // ������ �����                 
		}
		else
		{
			//cin.clear();
			cin.ignore(10, '\n');
			//cin.sync();
			//cin.clear();
					
			break;
		}
		
		
	}
	return i;
}

string encryptDecrypt(string toEncrypt) {
	char key = 'K'; 
	string output = toEncrypt;

	for (int i = 0; i < toEncrypt.size(); i++)
		output[i] = toEncrypt[i] ^ key;

	return output;
}



template <class T>
void SaveData(string fileName, const std::vector<T> objectList)
{
	string rootNodeName = std::filesystem::path(fileName).stem().string();
	transform(rootNodeName.begin(), rootNodeName.end(), rootNodeName.begin(), ::tolower);

	fileName = "DB\\" + fileName;

	tinyxml2::XMLDocument doc;
	XMLElement* root = doc.NewElement(rootNodeName.c_str());
	doc.LinkEndChild(root);
	for (int i = 0; i < objectList.size(); i++)
	{
		XMLElement* row = doc.NewElement("row");
		root->LinkEndChild(row);

		T obj = objectList[i];
		obj->Save(row);
	}
	doc.SaveFile(fileName.c_str());
}


bool AskRegistration(Database* db) {
	int answer;
	cout << " �� ��� ���� ����������������?" << endl;
	cout << "1.��" << setw(20) << "2.���"<<endl;
	answer = getInt();
	if (answer == 1)
		return true;
	if (answer == 2)

		return false;
	else
		return AskRegistration(db);
}
void SuggestRegistration(Database* db) {
	int answer;
	cout << " ������ ������������������?" << endl;
	cout << "1.��" << setw(20) << "2.���" << endl;
	answer = getInt();
	if (answer == 1)
	{ 
		db->Registration();
		db->Save();
	}
	else {shared_ptr <User> user(new User(db));

	db->GetloggedInUser() = user;
	db->SetLoggedInUser(user);
		db->SetViewer(NULL, 0);
	}
		
}
string Database::FormPosition(shared_ptr<User> user) {
	shared_ptr<UserRole> row = NULL;
	string full_position="", pos;
	int j = 0;
	for (int i = 0; i < UserRoles.size(); i++)
	{
		if (UserRoles[i]->getUserID() == user->getID())
		{
			row = UserRoles[i];
		
			if (row->IsDeleted()) 
			{
				continue;
			}
			pos = findRoleById(row->getRoleID())->GetName();
			if (j > 0) {
				
				pos[0] =pos[0]+32;
				full_position = full_position +"-"+ pos;
			}
			else{
			full_position = full_position + pos;
			j++;
}
			
		}
	}
	
	return full_position;
}
void Database::SetWorker(shared_ptr<User> user) {
	shared_ptr< Worker> worker(new Worker(FormPosition(user), this, user->GetName(), user->GetPassword(), user->GetLogin(), user->ReturnWorker()));
	curWorker = worker;
}
void Database::SetViewer(shared_ptr<User> user, bool regestrated) {
	if (user == NULL) {
		shared_ptr< Viewer> viewer(new Viewer(regestrated, this));
		curViewer = viewer;
	}
	else{
	shared_ptr< Viewer> viewer(new Viewer(regestrated, this, user->GetName(), user->GetPassword(), user->GetLogin(), 0));
	curViewer = viewer;}
}
 
void Database::Registration() {
	int answer;
	bool worker;
	bool loggedIn = false;
	string name = getString("������� ��� ������������:");
	string login = getString("������� �����:");
	string password = getString("������� ������:");
		worker = false;
	shared_ptr <User> user(new User(this, name, password, login, worker));
	addUser(user);
	SetViewer(user, 1);
	loggedInUser = user;
	loggedIn = true;
}

 void Role::SetValue(string name)
{
	this->name = name;
}
void Role::Load(XMLElement* row,Database* db)
{
	Base::Load(row, db);

	name = row->Attribute("name");

}
void Role::Save(XMLElement* row)
{
	Base::Save(row);
	row->SetAttribute("name", name.c_str());
}

void User::SetValue(string name, string password, string  login,  bool worker)
{
	this->name = name;
	this->encryptedPassword = encryptDecrypt(password);
	this->login = login;
	this->worker = worker;
}
void User::Load(XMLElement* row, /*Base*/Database* db)
{
	Base::Load(row, db);

	name = row->Attribute("name");
	encryptedPassword = row->Attribute("password");
	login = row->Attribute("login");
	worker = row->IntAttribute("worker");

}
void User::Save(XMLElement* row)
{
	Base::Save(row);
	row->SetAttribute("login", login.c_str());
	row->SetAttribute("password", encryptedPassword.c_str());
	row->SetAttribute("name", name.c_str());
	row->SetAttribute("worker", int(worker));
}


void UserRole::SetValue(shared_ptr<User> user, shared_ptr <Role> role)
{

	this->role = role;
	this->user = user;
}
void UserRole::SetUser(shared_ptr<User> user) {
	this->user = user;
}
void UserRole::SetRole(shared_ptr<Role> role) {
	this->role = role;
}


void UserRole::Load(XMLElement* row, /*Base*/Database* d)
{
	/*Database* d = (Database*)db;*/

	Base::Load(row, db);
	int role_id = row->IntAttribute("role_id", 0);
	role = d->findRoleById(role_id);

	int user_id = row->IntAttribute("user_id", 0);
	user = d->findUserById(user_id);

}

void UserRole::Save(XMLElement* row)
{
	Base::Save(row);
	row->SetAttribute("role_id", role->getID());
	row->SetAttribute("user_id", user->getID());
	
}

void RoleOperation::Load(XMLElement* row, /*Base*/Database* d)
{
	/*Database* d = (Database*)db;*/
	Base::Load(row, db);
	
	int role_id = row->IntAttribute("role_id", 0);
	int op = row->IntAttribute("operation", 0);

	role = d->findRoleById(role_id);	
	this->operation = static_cast<Operations>(op);
}

void RoleOperation::Save(XMLElement* row)
{
	Base::Save(row);
	
	int op = this->operation;

	row->SetAttribute("role_id", role -> getID());	
	row->SetAttribute("operation", op);	
}

void RoleOperation::Display(/*Base*/Database* db)
{
	cout << setw(4) << id << "|"
		<< setw(20) << role->GetName() << "|"
		<< setw(20) << getOperationName(operation) << "|"
		<< endl;
}
void RoleOperation::Header() {
	cout << "--------����� ����� � ����������---------------" << endl;
	cout << " id |        ����        |    ��������        |" << endl;
	cout << "-----------------------------------------------" << endl;
}
void RoleOperation::SetRole(shared_ptr<Role> role) {
	this->role = role;
}

void Database::addRole(shared_ptr <Role> role)
{
	Roles.push_back(role);
}
void Database::addUser(shared_ptr<User> user)
{
	Users.push_back(user);
}

void Database::addWorker(shared_ptr<Worker> worker) {
	Workers.push_back(worker);
}
void Database::addViewer(shared_ptr<Viewer> viewer) {
	Viewers.push_back(viewer);
}
void Database::addUserRole(shared_ptr <UserRole> userRole)
{
	UserRoles.push_back(userRole);
}
void Database::addRoleOperation(shared_ptr<RoleOperation> roleOperation)
{
	RoleOperations.push_back(roleOperation);
}



void Database::Load()
{
	LoadData("Role.xml", &Roles, this);
	LoadData("User.xml", &Users, this);
	LoadData("UserRole.xml", &UserRoles, this);
	LoadData("RoleOperation.xml", &RoleOperations, this);
	
	LoadData("Hall.xml", &Halls, this);
	LoadData("Film.xml", &Films, this);
	LoadData("Schedule.xml", &Schedules, this);
	LoadData("FilmRating.xml", &FilmRatings, this);
	LoadData("FilmSchedule.xml", &FilmSchedules, this);
	LoadData("Ticket.xml", &Tickets, this);
	
}
void Database::Save()
{
	SaveData("Role.xml", Roles);
	SaveData("User.xml", Users);
	SaveData("UserRole.xml", UserRoles);
	SaveData("RoleOperation.xml", RoleOperations);

	SaveData("Hall.xml", Halls);
	SaveData("Film.xml", Films);
	SaveData("Schedule.xml", Schedules);
	SaveData("FilmRating.xml", FilmRatings);
	SaveData("FilmSchedule.xml", FilmSchedules);
	SaveData("Ticket.xml", Tickets);
	
}

set<Operations> Database::availableOperations()
{
	set<Operations> operations;

	int userID = loggedInUser->getID();

	for (int i = 0; i < UserRoles.size(); i++)
	{
		shared_ptr<UserRole> userRole = UserRoles[i];
		if ( !userRole->IsDeleted() && (userRole->getUserID() == userID))
		{
			int roleID = userRole->getRoleID();
			for (int j = 0; j < RoleOperations.size(); j++) 
			{
				shared_ptr<RoleOperation> roleOperation = RoleOperations[j];
				if ( !roleOperation->IsDeleted() && (roleOperation->getRoleId() == roleID))
				{
					operations.insert(roleOperation->getOperation());
				}
			}
		}
	}

	return operations;
}

void Database::addFirstUser() 
{
	shared_ptr<User> user(User::CreateUser(this));
	addUser(user);

	shared_ptr<Role> role(new Role(this,"�������������"));
	addRole(role);

	shared_ptr<UserRole> userRole(new UserRole(this, user, role));
	addUserRole(userRole);

	shared_ptr<RoleOperation> roleOperation(new RoleOperation(this, opUserAdministration, role));
	addRoleOperation(roleOperation);

	Save();
}

bool Database::login() 
{
	string login;
	string password;
	shared_ptr<User> user;

	bool loggedIn = false;

	system("cls");
	cout << "��� ����� � ������� ������� �����\\������" << endl;

	while (true) 
	{
		User::enterLoginPassword(login, password);
		user = findUserByLoginAndPassword(login, password);
		if (user != NULL)
		{
			loggedInUser = user;
			loggedIn = true;
			if (user->ReturnWorker() == 1)
		{
			SetWorker(user);
		}
		else {
			SetViewer( user,1);
		}
			break;
		}
		
		
		
		system("cls");
		cout << "����� ��� ������ �� �����. ��������� ���� ������\\������" << endl;
	}

	/*shared_ptr <User> user(new User());
	user->CreateUser();
	shared_ptr <Role> role(new Role());
	role->CreateRole();
	?*/
	return loggedIn;
}

shared_ptr <Role> Database::findRoleById(int id)
{
	return findInTableById(id, Roles);
}

shared_ptr<User> Database::findUserById(int id)
{
	return findInTableById(id, Users);
}

shared_ptr<User> Database::findUserByLoginAndPassword(string login, string password)
{
	shared_ptr<User> foundUser = NULL;

	for (int i = 0; i < Users.size(); i++)
	{
		shared_ptr<User> user = Users[i];
		if ((user->GetLogin() == login) && (user->CheckPassword(password)))
		{
			foundUser = user;
			break;
		}
	}
	return foundUser;
}

shared_ptr<RoleOperation> Database::findRoleOperationById(int id)
{
	return findInTableById(id, RoleOperations);
}

shared_ptr<UserRole> Database::findUserRoleById(int id)
{
	return findInTableById(id, UserRoles);
}


void Database::Display()
{
	ShowTable(Roles, this);
	ShowTable(Users, this);
	ShowTable(UserRoles, this);
	ShowTable(RoleOperations, this);

}


//extern
int MainMenu(Database* db)
{
	
	MenuItem menuData[] = 
	{ 
		{ cMainMenuEditCurrentAccount, "������������� �������� �������� ������������", opUnknown, -1},
		{ cMainMenuUserAdministration, "����������������� ������������� �������", opUserAdministration, -1 },
		{ cMainMenuMovieAdministration, "����������������� �������", opMovieAdministration, -1 },
		{ cMainMenuMovieSchedule, "�������������� ���������� �������", opMovieSchedule, -1},
		{ cMainMenuTicketOrdering, "����� ������", opTicketOrdering, -1},
		{ cMainMenuExit, "�����", opUnknown, -1}
	};

	
	//db->Display();

	int choise = -1;
	int initalChoise = -12345;
	int displayChoise = initalChoise;

	shared_ptr<User> user = db->getLoggedInUser();
	set<Operations> operations = db->availableOperations();

	while (choise < 1) 
	{
		system("cls");
		cout << "����������� ��� '" << user->GetName() << "' � ����� ����������." << endl;
		if (displayChoise != initalChoise) 
		{
			cout << "�� ������� " << displayChoise << " - ������ �������� ���� �� ����������" << endl;
		}
		cout << "�� ����� ��� " << db->GetWorker()->GetPosition() << ", ���� ����:" << endl;
		cout << "�������� ����� �������� ��� �����������:" << endl;
		
		int displayNum = 1;
		for (int i = 0; i < std::size(menuData); i++)
		{
			Operations op = menuData[i].operation;
			if ((op == opUnknown) || (operations.find(op) != operations.end()))
			{
				cout << displayNum << ". " << menuData[i].name << endl;
				menuData[i].displayNum = displayNum;
				displayNum++;
			}
		}
		cout << "��� �����: ";
		cin.clear();
		displayChoise=getInt();

		for (int i = 0; i < std::size(menuData); i++)
		{
			if (menuData[i].displayNum == displayChoise) 
			{
				choise = menuData[i].Num;
				break;
			}
		}
	}

	return choise;
}

int AdministrationMenu()
{

	system("cls");
	int choise;
	cout << "���� ������������: " << endl;
	cout << "1.�������������� ������������� " << endl;
	cout << "2.�������������� ����� " << endl;
	cout << "3.�������������� ����� ������������ � ����� " << endl;
	cout << "4.�������������� ����� ���� � ��������� " << endl;
	cout << "5.����� �� ����������������� ������������� " << endl;

	cout << "��� �����: ";
	cin.clear();
	choise = getInt();
	return choise;
}

int EditRoleMenu();
void Database::EditRoles()
{
	while (true) 
	{
		system("cls");
		ShowTable(Roles, this);

		switch (EditRoleMenu())
		{
		case 1:
			newRole();//��������
			break;
		case 2:
			changeRole();
			break;
		case 3:
			deleteRole();//�������
			break;

		case 4:
			return;
			break;
		default:
			continue;
		}
	}

}


void Database::changeRole()
{
	int id;
	string new_role;
	
	cout << "������� id ����:";
	cin >> id;

	new_role = getString("������� ����� ��� ����:");

	findRoleById(id)->SetValue(new_role);

}
void Database::newRole() {
	string name = getString("������� ����:");
	shared_ptr <Role> role(new Role(this, name));
	addRole(role);
}
void Database::deleteRole() {
	int id;
	cout << "������� ID ����:";
	cin >> id;
	shared_ptr<Role> role = findRoleById(id);
	role->Delete();
}
int EditUserMenu() {	
	int choise;
	cout << "�������� ��������: " << endl;
	cout << "1.�������� ������������ " << endl;
	cout << "2.�������� ������ ������������ " << endl;
	cout << "3.������� ������������ " << endl;
	cout << "4.������������� " << endl;
	cout << "5.����� " << endl;
	cout << "6.����� " << endl;
	cout << "��� �����:";
	//cin >> choise;
	cin.clear();
	choise = getInt();
	return choise;
}

int SortUsersMenu() {
	int choise;

	cout << "�������������: " << endl;
	cout << "1.�� �������� " << endl;
	cout << "2.�� ID " << endl;
	cout << "��� �����:";
	//cin >> choise;
	cin.clear();
	choise = getInt();

	return choise;
}
int EditRoleMenu() {
	int choise;
	cout << "�������� ��������: " << endl;
	cout << "1.�������� ���� " << endl;
	cout << "2.�������� ������ ���� " << endl;
	cout << "3.������� ���� " << endl;
	cout << "4.����� " << endl;
	cout << "��� �����: ";
	cin.clear();
	 choise = getInt();
	return choise;
}
int EditUserRoleMenu() {
	int choise;
	cout << "�������� ��������: " << endl;
	cout << "1.�������� ����� ������������-���� " << endl;
	cout << "2.������� ����� ������������-���� " << endl;
	cout << "3.����� " << endl;
	cout << "��� �����: ";
	cin.clear();
	 choise = getInt();
	return choise;
}
int changeUserMenu() {
	int choise;
	cout << "��� �� ������ ��������: " << endl;
	cout << "1.��� ������������ " << endl;
	cout << "2.����� " << endl;
	cout << "3.������ " << endl;
	cout << "4.����� " << endl;
	cout << "��� �����: ";
	cin.clear();
	choise = getInt();
	return choise;
}
int MenuFindUser() {
	int choise;
	cout << "����� ��: " << endl;
	cout << "1.����� " << endl;
	cout << "2.ID " << endl;
	cout << "��� �����: ";
	cin.clear();
	choise = getInt();
	return choise;
}
void User::SetName(string name)
{
	this->name = name;
}
void  User::SetPassword(string password) {
	this->encryptedPassword = encryptDecrypt(password);
}
void  User::SetLogin(string  login) {
	this->login = login;
}


bool compareUserByID(shared_ptr<User> user1, shared_ptr<User> user2)
{
	return (user1->getID() < user2->getID());
}
bool compareUserByName(shared_ptr<User> user1, shared_ptr<User> user2)
{
	return (user1->GetName() < user2->GetName());
}

//shared_ptr<User> is_evenUserName = [](shared_ptr<User> i, shared_ptr<User>ex) { return i->GetName() == ex->GetName(); };
bool IfUserName(shared_ptr<User> user) {
	return user->GetName() == name;
}

void Database::EditUsers()
{   	
	int sortType;
	string password,login;

	while (true) 
	{
		system("cls");
		ShowTable(Users, this);

		switch (EditUserMenu())
		{
		case 1:
			newUser();//��������
			break;
		case 2:
			changeUsers();
			break;
		case 3:
			deleteUser();
			break;

		case 4:
			switch(SortUsersMenu())
			{ 
			case 1:
				sort(Users.begin(), Users.end(),compareUserByName );
		
			break;
			case 2:
				sort(Users.begin(), Users.end(), compareUserByID);
				break;
	
			default:
				continue;
				break;
			}
			/*if(sortType== SortAlphabetically|| sortType == SortById)
				Users=QuickSort(Users, 0, Users.size()-1,  sortType);*/
//			Users = Sort(Users, sortType);
		/*	else {
				cout << "�� ������ ����� ����������";
			}*/

			break;
		case 5:
		{
		/*string name;*/
		int id;
		
		shared_ptr <User>example;
		/*
		example->SetName(name)
		example = Find(Users, Users.size(), 1, example);;*/
		switch (MenuFindUser()) {
			case 1:
				
			{
				name = getString("������� ���:");

				//
				 //with a lambda function
				//vector<int>::iterator gt10 = find_if(myvec.begin(), myvec.end(), [](int x) {return x > 10; }); // >= C++11
				//if (gt10 != myvec.end()){}
				//

				vector<shared_ptr <User>>::iterator findNameIterator = find_if(Users.begin(), Users.end(), [](shared_ptr <User> u) {return u->GetName() == name; });
				if (findNameIterator != Users.end()) 
				{
					example = *findNameIterator;// found
					example->Display(this);
					_getch();
				}
				else {
					cout << "������ �� �������";
				}

			  /*try{vector<shared_ptr <User>>::iterator it = find_if(Users.begin(), Users.end(), IfUserName);
			  if (&it) {
				  throw runtime_error("������ �� �������");
			}*/
			/*example = *it;
			  }
			
			catch(runtime_error er)
			{
				cout << er.what();
				break;
			}example->Display(this);*/
			}
				
				break;
			case 2:
				cout << "������� id:";
				id = getInt();
				example = findUserById(id);
				if (example == NULL)
				{
					cout << "������ �� �������";
				}
			}
		
		/*if(example != NULL)*/
		/*example->Display(this);*/
		_getch();
		}
			//find
			break;
		case 6:
			return;
			break;
		default:
			continue;
		}
	}
	
	/*
	d->findUserById(id)->SetValue(new_role);*/
}
void Database::changeUsers()
{
	// ShowTable(Users, this);

	string name, password, login;

	int id;
	cout << "������� id ������������:";
	cin >> id;
	while (true)
		switch (changeUserMenu())
		{
		case 1:			
			name = getString("������� ����� ���:");
			findUserById(id)->SetName(name);
			break;
		case 2:
			login = getString("������� ����� �����:");
			findUserById(id)->SetLogin(login);
			break;
		case 3:			
			password = getString("������� ����� ������:");
			findUserById(id)->SetPassword(password);
			break;
		case 4:
			return;
			break;
		default:
			continue;
		}

	/*
	d->findUserById(id)->SetValue(new_role);*/
}

int changeUserRolesMenu() {
	int choise;
	cout << "������� ������ ������������� � �����: " << endl;
	cout << "1.�������� ����� ������������ � ����� " << endl;
	cout << "2.������� ����� ������������ � ����� " << endl;
	cout << "3.����� " << endl;
	cout << "��� �����: ";
	cin.clear();
	choise = getInt();
	return choise;
}
void Database::EditUserRoles()
{
	int id, id_role, id_user;

	while (true) 
	{
		system("cls");
		ShowTable(UserRoles, this);

		switch (changeUserRolesMenu())
		{
		case 1:
		{
			ShowTable(Roles, this);
			cout << "������� id ����:";
			cin.clear();
			 id_role = getInt();

			ShowTable(Users, this);
			cout << "������� id ������������:";
			cin.clear();
			 id_user = getInt();

			shared_ptr <UserRole> userRole(new UserRole(this, findUserById(id_user), findRoleById(id_role)));
			addUserRole(userRole);

			break;
		}
		case 2:
		{
			cout << "������� id ����� ������������ � ����� ��� �������� �����:";
			cin >> id;

			shared_ptr <UserRole> userRole = findUserRoleById(id);
			userRole->Delete();

			break;
		}
		case 3:
			return;
			break;
		default:
			continue;
		}
	}
}

int changeRoleOperationsMenu() {
	int choise;
	cout << "������� ������ ����� � ����������: " << endl;
	cout << "1.�������� ����� ���� � ��������� " << endl;
	cout << "2.������� ����� ���� � ��������� " << endl;
	cout << "3.����� " << endl;
	cout << "��� �����: ";
	cin.clear();
	choise = getInt();
	return choise;
}
void RoleOperation::SetOperation(int operation) 
{	
	this->operation = static_cast<Operations>(operation);//?
}
void Database::EditRoleOperations()
{
	int id, id_role, op_num;
	
	while (true) 
	{
		system("cls");
		ShowTable(RoleOperations, this);

		switch (changeRoleOperationsMenu())
		{
		case 1:
		{
			ShowTable(Roles, this);
			cout << "������� id ����:";
			cin.clear();
			id_role = getInt();
			if (id_role == 0) {
				break;
			}
			ShowOperations();
			cout << "������� ����� ��������:"; 
			cin.clear();
			op_num = getInt();
			if (op_num == 0) {
				break;
			}
			Operations op = static_cast<Operations>(op_num);
			
			shared_ptr <RoleOperation> roleOperation(new RoleOperation(this,op, findRoleById(id_role)));
			addRoleOperation(roleOperation);

			break;
		}
		case 2:
		{
			cout << "������� id ����� ���� � ��������� ��� �������� �����:";
			cin >> id;

			shared_ptr <RoleOperation> roleOperation = findRoleOperationById(id);
			roleOperation->Delete();

			break;
		}
		case 3:
			return;
			break;


		default:
			continue;
		}
	}
}

string operationNames[opTicketOrdering + 1] =
{
	"",
	"User Administration",
	"Movie Administration",
	"Movie Schedule",
	"Ticket Ordering"
};

string getOperationName(Operations op)
{
	return operationNames[op];
}

void ShowOperations()
{
	for (int i = opUserAdministration; i < std::size(operationNames); i++)
	{
		cout << i << ". " << operationNames[i] << endl;
	}
}
extern void ProcessSelfUserAdministration(Database* db)
{
	int id = db->GetWorker()->getID();
	string name,login, password;
	while (true)
	switch (changeUserMenu())
	{
	case 1:
		name = getString("������� ����� ���:");
		db->findUserById(id)->SetName(name);
		break;
	case 2:
		login = getString("������� ����� �����:");
		db->findUserById(id)->SetLogin(login);
		break;
	case 3:
		password = getString("������� ����� ������:");
		db->findUserById(id)->SetPassword(password);
		break;
	case 4:
		return;
		break;
	default:
		continue;
	}
}


extern void ProcessUserAdministration(Database* db)
{
	// �������������� �������������
	// �������������� �����
	// �������������� ����� ������������ � �����
	// �������������� ����� ���� � ���������
	// ����� �� ����������������� �������������
	while(true)
	switch (AdministrationMenu())
	{
	case 1:
		
		db->EditUsers();
		break;
	case 2:
		db->EditRoles();
		break;
	case 3:
		db->EditUserRoles();
		break;
	case 4:
		db->EditRoleOperations();
		break;
	case 5:
		db->Save();
		return;
		break;
	default:
		continue;
	}
}
void Database::newUser()
{
	int answer;
	bool worker;
	string name = getString("������� ��� ������������:");	
	string login = getString("������� �����:");
	string password = getString("������� ������:");
	cout << "�������� ����������� �����������?" << endl;
	cout << "1.��" << setw(20) << "2.���";
	answer = getInt();
	if (answer == 1)
		worker = true;
	else
		worker = false;
	shared_ptr <User> user(new User(this, name, password, login,worker));
	addUser(user);
}
void  Database::deleteUser() {
  int id;	
  cout << "������� ID ������������:";
  cin >> id;
  shared_ptr<User> user = findUserById(id);
  user->Delete();
}

