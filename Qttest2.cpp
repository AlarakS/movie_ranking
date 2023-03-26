#include "Qttest2.h"
#include<stdio.h>
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<QTextBrowser>
//#include<qlabel.h>
#include<algorithm>



#define MAX(a, b) ((a) > (b) ? (a) : (b))
const int MAX_LEVEL = 32;
const int P_FACTOR = RAND_MAX >> 2;
const int MaxTop = 10;


SkiplistNode* skiplistNodeCreat(douban val, int maxLevel) {
	//SkiplistNode* obj = (SkiplistNode*)malloc(sizeof(SkiplistNode));
	SkiplistNode* obj = new SkiplistNode;
	obj->val = val;
	obj->maxLevel = maxLevel;
	obj->forward = (SkiplistNode**)malloc(sizeof(SkiplistNode*) * maxLevel);
	for (int i = 0; i < maxLevel; i++) {
		obj->forward[i] = NULL;
	}
	return obj;
}

void skiplistNodeFree(SkiplistNode* obj) {
	if (obj->forward) {
		free(obj->forward);
		obj->forward = NULL;
		obj->maxLevel = 0;
	}
	free(obj);
}

Skiplist* skiplistCreate() {
	Skiplist* obj = (Skiplist*)malloc(sizeof(Skiplist));
	douban n;
	obj->head = skiplistNodeCreat(n, MAX_LEVEL);
	obj->level = 0;
	srand(time(NULL));
	return obj;
}

static inline int randomLevel() {
	int lv = 1;
	/* 随机生成 lv */
	while (rand() < P_FACTOR && lv < MAX_LEVEL) {
		lv++;
	}
	return lv;
}

bool skiplistSearch(Skiplist* obj, int target) {
	SkiplistNode* curr = obj->head;
	for (int i = obj->level - 1; i >= 0; i--) {
		/* 找到第 i 层小于且最接近 target 的元素*/
		while (curr->forward[i] && curr->forward[i]->val.click < target) {
			curr = curr->forward[i];
		}
	}
	curr = curr->forward[0];
	/* 检测当前元素的值是否等于 target */
	if (curr && curr->val.click == target) {
		return true;
	}
	return false;
}

void skiplistAdd(Skiplist* obj, douban num) {
	SkiplistNode* update[MAX_LEVEL];
	SkiplistNode* curr = obj->head;
	for (int i = obj->level - 1; i >= 0; i--) {
		/* 找到第 i 层小于且最接近 num 的元素*/
		while (curr->forward[i] && curr->forward[i]->val.click > num.click) {
			curr = curr->forward[i];
		}
		update[i] = curr;
	}
	int lv = randomLevel();
	if (lv > obj->level) {
		for (int i = obj->level; i < lv; i++) {
			update[i] = obj->head;
		}
		obj->level = lv;
	}
	SkiplistNode* newNode = skiplistNodeCreat(num, lv);
	for (int i = 0; i < lv; i++) {
		/* 对第 i 层的状态进行更新，将当前元素的 forward 指向新的节点 */
		newNode->forward[i] = update[i]->forward[i];
		update[i]->forward[i] = newNode;
	}
}

bool skiplistErase(Skiplist* obj, douban num) {
	SkiplistNode* update[MAX_LEVEL];
	SkiplistNode* curr = obj->head;
	for (int i = obj->level - 1; i >= 0; i--) {
		/* 找到第 i 层小于且最接近 num 的元素*/
		curr = obj->head;
		while (curr->forward[i] && curr->forward[i]->val.anumber != num.anumber) {
			curr = curr->forward[i];
		}
		update[i] = curr;
	}
	curr = curr->forward[0];
	/* 如果值不存在则返回 false */
	if (!curr || curr->val.anumber != num.anumber) {
		return false;
	}
	for (int i = 0; i < obj->level; i++) {
		if (update[i]->forward[i] != curr) {
			break;
		}
		/* 对第 i 层的状态进行更新，将 forward 指向被删除节点的下一跳 */
		update[i]->forward[i] = curr->forward[i];
	}
	skiplistNodeFree(curr);
	/* 更新当前的 level */
	while (obj->level > 1 && obj->head->forward[obj->level - 1] == NULL) {
		obj->level--;
	}
	return true;
}

void skiplistFree(Skiplist* obj) {
	for (SkiplistNode* curr = obj->head; curr; ) {
		SkiplistNode* prev = curr;
		curr = curr->forward[0];
		skiplistNodeFree(prev);
	}
	free(obj);
}

