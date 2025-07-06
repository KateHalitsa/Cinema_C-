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
#include <fstream>
#include "tinyxml2.h"
#include"Header.h"

using namespace std;
using namespace tinyxml2;
int date;
string film_name;
string genre;

istream& operator>>(istream& s, shared_ptr <Film> f)
{
	string str;
	cout << "������� �������� ������: ";
	getline(s, f->name);
	getline(s, f->name);
	return s;
}
ostream& operator<<(ostream& s, shared_ptr <Film> f)
{
	s << setw(4) << f->getID() << "|"
		<< setw(20) << f->GetName() << "|" << endl;
	return s;
}
namespace Operation{ // ������������ ����
	Database GetDataWithFilms() {
		Database b;
		LoadData("Film.xml", &b.Films, &b);
		return b;
	}
	Database GetDataWithSchedule() {
		Database b;
		LoadData("Schedule.xml", &b.Schedules, &b);
		return b;
	}
}

bool compareFilmByGenre(shared_ptr<Film> film1, shared_ptr<Film> film2)
{
	return (film1->GetGenre() < film2->GetGenre());
}
bool compareFilmByName(shared_ptr<Film> film1, shared_ptr<Film> film2)
{
	return (film1->GetName() < film2->GetName());
}
bool compareScheduleByTime(shared_ptr<Schedule> schedule1, shared_ptr<Schedule> schedule2)
{
	Time time1(schedule1->GetTime()), time2 (schedule2->GetTime());
	return ((time1.hours()> time2.hours())||(time1.hours() == time2.hours()&& time1.minutes() == time2.minutes()));
}
bool compareFilmScheduleById(shared_ptr<FilmSchedule> film1, shared_ptr<FilmSchedule> film2)
{
	return (film1->getID() < film2->getID());
}
bool compareFilmScheduleByTime(shared_ptr<FilmSchedule> film_schedule1, shared_ptr<FilmSchedule> film_schedule2)
{
	Database b = Operation::GetDataWithSchedule();
	shared_ptr<Schedule> schedule1 = b.findScheduleById(film_schedule1->GetScheduleID());
	shared_ptr<Schedule> schedule2 = b.findScheduleById(film_schedule2->GetScheduleID());
	return (schedule1->GetTime() < schedule2->GetTime());
	
}
bool compareFilmScheduleByDate  (shared_ptr<FilmSchedule> film1, shared_ptr<FilmSchedule> film2)
{
	Date date1(0,0,film1->GetFilmDate()), date2(0, 0, film2->GetFilmDate());
	return ((date1.year() < date2.year())||(date1.year() == date2.year()&& date1.mounth() < date2.mounth())|| (date1.year() == date2.year() && date1.mounth() == date2.mounth()&& date1.day() < date2.day()));
}

bool compareFilmScheduleByName(shared_ptr<FilmSchedule> schedule1, shared_ptr<FilmSchedule> schedule2) {
	
	Database b= Operation::GetDataWithFilms();
	shared_ptr<Film> film1 = b.findFilmById(schedule1->GetFilmID());
	shared_ptr<Film> film2 = b.findFilmById(schedule2->GetFilmID());
	return (film1->GetName() < film2->GetName());
}
bool compareFilmScheduleByGenre(shared_ptr<FilmSchedule> schedule1, shared_ptr<FilmSchedule> schedule2) {


	Database b = Operation::GetDataWithFilms();
	shared_ptr<Film> film1 = b.findFilmById(schedule1->GetFilmID());
	shared_ptr<Film> film2 = b.findFilmById(schedule2->GetFilmID());
	return (film1->GetGenre() < film2->GetGenre());
}

void FilterScheduleByDate(Database *d) {
	int year, month, day;
	while(true){
		try {
			cout << "������� ����:" << endl;
			cout << "���: ";
			year = getInt();
			cout << "�����: ";
			month = getInt();
			cout << "����: ";
			day = getInt();
			Date date1(year, month, day);
			if (year <= 1895 || month >= 12 || day >= 31)
			{
				throw ErrorDate(date1);
			}
			date = date1.GetDate();
			break;
		}
		catch (ErrorDate fe) {
			fe.showErrorMessage();
			cout << " ������� ��������"<<endl;
	}
	}
	
	
	shared_ptr <FilmSchedule> example;
	sort(d->FilmSchedules.begin(), d->FilmSchedules.end(), compareFilmScheduleByDate);
	vector<shared_ptr <FilmSchedule>>::iterator findNameIterator = find_if(d->FilmSchedules.begin(), d->FilmSchedules.end(), [](shared_ptr <FilmSchedule> u) {return u->GetFilmDate() == date; });
	if (findNameIterator != d->FilmSchedules.end())
	{
		example = *findNameIterator;// found

		cout << "���������� ������ �� ����:";
		while (example->GetFilmDate() == date)
		{

			example->Display(d);
			++findNameIterator;
			if (findNameIterator != d->FilmSchedules.end())
				example = *(findNameIterator);
			else
				break;
		}
		_getch();
	}
	else {
		cout << "������ �� �������";
	}

}

