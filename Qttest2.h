#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Qttest2.h"
#include<string>
#include<Windows.h>
#include<qlabel.h>
#include<qmessagebox.h>



extern int num;

using namespace std;

namespace Ui {
    class Qttest2;
}

class time {
public:
	int year;
	int month;
	int day;
	time() {
		year = 0; month = 0; day = 0;
	}
	time(int year, int month, int day) {
		this->year = year; this->month = month; this->day = day;
	}
	bool operator > (time* a) {
		if (this->year == a->year) {
			if (this->month == a->month) {
				return this->day > a->day;
			}
			else
				return this->month > a->month;
		}
		else
			return this->year > a->year;
	}
	bool operator < (time* a) {
		return a > this;
	}
	int forcompare() {
		return year * 10000 + month * 100 + day;
	}
};

class douban :public time {
public:
	time* date;
	string comment;
	string actor;
	string director;
	int tag;
	int comment_num;
	int click;
	string name;
	string address;
	string jj;
	int anumber;
	douban() {
		date = new time();
		comment = "null";
		click = 0;
		name = "null";
		tag = 0;
		actor = "null";
		anumber = 0;
		director = "null";
		comment_num = 0;
		address = "null";
		jj = "NULL";
	}
	douban(string name, int click, int year, int month, int day) {
		this->name = name;
		this->click = click;
		this->date = new time(year, month, day);
	}
	douban& operator = (const douban& a) {
		if (this != &a) {
			this->click = a.click;
			this->name = a.name;
			this->comment = a.comment;
			this->date = a.date;
			this->tag = a.tag;
			this->actor = a.actor;
			this->director = a.director;
			this->address = a.address;
			this->anumber = a.anumber;
			this->comment_num = a.comment_num;
			this->jj = a.jj;
		}
		return *this;
	}
};


typedef struct SkiplistNode {
	douban val;
	int maxLevel;
	struct SkiplistNode** forward;
} SkiplistNode;

typedef struct {
	SkiplistNode* head;
	int level;
} Skiplist;

class Qttest2 : public QMainWindow
{
    Q_OBJECT

public:
    Qttest2(QWidget *parent = nullptr);
    ~Qttest2();

signals:

public slots:
	void start();
    void initiat();
	void select1();
	void select2();
	void select3();
	void showdata();
	void tolbt1();
	void tolbt2();
	void tolbt3();
	void tolbt4();
	void tolbt5();
	void tolbt6();
	void resource_web();
	void exit();
	void launch();
	void regist();


private:
    Ui::Qttest2Class ui;
};
