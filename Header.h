#pragma once
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <conio.h>
#include <string>
#include <format>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include "tinyxml2.h"
#include <set>;
#include <queue>;
using namespace std;
using namespace tinyxml2;
extern int globalObjectID;

class Database;
extern string getString(string question);
extern int GetNewID();
extern int getInt();
extern enum Operations 
{
	opUnknown,
	opUserAdministration,
	opMovieAdministration,
	opMovieSchedule,
	opTicketOrdering
};
struct MenuItem
{
	int Num;
	string name;
	Operations operation;
	int displayNum;
};
const int cMainMenuEditCurrentAccount = 1;
const int cMainMenuUserAdministration = 2;
const int cMainMenuMovieAdministration = 3;
const int cMainMenuMovieSchedule = 4;
const int cMainMenuTicketOrdering = 5;
const int cMainMenuExit = 6;
extern class Base
{protected:
	Database* db;
	int id = 0;
	bool isDeleted = false;
public:
	Base(Database* db)
	{
		this->db = db;
		id = GetNewID();
	}
	~Base()
	{
	}
	virtual void Load(XMLElement* row, Database* db)
	{
		id = row->IntAttribute("id", 0);
		if (id > globalObjectID)
		{
			globalObjectID = id;
		}
		int deleted = row->IntAttribute("deleted", 0);
		isDeleted = deleted != 0;
	};
	virtual void Save(XMLElement* row)
	{
		row->SetAttribute("id", id);
		int deleted = isDeleted ? 1 : 0;		
		row->SetAttribute("deleted", deleted);
	};	
	int getID() {
		return id;
	}
	void Delete() 
	{
		isDeleted = true;
	}
	bool IsDeleted() 
	{
		return isDeleted;
	}
	
};
extern class Role :public Base {
protected:
	string name;
public:
	//static string TypeName() { return "Роль"; };

	Role(Database* db) : Base(db) {}
	Role(Database* db, string name) : Base(db) {
		this->name = name;
	}
	void SetValue(string name);
	void Load(XMLElement* row, Database* db);
	void Save(XMLElement* row);
	void Display(Database* db) {

		cout << setw(4) << id << "|"
			<< setw(20) << name << "|" << endl;
	}
	void Header() {
			cout << "----------Роли------------" << endl;
			cout << " id |      название      |" << endl;
			cout << "--------------------------" << endl;
	}
	string GetName() {
		return name;
	}
};

string encryptDecrypt(string toEncrypt);

