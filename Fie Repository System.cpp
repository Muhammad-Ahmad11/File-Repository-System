#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <list>
using namespace std;

class Queue_item {
public:
	int user_id;
	int file_id;
	int key;
	string command;
	list<Queue_item> LIST;
	
	Queue_item() {
		user_id = 0;
		key = 0;
		command = "";
		file_id = 0;
	}
	
	Queue_item(int file_id, int userid, int key, string operation) {
		this->file_id = file_id;
		this->user_id = userid;
		this->key = key;
		this->command = operation;
	}
};

class Queue: public Queue_item {
public:

	struct SETTER {
		bool operator()(Queue_item& item1, Queue_item& item2) {
			return item1.key > item2.key;
		}
	};

	vector<Queue_item> VEC;

	Queue() {
		VEC.push_back(Queue_item());
	}

	void SET_vector(vector<Queue_item>& V) {
		VEC = V;
	}

	void Buildqueue() {
		make_heap(VEC.begin(), VEC.end(), SETTER());
		sort_heap(VEC.begin(), VEC.end(), SETTER());
		//PrintQueue();
	}

	void InsertUser(Queue& Q, Queue_item new_item) {
		VEC.push_back(new_item);
		Buildqueue();
	}

	bool IsEmpty() {	
		return VEC.size() == 0;
	}

	int size() {
		return VEC.size();
	}

	Queue_item Remove_max(Queue& Q) {
		Queue_item Item;
		if (VEC.size() == 1) {
			Item = VEC.back();
			VEC.pop_back();
		}
		if (!IsEmpty()) {
			Item = VEC[0];
			VEC[0] = VEC.back();
			VEC.pop_back();
			Buildqueue();
		}
		else {
			cout << "\nHeap is Empty" << endl;
		}
		return Item;
	}

	void Search(int find) {
		if (!IsEmpty()) {
			cout << "\nSearching Maximum: " << endl;
			cout << "User_id: " << VEC.at(find).user_id << endl;
			cout << "Priority: " << VEC.at(find).key << endl;
			cout << "Command: " << VEC.at(find).command << endl;
		}
		else {
			cout << "\nHeap is Empty" << endl;
		}
	}

	void sortqueue(Queue& Q) {
		Q.Buildqueue();
	}

	void setfileid(int id) {
		file_id = id;
	}

	Queue_item Remove_last(int& file_id, Queue& Q) {
		if (!Q.IsEmpty()) {
			//PrintQueue();
			file_id = Q.file_id;
			Queue_item Q = VEC.back();
			//VEC.push_back(Q);
			VEC.pop_back();
			
			return Q;
		}
	}

	bool searchfile_id(int file_id, int& size) {
		int i = 0;
		int check = 0;
		while (i < VEC.size()) {
			if (file_id == VEC.at(i).file_id) {
				size++;
				check = 1;
			}
			i++;
		}
		if (check) {
			return true;
		}
		return false;
	}

	int Search_max() {
			return VEC.at(0).key;
	}

	int Search_min() {
			return VEC.at(VEC.size() - 1).key;
	}

	void PrintQueue() {
		cout << "Queue elements: " << endl;
		int i = 0;
		while (i < VEC.size()) {
			cout << "Priority: " << VEC.at(i).key << endl;
			i++;
		}
	}
};

class HashTable {
public:
	Queue* Q;
	
	int num_bucks;

	HashTable(int size) {
		if (size < 0)
			size = 10;
		num_bucks = size;
		Q = new Queue [num_bucks];
	}

	bool insertItem(int key, Queue_item table) {  //key is file_id
		int check = 0;
		int	hi = calcHash(key);
		if (Q[hi].LIST.empty())
		{
			Q[hi].LIST.push_back(table);
			return true;
		}
		else
		{
			list<Queue_item> temp = Q[hi].LIST;
			for (list<Queue_item>::iterator it = temp.begin(); it != temp.end(); it++) {
				if (it->key < table.key) {            // Priority based insertion
					check = 1;
					Q[hi].LIST.push_back(table);
					break;
				}
			}
			if(!check)
			      Q[hi].LIST.push_back(table);
			return true;
		}
		return false;
	}

	bool check_clash(int file_id, int user_id, int priority, string command) {
		int hi = calcHash(file_id);
		list<Queue_item> temp = (Q[hi].LIST);
		list<Queue_item>::iterator itr = temp.begin();
		if(!temp.empty()){
			if (itr->file_id == file_id && itr->key == priority && itr->command == "W" && command == "R" ) {
				cout << "Error USER A wants to update file but user B wants to read file" << endl;
				return false;
			}
			if (itr->file_id == file_id && itr->key == priority && itr->command == "R" && command == "W") {
				cout << "Error USER A wants to read file but user B wants to update file" << endl;
				return false;
			}
		}
		return true;
	}

	bool search_file(int id) {
		//int hi = calcHash(id);
		for (int i = 0; i < num_bucks; i++) {
			list<Queue_item> temp = (Q[i].LIST);
			if (!temp.empty()) {

				for (list<Queue_item>::iterator it = temp.begin(); it != temp.end(); it++) {
					if (it->file_id == id) {
						return true;
					}
				}

			}
		}
		return false;
	}