void init(char* a, douban* x, int num) {
	string name;
	string id;
	string actor;
	string director;
	string tags;
	int tag;
	int intime[3] = { 0 };
	int clicknum;
	int i = 0;
	while (*a != ' ') {
		name += *a;
		a++;
	}
	a++;
	id = name;
	name.clear();
	while (*a != ' ') {
		if (*a == '.') {
			a++;
			intime[i++] = stoi(name);
			name.clear();
			continue;
		}
		name += *a;
		a++;
	}
	intime[i++] = stoi(name);
	name.clear();
	a++;
	while (*a != ' ') {
		name += *a;
		a++;
	}
	a++;
	while(*a != ' '){
		director += *a;
		a++;
	}
	a++;
	while(*a != ' '){
		actor += *a;
		a++;
	}
	a++;
	while (*a != '#') {
		tags += *a;
		a++;
	}
	tag = stoi(tags);
	clicknum = stoi(name);
	x[num].name = id;
	x[num].click = clicknum;
	x[num].date->year = intime[0];
	x[num].date->month = intime[1];
	x[num].date->day = intime[2];
	x[num].actor = actor;
	x[num].tag = tag;
	x[num].director = director;
	x[num].anumber = num + 1;
	x[num].address = ":/Qttest2/picutre/" + to_string(num + 1) + ".jpg";
}


int SelsectPivot(int left, int right) {
	return (left + right) / 2;
}

void swap(douban* Array, int left, int right) {
	douban c = Array[left];
	Array[left] = Array[right];
	Array[right] = c;
}

int Partition(douban* Array, int left, int right) {
	int l = left;
	int r = right;
	douban TempRecord = Array[r];
	while (l != r) {
		while (Array[l].anumber <= TempRecord.anumber && l < r) l++;
		if (l < r) {
			Array[r] = Array[l];
			r--;
		}
		while (Array[r].anumber >= TempRecord.anumber && l < r)r--;
		if (l < r) {
			Array[l] = Array[r];
			l++;
		}
	}
	Array[l] = TempRecord;
	return l;
}

int Partition_for_time(douban* Array, int left, int right) {
	int l = left;
	int r = right;
	douban TempRecord = Array[r];
	while (l != r) {
		while (Array[l].date->forcompare() >= TempRecord.date->forcompare() && l < r) l++;
		if (l < r) {
			Array[r] = Array[l];
			r--;
		}
		while (Array[r].date->forcompare() <= TempRecord.date->forcompare() && l < r)r--;
		if (l < r) {
			Array[l] = Array[r];
			l++;
		}
	}
	Array[l] = TempRecord;
	return l;
}

void QuickSort(douban* Array, int left, int right) {
	if (right <= left)
		return;
	int pivot = SelsectPivot(left, right);
	swap(Array, pivot, right);
	pivot = Partition(Array, left, right);
	QuickSort(Array, left, pivot - 1);
	QuickSort(Array, pivot + 1, right);
}

void QuickSort_for_time(douban* Array, int left, int right) {
	if (right <= left)
		return;
	int pivot = SelsectPivot(left, right);
	swap(Array, pivot, right);
	pivot = Partition_for_time(Array, left, right);
	QuickSort_for_time(Array, left, pivot - 1);
	QuickSort_for_time(Array, pivot + 1, right);
}

int* capturenext(int* a) {
	int n = sizeof(a);
	int* next = new int[n];
	int k = -1;
	int i = 0;
	next[0] = -1;
	next[1] = 0;
	while (i < n - 1) {
		if (k == -1 || a[i] == a[k]) {
			k++;
			i++;
			next[i] = k;
		}
		else {
			k = next[k];
		}
	}
	return next;
}

int kmp(int* a, int num1, int* b, int num2) {
	int* next = capturenext(b);
	int i = 0;
	int j = 0;
	while (i < num1 && j < num2) {
		if (j == -1 || a[i] == b[j]) {
			i++;
			j++;
		}
		else {
			j = next[j];
		}
	}
	if (j == num2) {
		return i - j;
	}
	return -1;
}

int stringnum(string str) {
	int num = 0;
	for (unsigned z = 0; z < str.length(); z++) {
		if ((unsigned)str[z] < 0x80)
			num++;
		else {
			num++;
			z++;
		}
	}
	return num;
}

int* stringToVecInt(const string& str)
{
	union {
		char c[2];
		int  i;
	} convert;

	convert.i = 0;

	int num = 0;

	for (unsigned z = 0; z < str.length(); z++) {
		if ((unsigned)str[z] < 0x80)
			num++;
		else {
			num++;
			z++;
		}
	}

	int* vec = new int[num];

	int j = 0;

	for (unsigned i = 0; i < str.length(); i++) {
		if ((unsigned)str[i] > 0x80) {
			convert.c[1] = str[i];
			convert.c[0] = str[i + 1];
			vec[j] = convert.i;
			i++;
		}
		else
			vec[j] = (int)str[i];
		j++;
	}
	return vec;
}