extern class User :public Base {
protected:
	string name;
	string encryptedPassword;
	string login;
	bool worker=false;
	
public:
	//static string TypeName() { return "Пользователь"; };
	User(Database* db) : Base(db) {
		name = "";
		encryptedPassword = "";
		login = "";
		worker = false;
	}
	User(Database* db, string name, string password, string  login, bool worker) : Base(db) {
		this->name = name;
		this->encryptedPassword = encryptDecrypt(password);
		this->login = login;
		this->worker = worker;
	}
	static User* CreateUser(Database* db)
	{   int answer, worker;
		cout << "Создание нового пользователя" << endl;
		string name = getString("Введите имя пользователя:");
		string login;
  	    string password;
		cout << "Является сотрудником кинотеатра?"<<endl;
		cout << "1.Да" << setw(20) << "2.Нет";
		answer = getInt();
		if (answer == 1)
			worker = true;
		else
			worker = false;
		enterLoginPassword(login, password);
		return new User(db, name, password, login, worker);
	}
	static void enterLoginPassword(string& login, string& password)
	{
		login = getString("Введите логин:");
		password = getString("Введите пароль:");
	}
	void SetValue(string name, string password, string  login, bool worker);

	void SetName(string name);
	void SetPassword(string password);
	void SetLogin(string  login);
	void Load(XMLElement* row, Database* db);
	void Save(XMLElement* row);
	virtual void Display(Database* db) {
		cout << setw(4) << id << "|"
			<< setw(30) << name << "|"
			<< setw(20) << login << "|"

			<< endl;
	}
	void Header() {
		cout << "--------------Пользователи-------------------------------" << endl;
		cout << " id |              ФИО              |        Логин      |" << endl;
		cout << "---------------------------------------------------------" << endl;
	}
	string GetName() {
		return name;
	}
	string GetLogin() {
		return login;
	}
	string GetPassword() {
		return encryptedPassword;
	}
	bool CheckPassword(string password) 
	{
		string s = encryptDecrypt(password);
		return (this->encryptedPassword == s);
	}
	bool ReturnWorker() {
		return worker;
	}
  
};

 
extern class UserRole :public Base {
protected:
	shared_ptr<User> user ;
	shared_ptr<Role> role ;

public:
	
	UserRole(Database* db) : Base(db) { user = NULL; role = NULL; }
	UserRole(Database* db,shared_ptr<User> user, shared_ptr <Role> role) : Base(db)
	{

		string categoryName = role->GetName();

		this->role = role;
		this->user = user;
	}
	~UserRole()
	{
	}

	int getUserID() 
	{
		return user->getID();
	}
	int getRoleID()
	{
		return role->getID();
	}

	void SetValue(shared_ptr<User> user, shared_ptr <Role> role);
	void SetUser(shared_ptr<User> user);
	void SetRole(shared_ptr<Role> role);
	void Load(XMLElement* row, Database* db);
	void Save(XMLElement* row);
	void Display(Database* db) {
		cout << setw(4) << id << "|"
			<< setw(20) << role->GetName() << "|"
			<< setw(30) << user->GetName() << "|"
			<< endl;
	}
	void Header() {
		cout << "------------Связь пользователей с ролями ----------------" << endl;
		cout << " id |      Роль          |         Пользователь         |" << endl;
		cout << "---------------------------------------------------------" << endl;
	}

	string GetNameRole() {
		return role->GetName();
	}
	string GetNameUser() {
		return user->GetName();
	}
};

extern class RoleOperation : public Base {
protected:
	Operations operation;
	shared_ptr<Role> role = NULL;
public:
	
	RoleOperation(Database* db) : Base(db) {}
	RoleOperation(Database* db,Operations operation, shared_ptr<Role> role) : Base(db) 
	{
		this->operation = operation;
		this->role = role;
	}
	void Load(XMLElement* row, Database* db);
	void Save(XMLElement* row);
	int getRoleId()
	{
		return role->getID();
	}
	Operations getOperation() 
	{
		return operation;
	}
	void SetRole(shared_ptr<Role> role);
	void SetOperation(int operation);
	void Display(Database* db);
	void Header();
};


extern class Worker: public User{
	
	string position;
public:
	Worker(string position,Database* db, string name, string password, string  login, bool worker):User( db,  name,  password,   login,  worker)
	{
		this->position = position;
	}

	void GetAdminDependedActivity(Database* db);
	string GetPosition() {
		return position;
	}
};
extern class Viewer :public User {
	bool regestrated;
public:
	Viewer(bool regestrated, Database* db, string name, string password, string  login, bool worker) :User(db, name, password, login, worker)
	{
		this->regestrated = regestrated;
	}
	Viewer(bool regestrated, Database* db) :User(db)
	{
		this->regestrated = regestrated;
	}
	void GetAdminIndependedActivity(Database* db);
};

void ShowOperations();
string getOperationName(Operations op);

extern class Date {
	int int_date;
public:
	Date(int year = 0, int mounth = 0, int day = 0)
		: int_date(year * 10000 + mounth * 100 + day) {}

	int year() const { return int_date / 10000; }
	int mounth() const { return (int_date % 10000) / 100; }
	int day() const { return int_date % 100; }
	int GetDate() {
		return int_date;
	}
};