void FilterScheduleByGenre(Database* d) {

	genre = getString("������� ����:");
	shared_ptr <FilmSchedule> example;
	sort(d->FilmSchedules.begin(), d->FilmSchedules.end(), compareFilmScheduleByGenre);
	vector<shared_ptr <FilmSchedule>>::iterator findNameIterator = find_if(d->FilmSchedules.begin(), d->FilmSchedules.end(), [](shared_ptr <FilmSchedule> u) {Database b = Operation::GetDataWithFilms(); 
	return b.findFilmById(u->GetFilmID())->GetGenre() == genre; });
	if (findNameIterator != d->FilmSchedules.end())
	{
		example = *findNameIterator;// found

		cout << "���������� ������ �� �����:";
		while (d->findFilmById(example->GetFilmID())->GetGenre() == genre)
		{

			example->Display(d);
			++findNameIterator;
			if (findNameIterator != d->FilmSchedules.end())
				example = *(findNameIterator);
			else
				break;
		}
		_getch();
	}
	else {
		cout << "������ �� �������";
	}
}
void FilterScheduleByName(Database* d) {

	
	shared_ptr <FilmSchedule> example;
	sort(d->FilmSchedules.begin(), d->FilmSchedules.end(), compareFilmScheduleByName);
	vector<shared_ptr <FilmSchedule>>::iterator findNameIterator = find_if(d->FilmSchedules.begin(), d->FilmSchedules.end(), [](shared_ptr <FilmSchedule> u) {Database b = Operation::GetDataWithFilms();
	return b.findFilmById(u->GetFilmID())->GetName() == film_name; });
	if (findNameIterator != d->FilmSchedules.end())
	{
		example = *findNameIterator;// found

		cout << "���������� ������:";
		while (d->findFilmById(example->GetFilmID())->GetName() == film_name)
		{

			example->Display(d);
			++findNameIterator;
			if (findNameIterator != d->FilmSchedules.end())
				example = *(findNameIterator);
			else
				break;
		}
		_getch();
	}
	else {
		cout << "������ �� �������";
	}
}
Time my_time(0,0,0);
void FilterScheduleByTime(Database* d) {
	int hour, minute;
	//film_name = getString("������� �������� ������:");
	while(true){
	try{
	cout << "������� ����� ������ :" << endl;
	cout << "��� :";
	hour = getInt();
	if (hour > 24 || hour < 0)
		throw 1;
	cout << "������ :";
	minute = getInt();
	if (minute > 60 || minute < 0)
		throw "������";
	break;
	}
	catch (int ) {
		cout << "�� ����� ������ ���" << endl;
	}
	catch(string)
	{
		cout << "�� ����� ������� ������" << endl;
	}}
	my_time.SetTime(hour, minute, 0);
	shared_ptr <FilmSchedule> example;
	sort(d->FilmSchedules.begin(), d->FilmSchedules.end(), compareFilmScheduleByTime);
	vector<shared_ptr <FilmSchedule>>::iterator findNameIterator = find_if(d->FilmSchedules.begin(), d->FilmSchedules.end(), [](shared_ptr <FilmSchedule> u) {Database b = Operation::GetDataWithSchedule();
	return b.findScheduleById(u->GetScheduleID())->GetTime() == my_time.GetSec(); });
	if (findNameIterator != d->FilmSchedules.end())
	{
		example = *findNameIterator;// found

		cout << "���������� ������ �� �������:";
		while (d->findScheduleById(example->GetScheduleID())->GetTime() == my_time.GetSec())
		{

			example->Display(d);
			++findNameIterator;
			if (findNameIterator != d->FilmSchedules.end())
				example = *(findNameIterator);
			else
				break;
		}
		_getch();
	}
	else {
		cout << "������ �� �������";
	}
}

void Worker ::GetAdminDependedActivity(Database* d) {
	
	while (true)
	{
		switch (MainMenu(d))
		{
		case cMainMenuEditCurrentAccount:
			ProcessSelfUserAdministration(d);
			break;
		case cMainMenuUserAdministration:
			ProcessUserAdministration(d);
			break;
		case cMainMenuMovieAdministration:
			ProcessMovieAdministration(d);
			break;
		case cMainMenuMovieSchedule:
			ProcessMovieSchedule(d);
			break;
		case cMainMenuTicketOrdering:
			ProcessTicketOrdering(d);
			break;
		case cMainMenuExit:
			return ;
			break;
		default:
			continue;
		}
	};
}
int ViewerMenu() {
	int choise;
	system("cls");
	cout << "�������� ��������: " << endl;
	cout << "1.���������� ���������� " << endl;
	cout << "2.��������� ������" << endl;
	cout << "3.����� ����� " << endl;
	cout << "4.�������� ����� " << endl;
	cout << "5.����� " << endl;
	cout << "��� �����: ";
	choise = getInt();

	return choise;
}

int FilterScheduleMenu()
{
	int choise;
	cout << "��������� ��: " << endl;
	cout << "1.���� " << endl;
	cout << "2.�����" << endl;
	cout << "3.������� " << endl;
	cout << "4.����� " << endl;
	cout << "��� �����: ";
	choise = getInt();

	return choise;
}
void Filter(Database* db) {
	while(true)
	switch (FilterScheduleMenu()) {
	case 1:
		FilterScheduleByDate(db);
		break;
	case 2:
		FilterScheduleByGenre(db);
		break;
	case 3:
		FilterScheduleByTime(db);
		break;
	case 4:
		return;
		break;
	default:
		continue;
		break;
	}
}

void Viewer::GetAdminIndependedActivity(Database* d) {
	

	while (true)
	{
		switch (ViewerMenu())
		{
		
		case 1:
		
			sort(d->FilmSchedules.begin(), d->FilmSchedules.end(), compareFilmScheduleByName);
			ShowTable(d->FilmSchedules,d);
			_getch();
			break;
	
		case 2:
			Filter(d);
			break;
		case 3:
		{
			sort(d->Films.begin(), d->Films.end(), compareFilmByName);
		film_name = getString("������� ��������:");
		shared_ptr <Film> example;
		vector<shared_ptr <Film>>::iterator findNameIterator = find_if(d->Films.begin(), d->Films.end(), [](shared_ptr <Film> u) {return u->GetName() == film_name; });
		if (findNameIterator != d->Films.end())
		{
			example = *findNameIterator;// found
			cout << example;
			//example->Display(d);
			_getch();
		}
		else {
			cout << "������ �� �������";
		}/*FilterScheduleByName(d);*/
		}
		
			
			break;
		case 4:
			if(regestrated==1)
			ProcessTicketOrdering(d);
			else {
				cout << "��� ������ ������� ����������������� � �������";
				_getch();
			}
			break;
		case 5:
			d->Save();
			return;
			break;
		default:
			continue;
		}
	};
}