bool include(const string& str, const string& msg)
{
	auto sour = stringToVecInt(str);
	int num1, num2;
	num1 = stringnum(str);
	num2 = stringnum(msg);
	auto find = stringToVecInt(msg);
	return kmp(sour, num1, find, num2) != -1;
}

std::string WString2String(const std::wstring& ws)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const wchar_t* wchSrc = ws.c_str();
	size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
	char* chDest = new char[nDestSize];
	memset(chDest, 0, nDestSize);
	wcstombs(chDest, wchSrc, nDestSize);
	std::string strResult = chDest;
	delete[]chDest;
	setlocale(LC_ALL, strLocale.c_str());
	return strResult;
}

// string => wstring
std::wstring String2WString(const std::string& s)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const char* chSrc = s.c_str();
	size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
	wchar_t* wchDest = new wchar_t[nDestSize];
	wmemset(wchDest, 0, nDestSize);
	mbstowcs(wchDest, chSrc, nDestSize);
	std::wstring wstrResult = wchDest;
	delete[]wchDest;
	setlocale(LC_ALL, strLocale.c_str());
	return wstrResult;
}

douban* a;
Skiplist* lst;
int num;
int cout_search;
int kind_select = 0;

Qttest2::Qttest2(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	ifstream fin;
	fin.open("test.txt", ios::in);
	a = new douban[80];
	cout_search = 0;
	//int num = 0;
	num = 0;
	if (!fin.is_open())
	{
		cout << "读取文件失败" << endl;
		return;
	}
	char buf[2048];
	while (fin.getline(buf, sizeof(buf))) {
		init(buf, a, num);
		num++;
	}
	fin.close();
	fin.open("jj2.txt", ios::in);
	for (int i = 0; i < num; i++) {
		fin.getline(buf, sizeof(buf));
		string jj = buf;
		a[i].jj = jj;
	}
	fin.close();
	fin.open("cmm.txt", ios::in);
	for (int i = 0; i < num; i++) {
		fin.getline(buf, sizeof(buf));
		string cmment = buf;
		a[i].comment = cmment;
		fin.getline(buf, sizeof(buf));
		a[i].comment_num = stoi(buf);
	}
	fin.close();
	lst = skiplistCreate();
	for (int i = 0; i < num; i++) {
		skiplistAdd(lst, a[i]);
	}
	Skiplist* q = lst;
	SkiplistNode* res = q->head->forward[0];
	int coutk = 1;
	while (res != NULL && coutk <= MaxTop) {
		//string zxb = res->val.name;
		//wstring zxbb = String2WString(zxb);
		//QString zxbcontent = QString::fromStdWString(zxbb);
		//QString bnumber = QString("%1").arg(coutk);
		//ui.textBrowser->append(bnumber);
		//ui.textBrowser->append(zxbcontent);
		//for tool box
		string stag;
		if (res->val.tag == 1)
			stag = "（电影）";
		if (res->val.tag == 2)
			stag = "（电视剧）";
		if (res->val.tag == 3)
			stag = "（图书）";
		string date_box = "Date:" + to_string(res->val.date->year) + '/' + to_string(res->val.date->month) + '/' + to_string(res->val.date->day);
		QString qdate_box = QString::fromStdString(date_box);
		QString qclick = QString::fromStdString("Click:" + to_string(res->val.click));
		QString qnum = QString::fromStdString("Number:" + to_string(res->val.anumber));
		string director_box = "Director:" + res->val.director;
		wstring wdirector = String2WString(director_box);
		string actor_box = "Actor:" + res->val.actor;
		wstring wactor = String2WString(actor_box);
		QString qdirector = QString::fromStdWString(wdirector);
		QString qactor = QString::fromStdWString(wactor);
		string comment = "Comment:" + res->val.comment;
		wstring wcomm = String2WString(comment);
		QString qcomm = QString::fromStdWString(wcomm);
		string jianjie = "简介：" + res->val.jj;
		wstring wjj = String2WString(jianjie);
		QString qjj = QString::fromStdWString(wjj);
		
		QWidget* w = new QWidget(ui.toolBox);
		QTextBrowser* tb = new QTextBrowser(w);
		string sssss = to_string(coutk) + ':' + res->val.name + stag;
		wstring sswwww = String2WString(sssss);
		QString zxc = QString::fromStdWString(sswwww);
		
		QLabel* lb = new QLabel(w);
		lb->setPixmap(QPixmap(QString::fromStdString(res->val.address)));
		lb->setScaledContents(true);
		lb->resize(150, 200);
		for(int hh = 0; hh < 12; hh++)
			tb->append(QString(" "));
		
		tb->append(qdate_box);
		tb->append(qclick);
		tb->append(qdirector);
		tb->append(qactor);
		tb->append(qnum);
		tb->append(qcomm);
		tb->append(qjj);
		tb->resize(441, 560);


		QHBoxLayout* hLayout1 = new QHBoxLayout;
		w->setLayout(hLayout1);

		//	tb->append(zxc);
		ui.toolBox->addItem(w, zxc);
		res = res->forward[0];
		coutk++;
	}
	coutk = 1;


	QuickSort_for_time(a, 0, num - 1);
	for (int i = 0; i < 10; i++) {
		string stag;
		if (a[i].tag == 1)
			stag = "（电影）";
		if (a[i].tag == 2)
			stag = "（电视剧）";
		if (a[i].tag == 3)
			stag = "（图书）";
		string date_box = "Date:" + to_string(a[i].date->year) + '/' + to_string(a[i].date->month) + '/' + to_string(a[i].date->day);
		QString qdate_box = QString::fromStdString(date_box);
		QString qclick = QString::fromStdString("Click:" + to_string(a[i].click));
		QString qnum = QString::fromStdString("Number:" + to_string(a[i].anumber));
		string director_box = "Director:" + a[i].director;
		wstring wdirector = String2WString(director_box);
		string actor_box = "Actor:" + a[i].actor;
		wstring wactor = String2WString(actor_box);
		QString qdirector = QString::fromStdWString(wdirector);
		QString qactor = QString::fromStdWString(wactor);
		string comment = "Comment:" + a[i].comment;
		wstring wcomm = String2WString(comment);
		QString qcomm = QString::fromStdWString(wcomm);
		string jianjie = "简介：" + a[i].jj;
		wstring wjj = String2WString(jianjie);
		QString qjj = QString::fromStdWString(wjj);


		QWidget* w = new QWidget(ui.toolBox_2);
		QTextBrowser* tb = new QTextBrowser(w);
		string sssss = to_string(i+1) + ':' + a[i].name + stag;
		wstring sswwww = String2WString(sssss);
		QString zxc = QString::fromStdWString(sswwww);

		QLabel* lb = new QLabel(w);
		lb->setPixmap(QPixmap(QString::fromStdString(a[i].address)));
		lb->setScaledContents(true);
		lb->resize(150, 200);
		for (int hh = 0; hh < 12; hh++)
			tb->append(QString(" "));

		tb->append(qdate_box);
		tb->append(qclick);
		tb->append(qdirector);
		tb->append(qactor);
		tb->append(qnum);
		tb->append(qcomm);
		tb->append(qjj);
		tb->resize(441, 560);
		//	tb->append(zxc);
		ui.toolBox_2->addItem(w, zxc);
	}
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(start()));

	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(select1()));

	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(select2()));

	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(select3()));

	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(initiat()));

	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(start()));

	connect(ui.toolButton, SIGNAL(clicked()), this, SLOT(tolbt1()));

	connect(ui.toolButton_2, SIGNAL(clicked()), this, SLOT(tolbt2()));

	connect(ui.toolButton_3, SIGNAL(clicked()), this, SLOT(tolbt3()));

	connect(ui.toolButton_4, SIGNAL(clicked()), this, SLOT(tolbt4()));

	connect(ui.toolButton_5, SIGNAL(clicked()), this, SLOT(tolbt5()));

	connect(ui.toolButton_6, SIGNAL(clicked()), this, SLOT(tolbt6()));

	connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(resource_web()));

	connect(ui.pushButton_7, SIGNAL(clicked()), this, SLOT(exit()));

	connect(ui.pushButton_8, SIGNAL(clicked()), this, SLOT(launch()));

	connect(ui.pushButton_9, SIGNAL(clicked()), this, SLOT(regist()));
	//initiat();
	//制作toolbox的例子，用于排行榜显示
	/*QWidget* w = new QWidget(ui.toolBox);
	QTextBrowser* tb = new QTextBrowser(w);
	string sssss = "你好";
	wstring sswwww = String2WString(sssss);
	QString zxc = QString::fromStdWString(sswwww);
	tb->append(zxc);
	ui.toolBox->addItem(w, zxc);*/
	//ui.textBrowser_4->append(zxc);
	//w = new QWidget(ui.toolBox);
}

