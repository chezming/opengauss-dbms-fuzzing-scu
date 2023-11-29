#include "../include/connector.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <regex>
#include <algorithm>
#include <unistd.h>

using namespace std;

#define HOST "localhost"
#define USERNAME "omm"
#define PASSWORD "Aa1!@#$%^&*()"
#define PORT 5432

#define PIDFILEPATH "/home/omm/data/postmaster.pid"
const char* file = "/home/wx/openGauss-server/dest/bin/gs_ctl";
char* const options[] = { "/home/wx/openGauss-server/dest/bin/gs_ctl", "start", "-D", "/home/omm/data", "-Z", "single_node", "-l", "/home/omm/log/opengauss.log", NULL };

#define COREDIRPATH "/home/wx/core"
#define COREFILEPATH COREDIRPATH "/core"

#define GDBCMD "gdb /home/wx/openGauss-server/dest/bin/gaussdb " COREFILEPATH " -x /home/wx/check_unique_bug/test.gdb &> /dev/null"

#define UNIQUERESULTFILEPATH "/home/wx/check_unique_bug/unique_result"

#define STACKINFOFILEPATH "/home/wx/core/result_test"

vector<string> uniqueResult;
vector<string> skipFileName = { "bbox_create.cpp:404", "gs_bbox.cpp:112", "raise.c:55", "abort.c:90", "assert.cpp:46" };

bool get_all_files(string path, vector<string>& files) {
	path += "/";
	path.erase(path.find_last_not_of("/") + 1);

	DIR* dir = opendir(path.c_str());
	if (dir == nullptr) {
		return false;
	}

	struct dirent* entry = nullptr;
	while ((entry = readdir(dir)) != nullptr) {
		string fileName = entry->d_name;
		if (fileName == "." || fileName == "..") {
			continue;
		}

		if (entry->d_type == DT_DIR) {
			continue;
		}

		string filePath = path + "/" + fileName;
		files.push_back(filePath);
	}

	closedir(dir);

	return true;
}

void get_files_with_type(string path, vector<string>& files, string fileExt) {
	vector<string> filePaths;
	bool flag = get_all_files(path, filePaths);

	for (string filePath : filePaths) {
		if ((filePath.size() - fileExt.size()) <= 0 || strcmp(filePath.c_str() + filePath.size() - fileExt.size(), fileExt.c_str()) != 0) {
			continue;
		}

		files.push_back(filePath);
	}

	return;
}

void generate_gdb_info(string& filePath) {
	string cmd = "lz4 -d " + filePath + " " + COREFILEPATH + " &> /dev/null";
	system(cmd.c_str());

	cmd = GDBCMD;
	system(cmd.c_str());

	return;
}

string get_file_name(string& line) {
	string res;

	for (int i = line.length() - 1; i >= 0; i--) {
		if (line[i] == ' ' || line[i] == '/') {
			break;
		}

		res += line[i];
	}

	reverse(res.begin(), res.end());

	return res;
}

bool analyseResult(string outputPath, string& sql) {
	bool flag = true;
	string result;

	bool isUnique = false;
	string uniqueCrashLocation;

	ifstream inputFile(STACKINFOFILEPATH);
	if (inputFile.is_open()) {
		regex r(".*<signal handler called>.*");

		bool hasCheck = false;
		string line;
		while (getline(inputFile, line)) {
			result += line + "\n";

			if (hasCheck == false) {
				if (regex_match(line, r)) {
					continue;
				}

				string fileName = get_file_name(line);
				if (find(skipFileName.begin(), skipFileName.end(), fileName) != skipFileName.end()) {
					continue;
				}

				if (find(uniqueResult.begin(), uniqueResult.end(), fileName) == uniqueResult.end()) {
					uniqueCrashLocation = fileName;
					isUnique = true;
				}

				hasCheck = true;
			}
		}

		inputFile.close();
	}
	else {
		flag = false;
	}

	if (isUnique == true) {
		outputPath += "/";
		outputPath.erase(outputPath.find_last_not_of("/") + 1);

		ofstream outputFile(outputPath + "/crash" + to_string(uniqueResult.size() + 1));
		if (outputFile.is_open()) {
			outputFile << sql << endl;
			outputFile << endl << endl;
			outputFile << result << endl;

			outputFile.close();
			uniqueResult.push_back(uniqueCrashLocation);
		}
		else {
			flag = false;
		}
	}

	return flag;
}