extern class Film : public Base
{
	string name;
	int rent_start_date;
	int rent_end_date;
	int ticket_cost;
	int rating_id;
	string genre;
	
public:
	Film(Database* db) : Base(db) {}
	Film(Database* db,string name, int rent_start_date, int rent_end_date, int ticket_cost, int rating_id, string genre) : Base(db) {
		this->name = name;
		this->rent_start_date = rent_start_date;
		this->rent_end_date = rent_end_date;
		this->ticket_cost = ticket_cost;
		this->rating_id = rating_id;
		this->genre = genre;
	}
	void SetValue(string name, int rent_start_date, int rent_end_date, int ticket_cost, int rating_id, string genre) {
		this->name = name;
		this->rent_start_date = rent_start_date;
		this->rent_end_date = rent_end_date;
		this->ticket_cost = ticket_cost;
		this->rating_id = rating_id;
		this->genre = genre;
	}
	void Load(XMLElement* row, Database* db);
	void Save(XMLElement* row);
	void Display(Database* db);
	string GetName() {
		return name;
	}
	string GetGenre() {
		return genre;
	}
	int GetRating() {
		return rating_id;
	}
	void Header() {
		cout << "--------------------------------------------Фильмы-----------------------------------------------------------" << endl;
		cout << " id |      название      |начало проката| конец проката | цена за билет |возрасное ограничение|     жанр    |" << endl;
		cout << "-------------------------------------------------------------------------------------------------------------" << endl;
	}
	int GetCost() {
		return ticket_cost;
	}
	int GetStart() {
		return rent_start_date;
	}
	int GetEnd() {
		return rent_end_date;
	}
	

	friend bool operator < (const Film& t1, const Film& t2) {
		if (strcmp(t1.genre.c_str(), t2.genre.c_str()) == -1 || strcmp(t1.genre.c_str(), t2.genre.c_str()) == 0)
			return 0;
		else return 1;
		
	}
	friend bool operator == (const Film& t1, const Film& t2) {
		if (strcmp(t1.genre.c_str(), t2.genre.c_str()) == -1) {
			return 0;
		}
		else
			return 1;
	}
	friend istream& operator>>(istream& s, shared_ptr <Film> f);
	friend ostream& operator<<(ostream& s, shared_ptr <Film> f);
};