shared_ptr<Hall> Database::findHallById(int id)
{
	return findInTableById(id, Halls);
}
shared_ptr<Film> Database::findFilmById(int id)
{
	return findInTableById(id, Films);
}
shared_ptr<Schedule> Database::findScheduleById(int id)
{
	return findInTableById(id, Schedules);
}
shared_ptr<FilmRating> Database::findFilmRatingById(int id)
{
	return findInTableById(id, FilmRatings);
}
shared_ptr<FilmSchedule> Database::findFilmScheduleById(int id)
{
	return findInTableById(id, FilmSchedules);
}
shared_ptr<Ticket> Database::findTicketById(int id)
{
	return findInTableById(id, Tickets);
}
void Hall::SetValue(int row_count, int place_count, string name) {
	this->name = name;
	this->row_count = row_count;
	this->place_count = place_count;
}
void Hall::SetSpecialSeats(int comfort, int extra_comfort) {
	cout << "������� ���� ��� ���������� �����:";
	int row;
	
	for (int i=0; i < comfort; i++) {

		
		row = getInt();
		/*if (row == 0)
			break;*/
		comfort_seats[i] = row;
	}
	cout << "������� ���� ���  ����� ���������� �����:";
	for (int i = 0; i < extra_comfort; i++) {
		row = getInt();
		/*if (row == 0)
			break;*/
		extra_comfort_seats[i] = row;
	}
}
void Hall::Load(XMLElement* row, Database* db) {
	Base::Load(row, db);
	string str;
	row_count = row->IntAttribute("row_count");
	place_count = row->IntAttribute("place_count");
	name = row->Attribute("name");
	number_of_comfort_seats = row->IntAttribute("number_of_comfort_seats");
	number_of_extra_comfort_seats = row->IntAttribute("number_of_extra_comfort_seats");
	comfort_seats = new int[number_of_comfort_seats];
	extra_comfort_seats = new int[number_of_extra_comfort_seats];
	for (int i = 0; i < number_of_comfort_seats; i++)
	{
		str = "row_comfort_"+ to_string(i);
		comfort_seats[i]= row->IntAttribute(str.c_str());
	}

	for (int i = 0; i < number_of_extra_comfort_seats; i++)
	{
		str = "row_extra_comfort_" + to_string(i);
		extra_comfort_seats[i] = row->IntAttribute(str.c_str());
	}
}
void Hall::Save(XMLElement* row) {
	Base::Save(row);
	string str;
	row->SetAttribute("row_count", row_count);
	row->SetAttribute("place_count", place_count);
	row->SetAttribute("name", name.c_str());
	row->SetAttribute("number_of_comfort_seats", number_of_comfort_seats);
	row->SetAttribute("number_of_extra_comfort_seats ", number_of_extra_comfort_seats);
	for (int i = 0; i < number_of_comfort_seats;i++)
	{
		str = "row_comfort_"+ to_string(i);
		row->SetAttribute(str.c_str(), comfort_seats[i]);
	}

	for (int i = 0; i < number_of_extra_comfort_seats; i++)
	{
		str = "row_extra_comfort_" + to_string(i);
		row->SetAttribute("row_extra_comfort", extra_comfort_seats[i]);
	}
}
void Film::Display(Database* db) {
	Database* d = (Database*)db;
	Date date1(0, 0, rent_start_date), date2(0, 0, rent_end_date);

	
		cout << setw(4) << id << "|"
		<< setw(20) << name << "|";
if (date1.day() < 10 && date1.mounth()<10)
	cout << setw(5)<<"0" << date1.day() << "." <<"0" <<date1.mounth() << "." << date1.year() << "|";
if (date1.day() >= 10 && date1.mounth() < 10)
	cout << setw(6)<< date1.day() << "." << "0" << date1.mounth() << "." << date1.year() << "|";
if (date1.day() < 10 && date1.mounth() >= 10)
	cout << setw(5) << "0"  << date1.day() << "." << date1.mounth() << "." << date1.year() << "|";
if (date1.day() >= 10 && date1.mounth() >= 10)
	cout << setw(6) << date1.day() << "." << date1.mounth() << "." << date1.year() << "|";

if (date2.day() < 10 && date2.mounth() < 10)
	cout << setw(6) << "0" << date2.day() << "." << "0" << date2.mounth() << "." << date2.year() << "|";
if (date2.day() >= 10 && date2.mounth() < 10)
	cout << setw(7) << date2.day() << "." << "0" << date2.mounth() << "." << date2.year() << "|";
if (date2.day() < 10 && date2.mounth() >= 10)
	cout << setw(6) << "0" << date2.day() << "." << date2.mounth() << "." << date2.year() << "|";
if (date2.day() >= 10 && date2.mounth() >= 10)
cout << setw(7) << date2.day() << "." << date2.mounth() << "." << date2.year() << "|";

	cout << setw(15) << ticket_cost << "|"
		<< setw(21) << d->findFilmRatingById(rating_id)->GetName() << "|"
		<< setw(13) << genre << "|"
		<< endl;
	
}

void Film::Load(XMLElement* row, Database* db) {
	Base::Load(row, db);
	rent_start_date = row->IntAttribute("rent_start_date");
	rent_end_date = row->IntAttribute("rent_end_date");
	ticket_cost = row->IntAttribute("ticket_cost");
	rating_id = row->IntAttribute("rating_id");
	name = row->Attribute("name");
	genre = row->Attribute("genre");
}
void Film::Save(XMLElement* row) {
	Base::Save(row);
	row->SetAttribute("rent_start_date", rent_start_date);
	row->SetAttribute("rent_end_date", rent_end_date);
	row->SetAttribute("name", name.c_str());
	row->SetAttribute("ticket_cost", ticket_cost);
	row->SetAttribute("rating_id", rating_id);
	row->SetAttribute("genre", genre.c_str());
}


void Schedule::Load(XMLElement* row, Database* db) {
	Base::Load(row, db);
	time = row->IntAttribute("time");
	hall_id = row->IntAttribute("hall_id");
	rating_id = row->IntAttribute("rating_id");
	day_of_week = row->Attribute("day_of_week");
	
}
void Schedule::Save(XMLElement* row) {
	Base::Save(row);
	row->SetAttribute("time", time);
	row->SetAttribute("hall_id", hall_id);
	row->SetAttribute("day_of_week", day_of_week.c_str());
	row->SetAttribute("rating_id", rating_id);
	
}
void Schedule::Display(Database* d) {
	Time my_time(0, 0, time);
	Hall hall =*d->findHallById(hall_id);
	string hall_name = hall.GetName();
	string rating_name = d->findFilmRatingById(rating_id)->GetName();
	cout << setw(4) << id << "|"
		<< setw(13) << day_of_week << "|"
		<< setw(9) << rating_name << "|"
		<< setw(13) << hall_name << "|"
		<< setw(6) << my_time.hours() << ":" << my_time.minutes() << "|" << endl;
}