bool clear_core_dir() {
	vector<string> filePaths;
	bool flag = get_all_files(COREDIRPATH, filePaths);

	if (flag == false) {
		return false;
	}

	if (!filePaths.empty()) {
		for (string filePath : filePaths) {
			if (remove(filePath.c_str()) == 0) {
				continue;
			}

			return false;
		}
	}

	return true;
}

bool process_crash(string outputPath, string& sql) {
	vector<string> filePaths;
	get_files_with_type(COREDIRPATH, filePaths, ".lz4");

	int i = 0;
	while (filePaths.empty() && i < 10) {
		sleep(5);

		get_files_with_type(COREDIRPATH, filePaths, ".lz4");
		i++;
	}

	if (filePaths.size() != 1) {
		return false;
	}

	generate_gdb_info(filePaths[0]);

	bool tmpFlag = analyseResult(outputPath, sql);
	clear_core_dir();

	return tmpFlag;
}

void write_log(string outputPath, string& log) {
	outputPath += "/";
	outputPath.erase(outputPath.find_last_not_of("/") + 1);

	ofstream logFile(outputPath + "/" + "logFile.log");
	if (logFile.is_open()) {
		logFile << log << endl;
		logFile.close();

		cout << "Success to Create Log File!!!" << endl;
	}
	else {
		cerr << "Fail to Create Log File!!!" << endl;
	}

	return;
}

bool load_unique_result() {
	ifstream inputFile(UNIQUERESULTFILEPATH);

	if (inputFile.is_open()) {
		string line;
		while (getline(inputFile, line)) {
			if (line == "") {
				continue;
			}

			uniqueResult.push_back(line);
		}

		inputFile.close();
	}
	else {
		return false;
	}

	return true;
}

void save_unique_result() {
	ofstream outputFile(UNIQUERESULTFILEPATH);

	if (outputFile.is_open()) {
		for (string& str : uniqueResult) {
			outputFile << str << endl;
		}

		outputFile.close();
	}

	return;
}

int main(int argc, char** argv) {
	if (argc != 3) {
		cerr << "ERROR: Lack of Argument!!!" << endl;
		exit(0);
	}

	Connector connector(HOST, USERNAME, PASSWORD, PORT, PIDFILEPATH);

	string inputPath = argv[1];
	string outputPath = argv[2];

	string log;

	vector<string> filePaths;
	bool flag = get_all_files(inputPath, filePaths);
	if (flag == false) {
		cerr << "ERROR: Can't open the dir: " << inputPath << endl;
		return 0;
	}

	flag = clear_core_dir();
	if (flag == false) {
		cerr << "ERROR: Fail to clear core dir!!!" << endl;
		return 0;
	}

	flag = load_unique_result();
	if (flag == false) {
		cerr << "WARNING: Fail to load unique result!!!" << endl;
	}

	cout << "===============Start Process===============" << endl;

	for (string& filePath : filePaths) {
		cout << "Open File: " << filePath << ", Status: " << flush;
		log += "Open File: " + filePath + ", Status: ";

		ifstream inputFile(filePath);
		if (inputFile.is_open()) {
			cout << "Successed" << flush;
			log += "Successed";

			string sql;
			string tmp;

			while (getline(inputFile, tmp)) {
				sql += tmp + "\n";
			}
			inputFile.close();

			sql.erase(sql.find_last_not_of("\n") + 1);

			while (!connector.start_db(file, options));

			SQLSTATUS status = connector.execute(sql.c_str());

			if (status == SQLSTATUS::ssServerCrash) {
				bool tmpFlag = process_crash(outputPath, sql);

				if (tmpFlag == false) {
					cout << ", ProcFailed" << flush;
					log += ", ProcFailed";
				}
				else {
					cout << ", ProcSuccessful" << flush;
					log += ", ProcSuccessful";
				}
			}
			else {
				cout << ", NoCrash" << flush;
				log += ", NoCrash";

				connector.close_db();
			}
		}
		else {
			cout << "Failed" << flush;
			log += "Failed";
		}

		cout << endl;
		log += "\n";
	}

	cout << "===============End Process===============" << endl;

	write_log(outputPath, log);
	save_unique_result();

	return 0;
}