void Qttest2::start() {
    //ui.textBrowser_3->clear();
	//ui.textBrowser->clear();
	//cout_search++;
    QString content = ui.lineEdit->text();
    wstring s = content.toStdWString();
	string ans_s = WString2String(s);
	QWidget* w;
	string date_box;
	QString qdate_box;
	QString qclick;
	//QString test = QString::fromStdString(s);
	//ui.textBrowser_3->append(test);
	int c = 0;

	if (cout_search != 0) {
		ui.textBrowser_2->clear();
		int tb3_cout = ui.toolBox_3->count() - 1;
		for (int i = 1; i <= tb3_cout; i++) {
			ui.toolBox_3->removeItem(1);
		}
	}

	if (ans_s != "") {
		for (int i = 0; i < num; i++) {
			if (include(a[i].name, ans_s) || include(a[i].actor, ans_s) || include(a[i].director, ans_s)) {
				string stag;
				if (a[i].tag == 1)
					stag = "（电影）";
				if (a[i].tag == 2)
					stag = "（电视剧）";
				if (a[i].tag == 3)
					stag = "（图书）";
				a[i].click++;
				//cout << x[i].name << endl;
				date_box = "Date:" + to_string(a[i].date->year) + '/' + to_string(a[i].date->month) + '/' + to_string(a[i].date->day);
				qdate_box = QString::fromStdString(date_box);
				QString qclick = QString::fromStdString("Click:" + to_string(a[i].click));
				string director_box = "Director:" + a[i].director;
				QString qnum = QString::fromStdString("Number:" + to_string(a[i].anumber));
				wstring wdirector = String2WString(director_box);
				string actor_box = "Actor:" + a[i].actor;
				wstring wactor = String2WString(actor_box);
				QString qdirector = QString::fromStdWString(wdirector);
				QString qactor = QString::fromStdWString(wactor);
				string comment = "Comment:" + a[i].comment;
				wstring wcomm = String2WString(comment);
				QString qcomm = QString::fromStdWString(wcomm);
				string jianjie = "简介：" + a[i].jj;
				wstring wjj = String2WString(jianjie);
				QString qjj = QString::fromStdWString(wjj);


				w = new QWidget(ui.toolBox_3);
				QTextBrowser* tb = new QTextBrowser(w);
				string sssss = to_string(c + 1) + ':' + a[i].name + stag;
				wstring sswwww = String2WString(sssss);
				QString zxc = QString::fromStdWString(sswwww);

				QLabel* lb = new QLabel(w);
				lb->setPixmap(QPixmap(QString::fromStdString(a[i].address)));
				lb->setScaledContents(true);
				lb->resize(150, 200);
				for (int hh = 0; hh < 12; hh++)
					tb->append(QString(" "));

				tb->append(qdate_box);
				tb->append(qclick);
				tb->append(qdirector);
				tb->append(qactor);
				tb->append(qnum);
				tb->append(qcomm);
				tb->append(qjj);
				tb->resize(441, 560);
				//	tb->append(zxc);
				ui.toolBox_3->addItem(w, zxc);
				skiplistErase(lst, a[i]);
				skiplistAdd(lst, a[i]);
				c++;
			}
		}
	}
	if (ans_s == "")
		c++;
	if (c == 0) {
		string sss = "没有相关结果";
		wstring wsss = String2WString(sss);
		QString zsss = QString::fromStdWString(wsss);
		ui.textBrowser_2->append(zsss);
	}
	else {//ui.toolBox->removeItem(1);
		for (int i = 1; i <= MaxTop; i++) {
			//ui.toolBox.removeItem(i);
			ui.toolBox->removeItem(1);
		}
		for (int i = 1; i <= MaxTop; i++) {
			//ui.toolBox.removeItem(i);
			ui.toolBox_2->removeItem(1);
		}
		SkiplistNode* res = lst->head->forward[0];
		int coutk = 1;
		while (res != NULL && coutk <= MaxTop) {
			string stag;
			if (res->val.tag == 1)
				stag = "（电影）";
			if (res->val.tag == 2)
				stag = "（电视剧）";
			if (res->val.tag == 3)
				stag = "（图书）";
			date_box = "Date:" + to_string(res->val.date->year) + '/' + to_string(res->val.date->month) + '/' + to_string(res->val.date->day);
			qdate_box = QString::fromStdString(date_box);
			QString qclick = QString::fromStdString("Click:" + to_string(res->val.click));
			string director_box = "Director:" + res->val.director;
			wstring wdirector = String2WString(director_box);
			string actor_box = "Actor:" + res->val.actor;
			wstring wactor = String2WString(actor_box);
			QString qnum = QString::fromStdString("Number:" + to_string(res->val.anumber));
			QString qdirector = QString::fromStdWString(wdirector);
			QString qactor = QString::fromStdWString(wactor);
			string comment = "Comment:" + res->val.comment;
			wstring wcomm = String2WString(comment);
			QString qcomm = QString::fromStdWString(wcomm);
			string jianjie = "简介：" + res->val.jj;
			wstring wjj = String2WString(jianjie);
			QString qjj = QString::fromStdWString(wjj);


			QWidget* w = new QWidget(ui.toolBox);
			QTextBrowser* tb = new QTextBrowser(w);
			string sssss = to_string(coutk) + ':' + res->val.name + stag;
			wstring sswwww = String2WString(sssss);
			QString zxc = QString::fromStdWString(sswwww);

			QLabel* lb = new QLabel(w);
			lb->setPixmap(QPixmap(QString::fromStdString(res->val.address)));
			lb->setScaledContents(true);
			lb->resize(150, 200);
			for (int hh = 0; hh < 12; hh++)
				tb->append(QString(" "));

			tb->append(qdate_box);
			tb->append(qclick);
			tb->append(qdirector);
			tb->append(qactor);
			tb->append(qnum);
			tb->append(qcomm);
			tb->append(qjj);
			tb->resize(441, 560);
			//	tb->append(zxc);
			ui.toolBox->addItem(w, zxc);
			res = res->forward[0];
			coutk++;
		}

		for (int i = 0; i < 10; i++) {
			string stag;
			if (a[i].tag == 1)
				stag = "（电影）";
			if (a[i].tag == 2)
				stag = "（电视剧）";
			if (a[i].tag == 3)
				stag = "（图书）";
			string date_box = "Date:" + to_string(a[i].date->year) + '/' + to_string(a[i].date->month) + '/' + to_string(a[i].date->day);
			QString qdate_box = QString::fromStdString(date_box);
			QString qclick = QString::fromStdString("Click:" + to_string(a[i].click));
			string director_box = "Director:" + a[i].director;
			wstring wdirector = String2WString(director_box);
			string actor_box = "Actor:" + a[i].actor;
			wstring wactor = String2WString(actor_box);
			QString qnum = QString::fromStdString("Number:" + to_string(a[i].anumber));
			QString qdirector = QString::fromStdWString(wdirector);
			QString qactor = QString::fromStdWString(wactor);
			string comment = "Comment:" + a[i].comment;
			wstring wcomm = String2WString(comment);
			QString qcomm = QString::fromStdWString(wcomm);
			string jianjie = "简介：" + a[i].jj;
			wstring wjj = String2WString(jianjie);
			QString qjj = QString::fromStdWString(wjj);

			QWidget* w = new QWidget(ui.toolBox_2);
			QTextBrowser* tb = new QTextBrowser(w);
			string sssss = to_string(i + 1) + ':' + a[i].name + stag;
			wstring sswwww = String2WString(sssss);
			QString zxc = QString::fromStdWString(sswwww);

			QLabel* lb = new QLabel(w);
			lb->setPixmap(QPixmap(QString::fromStdString(a[i].address)));
			lb->setScaledContents(true);
			lb->resize(150, 200);
			for (int hh = 0; hh < 12; hh++)
				tb->append(QString(" "));

			tb->append(qdate_box);
			tb->append(qclick);
			tb->append(qdirector);
			tb->append(qactor);
			tb->append(qnum);
			tb->append(qcomm);
			tb->append(qjj);
			tb->resize(441, 560);
			//	tb->append(zxc);
			ui.toolBox_2->addItem(w, zxc);
		}
	}
	cout_search++;
}

