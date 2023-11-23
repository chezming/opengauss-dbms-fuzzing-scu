#include "../include/connector.h"

static void myproc(void* arg, const PGresult* res) {
	return;
}

Connector::Connector(const char* host, const char* userName, const char* password, unsigned int port, const char* pidFIlePath) {
	conninfo_ += "host=" + string(host) + " ";
	conninfo_ += "user=" + string(userName) + " ";
	conninfo_ += "password=" + string(password) + " ";
	conninfo_ += "port=" + to_string(port);
	
	pidFIlePath_ = pidFIlePath;

	return;
}

bool Connector::start_db(const char* file, char* const options[]) {
	int dbPid = get_pid();

	int i = 0;
	while (dbPid == 1 && i < 10) {
		pid_t pid = fork();

		if (pid == 0) {
			execvp(file, options);
			exit(1);
		}
		else {
			wait(NULL);
			sleep(5);
		}

		dbPid = get_pid();
		i++;
	}

	if (dbPid != 1) {
		SQLSTATUS status = reset_db();

		if (status == SQLSTATUS::ssNormal) {
			return true;
		}		
	}

	return false;
}
bool Connector::close_db() {
	int dbPid = get_pid();
	int res = -1;

	if (dbPid != 1) {
		res = kill(dbPid, SIGKILL);
	}

	if (res == -1) {
		return false;
	}

	sleep(5);

	return true;
}

SQLSTATUS Connector::execute(const char* sql) {
	SQLSTATUS res = SQLSTATUS::ssNormal;
	string curConninfo = "dbname=test " + conninfo_;

	PGconn* conn = PQconnectdb(curConninfo.c_str());
	if (PQstatus(conn) != CONNECTION_OK) {
		PQfinish(conn);
		return SQLSTATUS::ssConnectFailed;
	}

	PQsetNoticeReceiver(conn, myproc, nullptr);

	reset_database(conn);

	PGresult* pgres = PQexec(conn, sql);
	if (PQstatus(conn) != CONNECTION_OK) {
		res = SQLSTATUS::ssServerCrash;
	}
	else if (PQresultStatus(pgres) != PGRES_COMMAND_OK && PQresultStatus(pgres) != PGRES_TUPLES_OK) {
		res = SQLSTATUS::ssSemanticError;
	}

	PQclear(pgres);
	PQfinish(conn);

	return res;
}

void Connector::reset_database(PGconn* conn) {
	PGresult* res = PQexec(conn, "DROP SCHEMA public CASCADE; CREATE SCHEMA public;");
	PQclear(res);

	return;
}

SQLSTATUS Connector::reset_db() {
	SQLSTATUS res = SQLSTATUS::ssNormal;
	string curConninfo = "dbname=postgres " + conninfo_;

	PGconn* conn = PQconnectdb(curConninfo.c_str());
	if (PQstatus(conn) != CONNECTION_OK) {
		PQfinish(conn);
		return SQLSTATUS::ssConnectFailed;
	}

	PQsetNoticeReceiver(conn, myproc, nullptr);

	int i = 0;

	PGresult* pgres = PQexec(conn, "DROP DATABASE IF EXISTS test;");
	PQclear(pgres);
	pgres = PQexec(conn, "CREATE DATABASE IF NOT EXISTS test;");
	PQclear(pgres);

	PQfinish(conn);

	res = execute("SELECT 1;");

	return res;
}

int Connector::get_pid() {
	int res = 1;

	ifstream pidFile(pidFIlePath_);
	if (pidFile.is_open()) {
		string pidStr;
		getline(pidFile, pidStr);

		pidFile.close();

		bool flag = true;

		for (char c : pidStr) {
			if (!isdigit(c)) {
				flag = false;
				break;
			}
		}

		if (flag == false || pidStr.empty()) {
			cerr << "ERROR: Unrecognized PID --- " << pidStr << endl;
			assert(flag == false || pidStr.empty());
		}

		res = stoi(pidStr);
	}

	if (res != 1) {
		string processStatusFilePath = "/proc/" + to_string(res) + "/status";
		ifstream statusFile(processStatusFilePath);

		if (statusFile.is_open()) {
			statusFile.close();
		}
		else {
			res = 1;
		}
	}

	return res;
}