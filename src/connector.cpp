#include "../include/connector.h"

static void myproc(void* arg, const PGresult* res) {
	return;
}

Connector::Connector(const char* host, const char* userName, const char* password, unsigned int port, const char* serverName) {
	conninfo_ += "host=" + string(host) + " ";
	conninfo_ += "user=" + string(userName) + " ";
	conninfo_ += "password=" + string(password) + " ";
	conninfo_ += "port=" + to_string(port);
	
	serverName_ = serverName;

	return;
}

bool Connector::start_db(const char* file, char* const options[]) {
	pid_t dbPid = get_pid_by_name();

	if (dbPid != 1) {
		SQLSTATUS status = reset_db();

		if (status == SQLSTATUS::ssNormal) {
			return true;
		}

		return false;
	}

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

		dbPid = get_pid_by_name();
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
	pid_t dbPid = get_pid_by_name();
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

pid_t Connector::get_pid_by_name() {
    DIR* dir;
    struct dirent* ptr;
    FILE* fp;
    char filepath[50];
    char cur_task_name[50];
    char buf[BUF_SIZE];

    pid_t pid = 1;

    dir = opendir("/proc");
    if (NULL != dir)
    {
        while ((ptr = readdir(dir)) != NULL)
        {

            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                continue;
            if (DT_DIR != ptr->d_type)
                continue;

            sprintf(filepath, "/proc/%s/status", ptr->d_name);
            fp = fopen(filepath, "r");
            if (NULL != fp)
            {
                if (fgets(buf, BUF_SIZE - 1, fp) == NULL) {
                    fclose(fp);
                    continue;
                }
                sscanf(buf, "%*s %s", cur_task_name);

                if (!strcmp(serverName_.c_str(), cur_task_name)) {
                    sscanf(ptr->d_name, "%d", &pid);
                }
                fclose(fp);
            }
        }
        closedir(dir);
    }

    return pid;
}