void Qttest2::initiat() {
	QString content = ui.lineEdit_2->text();
	wstring wcomm = content.toStdWString();
	string snum = WString2String(wcomm);
	QString qcomment = ui.textEdit->toPlainText();
	wstring wcomment = qcomment.toStdWString();
	string comment = WString2String(wcomment);
	QString user = ui.label_10->text();
	wstring wuser = user.toStdWString();
	string suser = WString2String(wuser);
	int comm_num = atoi(snum.c_str());
	for (int i = 0; i < num; i++) {
		if (suser == "null") {
			if (a[i].anumber == comm_num) {
				if (a[i].comment_num == 0) {
					a[i].comment = "用户1:" + comment + " ";
				}
				else {
					a[i].comment += "用户" + to_string(a[i].comment_num + 1) + ":" + comment + " ";
				}
				a[i].comment_num++;
				skiplistErase(lst, a[i]);
				skiplistAdd(lst, a[i]);
				break;
			}
		}
		else {
			if (a[i].anumber == comm_num) {
				if (a[i].comment_num == 0) {
					a[i].comment = suser + ":" + comment + " ";
				}
				else {
					a[i].comment += suser + ":" + comment + " ";
				}
				a[i].comment_num++;
				skiplistErase(lst, a[i]);
				skiplistAdd(lst, a[i]);
				break;
			}
		}
	}
}