extern class Time {
	int sec;
public:
	Time() : sec(0) {}
	Time(int hours = 0, int minutes = 0, int seconds = 0)
		: sec(hours * 60 * 60 + minutes * 60 + seconds) {}
	void SetTime(int hours, int minutes, int seconds)
	{
		sec = hours * 60 * 60 + minutes * 60 + seconds;
	}
	
	
	int hours() const { return sec / (60 * 60); }
	int minutes() const { return (sec / 60) % 60; }
	int seconds() const { return sec % 60; }
	Time(const Time& ob)
		 {
		sec = ob.sec;
	}
	int GetSec() {
		return sec;
	}

};
extern class Schedule : public Base
{
	int time;
	string day_of_week;
	int rating_id;
	int hall_id;
public:
	Schedule(Database* db) : Base(db) {}
	Schedule(Database* db, string day_of_week, int time, int rating_id, int hall_id) : Base(db) {
		this->day_of_week = day_of_week;
		this->time = time;
		this->rating_id = rating_id;
		this->hall_id = hall_id;
		
	}
	void SetValue(string day_of_week, int time, int rating_id, int hall_id) {
		this->day_of_week = day_of_week;
		this->time = time;
		this->rating_id = rating_id;
		this->hall_id = hall_id;
	}
	void Load(XMLElement* row, Database* db);
	void Save(XMLElement* row);
	void Display(Database* db);
	void Header() {
		cout << "---------------------Расписание---------------------" << endl;
		cout << " id | день недели | возраст |     зал     |  время  |" << endl;
		cout << "----------------------------------------------------" << endl;
	}
	int GetHallID() {
		return hall_id;
	}
	int GetTime() {
		return time;
	}
	
};
extern class FilmRating :public Base 
{
	string name;
public:
	FilmRating(Database* db) : Base(db) {}
	FilmRating(Database* db, string name) : Base(db) {
		this->name = name;
	}
	void SetValue( string name) {
		this->name = name;
	}
	void Load(XMLElement* row, Database* db);
	void Save(XMLElement* row);
	void Display(Database* db) {

		cout << setw(4) << id << "|"
			<< setw(12) << name << "|" << endl;
	}
	void Header() {
		cout << "-Рэтинги фильмов-" << endl;
		cout << " id |   Рэтинг   |" << endl;
		cout << "-----------------" << endl;
	}
	string GetName() {
		return name;
	}
	
};
extern class FilmSchedule :public Base
{
	int film_id;
	int schedule_id;
	int date;

public:
	FilmSchedule(Database* db) : Base(db) {}
	FilmSchedule(Database* db, int film_id, int schedule_id, int date) : Base(db) {
		this->film_id = film_id;
		this->schedule_id = schedule_id;
		this->date = date;
	}
	void SetValue(int film_id, int schedule_id, int date) {
		this->film_id = film_id;
		this->schedule_id = schedule_id;
		this->date = date;
	}
	void Load(XMLElement* row, Database* db);
	void Save(XMLElement* row);
	void Display(Database* db); 
	void Header() {
		cout << "------------------------------------Расписание фильма-----------------------------------" << endl;
		cout << " id |          фильм          |     жанр фильма    | расписание |    дата    |  время  |" << endl;
		cout << "----------------------------------------------------------------------------------------" << endl;
	}
	int GetScheduleID(){
		return schedule_id;
}
	int GetFilmID() {
		return film_id;
	}
	int GetFilmDate() {
		return date;
	}
};
extern class Hall :public Base
{
	int row_count;
	int place_count;
	string name;
	int number_of_comfort_seats;
	int number_of_extra_comfort_seats;
	int* comfort_seats, * extra_comfort_seats;
public:
	Hall(Database* db) : Base(db) {}
	Hall(Database* db, int row_count, int place_count, string name, int comfort,int extra_comfort) : Base(db) {
		this->name = name;
		this->row_count = row_count;
		this->place_count = place_count;
		this->number_of_comfort_seats = comfort;
		this->number_of_extra_comfort_seats = extra_comfort;
		comfort_seats = new int[comfort];
		extra_comfort_seats= new int[extra_comfort];
	}
	~Hall() {
		delete comfort_seats;
		delete extra_comfort_seats;
	}
	
	void SetValue(int row_count, int place_count, string name);
	void SetSpecialSeats(int comfort, int extra_comfort);
	void SetRow(int row_count) {
		this->row_count = row_count;
	}
	void SetPlace(int place_count) {
		this->place_count = place_count;
	}
	void SetName(string name) {
		this->name = name;
	}

	void Load(XMLElement* row, Database* db);
	void Save(XMLElement* row);
	void Display(Database* db) {

		cout << setw(4) << id << "|"
			<< setw(6) << row_count<< "|"
			<< setw(7) << place_count << "|"
			<< setw(21) << name << "|" << endl;
	}
	void Header() {
			cout << "------------------Залы--------------------" << endl;
			cout << " id | ряды | места |       название      |" << endl;
			cout << "------------------------------------------" << endl;
	}
	string GetName() 
	{
		return name;
	}
	int GetRowCount() 
	{
		return row_count;
	}
	int GetPlaceCount() 
	{
		return place_count;
	}
	bool CheckComfort(int row) {
		for (int i=0; i < number_of_extra_comfort_seats; i++) {
			if (comfort_seats[i] == row) {
				return true;
			}
		}
		return false;
	}
	bool CheckExtraComfort(int row) {
		for (int i = 0; i < number_of_comfort_seats; i++) {
			if (extra_comfort_seats[i] == row) {
				return true;
			}
		}
		return false;
	}
};
extern class Ticket :public Base 
{
	int user_id;
	int row;
	int place;
	int cost;
	int film_shedule_id;
public:
	Ticket(Database* db) : Base(db) {}
	Ticket(Database* db, int user_id, int row, int place, int cost, int film_shedule_id) : Base(db) {
		this->user_id = user_id;
		this->row = row;
		this->place = place;
		this->cost = cost;
		this->film_shedule_id = film_shedule_id;
	}
	void SetValue(int user_id, int row, int place, int cost, int film_shedule_id) {
		this->user_id = user_id;
		this->row = row;
		this->place = place;
		this->cost = cost;
		this->film_shedule_id = film_shedule_id;
	}
	void Load(XMLElement* row, Database* db);
	void Save(XMLElement* row);
	void Display(Database* db);
	void Header() {
		cout << "----------------------------Билеты-------------------------------" << endl;
		cout << " id |            зрители           | ряды | место | цена | время | "<< endl;
		cout << "------------------------------------------------------------------" << endl;
	}
	bool TellIsItFree(int row, int place) {
		if (this->row == row && this->place == place) {
			return false;
		}
		
		return true;
	}
	void Print(ofstream& F,Database* db);
	int GetCost() { 
		return cost; 
	}
};
extern class Database 
{
protected:
	shared_ptr<User> loggedInUser = NULL;
	shared_ptr<Worker> curWorker;
	shared_ptr<Viewer> curViewer;
public:

	vector< shared_ptr<Role> > Roles;
	vector< shared_ptr<User> > Users;
	vector< shared_ptr<UserRole> > UserRoles;
	vector< shared_ptr<RoleOperation> > RoleOperations;
	vector< shared_ptr<Worker> > Workers;
	vector< shared_ptr<Viewer> > Viewers;

	vector< shared_ptr<Hall> >Halls;
	vector< shared_ptr<Film> >Films;
	vector< shared_ptr<Schedule> >Schedules;
	vector< shared_ptr<FilmRating> >FilmRatings;
	vector< shared_ptr<FilmSchedule> >FilmSchedules;
	vector< shared_ptr<Ticket> >Tickets;


	void addFirstUser();
	bool login();
	shared_ptr<User> getLoggedInUser() 
	{
		return loggedInUser;
	}
	void SetLoggedInUser(shared_ptr<User> user)
	{
		loggedInUser = user;
		
	}

	set<Operations> availableOperations();

	void Load();
	void Save();
	void Display();	
	shared_ptr<Role> findRoleById(int id);
	shared_ptr<User> findUserById(int id);
	shared_ptr<User> findUserByLoginAndPassword(string login, string password);
	shared_ptr<UserRole> findUserRoleById(int id);
	shared_ptr<RoleOperation> findRoleOperationById(int id);	
	void addRole(shared_ptr<Role> role);
	void addUser(shared_ptr<User> user);
	void addWorker(shared_ptr<Worker> worker);
	void addViewer(shared_ptr<Viewer> viewer);
	void addUserRole(shared_ptr <UserRole> userRole);
	void addRoleOperation(shared_ptr<RoleOperation> roleOperation);
	void EditRoles();
	void EditUsers();
	void EditUserRoles();
	void EditRoleOperations();
	void changeUsers();
	void newUser();
	void deleteUser();
	void changeRole();
	void newRole();
	void deleteRole();

	shared_ptr<Hall> findHallById(int id);
	shared_ptr<Film> findFilmById(int id);
	shared_ptr<Schedule> findScheduleById(int id);
	shared_ptr<FilmRating> findFilmRatingById(int id);
	shared_ptr<FilmSchedule> findFilmScheduleById(int id);
	shared_ptr<Ticket> findTicketById(int id);
	void addHall(shared_ptr<Hall> hall);
	void addFilm(shared_ptr<Film> film);
	void addSchedule(shared_ptr<Schedule> schedule);
	void addFilmRating(shared_ptr<FilmRating> film_rating);
	void addFilmSchedule(shared_ptr<FilmSchedule> film_schedule);
	void addTicket(shared_ptr<Ticket> ticket);

	void EditHalls();
	void EditFilms();
	void EditSchedules();
	void EditFilmRatings();
	void EditFilmSchedules();
	void EditTickets();

	void changeHalls();
	void changeFilms();
	void changeSchedules();
	void changeFilmRatings();
	void changeFilmSchedules();
	void changeTickets();

	void newHall();
	void newFilm();
	void newSchedule();
	void newFilmRating();
	void newFilmSchedule();
	void newTicket();

	void deleteHall();
	void deleteFilm();
	void deleteSchedule();
	void deleteFilmRating();
	void deleteFilmSchedule();
	void deleteTicket();
	
	void ShowCinemaHall(int row, int place, int hall_id);
	void Registration();

	void SetWorker(shared_ptr<User> user);
	void SetViewer(shared_ptr<User>user, bool regestrated);

	string FormPosition(shared_ptr<User> user);

	shared_ptr<User> GetloggedInUser() {
		return loggedInUser;
	}

	shared_ptr<Worker> GetWorker() {
		return curWorker;
	}
	shared_ptr<Viewer> GetViewer() {
		return curViewer;
	}
	friend void Filter(Database* db);
};