	bool release_file(int user_id, int file_id) {
		int hi = calcHash(file_id);
		if (!Q[hi].LIST.empty()) {
			list<Queue_item> temp = Q[hi].LIST;
			for (list<Queue_item>::iterator it = temp.begin(); it != temp.end(); it++) {
				if (it->user_id != user_id) {
					Queue_item z = Q[hi].LIST.front();
					Q[hi].LIST.pop_front();
					Q[hi].LIST.push_back(z);
				}
				else {
					Q[hi].LIST.pop_front();
					Queue_item q = Q[hi].LIST.front();
					while (q.command == "R") {   //code for releasing all files with read access
						Q[hi].LIST.pop_front();
						q = Q[hi].LIST.front();
					}
					break;
				}
			}
			return true;
		}
		return false;
	}

	int calcHash(int key) {
		return key % num_bucks;
	}

	void clearlist(int id) {
		int hi = calcHash(id);
		Q[hi].LIST.clear();
	}

	void loadfile(int file_id) {
		int next_user_check = 0;
		int waiting_list = 0;
		int hi = calcHash(file_id);
		list<Queue_item> temp = Q[hi].LIST;
		if (!temp.empty()) {
			for (list<Queue_item>::iterator it = temp.begin(); it != temp.end(); it++) {
				if (next_user_check == 0) {
					cout << "\nH" << hi << " --> File " << it->file_id << "  ...  ";
					cout << "Access granted to user " << it->user_id << ", " << it->command << "\n";
					if (temp.size() > 1 && next_user_check == 0) {
						it++;
						next_user_check = 1;
						if (it != temp.end()) {
							cout << "Next user" << (it)->user_id << ", " << (it)->command << endl;
						}

					}
				}
				if (next_user_check == 1) {
					waiting_list++;
				}
			}
			if (waiting_list == 0) {
				cout << "Next none\n";
				cout << "Waiting None\n";
			}
			else {
				cout << "Waiting " << waiting_list << " user" << endl;
			}
		}
	}

	void Delete_file(int id) {
		int hi = calcHash(id);
		if (Q[hi].LIST.empty()) {
			cout << "File does not exist" << endl;
		}
		else {
			cout << "Someone is accessing the file" << endl;
		}
	}

	void hashDisplay()
	{

		int next_user_check = 0;
		int waiting_list = 0;
		cout << "\nHash Table: " << endl;
		
		for (int i = 0; i < num_bucks; i++) {
			
			list<Queue_item> temp = (Q[i].LIST);
		
			if (!temp.empty()) {

				for (list<Queue_item>::iterator it = temp.begin(); it != temp.end(); it++) {
					
					if (next_user_check == 0) {
						cout << "\nH" << i << " --> File " << it->file_id << "  ...  ";
						cout << "Access granted to user " << it->user_id << ", " << it->command << "\n";
						if (temp.size() > 1 && next_user_check == 0) {
							it++;
							next_user_check = 1;
							if (it != temp.end()) {
								cout << "Next user" << (it)->user_id << ", " << (it)->command << endl;
							}

						}
					}
					if (next_user_check == 1) {
						waiting_list++;
					}
					
				}
				if (waiting_list == 0) {
					cout << "Next none\n";
					cout << "Waiting None\n";
				}
				else {
					cout << "Waiting " << waiting_list << " user" << endl;
				}
				waiting_list = 0;
				next_user_check = 0;
			}
		}
	}
};

