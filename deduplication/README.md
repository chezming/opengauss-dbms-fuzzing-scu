## 去重工具
本工具用于crash去重。主要包含3个部分：1、从输入目录中逐个读取crash SQL，输入OpenGaussDB使其生成压缩的core文件。2、使用lz4解压压缩的core文件，调用gdb打开core文件，并用python提取堆栈信息，保存为文件。3、从文件中读取堆栈信息，根据堆栈信息判断crash十分重复，判断方式是比较crash出现的位置（文件名：行数）。该工具目前不支持对ASAN的去重。

**check_uniq_crash.cpp参数调整**  
数据库连接部分，该部分的设置与DBMS模糊测试工具相同  

	#define HOST "localhost"
	#define USERNAME "omm"
	#define PASSWORD "Aa1!@#$%^&*()"
	#define PORT 5432

	#define PIDFILEPATH "/home/omm/data/postmaster.pid"
	const char* file = "/home/wx/openGauss-server/dest/bin/gs_ctl";
	char* const options[] = { "/home/wx/openGauss-server/dest/bin/gs_ctl", "start", "-D", "/home/omm/data", "-Z", "single_node", "-l", "/home/omm/log/opengauss.log", NULL }; 

core文件路径，由于OpenGaussDB生成core时会自动生成文件名，为了确定文件名，需要确保core文件目录中没有其他文件，在生成core文件后目录中唯一文件是core文件。在处理完毕后会清空目录，确保目录下没有其他文件。  

	#define COREDIRPATH "/home/wx/core" //core文件目录
	#define COREFILEPATH COREDIRPATH "/core"  //core文件路径，lz4 -d 命令的输出参数，在解压时会将core文件名修改为默认名称

GDB指令，需要修改的部分是gaussdb的路径和test.gdb的路径  

	#define GDBCMD "gdb /home/wx/openGauss-server/dest/bin/gaussdb " COREFILEPATH " -x /home/wx/check_unique_bug/test.gdb &> /dev/null"

去重结果文件路径，可以将目前已发现的出现crash的位置记录下来，并在之后的去重中使用  

	#define UNIQUERESULTFILEPATH "/home/wx/check_unique_bug/unique_result"

**test.gdb参数调整**  
test.gdb是gdb命令脚本，该脚本的唯一作用是在gdb启动后加载test.py脚本，需要修改test.py的路径  

	source /home/wx/check_unique_bug/test.py

**test.py参数调整**  
该脚本会将gdb堆栈信息保存在文件中，该文件路径就是check_uniq_crash.cpp中的UNIQUERESULTFILEPATH

	with open("/home/wx/core/result_test", "w+") as fd:
		fd.write(BTInfo);

**编译命令**  

	g++ check_uniq_bug.cpp connector.cpp路径/connector.cpp -o check_uniq_bug -I openGauss-server安装路径/include -L openGauss-server安装路径/lib -lpq

**启动命令**  
输入目录是crash SQL所在的目录，输出目录只会保留unique crash

	check_uniq_bug 输入目录 输出目录