void FilmRating::Load(XMLElement* row, Database* db) {
	Base::Load(row, db);
	name = row->Attribute("name");
}
void FilmRating::Save(XMLElement* row) {
	Base::Save(row);
	row->SetAttribute("name", name.c_str());
}

void FilmSchedule::Load(XMLElement* row, Database* db) {
	Base::Load(row, db);
	film_id = row->IntAttribute("film_id");
	schedule_id = row->IntAttribute("schedule_id");
	date = row->IntAttribute("date");

}
void FilmSchedule::Save(XMLElement* row) {
	Base::Save(row);
	row->SetAttribute("film_id", film_id);
	row->SetAttribute("schedule_id", schedule_id);
	row->SetAttribute("date", date);
}
void FilmSchedule::Display(Database* d) {
	Date date1(0, 0, date);
	shared_ptr<Schedule> schedule = d->findScheduleById(GetScheduleID());
	Time my_time(0, 0, schedule->GetTime());
	cout << setw(4) << id << "|"
		<< setw(25) << d->findFilmById(film_id)->GetName() << "|"
		<< setw(20) << d->findFilmById(film_id)->GetGenre() << "|"
		<< setw(12) << schedule_id << "|"
		<< setw(4) << date1.day() << "." << date1.mounth() << "." << date1.year() << "|" 
	<< setw(6) << my_time.hours() << ":" << my_time.minutes() << "|" << endl;
}
void Ticket::Load(XMLElement* row, Database* db) {
	Base::Load(row, db);
	user_id = row->IntAttribute("user_id");
	this->row = row->IntAttribute("row");
	place = row->IntAttribute("place");
	cost = row->IntAttribute("cost");
	film_shedule_id = row->IntAttribute("film_shedule_id");
}
void Ticket::Save(XMLElement* row) {
	Base::Save(row);
	row->SetAttribute("user_id", user_id);
	row->SetAttribute("row", this->row);
	row->SetAttribute("cost", cost);
	row->SetAttribute("film_shedule_id", film_shedule_id);
	row->SetAttribute("place", place);
}
void Ticket::Display(Database* d) {
	
	shared_ptr < Schedule > schedule = d->findScheduleById(d->findFilmScheduleById(film_shedule_id)->GetScheduleID());
	Time my_time(0, 0, schedule->GetTime());
	cout << setw(4) << id << "|"
		<< setw(30) << d ->findUserById( user_id)->GetName() << "|"
		<< setw(6) << row << "|"
		<< setw(7) << place << "|"
		<< setw(6) << cost << "|"
		<< setw(4) << my_time.hours() << ":" << my_time.minutes() << "|" << endl;
}
void Ticket::Print(ofstream& F, Database* db) { 
	Database * d = (Database*)db;
	shared_ptr < Schedule > schedule = d->findScheduleById(d->findFilmScheduleById(film_shedule_id)->GetScheduleID());
	Time my_time(0, 0, schedule->GetTime());
	F << "---------------------------"<<endl;
	F << "ID ������������:" << user_id << endl;
	F <<"�����:" <<row << endl;
	F <<"���:" << place << endl;
	F <<"����:"<< cost << endl;
	F <<"�����:" << d->findFilmById(d->findFilmScheduleById(film_shedule_id)->GetFilmID())->GetName() << endl;
	F <<"�����:" << my_time.hours()<<":"<< my_time.minutes() << endl;
	F << "---------------------------" << endl;
}
void Database::addHall(shared_ptr<Hall> hall)
{
	Halls.push_back(hall);
}

int EditHallsMenu() {
	int choise;

	cout << "�������� ��������: " << endl;
	cout << "1.�������� ��� " << endl;
	cout << "2.�������� ������ ����" << endl;
	cout << "3.������� ��� " << endl;
	cout << "4.����� " << endl;
	cout << "��� �����: ";
	 choise=getInt();

	return choise;
}
void Database::EditHalls() {
	while (true)
	{
		system("cls");
		
		ShowTable(Halls, this);
		
		switch (EditHallsMenu())
		{
		case 1:
			newHall();//��������
			break;
		case 2:
			changeHalls();
			break;
		case 3:
			deleteHall();
			break;

		case 4:
			return;
			break;
		default:
			continue;
		}
	}

}
void Database::changeHalls() {
	int id, rows, places;
	string new_hall;
	
	cout << "������� id ����:";
	cin >> id;
	cout << "������� ���������� �����:";
	cin >> rows;
	cout << "������� ���������� ���� � ����:";
	cin >> places;
	new_hall = getString("������� ����� ��� ����:");

	findHallById(id)->SetValue(rows, places, new_hall);
}
void Database::newHall() {
	string name = getString("������� ���:");
	int comfort = 0, extracomfort = 0;
	cout << "������� ���������� �����:" << endl;
	int row_count = getInt();
	cout << "������� ���������� ���� � ����:" << endl;
	int place_count = getInt();
	cout << "������� ���������� ����� ���������� ���� :" << endl;
	comfort = getInt();
	cout << "������� ���������� ���� � ���� ������-���������� ����:" << endl;
	extracomfort = getInt();

	shared_ptr <Hall> hall(new Hall(this,  row_count, place_count, name,comfort,extracomfort));
	hall->SetSpecialSeats(comfort, extracomfort);
	addHall(hall);
}
void Database::deleteHall() {
	int id;
	cout << "������� ID ����:";
	cin >> id;
	shared_ptr<Hall> hall = findHallById(id);
	hall->Delete();
}
int MovieAdministrationMenu()
{
	system("cls");
	int choise;
	cout << "���� ��������� �� �������: " << endl;
	cout << "1.�������������� ������� " << endl;
	cout << "2.�������������� ���������� ��������� " << endl;//?
	cout << "3.����� �� ����������������� ������� " << endl;
	cout << "��� �����: ";
	 choise=getInt();
	return choise;
}