int main() {
	fstream fin;
	fin.open("input.txt", ios::in);
	char str[200];
	vector<Queue_item> vec;
	Queue pq1;
	HashTable H(10);
	char c;
	int total_files = 0, file_id = 0, user_id = 0, priority = 0;
	string command = "";

	if (fin.is_open()) {

		fin >> str;
		fin.getline(str, 200, '\n');
		while (!fin.eof()) {
			total_files++;
			fin >> file_id;                           //reading file_id

			if (file_id > 5000 && file_id < 99999) {  //checking file_id in range

				fin >> c;                             //reading comma
				fin >> user_id;                       //reading user_id

				if (user_id > 0 && user_id < 10000) { //checking user_id in range

					fin >> c;                             //reading comma
					fin >> priority;                      //reading priority

					if (priority > 0 && priority < 100) { //checking priority in range

						fin >> c;                             //reading comma
						fin >> command;                       //reading access mode (read or write)

						Queue_item A(file_id, user_id, priority, command);   //key, id, choice

						vec.push_back(A);

						pq1.SET_vector(vec);

						pq1.Buildqueue();

					}
					else {
						cout << "\nPriority out of range" << endl;
						break;
					}
				}
				else {
					cout << "\nUser_id out of range" << endl;
					break;
				}
			}
			else {
				cout << "\nFile does not exist" << endl;
				break;
			}
		}
	}           //reading dadta from file
	else {
		cout << "\nUnable to open file input.txt" << endl;
	}
	
	for (int i = 0; i < total_files; i++) {                           //Priority based insertion
		Queue_item item = pq1.Remove_last(file_id, pq1);
		H.insertItem(item.file_id, item);
	}

	int choice = 0;
	cout << "-----------------------------Priority Queue AND Hashing mini-Project-------------------------------------" << endl;
	cout << "\n\n";
	cout << "--------------------------------------------Command Menu--------------------------------------------\n\n" << endl;
	cout << "1.Print Hash Table with file information " << endl;
	cout << "2.Load a file" << endl;
	cout << "3.Insert a file" << endl;
	cout << "4.Delete a file" << endl;
	cout << "5.Request access to file by user" << endl;
	cout << "6.Release a file by user" << endl;
	cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "\n\n";

	cout << "Select any option or -1 to exit: " << endl;
	cin >> choice;
	while (choice != -1) {
		if (choice == 1) {
			H.hashDisplay();        //display hash table
			
		}
		else if (choice == 2) {
			//case 2:
			cout << "Enter file_id to load it" << endl;
			cin >> file_id;
			if (H.search_file(file_id)) {       //searching for file
				H.loadfile(file_id);            //loading file data 
			}
			else {
				cout << "File not found" << endl;
			}
			
		}
		else if (choice == 3) {
			cout << "Enter file_id" << endl;                            //getting file_id
			cin >> file_id;
			cout << "Enter user_id" << endl;                            //getting user_id
			cin >> user_id;
			cout << "Set priority: " << endl;                           //getting priority
			cin >> priority;
			cout << "Enter access method: " << endl;                    //getting access method
			cin >> command;
			if (H.check_clash(file_id, user_id, priority, command)) {   //access to file if there is no clash
				Queue_item A(file_id, user_id, priority, command);
				vec.push_back(A);

				pq1.SET_vector(vec);

				pq1.Buildqueue();
				total_files++;

				for (int i = 0; i < total_files; i++) {                          //setting queue
					
						Queue_item R = vec.back();

						H.clearlist(R.file_id);

						vec.pop_back();
				
				}

				for (int i = 0; i < total_files; i++) {                           //Priority based insertion
					Queue_item item = pq1.Remove_last(file_id, pq1);

					H.insertItem(item.file_id, item);
				}
				H.hashDisplay();                                                  //Dsplaying hash table
			}
			
		}
		else if (choice == 4) {
			cout << "Deleting a file: " << endl;
			cout << "Enter file_id" << endl;
			cin >> file_id;
			if (!H.search_file(file_id)) {              //searching for file
				cout << "File not found" << endl;
			}
			else {
				H.Delete_file(file_id);                  //deleting file
			}
		}
		else if (choice == 5) {

			string p;
			//read data from file to request file access
			cout << "\nRequest File Access " << endl;
			cout << "Enter file_id" << endl;
			cin >> file_id;
			cout << "Enter user_id" << endl;
			cin >> user_id;
			cout << "Set priority high or low or none" << endl;
			cin >> p;
			if (p == "N") {
				cout << "Enter your priority: " << endl;                   //if user enter no priority range then he can enter it by himself
				cin >> priority;
			}
			cout << "Enter access method R for read and W for write and N for None" << endl;
			cin >> command;
			if (H.search_file(file_id)) {                    //searching for file
				if (H.check_clash(file_id, user_id, priority, command)) {  //inserts file if there is no clash
					
					if (p == "H") {
						priority = H.Q[H.calcHash(file_id)].LIST.front().key;  //setting highest priority from the queue
						if (priority > 2) {
							priority--;
						}
						if (priority == 1) {
							priority = priority;
						}
					}
					else if (p == "L") {
						priority = H.Q[H.calcHash(file_id)].LIST.back().key + 1; //setting lowest priority from the queue
					}
					else if (p == "N") {
						priority = priority;
					}
					Queue_item A(file_id, user_id, priority, command);
					vec.push_back(A);

					pq1.SET_vector(vec);

					pq1.Buildqueue();
					total_files++;


					for (int i = 0; i < total_files-1; i++) {                        //setting queue
						Queue_item R = vec.back();
						H.clearlist(R.file_id);
						vec.pop_back();
					}

					for (int i = 0; i < total_files; i++) {                           //Priority based insertion
						Queue_item item = pq1.Remove_last(file_id, pq1);

						H.insertItem(item.file_id, item);
					}
					H.hashDisplay();
				}
			}
			else {
				cout << "\nFile not found" << endl;
			}
		}
		else if (choice == 6) {
			//case 5:
			user_id = 10;  //release file
			file_id = 23457;
			
			cout << "Enter user_id: " << endl;          //taking user_id
			cin >> user_id;
			cout << "Enter file_id: " << endl;          //taking file_id
			cin >> file_id;
			if (H.search_file(file_id)) {               //searching for file in hash table
				H.release_file(user_id, file_id);
				cout << "\n\n";
				H.hashDisplay();                       //displaying hash table
			}
			else {
				cout << "File not found" << endl;
			}
		}
		else {
			cout << "\nWrong choice selected " << endl;       
		}
		cin >> choice;
	}
	return 0;
}