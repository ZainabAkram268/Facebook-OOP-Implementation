//23L-0909
#include<iostream>
#include<fstream>
using namespace std;
//----------------------------------------Helper class--------------------------------
class Helper
{
public:
	static int mystrlen(char* str)
	{
		int len = 0;
		for (char* temp = str; *temp != '\0'; temp++)
			len++;
		return len;
	}
	static void mystrcopy(char*& str1, char*& str2)
	{
		char* temp1 = str1;
		for (char* temp2 = str2; *temp2 != '\0'; temp1++, temp2++)
		{
			*temp1 = *temp2;
		}
		*temp1 = '\0';
	}
	static char* GetStringFromBuffer(char* buffer)
	{
		int len = mystrlen(buffer);
		char* str = 0;
		if (len > 0)
		{
			str = new char[len + 1];
			mystrcopy(str, buffer);
		}
		return str;
	}
	static bool mystrcmp(char* str1, char* str2)
	{
		for (int i = 0; str1[i] != '\0' || str2[i] != '\0'; i++)
		if (str1[i] != str2[i])
			return false;
		return true;

	}
	template<class T >
	static void Destructor(T** arr,int size)
	{
		for (int i = 0; i < size; i++)
			delete arr[i];
		delete[] arr;
	}

};
//-------------------------------------------Date class------------------------------------------------
class Date
{
	int Day;
	int Month;
	int Year;
public:
	Date();
	Date(int d, int m, int y);
	void Print();
	bool SameDay(Date);
	int operator-(Date);
	bool operator<(Date);
	bool operator==(Date);
	int YearDifference(Date);
};
Date::Date(){};
Date::Date(int d, int m, int y) {

	Day = d;
	Month = m;
	Year = y;
}
void Date::Print() {
	cout << Day << "/" << Month << "/" << Year;

}
bool Date::SameDay(Date d1)
{
	if (Day==d1.Day && Month==d1.Month&& Year!=d1.Year)
		return true;
	return false;

}
int Date::operator-(Date d1)
{//Finds difference in date
	int days1 = Year * 365 + Month * 30 + Day;
	int days2 = d1.Year * 365 + d1.Month * 30 + d1.Day;
	return days1 - days2;

}
bool Date::operator<(Date d1)
{//Finds which date is less. used in sort function of posts for viewing memory
	int days1 = Year * 365 + Month * 30 + Day;
	int days2 = d1.Year * 365 + d1.Month * 30 + d1.Day;
	if (days1<days2)
		return true;
	return false;
}
bool Date:: operator==(Date d1)
{
	int days1 = Year * 365 + Month * 30 + Day;
	int days2 = d1.Year * 365 + d1.Month * 30 + d1.Day;
	if (days1==days2)
		return true;
	return false;

}
int Date::YearDifference(Date d1)
{
	return Year - d1.Year;
}
class Object;
class Post;
//---------------------------------------------Comment class Declaration-------------------------------------
class Comment
{
	char* ID;
	Object* CommentBy;
	char* Text;
public:
	Comment();
	Comment(char*, Object*, char*);
	void SetID(char*);
	void SetCommentBy(Object*);
	void SetText(char*);
	void ViewComment();
	~Comment();

};
//--------------------------------------------------PostContent class---------------------------------------
class PostContent
{public:
	virtual void Print()=0;//Pure virtual function
};
//--------------------------------------------------Activity class---------------------------------------
class Activity: public PostContent
{
	int Type;
	char* value;
	static char TypeName[4][20];
public:
	Activity();
	void ReadDataFromFile(ifstream&);
	void Print();
	~Activity();
};
char Activity::TypeName[4][20] = { "feeling", "thinking about", "making", "celebrating" };
Activity::Activity()
{
	value = 0;
}
void Activity::ReadDataFromFile(ifstream& fin)
{
	fin >> Type;
	char temp[100];
	fin.getline(temp,100);
	value=Helper::GetStringFromBuffer(temp);
}
void  Activity::Print()
{
	cout << " is "<<TypeName[Type-1] << "" << value;
}
Activity::~Activity()
{
	if (value)delete value;
}
//--------------------------------------------Object class Declaration-------------------------------------
class Object
{
private:
	char* ID;
	int postcount;

protected:
	Post** Timeline;
public:
	Object();
	char* GetID();
	int Getpostcount();
	void SetID(char*);
	void AddTimeLine(Post*);
	void PrintLatest();
	void SwapTimeLine(Post*&, Post*&);
	void SortTimeLine();
	virtual void PrintAccName();
	virtual void ListView();
	virtual void ViewTimeLine();
	~Object();
};
//--------------------------------------------Post class Declaration-------------------------------------
class Post : public Object
{
private:
	int likes;
	int comcount;
	char* Text;
	char* ID;
	Date SharedDate;
	Object* SharedBy;
	Object** LikedBy;
	Comment** Comments;
	PostContent* content;

public:
	Post();
	void ReadDataFromFile(ifstream&);
	void SetSharedBy(Object*);
	void SetText(char*);
	char* GetText();
	virtual void PostListView();
	void AddLikedBy(Object*);
	void AddComment(Comment*);
	void AddContent(Activity*);
	void ViewLikedBy();
	char* GetID();
	int Getlikes();
	bool IsLatest();
	int DifYear();
	bool SameDay();
	static Date CurrentDate;
	void SetSharedDate(Date);
	Date GetSharedDate();
	void DatePrint();
	Object* GetSharedBy();
	~Post();

};
//--------------------------------------------Object class Definition-------------------------------------
Object::Object()
{
	ID = 0;

}
													//Getters
