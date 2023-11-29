# DBMS Fuzz Testing Tool User Guide

This tool only includes the Fuzzer part, not the instrumentation part. The instrumentation part is the same as AFL, please refer to AFL for specific usage. It is best to use sparse instrumentation during instrumentation, or to instrument key modules of the database. If sparse instrumentation is used, the instrumentation ratio of OpenGaussDB should not exceed 10%. If only key modules are instrumented, you need to judge where to instrument.

This tool was initially used to test MySQL, so it uses MySQL syntax. OpenGaussDB has limited compatibility with MySQL in terms of syntax, so the test effect will decrease to a certain extent.
Before using this tool, you need to adjust some specific parameters according to the actual environment.

## AFL/afl-fuzz.c  

```c
#define ABPATH "test" //Absolute path of the tool directory, modify according to the actual situation
#define RLPATH_LIB "/corpus/initlib"  //Relative path of the initial corpus
#define RLPATH_IRINFO "/corpus/IR_config"  //Relative path of the grammar information file

#define INIT_LIB ABPATH RLPATH_LIB
#define GRAMMAR_INFORMATION ABPATH RLPATH_IRINFO
#define OUTPUT_PATH ABPATH RLPATH_OUTPUT

//Database connection parameters, these parameters may change a lot depending on the implementation of connector.cpp
#define HOST "localhost"  //ip
#define USERNAME "omm"  //username
#define PASSWORD "Aa1!@#$%^&*()"  //password
#define PORT 5432  //port

#define PIDFILEPATH "/home/omm/data/postmaster.pid"  //Path of the pid file, used to find the server pid, used when starting and shutting down the database
//start_db parameters, passed into the execvp function using start_db, to start the database
const char* file = "/home/wx/openGauss-server/inst_build/bin/gs_ctl";
char* const options[] = { "/home/wx/openGauss-server/inst_build/bin/gs_ctl", "start", "-D", "/home/omm/data", "-Z", "single_node", "-l", "/home/omm/log/opengauss.log", NULL };
//Connector, used to connect to the database, needs to be modified according to the database. The connector provided by this tool is used to connect to openGaussDB, using libpq to connect
Connector g_connector(HOST, USERNAME, PASSWORD, PORT, PIDFILEPATH);	//The database name is test by default, hard-coded in connector.cpp
```

## AFL/Makefile  

```makefile
PGSQL_LIB_PATH = /home/wx/openGauss-server/inst_build/lib  //Directory of libpq.so
PGSQL_INCLUDE_PATH = /home/wx/openGauss-server/inst_build/include //Directory of libpq-fe.h
```

## include/instantiate.h

```c
//These parameters are used to limit the length of SQL statements to prevent performance degradation due to too long statements. The maximum length will increase over time
//The specific calculation method is: max(CAPACITYBASE * pow(CAPACITYGROWTHRATE, T), MAXCAPACITY), where T is the current execution time (hours), rounded down
//Adjust according to the specific situation
#define CAPACITYBASE 80  
#define MAXCAPACITY 120
#define CAPACITYGROWTHRATE 1.02
```

## corpus/input

This directory contains the initial seeds used for testing. In principle, you can add new seeds yourself. However, since this tool has limited support for OpenGaussDB syntax, you need to ensure that the added seeds can be parsed by the built-in parser of this tool. This tool does not come with a detection program, but the entry point of the parser is the IR_parser function in mutate.cpp. You can write a simple program yourself and call this function for testing. The added seeds need to have a complete context, for specific reference, see the existing seeds. When reading, it will read the SQL statement of the entire file. If only the first sentence is read, it may be due to the difference between Linux and Windows line breaks. This situation will occur if you write seeds in a Windows environment and execute them in a Linux environment.

## corpus/initlib

This directory contains the initial corpus used by the tool. In principle, you can add new corpus to it. However, you need to ensure that the added corpus can be parsed by the tool. The added corpus does not need to have a complete context. When actually reading, the SQL statement is read line by line, so you need to ensure that a SQL statement is a line. corpus/initlib/init_lib is related to the normal operation of the program and cannot be deleted.

## Compilation

This tool can run under Ubuntu20.04 and CentOS7 operating systems, and other Linux systems should also be possible.  
First, execute the make command in the parser/ directory. It can successfully generate related files under bison 3.0.4 version and flex 2.5.37 version. It can also be lower than this version, as long as it can be executed successfully.  
Then go to the AFL/ directory and execute the make command. It can be successfully compiled under gcc 10.3.0 version. Any version that supports the C++17 standard should be able to compile successfully. The minimum version is probably GCC 7.1.  
The location of the header files and dynamic libraries required to connect to the database needs to be set by yourself. If you encounter problems such as not finding the corresponding header files or errors in dynamic library linking, please handle it yourself.

## Start command

```sh
/Tool path/AFL/afl-fuzz -i /Tool path/corpus/input -o /Custom output directory OpenGaussDB
```
The last "OpenGaussDB" parameter has no actual meaning. In AFL, you need to enter the path of the program to be tested at the end. In this tool, this parameter is not needed, but the AFL command line parameter parsing part has not been modified. Therefore, in order to ensure the integrity of the command, this parameter needs to be added.

## Unique Crash

This tool inherits the original method of AFL to exclude duplicate crashes. This method is invalid in DBMS testing, so the Unique Crash part of the tool interface actually includes duplicate crashes. It is recommended to manually or write scripts to remove duplicate crashes after obtaining the statements that trigger the crash.