void Qttest2::resource_web() {
	QString content = ui.lineEdit_2->text();
	wstring wcomm = content.toStdWString();
	string snum = WString2String(wcomm);
	int comm_num = atoi(snum.c_str());
	string web = "start https://cupfox.app/s/";
	for (int i = 0; i < num; i++) {
		if (a[i].anumber == comm_num) {
			web += a[i].name;
			system(web.c_str());
		}
	}
}

void Qttest2::select1() {
	kind_select = 1;
	Qttest2::showdata();
}

void Qttest2::select2() {
	kind_select = 2;
	Qttest2::showdata();
}

void Qttest2::select3() {
	kind_select = 3;
	Qttest2::showdata();
}

void Qttest2::tolbt1() {
	system("E:\\预告片\\xjmh.mp4");
}

void Qttest2::tolbt2() {
	system("E:\\预告片\\afd.mp4");
}

void Qttest2::tolbt3() {
	system("E:\\预告片\\thwj.mp4");
}

void Qttest2::tolbt4() {
	system("E:\\预告片\\bysj.mp4");
}

void Qttest2::tolbt5() {
	system("E:\\预告片\\xjmh.mp4");
}

void Qttest2::tolbt6() {
	system("E:\\预告片\\mgkbgs.mp4");
}

void Qttest2::showdata() {
	for (int i = 1; i <= MaxTop; i++) {
		//ui.toolBox.removeItem(i);
		ui.toolBox->removeItem(1);
	}
	for (int i = 1; i <= MaxTop; i++) {
		//ui.toolBox.removeItem(i);
		ui.toolBox_2->removeItem(1);
	}
	int scout = 0;
	SkiplistNode* res = lst->head->forward[0];
	int coutk = 1;
	string date_box;
	QString qdate_box;
	while (res != NULL && coutk <= MaxTop) {
		if (kind_select != 0 && res->val.tag == kind_select) {
			string stag;
			if (res->val.tag == 1)
				stag = "（电影）";
			if (res->val.tag == 2)
				stag = "（电视剧）";
			if (res->val.tag == 3)
				stag = "（图书）";
			date_box = "Date:" + to_string(res->val.date->year) + '/' + to_string(res->val.date->month) + '/' + to_string(res->val.date->day);
			qdate_box = QString::fromStdString(date_box);
			QString qclick = QString::fromStdString("Click:" + to_string(res->val.click));
			string director_box = "Director:" + res->val.director;
			wstring wdirector = String2WString(director_box);
			string actor_box = "Actor:" + res->val.actor;
			wstring wactor = String2WString(actor_box);
			QString qnum = QString::fromStdString("Number:" + to_string(res->val.anumber));
			QString qdirector = QString::fromStdWString(wdirector);
			QString qactor = QString::fromStdWString(wactor);
			string comment = "Comment:" + res->val.comment;
			wstring wcomm = String2WString(comment);
			QString qcomm = QString::fromStdWString(wcomm);
			string jianjie = "简介：" + res->val.jj;
			wstring wjj = String2WString(jianjie);
			QString qjj = QString::fromStdWString(wjj);


			QWidget* w = new QWidget(ui.toolBox);
			QTextBrowser* tb = new QTextBrowser(w);
			string sssss = to_string(coutk) + ':' + res->val.name + stag;
			wstring sswwww = String2WString(sssss);
			QString zxc = QString::fromStdWString(sswwww);

			QLabel* lb = new QLabel(w);
			lb->setPixmap(QPixmap(QString::fromStdString(res->val.address)));
			lb->setScaledContents(true);
			lb->resize(150, 200);
			for (int hh = 0; hh < 12; hh++)
				tb->append(QString(" "));

			tb->append(qdate_box);
			tb->append(qclick);
			tb->append(qdirector);
			tb->append(qactor);
			tb->append(qnum);
			tb->append(qcomm);
			tb->append(qjj);
			tb->resize(441, 560);
			//	tb->append(zxc);
			ui.toolBox->addItem(w, zxc);
			res = res->forward[0];
			coutk++;
		}
		else
			res = res->forward[0];
	}
	int ncout = 0;
	for (int i = 0; i < num; i++) {
		if (ncout == 10)
			break;
		if (kind_select != 0 && a[i].tag == kind_select) {
			string stag;
			ncout++;
			if (a[i].tag == 1)
				stag = "（电影）";
			if (a[i].tag == 2)
				stag = "（电视剧）";
			if (a[i].tag == 3)
				stag = "（图书）";
			string date_box = "Date:" + to_string(a[i].date->year) + '/' + to_string(a[i].date->month) + '/' + to_string(a[i].date->day);
			QString qdate_box = QString::fromStdString(date_box);
			QString qclick = QString::fromStdString("Click:" + to_string(a[i].click));
			string director_box = "Director:" + a[i].director;
			wstring wdirector = String2WString(director_box);
			string actor_box = "Actor:" + a[i].actor;
			wstring wactor = String2WString(actor_box);
			QString qnum = QString::fromStdString("Number:" + to_string(a[i].anumber));
			QString qdirector = QString::fromStdWString(wdirector);
			QString qactor = QString::fromStdWString(wactor);
			string comment = "Comment:" + a[i].comment;
			wstring wcomm = String2WString(comment);
			QString qcomm = QString::fromStdWString(wcomm);
			string jianjie = "简介：" + a[i].jj;
			wstring wjj = String2WString(jianjie);
			QString qjj = QString::fromStdWString(wjj);


			QWidget* w = new QWidget(ui.toolBox_2);
			//w->resize(450, 400);
			QTextBrowser* tb = new QTextBrowser(w);
			//tb.set
			string sssss = to_string(ncout) + ':' + a[i].name + stag;
			wstring sswwww = String2WString(sssss);
			QString zxc = QString::fromStdWString(sswwww);

			QLabel* lb = new QLabel(w);
			lb->setPixmap(QPixmap(QString::fromStdString(a[i].address)));
			lb->setScaledContents(true);
			lb->resize(150, 200);
			for (int hh = 0; hh < 12; hh++)
				tb->append(QString(" "));

			tb->append(qdate_box);
			tb->append(qclick);
			tb->append(qdirector);
			tb->append(qactor);
			tb->append(qnum);
			tb->append(qcomm);
			tb->append(qjj);
			tb->resize(441, 560);
			//	tb->append(zxc);
			ui.toolBox_2->addItem(w, zxc);
		}
	}
}