int Object::Getpostcount()
{
	return postcount;
}
char* Object::GetID()
{
	return ID;
} 
													//Setters
void  Object::SetID(char* ptr)
{
	ID = ptr;
}
void Object::PrintAccName()
{
	this->PrintAccName();
}
void Object::ListView()
{
	this->ListView();
}
void Object::PrintLatest()
{//Finds which post is in the range of last two days and prints it
	for (int i = 0; i<postcount; i++)
	{
		if (Timeline[i]->IsLatest())
			Timeline[i]->PostListView();
	}

}
void Object :: SwapTimeLine(Post* &p1,Post* &p2)
{//Swap function used in sorting
	Post* temp = p1;
	p1 = p2;
	p2 = temp;

}
void Object::SortTimeLine()
{//Sorts timeline on the basis of date difference
	for (int i = 0; i < Getpostcount() - 1; i++)
	{
		for (int j = 0; j < Getpostcount() - 1; j++)
		{
			Date d1 = Timeline[j]->GetSharedDate();
			Date d2 = Timeline[j + 1]->GetSharedDate();
			if (d1 < d2|| d1==d2)
				SwapTimeLine(Timeline[j], Timeline[j + 1]);
		}

	}
}
void Object::ViewTimeLine()
{
	PrintAccName();
	cout << "-Time Line\n\n";
	SortTimeLine();
	for (int i = 0; i < Getpostcount() ; i++)
	{
		Timeline[i]->PostListView();
	}
}
void Object::AddTimeLine(Post* post)
{//Allocates memory if it is going to be first post else just adds it in the timeline
	if (Getpostcount() == 0)
	{
		Timeline = new Post*[10];
		for (int i = 0; i < 10; i++)
			Timeline[i] = new Post;

	}
	if (postcount < 10)
		Timeline[postcount] = post;
	postcount++;
}
Object::~Object()
{
	if (ID)delete ID;
	Helper::Destructor(Timeline, postcount);
}
//--------------------------------------------Post class Definition-------------------------------------
Date Post::CurrentDate; 
Post::Post()
{
	Text = ID = 0;
	SharedBy = 0;
	LikedBy = 0;

}
											//Getters
char* Post::GetID()
{
	return ID;
}
int Post::Getlikes()
{
	return likes;
}
Object* Post::GetSharedBy()
{
	return SharedBy;
}
char* Post::GetText()
{
	return Text;
}
Date Post::GetSharedDate()
{
	return SharedDate;
}
											//Setters
void Post::SetText(char* c)
{
	Text = Helper::GetStringFromBuffer(c);
}
void Post::SetSharedBy(Object* obj)
{
	SharedBy = obj;
}
void Post::SetSharedDate(Date d)
{
	SharedDate = d;
}
void Post::DatePrint()
{//Prints date in required format
	int dif = CurrentDate-SharedDate;
	if (dif > 3)
		SharedDate.Print();
	else if (dif < 1)
		cout << "1h";
	else
		cout << dif << "d";
}
bool Post::IsLatest()
{
	int dif = CurrentDate - SharedDate;;
	if (dif < 2 || dif==2)
		return true;
	return false;
}
int Post::DifYear()
{
	return CurrentDate.YearDifference(SharedDate);
}