int EditFilmsMenu() {
	int choise;

	cout << "�������� ��������: " << endl;
	cout << "1.�������� ����� " << endl;
	cout << "2.�������� ������ ������" << endl;
	cout << "3.������� ����� " << endl;
	cout << "4.������������� " << endl;
	cout << "5.����� ����� " << endl;
	cout << "6.����� " << endl;
	cout << "��� �����: ";
	 choise = getInt();

	return choise;
}
int SortFilmsMenu() {
	int choise;

	cout << "�������� ��������: " << endl;
	cout << "1.�� �������� " << endl;
	cout << "2.�� �����" << endl;
	cout << "��� �����: ";
	choise = getInt();

	return choise;
}	
void Database::EditFilms() {

	while (true)
	{
		system("cls");

		ShowTable(Films, this); 
	
		switch (EditFilmsMenu())
		{
		case 1:
			newFilm();//��������
			break;
		case 2:
			changeFilms();
			break;
		case 3:
			deleteFilm();
			break;
			
		case 4:
			switch (SortFilmsMenu())
			{
			case 1:
				sort(Films.begin(), Films.end(), compareFilmByName);
				break;
			case 2:
				sort(Films.begin(), Films.end(), compareFilmByGenre);
				break;
			default:
				continue;
				break;
			}

			
			break;
		case 5:
		{sort(Films.begin(), Films.end(), compareFilmByName);
		film_name = getString("������� ���:");
		shared_ptr <Film> example;
		vector<shared_ptr <Film>>::iterator findNameIterator = find_if(Films.begin(), Films.end(), [](shared_ptr <Film> u) {return u->GetName() == film_name; });
		if (findNameIterator != Films.end())
		{
			example = *findNameIterator;// found
			example->Display(this);
			_getch();
		}
		else {
			cout << "������ �� �������";
		}
		}
			break;
		case 6:

			return;
			break;
		default:
			continue;
		}
	}
}
int ChangeFilmsMenu() {
	int choise;

	cout << "��������: " << endl;
	cout << "1.�������� " << endl;
	cout << "2.����� �������" << endl;
	cout << "3.����" << endl;
	cout << "4.������� " << endl;
	cout << "5.���� " << endl;
	cout << "6.����� " << endl;
	cout << "��� �����: ";
	choise = getInt();

	return choise;
}


