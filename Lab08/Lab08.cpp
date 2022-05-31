// Zachary Thomas
// Lab08

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

const int NAME_LEN = 21;
const int AGE_LEN = 3;
const int RECORD_LEN = NAME_LEN + AGE_LEN + 2;

int main()
{
	// initialize variables
	int numRecords = 0, delRecords = 0;
	int age, search_age, choice, count, record, compare_age_left, compare_age_right;
	string name, temp, search_name, compare_name_left, compare_name_right;
	bool is_name_in_db = false;
	char change;
	char filename[] = "sample.dat";
	fstream iofile(filename, ios::in | ios::out);


	//open file and count records
	ws(iofile);
	getline(iofile, name, ',');
	while (!iofile.eof() && !iofile.fail())
	{
		iofile >> age;
		cout << name << " " << age << endl;
		if (name[0] == '~') {
			delRecords++;
		}
		else {
			numRecords++;
		}
		ws(iofile);
		getline(iofile, name, ',');
	}
	iofile.close();
	cout << "Have " << numRecords << " records" << endl;

	do
	{
		// menu
		cout << "\nMenu\n"
			<< "1 - Add record\n"
			<< "2 - Change record\n"
			<< "3 - Print all records\n"
			<< "4 - Sort records\n"
			<< "5 - Search records\n"
			<< "6 - Delete record\n"
			<< "0 - Quit\n"
			<< "Enter choice: ";
		cin >> choice;
		getline(cin, temp); // phantom newline

		//
		switch (choice)
		{
		case 1: // add record

			// stream mode
			if (numRecords > 0) {
				iofile.open(filename, ios::in | ios::out);
			}
			else {
				iofile.open(filename, ios::in | ios::out | ios::app);
			}

			// get input
			cout << "Enter name: ";
			getline(cin, name);
			cout << "Enter age: ";
			cin >> age;

			// add new record
			if(delRecords > 0) { // if there is at least one deleted record replace with new added record
				iofile.seekg(0, ios::beg);
				ws(iofile);
				getline(iofile, search_name, ',');
				
				count = 1;
				while (!iofile.eof() && !iofile.fail())
				{
					iofile >> search_age;

					if (search_name[0] == '~') {
						
						// update database
						iofile.seekp((count - 1) * RECORD_LEN, ios::beg);
						iofile << setw(NAME_LEN) << name << ","
							<< setw(AGE_LEN) << age << endl;
						break;
					}

					count++;
					ws(iofile);
					getline(iofile, search_name, ',');
				}
				delRecords--; 
			}
			else{ // add to the end of the file
				iofile.seekp(0, ios::end);
				iofile << setw(NAME_LEN) << name << ","
					<< setw(AGE_LEN) << age << endl;
			}

			numRecords++;
			
			iofile.close();
			break;

		case 2: // change record
			iofile.open(filename, ios::in | ios::out);
			cout << "Enter record number: ";
			cin >> record;

			if (record > numRecords){
				cout << "Record does not exist\n";
			}
			else{
				iofile.seekg((record - 1) * RECORD_LEN, ios::beg);
				ws(iofile);
				getline(iofile, name, ',');
				iofile >> age;

				// get input
				cout << "Record " << record << " " << name << " " << age << endl;
				cout << "Change name? ";
				cin >> change;
				getline(cin, temp); // phantom newline
				
				if (change == 'y' || change == 'Y') {
					cout << "Enter name: ";
					getline(cin, name);
				}
				cout << "Change age? ";
				cin >> change;
				getline(cin, temp);
				if (change == 'y' || change == 'Y') {
					cout << "Enter age: ";
					cin >> age;
				}

				// update database
				iofile.seekp((record - 1) * RECORD_LEN, ios::beg);
				iofile << setw(NAME_LEN) << name << ","
					<< setw(AGE_LEN) << age << endl;
			}

			iofile.close();
			break;

		case 3: // print all records
			iofile.open(filename, ios::in | ios::out);
			iofile.seekg(0, ios::beg);
			ws(iofile);
			getline(iofile, name, ',');
			count = 1;

			// print output 
			cout << endl;
			while (!iofile.eof() && !iofile.fail())
			{
				iofile >> age;
				if (name[0] != '~') {
					cout << "Record " << count << " " << name << " " << age << endl;
					count++;
				}

				ws(iofile);
				getline(iofile, name, ',');
			}

			iofile.close();
			break;

		case 4: // sort
			cout << "\nSorting Database: " << endl;

			for (int step = 0; step < numRecords + delRecords; step++) {
				iofile.open(filename, ios::in | ios::out);
				iofile.seekg(0, ios::beg);
				for (int i = 0; i < (numRecords + delRecords -1) - step; i++) {

					//compare two adjacent elements 
					iofile.seekp((i)* RECORD_LEN, ios::beg);
					ws(iofile);
					getline(iofile, compare_name_left, ',');
					iofile >> compare_age_left;

					iofile.seekp((i+1)*RECORD_LEN, ios::beg);
					ws(iofile);
					getline(iofile, compare_name_right, ',');
					iofile >> compare_age_right;

					//cout << "Left: " << compare_name_left << endl; 
					//cout << "Right: " << compare_name_right << endl; 
					//cout << endl;

					if (compare_name_left.compare(compare_name_right) > 0) {
						//cout << "Switch" << endl;
						iofile.close();

						iofile.open(filename, ios::in | ios::out);
						iofile.seekg(0, ios::beg);

						iofile.seekp((i)* RECORD_LEN, ios::beg);
						iofile << setw(NAME_LEN) << compare_name_right << ","
							<< setw(AGE_LEN) << compare_age_right << endl;

						iofile.seekp((i+1)*RECORD_LEN, ios::beg);
						iofile << setw(NAME_LEN) << compare_name_left << ","
							<< setw(AGE_LEN) << compare_age_left << endl;

					}

				}
				iofile.close();
			}

			break;

		case 5: // search
			cout << "Enter search name: ";
			getline(cin, search_name);

			iofile.open(filename, ios::in | ios::out);
			iofile.seekg(0, ios::beg);
			ws(iofile);
			getline(iofile, name, ',');

			count = 1;
			is_name_in_db = false;
			while (!iofile.eof() && !iofile.fail())
			{
				iofile >> age;
				
				if (search_name.compare(name) == 0) {
					cout << "Found at record #" << count << endl;			
					is_name_in_db = true;
					break;
				}
				count++;
				ws(iofile);
				getline(iofile, name, ',');
			}

			if (!is_name_in_db) { cout << search_name << " not found in db" << endl; }

			iofile.close();
			break;

		case 6: // delete
			cout << "Enter record number to delete: ";
			cin >> record;

			iofile.open(filename, ios::in | ios::out);
			iofile.seekg(0, ios::beg);
			ws(iofile);
			getline(iofile, name, ',');

			if (record > numRecords || record < 1)
			{
				cout << "Record does not exist\n";
			}
			else
			{
				iofile.seekg((record - 1) * RECORD_LEN, ios::beg);
				ws(iofile);
				getline(iofile, name, ',');
				iofile >> age;

				cout << "record is " << name << " " << age << endl;

				cout << "Delete? "; 
				cin >> change; 

				if (change == 'y' || change == 'Y') {
					name[0] = '~';

					// update file
					iofile.seekp((record - 1)* RECORD_LEN, ios::beg);
					iofile << setw(NAME_LEN) << name << ","
						<< setw(AGE_LEN) << age << endl;

					delRecords++;
					numRecords--;
				}
			}

			// print deletion information 
			cout << delRecords << " deleted record(s)" << endl;

			iofile.close();
			break;

		case 0:
			cout << "Quit\n";
			break;
		default: 
			cout << "Invalid selection\n";
			break;
		}
	} while (choice != 0);
	system("pause");
}