bool Post::SameDay()
{
		return CurrentDate.SameDay(SharedDate);
}
void Post::AddContent(Activity* ptr)
{
	content = ptr;
}
void Post::AddComment(Comment* cptr)
{
	if (comcount == 0)
	{
		Comments = new Comment*[10];
		for (int i = 0; i < 10; i++)
			Comments[i] = new Comment;
	}
	if (comcount < 10)
	{
		Comments[comcount] = cptr;
		comcount++;
	}
}
void Post::ReadDataFromFile(ifstream& fin)
{
	char temp[100];
	int day, month, year;
	if (fin)
	{
		fin >> temp;
		ID = Helper::GetStringFromBuffer(temp);
		fin >> day >> month >> year;
		Date d(day, month, year);
		SharedDate = d;
		fin.ignore();
		fin.getline(temp, 100);
		Text = Helper::GetStringFromBuffer(temp);
	}

}

void Post::PostListView()
{
	SharedBy->PrintAccName();
	if(content)
		content->Print();
	cout<< "(";
	DatePrint();
	cout << "):" << endl;
	cout << "\t"<<Text << endl;
	for (int i = 0; i < comcount; i++)
		Comments[i]->ViewComment();
	cout << endl;

}
void Post::ViewLikedBy()
{
	cout << " Post Liked By\n\n";
	for (int i = 0; i < likes; i++)
	{
		LikedBy[i]->ListView();
	}
}

void Post::AddLikedBy(Object* obj)
{
	if (likes == 0)
	{
		LikedBy = new Object*[10];
		for (int i = 0; i < 10; i++)
			LikedBy[i] = new Object;

	}
	LikedBy[likes] = obj;
	likes++;

}
Post::~Post()
{
	if (ID)delete ID;
	if (Text) delete Text;
	if (content) delete content;
	Helper::Destructor(Comments, comcount);
	delete[] LikedBy;
}
//---------------------------------------------Memory class------------------------------------------
class Memory:public Post
{
	Post* Originalpost;
public:
	Memory(Post*);
	void PostListView();
};
Memory::Memory(Post* p)
{
	Originalpost = p;
	SetSharedDate(CurrentDate);
	SetSharedBy(p->GetSharedBy());
	
}
void Memory::PostListView()
{
	cout << "~~~";
	GetSharedBy()->PrintAccName();
	cout << " shared a memory~~~ (1h)\n";
	cout << "   "<<GetText()<<endl;
	cout << "\t  ~~~";
	cout << CurrentDate.YearDifference(Originalpost->GetSharedDate());
	cout<< " years ago~~~\n";

	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	Originalpost->Post::PostListView();
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}
//-----------------------------------------------Page class------------------------------------------
class Page :public Object
{
	char* Title;
public:
	Page();
	char* GetID();
	void ListView();
	void PrintAccName();
	void ReadDataFromFile(ifstream&);
	~Page();
};
Page::Page()
{
	SetID(0);
	Title = 0;
}
char* Page::GetID()
{
	return Object::GetID();
}
void Page::ListView()
{
	cout << GetID() << " - " << Title << endl;

}
void Page::PrintAccName()
{
	cout << Title;
}
void Page::ReadDataFromFile(ifstream& fin)
{
	char temp[50];
	if (fin)
	{
		fin >> temp;
		SetID(Helper::GetStringFromBuffer(temp));
		fin.ignore();
		fin.getline(temp, 50);
		Title = Helper::GetStringFromBuffer(temp);

	}
}
Page::~Page()
{
	if (Title)delete Title;
}
//-----------------------------------------------User class------------------------------------------
class User :public Object
{
	char* Fname;
	char* Lname;
	int fcount;
	int pcount;
	User** FriendList;
	Page** LikedPages;
public:
	User();
	char* GetID();
	char* GetFname();
	char* GetLname();
	void ReadDataFromFile(ifstream&);
	void AddFriend(User*&);
	void LikePage(Page*&);
	void ListView();
	void ViewFriendList();
	void ViewLikedPages();
	void ViewHome();
	void PrintAccName();
	void SeeYourMemories();
	void ShareMemory(Post*,char*);
	void Setfcount(int);
	void Setpcount(int);
	~User();
};
														//Getters
char* User::GetID()
{
	return Object::GetID();
}
char* User::GetFname()
{
	return Fname;
}
char* User::GetLname()
{
	return Lname;
}
														//Setters