void Database::addFilm(shared_ptr<Film> film) {
	Films.push_back(film);
}
void Database::changeFilms() {
	int year, month, day, ticket_cost, rating_id;
	string name;
	string genre;
	cout << "������� id ������:";
	int id = getInt();
	Film film = *findFilmById(id);
	switch (ChangeFilmsMenu())
	{
	case 1:
		name = getString("������� �������� ������:");
		findFilmById(id)->SetValue(name, film.GetStart(), film.GetEnd(), film.GetCost(), film.GetRating(), film.GetGenre());
		break;
	case 2:	
	{cout << "������� ���� ������ �������:" << endl;
	cout << "���: ";
	year = getInt();
	cout << "�����: ";
	month = getInt();
	cout << "����: ";
	day = getInt();
	Date date1(year, month, day);
	int rent_start_date = date1.GetDate();
	cout << "������� ���� ����� �������:" << endl;
	cout << "���: ";
	year = getInt();
	cout << "�����: ";
	month = getInt();
	cout << "����: ";
	day = getInt();
	Date date2(year, month, day);
	int rent_end_date = date2.GetDate();

	findFilmById(id)->SetValue(film.GetName(), rent_start_date, rent_end_date, film.GetCost(), film.GetRating(), film.GetGenre());
	}
		break;
	case 3:	cout << "������� ���� �� �����:" << endl;
		 ticket_cost = getInt();
		 findFilmById(id)->SetValue(film.GetName(), film.GetStart(), film.GetEnd(), ticket_cost, film.GetRating(), film.GetGenre());
		break;
	case 4:
		ShowTable(FilmRatings, this);
		cout << "������� id �������� ������:" << endl;
		rating_id = getInt();
		findFilmById(id)->SetValue(film.GetName(), film.GetStart(), film.GetEnd(), film.GetCost(), rating_id, film.GetGenre());
		break;
	case 5:
		 genre = getString("������� �������� ����:");
		findFilmById(id)->SetValue(film.GetName(), film.GetStart(), film.GetEnd(), film.GetCost(), film.GetRating(), genre);
		break;

		
	}
}
void Database::newFilm() {
	int year, month, day;
	string name = getString("������� �������� ������:");
	cout << "������� ���� ������ �������:" << endl;
	cout << "���: ";
	year = getInt();
	cout << "�����: ";
	month = getInt();
	cout << "����: ";
	day = getInt();
	Date date1(year, month, day);
	int rent_start_date = date1.GetDate();
	cout << "������� ���� ����� �������:" << endl;
	cout << "���: ";
	year = getInt();
	cout << "�����: ";
	month = getInt();
	cout << "����: ";
	day = getInt();
	Date date2(year, month, day);
	int rent_end_date = date2.GetDate();

	cout << "������� ���� �� �����:" << endl;
	int ticket_cost = getInt();
	ShowTable(FilmRatings, this);
	cout << "������� id ��������:" << endl;
	int rating_id = getInt();
	string genre = getString("������� �������� ����:");
	shared_ptr <Film> film(new Film(this,  name, rent_start_date, rent_end_date,  ticket_cost, rating_id,  genre));
	addFilm(film);
}
void Database::deleteFilm() {
	int id;
	cout << "������� ID ������:";
	cin >> id;
	shared_ptr<Film> film = findFilmById(id);
	film->Delete();
}
int EditSchedulesMenu() {
	int choise;

	cout << "�������� ��������: " << endl;
	cout << "1.�������� ���������� " << endl;
	cout << "2.�������� ������ ����������" << endl;
	cout << "3.������� ���������� " << endl;
	cout << "4.����� " << endl;
	cout << "��� �����: ";
	 choise = getInt();

	return choise;
}
void Database::EditSchedules() {
	while (true)
	{
		system("cls");

		ShowTable(Schedules, this);

		switch (EditSchedulesMenu())
		{
		case 1:
			newSchedule();//��������
			break;
		case 2:
			changeSchedules();//��������
			break;
		case 3:
			deleteSchedule();//�������
			break;

		case 4:
			return;
			break;
		default:
			continue;
		}
	}
}
void Database::addSchedule(shared_ptr<Schedule> schedule) {
	Schedules.push_back(schedule);
}
void Database::changeSchedules() {
	int id;

	cout << "������� id ����������:";
	id = getInt();
	int hour, minute, time, rate, hall_id;
	string str;
	string day_of_week = getString("������� ���� ������:");
	ShowTable(FilmRatings, this);
	cout << "������� id ��������:" << endl;
	int rating_id = getInt();
	shared_ptr<FilmRating> rating = findFilmRatingById(rating_id);
	str = rating->GetName();
	rate = atoi(str.erase(str.find('+'), 1).c_str());

	while (true) {

		while (true) {
			try {
				cout << "������� ����� ������ :" << endl;
				cout << "��� :";
				hour = getInt();
				if (hour > 24 || hour < 0)
					throw 1;
				cout << "������ :";
				minute = getInt();
				if (minute > 60 || minute < 0)
					throw "������";
				break;
			}
			catch (int) {
				cout << "�� ����� ������ ���" << endl;
			}
			catch (string)
			{
				cout << "�� ����� ������� ������" << endl;
			}
			
		Time my_time(hour, minute, 0);
		time = my_time.GetSec();
		if (rate <= 16 && my_time.hours() > 21) {
			cout << "������� ����� ����� �����";
		}
		else {
			break;
		}
		}
	
			ShowTable(Halls, this);
			cout << "������� id ����:" << endl;
			 hall_id = getInt();
			break; 
	}	
		
	
	//string day_of_week = getString("������� ���� ������:");
	//cout << "������� ����� ������ :" << endl;
	//int time = getInt();
	//cout << "������� id ��������:" << endl;
	//int rating_id = getInt();
	//cout << "������� ���������� ���� � ����:" << endl;
	//int hall_id = getInt();

	findScheduleById(id)->SetValue( day_of_week,  time,  rating_id,  hall_id);
}
void Database::newSchedule() {
	int hour, minute, time,rate;
	string str;
	string day_of_week = getString("������� ���� ������:");
	ShowTable(FilmRatings, this);
	cout << "������� id ��������:" << endl;
	int rating_id = getInt();
	shared_ptr<FilmRating> rating = findFilmRatingById(rating_id);
	str = rating->GetName();
	rate= atoi(str.erase(str.find('+'), 1).c_str());

	while(true){
		
		while (true) {
			try {
				cout << "������� ����� ������ :" << endl;
				cout << "��� :";
				hour = getInt();
				if (hour > 24 || hour < 0)
					throw 1;
				cout << "������ :";
				minute = getInt();
				if (minute > 60 || minute < 0)
					throw "������";
				break;
			}
			catch (int) {
				cout << "�� ����� ������ ���" << endl;
			}
			catch (string)
			{
				cout << "�� ����� ������� ������" << endl;
			}
		}
	Time my_time(hour, minute, 0);
	
	if (rate <= 16 && my_time.hours() > 21) {
		cout << "������� ����� ����� �����";
	}
	else {
		ShowTable(Halls, this);
cout << "������� id ����:" << endl;
	int hall_id = getInt();
	shared_ptr <Schedule> schedule(new Schedule(this, day_of_week, my_time.GetSec(), rating_id,  hall_id));	
	addSchedule(schedule);
	break;}
}
	
}
void Database::deleteSchedule() {
	int id;
	cout << "������� ID ����������:";
	cin >> id;
	shared_ptr<Schedule> schedule = findScheduleById(id);
	schedule->Delete();
}
int EditFilmRatingsMenu() {
	int choise;

	cout << "�������� ��������: " << endl;
	cout << "1.�������� ������� " << endl;
	cout << "2.�������� ������ �������" << endl;
	cout << "3.������� ������� " << endl;
	cout << "4.����� " << endl;
	cout << "��� �����: ";
	 choise = getInt();

	return choise;
}
void Database::EditFilmRatings() {
	while (true)
	{
		system("cls");

		ShowTable(FilmRatings, this);

		switch (EditFilmRatingsMenu())
		{
		case 1:
			newFilmRating();//��������
			break;
		case 2:
			changeFilmRatings();
			break;
		case 3:
			deleteFilmRating();
			break;

		case 4:
			return;
			break;
		default:
			continue;
		}
	}
}
void Database::addFilmRating(shared_ptr<FilmRating> film_rating){
	FilmRatings.push_back(film_rating);
	
}
void Database::changeFilmRatings() {
	int id;
	cout << "������� ID ������:";
	 id=getInt();
	string name = getString("������� ���:");
	findFilmRatingById(id)->SetValue(name);
}
void Database::newFilmRating() {
	string name = getString("������� �������:");
	shared_ptr <FilmRating> film_rating(new FilmRating(this, name));
	addFilmRating(film_rating);
}
void Database::deleteFilmRating() {
	int id;
	cout << "������� ID ������:";
	cin >> id;
	shared_ptr<FilmRating> film_rating = findFilmRatingById(id);
	film_rating->Delete();
}