int MainMenu(Database* db);
extern void ProcessSelfUserAdministration(Database* db);
extern void ProcessUserAdministration(Database* db);
extern void ProcessMovieAdministration(Database* db);
extern void ProcessMovieSchedule(Database* db);
extern void ProcessTicketOrdering(Database* db);

template <class T>
extern void ShowTable(T table, Database* db)
{
	int rowCount;
	rowCount = table.size();
	if (rowCount > 0)
	{
		table[0]->Header();
		for (int i = 0; i < rowCount; i++)
		{
			if (!table[i]->IsDeleted())
			{
				table[i]->Display(db);
			}
		}
		cout << endl;
	}
}
template<class T>
extern shared_ptr<T> findInTableById(int id, vector< shared_ptr<T> > table) {
	shared_ptr<T> row = NULL;
	for (int i = 0; i < table.size(); i++)
	{
		if (table[i]->getID() == id)
		{
			row = table[i];
			break;
		}
	}
	/*if (row == NULL)
	{
		
	}*/
	return row;
}
extern bool AskRegistration(Database* db);
extern void SuggestRegistration(Database* db);
template<class T>
 class Printer {
	queue< shared_ptr <T>> Queue;
	string file_name;
	
public:
	Printer(string file_name) {
		this->file_name = file_name;
		
	}
	void put(shared_ptr < T> element);
	void print(Database* db);
};
template<class T>
void Printer<T>::put(shared_ptr < T> element) {
	Queue.push(element);
}
template<class T>
void Printer<T>::print(Database* db)
{
	Database* d = (Database*)db;
	ofstream outfile(file_name);
	if (outfile.is_open())
	{for(int i= Queue.size(); i>0;i--)
	{
		Queue.back()->Print(outfile, d);
		Queue.pop();
	}
	}
	outfile.close();
}
extern class Seat {
protected:
	int row;
	float full_cost;
public:
	virtual float ReturnFullCost() {
		return full_cost;
	};
	virtual void showCost() = 0;
};
class StandartSeat :public Seat {
public:
	StandartSeat(int row, float cost) {
		full_cost = cost;
		this->row = row;
	}
	void showCost() {
		cout << "Цена за билет :" << full_cost;
	}
};
class ComfortSeat :public Seat {
	float extra_cost;
public:
	ComfortSeat(int row, float cost) {
		extra_cost = 1;
		full_cost = cost + extra_cost;
		this->row = row;
	}
	void showCost() {
		cout << "Цена за билет из ряда комфортных мест:" << full_cost << endl;
	}
};
class ExtraComfortSeat :public Seat {
	float extra_cost;
public:
	ExtraComfortSeat(int row, float cost) {
		extra_cost = 2;
		full_cost = cost + extra_cost;
		this->row = row;
	}
	void showCost() {
		cout << "Цена за билет из ряда  очень комфортных мест:" << full_cost<<endl;
	}
};
template <class T>
void LoadData(string fileName, std::vector<T>* objectList, Database* db)
{
	tinyxml2::XMLDocument doc;
	fileName ="DB\\"  + fileName;
	doc.LoadFile(fileName.c_str());

	XMLElement* root = doc.RootElement();
	if (root != NULL)
	{
		XMLElement* row = root->FirstChildElement();
		while (row != NULL)
		{
			T obj = make_shared<typename T::element_type>(db);

			obj->Load(row, db);
			objectList->push_back(obj);

			row = row->NextSiblingElement();
		}
	}
}
void SetColor(int color);
class Exception
{
public:
	virtual string message() { return ""; };
	void showErrorMessage()
	{
		SetColor(192);

		cout << message() << endl;
		/*_getch();*/

		SetColor(240);
	}
};
class ErrorDate : public Exception {
protected:
	Date date;
	
public:
	ErrorDate(Date date)
	{
		this->date = date;
	}
	string message()
	{
		if(date.day()>=31)
		return "Не коректно задан день";
		if (date.mounth() >= 12)
			return "Не коректно задан месяц";
		if (date.year() >=1895)
			return "Не коректно задан год";
	}

};