void Qttest2::exit() {
	ofstream ofs;
	int i, j, temp;
	QuickSort(a, 0, num - 1);
	ofs.open("save.txt", ios::out);
	for (int i = 0; i < num; i++) {
		ofs << a[i].name << " " << a[i].date->year << "." << a[i].date->month << "." << a[i].date->day << " " << a[i].click << " " << a[i].director << " " << a[i].actor << " " << a[i].tag << "#" << endl;
	}
	ofs.close();
	ofs.open("cmm.txt", ios::out);
	for (int i = 0; i < num; i++) {
		ofs << a[i].comment << endl;
		ofs << a[i].comment_num << endl;
	}
	qApp->exit(0);
}

void Qttest2::launch() {
	int exist = 0;
	QString content = ui.lineEdit_3->text();
	wstring wcomm = content.toStdWString();
	string snum = WString2String(wcomm);
	QString content2 = ui.lineEdit_4->text();
	wstring wcomm2 = content2.toStdWString();
	string snum2 = WString2String(wcomm2);
	
	fstream f("customer.txt");
	string line;
	while (getline(f, line))
	{
		if (line == snum) {
			getline(f, line);
			if (line == snum2) {
				exist++;
				break;
			}
		}
		else
			getline(f, line);
	}
	if (exist != 0) {
		ui.label_10->setText(content);
	}
	else {
		QMessageBox::question(this,
			tr("Error"),
			tr("Mistake in User Name or Password\nPlease Rechick"),
			QMessageBox::Ok | QMessageBox::Cancel,
			QMessageBox::Ok);
	}
}

void Qttest2::regist() {
	QString content = ui.lineEdit_3->text();
	wstring wcomm = content.toStdWString();
	string snum = WString2String(wcomm);
	QString content2 = ui.lineEdit_4->text();
	wstring wcomm2 = content2.toStdWString();
	string snum2 = WString2String(wcomm2);
	ofstream ofs;
	ofs.open("customer.txt", ios::out | ios::app);
	ofs << snum << endl;
	ofs << snum2 << endl;
}

Qttest2::~Qttest2()
{}