int EditFilmSchedulesMenu() {
	int choise;

	cout << "�������� ��������: " << endl;
	cout << "1.�������� ���������� ������ " << endl;
	cout << "2.�������� ������ ���������� ������ " << endl;
	cout << "3.������� ���������� ������  " << endl;
	cout << "4.����� " << endl;
	cout << "��� �����: ";
	 choise = getInt();

	return choise;
}
void Database::EditFilmSchedules() {
	while (true)
	{
		system("cls");

		ShowTable(FilmSchedules, this);
	
		switch (EditFilmSchedulesMenu())
		{
		case 1:
			newFilmSchedule();//��������
			
			break;
		case 2:
			changeFilmSchedules();
			break;
		case 3:
			deleteFilmSchedule();
			break;

		case 4:
			Save();
			return;
			break;
		default:
			continue;
		}
	}
}
void Database::addFilmSchedule(shared_ptr<FilmSchedule> film_schedule) {
	FilmSchedules.push_back(film_schedule);
}
void Database::newFilmSchedule() {
	int year, month, day, date;
	
	ShowTable(Films, this);
	cout << "������� id ������:" << endl;
	int film_id = getInt();
	ShowTable(Schedules, this);
	cout << "������� id ����������:" << endl;
	int schedule_id = getInt();
	while (true) {
		try {
			cout << "������� ����:" << endl;
			cout << "���: ";
			year = getInt();
			cout << "�����: ";
			month = getInt();
			cout << "����: ";
			day = getInt();
			Date date1(year, month, day);
			if (year <= 1895 || month > 12 || day > 31)
			{
				throw ErrorDate(date1);
			}

			//Date date1(year, month, day);
			date = date1.GetDate();
			if (findFilmById(film_id)->GetStart() > date || findFilmById(film_id)->GetEnd() < date) {
				cout << "�� �������� � ��������� �����";
				continue;
			}
			//date = date1.GetDate();
			else { 
				break;
			}
		}
		catch (ErrorDate fe) {
			fe.showErrorMessage();
			cout << " ������� ��������" << endl;
		}
	}
	
	shared_ptr <FilmSchedule> film_schedule(new FilmSchedule(this,  film_id,  schedule_id,  date));
	addFilmSchedule(film_schedule);
}
void Database::changeFilmSchedules() {
	int year, month, day;
	int id;
	int date;
	cout << "������� ID ���������� ������:";
	id = getInt();
	shared_ptr<FilmSchedule> film_schedule = findFilmScheduleById(id);
	
	cout << "������� id ������:" << endl;
	int film_id = getInt();
	cout << "������� id ����������:" << endl;
	int schedule_id = getInt();
	while(true){
		try {
			cout << "������� ����:" << endl;
			cout << "���: ";
			year = getInt();
			cout << "�����: ";
			month = getInt();
			cout << "����: ";
			day = getInt();
			Date date1(year, month, day);
			if (year <= 1895 || month >= 12 || day >= 31)
			{
				throw ErrorDate(date1);
			}
			
			//Date date1(year, month, day);
	 date = date1.GetDate();
	if (findFilmById(film_id)->GetStart() < date && findFilmById(film_id)->GetEnd() > date) {
		cout << "�� �������� � ��������� �����";
		continue;
	}
			//date = date1.GetDate();
			break;
		}
		catch (ErrorDate fe) {
			fe.showErrorMessage();
			cout << " ������� ��������"<<endl;
	}
	}
	findFilmScheduleById(id)->SetValue(film_id, schedule_id, date);
	
}
void Database::deleteFilmSchedule() {
	int id;
	cout << "������� ID ���������� ������:";
	 id=getInt();
	shared_ptr<FilmSchedule> film_schedule = findFilmScheduleById(id);
	film_schedule->Delete();
}

