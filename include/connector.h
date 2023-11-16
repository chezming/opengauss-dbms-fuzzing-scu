#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include <string>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "libpq-fe.h"

using namespace std;

#define BUF_SIZE 1024

enum class SQLSTATUS {
	ssNormal,
	ssConnectFailed,
	ssServerCrash,
	ssSyntaxError,
	ssSemanticError,
	ssNoDatabase
};

class Connector {
public:
	Connector(const char* host, const char* userName, const char* password, unsigned int port, const char* serverName);

	SQLSTATUS execute(const char* sql);
	bool start_db(const char* file, char* const options[]);
	bool close_db();

private:
	string conninfo_;
	string serverName_;

	void reset_database(PGconn* conn);
	SQLSTATUS reset_db();
	pid_t get_pid_by_name();
};

#endif