void User::Setfcount(int i)
{
	fcount = i;
}
void User::Setpcount(int i)
{
	pcount = i;
}
User::User()
{
	SetID(0);
	Fname = Lname = 0;
	FriendList = 0;
	LikedPages = 0;
}
void User::PrintAccName()
{
	cout << Fname << " " << Lname;
}
void User::ListView()
{
	cout << GetID() << "-" << Fname << " " << Lname << endl;

}
void User::ViewFriendList()
{
	cout << Fname << " " << Lname << "-Friend List\n\n";
	for (int i = 0; i < fcount; i++)
		FriendList[i]->ListView();
	cout << endl << endl;

}
void User::ViewLikedPages()
{
	cout << Fname << " " << Lname << "-Liked Pages\n\n";
	for (int i = 0; i < pcount; i++)
		LikedPages[i]->ListView();
	cout << endl << endl;
}
void User::ViewHome()
{
	PrintAccName();
	cout<< "-Home Page\n\n";
	for (int i = 0; i < fcount; i++)
		FriendList[i]->PrintLatest();
	for (int i = 0; i < pcount; i++)
		LikedPages[i]->PrintLatest();

}
void User::ShareMemory(Post*p,char* c)
{
	Memory* m=new Memory(p);
	m->SetText(c);
	AddTimeLine(m);
}
void User::AddFriend(User* &frnd)
{
	if (fcount == 0)
	{
		FriendList = new User*[10];
		for (int i = 0; i < 10; i++)
			FriendList[i] = new User;
	}
	if (fcount<10)
		FriendList[fcount] = frnd;
	fcount++;

}
void User::LikePage(Page* &pg)
{
	if (pcount == 0)
	{
		LikedPages = new Page*[10];
		for (int i = 0; i < 10; i++)
			LikedPages[i] = new Page;
	}
	if (pcount<10)
		LikedPages[pcount] = pg;
	pcount++;

}
void User::SeeYourMemories()
{
	cout << "We hope you enjoy looking back and sharing your memories on Facebook,\n";
	cout<<"from the most recent to those long ago.\n\n";
	cout << "On This Day\n";
	for (int i = 0; i < Getpostcount();i++)
	if (Timeline[i]->SameDay())
	{
		cout << Timeline[i]->DifYear()<< " Years Ago.\n";
		Timeline[i]->PostListView();
	}
}
void User::ReadDataFromFile(ifstream& fin)
{
	char temp[20];
	if (fin)
	{
		fin >> temp;
		SetID(Helper::GetStringFromBuffer(temp));
		fin >> temp;
		Fname = Helper::GetStringFromBuffer(temp);
		fin >> temp;
		Lname = Helper::GetStringFromBuffer(temp);

	}
}
User::~User()
{
	if (Fname)delete Fname;
	if (Lname)delete Lname;
	delete[] FriendList;
	delete[] LikedPages;
}
//----------------------------------------Comment class Definition----------------------------------
Comment::Comment()
{
	ID = 0;
	CommentBy = 0;
	Text = 0;
}
Comment::Comment(char* id, Object* obj,char* text)
{
	ID = Helper::GetStringFromBuffer(id);
	CommentBy = obj;
	Text = Helper::GetStringFromBuffer(text);
}
									//Setters