int EditTicketsMenu() {
	int choise;

	cout << "���� ������ ������: " << endl;
	cout << "1.�������� ������� ������ " << endl;
	cout << "2.�������� �������� ����� " << endl;
	cout << "3.�������� ����� �� ������  " << endl;

	cout << "4.����� " << endl;
	cout << "��� �����: ";
	 choise = getInt();

	return choise;
}
void Database::EditTickets() {
	
	while (true)
	{
		system("cls");
		if(GetloggedInUser()->ReturnWorker()==1)
		{
		ShowTable(Tickets, this);
		}
		switch (EditTicketsMenu())
		{
		case 1:
			newTicket();//��������
			break;
		case 2:
			changeTickets();
			break;
		case 3:
			deleteTicket();
			break;

		case 4:
			return;
			break;
		default:
			continue;
		}
	}
}
void Database::addTicket(shared_ptr<Ticket> ticket) {
	Tickets.push_back(ticket);
}
void Database::changeTickets() {
	int id;
	deleteTicket();
	newTicket();
	//cout << "������� id �����:";
	//id = getInt();
	//cout << "������� ����� ������ :" << endl;
	//int time = getInt();
	//cout << "������� id ��������:" << endl;
	//int user_id = getInt();
	///*cout << "������� ���������� ���� � ����:" << endl;
	//int cost = getInt();*/
	//cout << "������� �����:" << endl;
	//int place = getInt();
	//cout << "������� ���:" << endl;
	//int row= getInt();
	//cout << "������� id ���������� ������:" << endl;
	//int film_shedule_id = getInt();

	//findTicketById(id)->SetValue( user_id,  row, place, findTicketById(id)->GetCost(), film_shedule_id);


}
void SetColor(int color)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}
void Database::ShowCinemaHall(int row, int place, int hall_id) {
	bool** cinima_hall;
	int color, hall_places=findHallById(hall_id)->GetPlaceCount(), hall_rows= findHallById(hall_id)->GetRowCount();
	cinima_hall = new bool* [hall_places];
	for (int i = 0; i < hall_places; i++)
	{
		cinima_hall[i] = new bool[hall_rows];
	}
	cout << setw(hall_rows*2) << "*�����*" << endl;
	cout << "  ";
	for (int i = 1; i <=  hall_rows; i++)
	{
		
		if (i==1)
		{
			for (int k = 1; k <= hall_places; k++)
			{
				if (k < 10)
					cout << " " << k << " ";
				else
					cout << " " << k;
			}
			cout << endl;
		}
		if (i < 10)
		{
			cout << i << " ";
		}
		else{
		cout << i;
		}
		for (int j = 1; j <=hall_places; j++)
		{
			
			if (row != 0 && i == row)
		{
			SetColor(224);
			
		}
			if (place != 0 && j== place)
			{
				SetColor(176);
				if( j== place &&  i== row)
					SetColor(160);

			}

			for (int k = 0; k < Tickets.size(); k++)
			{
				cinima_hall[i][j] = Tickets[k]->TellIsItFree(i, j);
				if (!cinima_hall[i][j]) {
					if (j == place && i == row)
						SetColor(192);
					cout << " X ";
					
					break;
				}
			}
			if (cinima_hall[i][j]) {
				cout <<" O ";
			}
				
			SetColor(240);

		}
		cout << endl;
		
	}
	
}
int CheakTicket(int row, int place,Database*d) {
	int choise=0;
	for (int k = 0; k < d->Tickets.size(); k++)
	{

		if (!d->Tickets[k]->TellIsItFree(row, place)) {

			cout << "����� ������" << endl;
			cout << "1. ������ ������" << endl;
			cout << "2. ������  �������" << endl;
			cout << "3. �����" << endl;
			choise = getInt();
			break;
		}
	}
	return choise;
}
void Database::newTicket() {
	Printer<Ticket> printer("Ticket.txt");
	int user_id=0, film_id,choise;
	int row = 0, place = 0, cost;
	while(true){
	system("cls");
	ShowTable(Films,this);
	cout << "������� id ������:" << endl;
	 film_id = getInt();
	system("cls");
	if(loggedInUser->ReturnWorker()==1)
	{
	ShowTable(Users, this);
	cout << "������� id ������������:" << endl;
	 user_id = getInt();
	}
	else
	{
		 user_id = curViewer->getID();
	}
	system("cls");

	film_name = findFilmById(film_id)->GetName();
	//ShowTable(FilmSchedules, this);
	FilterScheduleByName(this);
	cout << endl << "������� id ����������:" << endl;
	int film_shedule_id = getInt();
	if (film_shedule_id == 0) {
		return;
	}
	cost = findFilmById(film_id)->GetCost();
	int hall_id =findScheduleById( findFilmScheduleById(film_shedule_id)->GetScheduleID())->GetHallID();
	system("cls");
	cout << "���:" << endl;
	row = 0; place = 0;

	ShowCinemaHall( row, place,  hall_id);
	cout << "������� ���:" << endl;
	row = getInt();
	
	if (findHallById(hall_id)->CheckComfort(row)) {
		ComfortSeat seat(row, findFilmById(film_id)->GetCost());
		seat.showCost();
		_getch();
		cost = seat.ReturnFullCost();
	}
	if (findHallById(hall_id)->CheckExtraComfort(row)) {
		ExtraComfortSeat seat(row, findFilmById(film_id)->GetCost());
		seat.showCost();
		_getch();
		cost = seat.ReturnFullCost();
	}
	else {
		StandartSeat seat(row, findFilmById(film_id)->GetCost());
		seat.showCost();
	
	}
	cout << endl;
	system("cls");
	cout  << "���:" << endl;
	ShowCinemaHall(row, place, hall_id);
	cout << "������� �����:" << endl;
	place = getInt();
	ShowCinemaHall(row, place, hall_id);
	choise = 0;
	choise=CheakTicket(row, place, this);
while(choise!=0)
	switch (choise)
	{
	
	case 1:
		system("cls");
		cout << "���:" << endl;
		row = 0; place = 0;

		ShowCinemaHall(row, place, hall_id);
		cout << "������� ���:" << endl;
		row = getInt();

		if (findHallById(hall_id)->CheckComfort(row)) {
			ComfortSeat seat(row, findFilmById(film_id)->GetCost());
			seat.showCost();
			cost = seat.ReturnFullCost();
		}
		if (findHallById(hall_id)->CheckExtraComfort(row)) {
			ExtraComfortSeat seat(row, findFilmById(film_id)->GetCost());
			seat.showCost();
			cost = seat.ReturnFullCost();
		}
		else {
			StandartSeat seat(row, findFilmById(film_id)->GetCost());
			seat.showCost();

		}
		cout << endl;
		system("cls");
		cout  << "���:" << endl;
		ShowCinemaHall(row, place, hall_id);
		cout << "������� �����:" << endl;
		place = getInt();
		choise = CheakTicket(row, place, this);
		continue;
		break;
	case 2:
		break;
		break;
	case 3:
		return;
		break;
	}
	if (choise == 2)
		continue;
	cout << "����:"<<cost << endl;
	cout << "��������?" << endl;
	cout << "1. ��" << endl;
	cout << "2. ���" << endl;
	int choice = getInt();
	if(choice==1){
	shared_ptr <Ticket> ticket(new Ticket(this, user_id, row, place, cost, film_shedule_id));
	addTicket(ticket);
	printer.put(ticket);
	cout << "�����������?" << endl;
	cout << "1. ��" << endl;
	cout << "2. ���" << endl;
	cout << "3. �������� ���" << endl;
	 choice = getInt();
	if (choice == 1) {

		printer.print(this);
		return;
	}
	}
	if (choice == 2) {
		return;
	}
	if (choice == 3) {
		continue;
	}
	
}

}
void Database::deleteTicket() {
	int id;
	cout << "������� ID ������:";
	cin >> id;
	shared_ptr<Ticket> ticket = findTicketById(id);
	ticket->Delete();
}


int MovieScheduleMenu()
{

	system("cls");
	int choise;
	cout << "���� ��������� �� ����������: " << endl;
	cout << "1.�������������� ���������� " << endl;
	cout << "2.�������������� ���������� ������" << endl;//?
	cout << "3.�������������� �����" << endl;
	cout << "4.����� �� ����������������� ������� " << endl;
	cout << "��� �����: ";
	 choise = getInt();
	return choise;
}


extern void ProcessMovieAdministration(Database* db)
{
	
	while (true)
		switch (MovieAdministrationMenu())
		{
		case 1:

			db->EditFilms();
			break;
		case 2:
			db->EditFilmRatings();
			break;
		
		case 3:
			db->Save();
			return;
			break;
		default:
			continue;
		}
	
}

extern void ProcessMovieSchedule(Database* db) {
	while (true)
		switch (MovieScheduleMenu())
		{
		case 1:

			db->EditSchedules();
			break;
		case 2:
			db->EditFilmSchedules();
			break;
		case 3:
			db->EditHalls();
			break;
		
		case 4:
			db->Save();
			return;
			break;
		default:
			continue;
		}
}
int TicketOrderingMenu()
{

	system("cls");
	int choise;
	cout << "���� ������ �������: " << endl;
	cout << "1.��������������  " << endl;
	cout << "2.�������������� " << endl;//?
	cout << "3.��������������  " << endl;
	cout << "4.��������������  " << endl;
	cout << "5.����� �� ����������������� ������� " << endl;
	cout << "��� �����: ";
	 choise = getInt();
	return choise;
}
extern void ProcessTicketOrdering(Database* db) {

	db->EditTickets();
	db->Save();
			return;
}