void Comment::SetCommentBy(Object* obj)
{
	CommentBy = obj;
}
void Comment::SetText(char* c)
{
	Text = c;
}
void Comment::SetID(char* ptr)
{
	ID =Helper::GetStringFromBuffer( ptr);
}
void Comment::ViewComment()
{
	cout << "\t\t";
	CommentBy->PrintAccName();
	cout<< ": " << Text << endl;

}
Comment::~Comment()
{
	if (ID)delete ID;
	if (Text) delete Text;
}
//-----------------------------------------------Controller class------------------------------------------
class Controller{
	User* CurrentUser;
	User** AllUsers;
	Page** AllPages;
	Post** AllPosts;
	static int totaluser;
	static int totalpage;
	static int totalpost;
	static int totalcomment;
	static int totalactpost;
public:
	void SetCurrentUser(char*);
	void LoadAllUsers(char*);
	void LoadAllPages(char*);
	void LoadAllPosts(char*);
	void LoadAllComments(char*);
	void LoadAllActivities(char*);
	void ViewLikedPost(char*);
	void LikePost(char*);
	User* SearchUserByID(char*);
	Page* SearchPageByID(char*);
	Object* SearchObjectByID(char*);
	Post* SearchPostByID(char*);
	void LinkUsersAndPages(char*);
	void LoadData();
	void Run();
	~Controller();
};
int Controller::totaluser = 0;
int Controller::totalpage = 0;
int Controller::totalpost = 0;
int Controller::totalcomment = 0;
int Controller::totalactpost = 0;
void Controller::SetCurrentUser(char* str)
{
	User* u = SearchUserByID(str);
	CurrentUser = u;
}
void Controller::LoadAllUsers(char* name)
{
	ifstream fin;
	fin.open(name);
	fin >> totaluser;
	if (fin)
	{
		AllUsers = new User*[totaluser];
		for (int i = 0; i < totaluser; i++)
		{
			AllUsers[i] = new User;
			AllUsers[i]->ReadDataFromFile(fin);

		}
	}
	fin.close();
}
User* Controller::SearchUserByID(char* Id)
{
	for (int i = 0; i <totaluser; i++)
	{
		if (Helper::mystrcmp(AllUsers[i]->GetID(), Id))
			return AllUsers[i];
	}
	return nullptr;

}
void Controller::LoadAllPages(char* name)
{
	ifstream fin;
	fin.open(name);
	fin >> totalpage;
	if (fin)
	{
		AllPages = new Page*[totalpage];
		for (int i = 0; i < totalpage; i++)
		{
			AllPages[i] = new Page;
			AllPages[i]->ReadDataFromFile(fin);

		}
	}
	fin.close();
}
Page* Controller::SearchPageByID(char* ID)
{
	for (int i = 0; i < totalpage; i++)
	{
		if (Helper::mystrcmp(AllPages[i]->GetID(), ID))
			return AllPages[i];
	}
	return nullptr;

}
void Controller::LoadAllPosts(char* name)
{
	char* objectID = new char[5];
	char* likedID = new char[5];
	ifstream fin;
	fin.open(name);
	fin >> totalpost;
	if (fin)
	{
		AllPosts = new Post*[totalpage];
		for (int i = 0; i < totalpage; i++)
		{
			AllPosts[i] = new Post;
			AllPosts[i]->ReadDataFromFile(fin);
			fin >> objectID;
			Object* obj = SearchObjectByID(objectID);
			AllPosts[i]->SetSharedBy(obj);
			obj->AddTimeLine(AllPosts[i]);
			Object* temp;
			while (fin >> likedID && !Helper::mystrcmp(likedID, "-1"))
			{
				temp = SearchObjectByID(likedID);
				AllPosts[i]->AddLikedBy(temp);

			}

		}
	}
	fin.close();

}
void Controller::LoadAllComments(char* name)
{
	ifstream fin;
	fin.open(name);
	fin >> totalcomment;
	char* PostID = new char[5];
	char* ComID = new char[5];
	char* ComByID = new char[5];
	char temp[100];
	if (fin)
	{
	
		for (int i = 0; i < totalcomment; i++)
		{
			Comment* cptr = new Comment;
			fin >> ComID;
			cptr->SetID(PostID);
			fin >> PostID;
			Post* ptr = SearchPostByID(PostID);
			ptr->AddComment(cptr);
			fin >> ComByID;
			cptr->SetCommentBy(SearchObjectByID(ComByID));
			fin.ignore();
			fin.getline(temp, 100);
			char* text = Helper::GetStringFromBuffer(temp);
			cptr->SetText(text);
		}
	}
	fin.close();
}
void Controller::LoadAllActivities(char* name)
{
	ifstream fin;
	fin.open(name);
	fin >> totalactpost;
	if (fin)
	{
		for (int i = 0; i < totalactpost; i++)
		{
			char* PostID = new char[5];
			Activity* aptr = new Activity;
			fin >> PostID;
			Post* ptr = SearchPostByID(PostID);
			aptr->ReadDataFromFile(fin);
			ptr->AddContent(aptr);
		}
	}
	fin.close();

}
Post* Controller::SearchPostByID(char* ID)
{
	for (int i = 0; i < totalpost; i++)
	{
		if (Helper::mystrcmp(AllPosts[i]->GetID(), ID))
			return AllPosts[i];
	}
	return nullptr;

}
Object* Controller::SearchObjectByID(char* ID)
{
	User* temp1 = SearchUserByID(ID);
	Page* temp2 = SearchPageByID(ID);
	if (temp1)
		return temp1;
	if (temp2)
		return temp2;
	else
		return nullptr;

}
void Controller::LinkUsersAndPages(char* name)
{
	char* userID = new char[5];
	char* friendID = new char[5];
	char* pageID = new char[5];
	ifstream fin;
	fin.open(name);
	if (fin)
	{
		while (fin >> userID)
		{

			User* temp = SearchUserByID(userID);
			if (!temp)
				continue;
			while (fin >> friendID && !Helper::mystrcmp(friendID, "-1"))
			{

				User* user = SearchUserByID(friendID);
				temp->AddFriend(user);
			}
			while (fin >> pageID && !Helper::mystrcmp(pageID, "-1"))
			{
				Page* page = SearchPageByID(pageID);
				temp->LikePage(page);
			}
		}
	}
	fin.close();

}
void Controller::ViewLikedPost(char* str)
{
	Post* p = SearchPostByID(str);
	p->ViewLikedBy();
}
void Controller::LikePost(char* str)
{
	Post* p = SearchPostByID(str);
	p->AddLikedBy(CurrentUser);
}
void Controller::LoadData()
{
	LoadAllUsers("User.txt");
	LoadAllPages("Page.txt");
	LoadAllPosts("Post.txt");
	LoadAllActivities("Activity.txt");
	LoadAllComments("Comment.txt");
	LinkUsersAndPages("Link.txt");

}
void Controller::Run()
{
	SetCurrentUser("u7");
	Date d(17,4, 2024);
	Post::CurrentDate=d;
	cout << "------------------------------------------------------------------" << endl << endl;
	cout << "                     Social Network App\n\n";
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:           Set System Date "; d.Print(); cout << endl;
	cout << "------------------------------------------------------------------" << endl;
	cout << "System Date:       "; d.Print();cout << endl;
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:          Set current user '" << CurrentUser->GetID() << "'" << endl;
	cout << "------------------------------------------------------------------" << endl;
	cout << CurrentUser->GetFname() << " " << CurrentUser->GetLname() << " is set as current user." << endl << endl;
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:          View Friend List  " << endl;
	cout << "------------------------------------------------------------------" << endl;
	CurrentUser->ViewFriendList();
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:          View Liked Pages " << endl;
	cout << "------------------------------------------------------------------" << endl;
	CurrentUser->ViewLikedPages();
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:          View TimeLine" << endl;
	cout << "------------------------------------------------------------------" << endl;
	CurrentUser->ViewTimeLine();
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:		   View Liked List(Post 5) " << endl;
	cout << "------------------------------------------------------------------" << endl;
	ViewLikedPost("post5");
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:	       Like a post(Post 5) " << endl;
	LikePost("post5");
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:		  View Liked List(Post 5)" << endl;
	cout << "------------------------------------------------------------------" << endl;
	ViewLikedPost("post5");
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:		  View Page(Page 1)" << endl;
	cout << "------------------------------------------------------------------" << endl;
	Page* page = SearchPageByID("p1"); 
	page->ViewTimeLine();
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:		  View Home" << endl;
	cout << "------------------------------------------------------------------" << endl;
	CurrentUser->ViewHome();
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:         Post Comment(post4,When is your result coming?)" << endl;
	Comment* cptr1 = new Comment("c14", CurrentUser, "When is your result coming?");
	Post* post4 = SearchPostByID("post4"); 
	post4->AddComment(cptr1);
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:         View Post(Post 4)" << endl;
	cout << "------------------------------------------------------------------" << endl;
	post4->PostListView();
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:         PostComment(post8, Thanks for the wishes)" << endl;
	Comment* cptr2 = new Comment("c15", CurrentUser, "Thanks for the wishes");
	Post* post8 = SearchPostByID("post8"); 
	post8->AddComment(cptr2);
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:         View Post(Post 8)" << endl;
	cout << "------------------------------------------------------------------" << endl;
	post8->PostListView();
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:         See Your Memories" << endl;
	cout << "------------------------------------------------------------------" << endl;
	CurrentUser->SeeYourMemories();
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:        Share Memory(post10, Never thought I will be specialist in this field...)"<< endl;
	Post* post10 = SearchPostByID("post10");
	CurrentUser->ShareMemory(post10, "Never thought I will be specialist in this field...");
	cout << "------------------------------------------------------------------" << endl;
	cout << "Command:          View TimeLine" << endl;
	cout << "------------------------------------------------------------------" << endl;
	CurrentUser->ViewTimeLine();

}
Controller::~Controller()
{
	Helper::Destructor(AllUsers, totaluser);
	Helper::Destructor(AllPages, totalpage);
	if (AllPosts)
		delete[] AllPosts;
}
int main()
{
	Controller mainApp;
	mainApp.LoadData();
	mainApp.Run();
	system("pause");
	return 0;
}