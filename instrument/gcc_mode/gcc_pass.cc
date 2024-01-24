
#define AFL_GCC_PASS




#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// This is the first gcc header to be included
#include <gcc-plugin.h>
#include <plugin-version.h>
#include <toplev.h>
#include <tree-pass.h>
#include <context.h>
#include <function.h>
#include <tree.h>
#include <gimplify.h>
#include <basic-block.h>
#include <tree-ssa-alias.h>
#include <gimple-pretty-print.h>
#include <gimple-expr.h>
#include <gimple.h>
#include <gimple-iterator.h>
#include <stringpool.h>
#include <gimple-ssa.h>
#if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) >= \
    60200                                               /* >= version 6.2.0 */
  #include <tree-vrp.h>
#endif
#include <tree-ssanames.h>
#include <tree-phinodes.h>
#include <ssa-iterators.h>
#include <debug.h>

#include <intl.h>

#if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) >= \
    60200                                               /* >= version 6.2.0 */
  #include "memmodel.h"
#endif

#if BUILDING_GCC_VERSION >= 7000
#include "tree-vrp.h"
#endif

// We must assert that this plugin is GPL compatible
int plugin_is_GPL_compatible;

static struct plugin_info aflgo_gcc_plugin_info = { 
   
        "1.0", 
        "This is a very simple plugin" 
   
};

#define AFL_R(x) (random() % (x))
#define MAP_SIZE_POW2       16
#define MAP_SIZE            (1 << MAP_SIZE_POW2)

/* 全局变量 */
std::string TargetsFile;        // 目标点文件
std::string DistanceFile;       // 距离文件
std::string OutDirectory;       // 输出目录

static std::list<std::string> allowListFiles;
static std::list<std::string> allowListFunctions;
std::list<std::string> targets;
std::map<std::string, int> bb_to_dis;
std::vector<std::string> basic_blocks;

std::string dotfiles;

bool is_aflgo               = 0;
bool is_aflgo_preprocessing = 0;

unsigned int inst_ratio  = 100;
unsigned int dinst_ratio = 100;

/* 测试，完成后删除 */
int execute_time = 0;

#ifdef MESSAGES_TO_STDOUT
#  define SAYF(x...)    printf(x)
#else 
#  define SAYF(x...)    fprintf(stderr, x)
#endif /* ^MESSAGES_TO_STDOUT */

#  define cBLK "\x1b[0;30m"
#  define cRED "\x1b[0;31m"
#  define cGRN "\x1b[0;32m"
#  define cBRN "\x1b[0;33m"
#  define cBLU "\x1b[0;34m"
#  define cMGN "\x1b[0;35m"
#  define cCYA "\x1b[0;36m"
#  define cLGR "\x1b[0;37m"
#  define cGRA "\x1b[1;90m"
#  define cLRD "\x1b[1;91m"
#  define cLGN "\x1b[1;92m"
#  define cYEL "\x1b[1;93m"
#  define cLBL "\x1b[1;94m"
#  define cPIN "\x1b[1;95m"
#  define cLCY "\x1b[1;96m"
#  define cBRI "\x1b[1;97m"
#  define cRST "\x1b[0m"

#  define bgBLK "\x1b[40m"
#  define bgRED "\x1b[41m"
#  define bgGRN "\x1b[42m"
#  define bgBRN "\x1b[43m"
#  define bgBLU "\x1b[44m"
#  define bgMGN "\x1b[45m"
#  define bgCYA "\x1b[46m"
#  define bgLGR "\x1b[47m"
#  define bgGRA "\x1b[100m"
#  define bgLRD "\x1b[101m"
#  define bgLGN "\x1b[102m"
#  define bgYEL "\x1b[103m"
#  define bgLBL "\x1b[104m"
#  define bgPIN "\x1b[105m"
#  define bgLCY "\x1b[106m"
#  define bgBRI "\x1b[107m"

#define VERSION "1.00"

static bool isBlacklisted(const char* funName) {

    static const std::vector<std::string> Blacklist = {
    "asan.",
    "llvm.",
    "sancov.",
    "__ubsan_handle_",
    "free",
    "malloc",
    "calloc",
    "realloc"
  };

  for (std::string BlackListFunc : Blacklist) {

    if (memcmp(funName, BlackListFunc.c_str(), BlackListFunc.size()) == 0) 
        return true;

  }

  return false;

}

std::string get_function_full_name(function& fun) {
    std::string funcName = function_name(&fun);
    std::string funcSuffix(":");

    // 获取当前处理函数参数列表
    tree funcDecl = fun.decl;
    tree funcArgs = DECL_ARGUMENTS(funcDecl);

    for (tree funcArg = funcArgs; funcArg; funcArg = TREE_CHAIN(funcArg)) {
        
            tree arg_type = TREE_TYPE(funcArg);
            const char *arg_name_str = get_tree_code_name(TREE_CODE(arg_type));
            funcSuffix.append(arg_name_str);
            funcSuffix.append(":");

    }

    funcName.append(funcSuffix);

    return funcName;

}

/* 预处理pass */
namespace {

    /* pass元数据 */
    const pass_data preprocessing_pass_data = {

        GIMPLE_PASS,                    // type
        "aflgo_preprocessing_pass",     // name
        OPTGROUP_NONE,                  // optinfo_flags
        TV_NONE,                        // tv_id
        PROP_gimple_any,                // properties_required
        0,                              // properties_provided
        0,                              // properties_destroyed
        0,                              // todo_flags_start
        0,                              // todo_flags_finish

    };

    /* pass */
    struct preprocessing_pass : gimple_opt_pass {

        /* 构造函数 */
        preprocessing_pass(gcc::context* ctx) : gimple_opt_pass(preprocessing_pass_data, ctx) {

        }

        /* pass主体，该函数将被gcc调用 */
        virtual unsigned int execute(function* fun) override {            
            
            // 测试pass执行次数
            //std::cerr << "executing preprocessing pass, the " << execute_time << "th time， " << "target size : " << targets.size() << "\n";
            //execute_time++;
            
            /* 预处理阶段主要目标为生成一下文件，因此均为ofstream */
            std::ofstream bbnames(OutDirectory + "/BBnames.txt", std::ofstream::out | std::ofstream::app);        // 记录基本块名字的文件
            std::ofstream bbcalls(OutDirectory + "/BBcalls.txt", std::ofstream::out | std::ofstream::app);        // 记录callsite的文件，格式为[基本快，函数名]
            std::ofstream fnames(OutDirectory + "/Fnames.txt", std::ofstream::out | std::ofstream::app);          // 记录函数名的文件
            std::ofstream ftargets(OutDirectory + "/Ftargets.txt", std::ofstream::out | std::ofstream::app);      // 记录目标基本块所在函数名的文件

            /* 遍历基本块 */
            // 获取当前处理函数名称
            std::string funcName = get_function_full_name(*fun);


            // 跳过黑名单函数
            if (isBlacklisted(funcName.c_str())) 
                return 1;

            basic_block bb;
            bool has_BBs = false;
            bool is_target = false;
            
            
            std::string fjjname =function_name(fun);
	        for (auto & allowFunctions: allowListFunctions) {
                if(fjjname.length()==allowFunctions.length()){
                    if(!fjjname.compare(allowFunctions)){
                        is_target = true;
                        break;
                    }
                }
            }
	    
            FOR_ALL_BB_FN(bb, fun) {

                std::string bb_name("");
                std::string filename;
                unsigned line;

                /* 遍历gimple指令 */
                gimple_stmt_iterator gsi;

                for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {

                    // 注意，有可能多个gimle语句对应源码中的一行，

                    // 获取gimple
                    gimple* stmt = gsi_stmt(gsi);

                    // 获取gimple的位置信息
                    location_t l = gimple_location(stmt);
                    filename = LOCATION_FILE(l) ? LOCATION_FILE(l) : "";
                    line = LOCATION_LINE(l) ? LOCATION_LINE(l) : 0;

                    // 获取gimple语句类型
                    enum gimple_code stmt_code = gimple_code(stmt);

                    // 检查位置信息
                    /*
                    std::cerr << "[" << gimple_code_name[stmt_code]
                                  << "] " << filename << ":" << line << "\n"; 
                    */

                    // 跳过external libs和没有位置信息的指令
                    static const std::string Xlibs("/usr/");                          
                    if (filename.empty() || line == 0 || !filename.compare(0, Xlibs.size(), Xlibs))
                        continue;

                    // 当前语句为基本块第一条gimple语句时，以该语句位置作为基本块名
                    if (bb_name.empty()) {
                        
                        std::size_t found = filename.find_last_of("/\\");
                        if (found != std::string::npos)
                            filename = filename.substr(found + 1);
                        
                        bb_name = filename + ":" + std::to_string(line);

                    }

                    // 如果is_target还没有置为true，则比对target的位置信息和当前语句位置信息

		    if (!is_target) {
                        for (auto & allowFiles: allowListFiles) {
                            std::size_t found = allowFiles.find_last_of("/\\");
                            if (found != std::string::npos)
                                allowFiles = allowFiles.substr(found + 1);
                            if(filename.length()==allowFiles.length()){
                                if(!filename.compare(allowFiles)){
                                    is_target = true;
                                }
                            }
                        }
                    }

                    if (!is_target) {

                        

                        for (auto& target : targets) {

                            std::size_t found = target.find_last_of("/\\");
                            if (found != std::string::npos)
                                target = target.substr(found + 1);

                            std::size_t pos = target.find_last_of(":");
                            std::string target_file = target.substr(0, pos);
                            unsigned int target_line = atoi(target.substr(pos + 1).c_str());

                            if (!target_file.compare(filename) && target_line == line) 
                                is_target = true;

                        }

                    }

                
                    // 查看gimple
                    /*
                    FILE* fp = fopen("./fuz.txt", "a");
                    print_gimple_stmt(fp, stmt, 0);
                    fclose(fp);*/

                    // 如果遇到call语句，则记录[当前基本块名，被调函数名]
                    if (stmt_code == GIMPLE_CALL) {
                            
                        std::size_t found = filename.find_last_of("/\\");
                        if (found != std::string::npos)
                            filename = filename.substr(found + 1);

                        // 获取函数名
                        std::string called_funName;
                        tree current_fn_decl = gimple_call_fndecl(stmt);

                        if (current_fn_decl) {

                            std::string funNameA(get_name(current_fn_decl));
                            std::string funNameB(function_name(DECL_STRUCT_FUNCTION(current_fn_decl)));
                    
                            if (funNameB != "(nofn)")
                                called_funName.append(funNameB);
                            else
                                called_funName.append(funNameA);

                            // 获取函数后缀，由所在文件名、函数参数列表组成，用于区分函数重载
                            std::string funName_suffix("");
                            funName_suffix.append(get_called_arg_types(*stmt));


                            if (!isBlacklisted(called_funName.c_str()))
                                bbcalls << bb_name << "," << called_funName  << funName_suffix << "\n";

                        }

                        

                    }
                    
                }  // 遍历gimple语句

                if (!bb_name.empty()) {

                    // 待解决：基本块重名问题
                    // 有一些基本块在源码中位置在同一行，因此会导致重名

                    bbnames << bb_name << "\n";
                    has_BBs = true;

                }
                
            } // 遍历基本块
        
            // 如果当前函数有基本块，则将CFG写入文件
            if (has_BBs) {

                std::string cfgFileName = dotfiles + "/cfg." + funcName + ".dot";

                write_cfgraph(cfgFileName, fun);
                if (is_target)
                    ftargets << funcName << "\n";

                fnames << funcName << "\n";

            }

            // 生成CG

            std::string cgFileName = dotfiles + "/callgraph_log.txt";
            
            write_cgraph_info(cgFileName, fun);
            

            return 0;

        }

        /* 不复制自身 */
        virtual preprocessing_pass* clone() override {

            return this;

        }

    private:
        /* 生成Graphviz风格的函数的控制流图 */
        void write_cfgraph(const std::string& graph_file, function* fun) {
            
            if (fileExists(graph_file)) 
                return;

            std::ofstream dotfile(graph_file, std::ofstream::out);

            // 绘制头部
            dotfile << "digraph \"CFG for '" << function_name(fun) <<"' function\" {" << "\n"; 
            dotfile << "\tlabel=\"CFG for '" << function_name(fun) << "' function\";\n\n";

            // 绘制节点与边
            basic_block bb;

            FOR_ALL_BB_FN(bb, fun) {

                gimple_bb_info* bb_info = &bb->il.gimple;
                
                gimple_stmt_iterator gsi;

                std::string bb_name("");

                for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {

                    // 获取gimple
                    gimple* stmt = gsi_stmt(gsi);

                    std::string filename;
                    unsigned line;

                    // 获取gimple的位置信息
                    location_t l = gimple_location(stmt);
                    filename = LOCATION_FILE(l) ? LOCATION_FILE(l) : "";
                    line = LOCATION_LINE(l) ? LOCATION_LINE(l) : 0;

                    // 当前语句为基本块第一条gimple语句时，以该语句位置作为基本块名
                    if (bb_name.empty()) {
                        
                        std::size_t found = filename.find_last_of("/\\");
                        if (found != std::string::npos)
                            filename = filename.substr(found + 1);
                        
                        bb_name = filename + ":" + std::to_string(line);

                        break;

                    }

                }
                
                //dotfile << "\tNode" << fun << "_" << bb->index << "[shape=record,label=\"{" << bb_name <<":}\"];" << "\n";

                dotfile << "\tNode" << fun << "_" << bb->index << "[shape=record,label=\"{";
                if (bb->index == 0) {

                    dotfile << (LOCATION_FILE(fun->function_start_locus) ? : "<unknown>")
                            << ":" << LOCATION_LINE(fun->function_start_locus);

                }
                else if (bb->index == 1) {

                    dotfile << (LOCATION_FILE(fun->function_end_locus) ? : "<unknown>")
                            << ":" << LOCATION_LINE(fun->function_end_locus);

                }
                else {

                    dotfile << bb_name;

                }
                dotfile << ":}\"];" << "\n";

                edge e;
                edge_iterator ei;

                FOR_EACH_EDGE(e, ei, bb->succs) {

                    basic_block dest = e->dest;
                    dotfile << "\t"
                            << "Node" << fun << "_" 
                            << bb->index
                            << " -> Node" << fun << "_"
                            << dest->index << ";\n";

                }

            }

            // 绘制尾部
            dotfile << "}\n";


            dotfile.close();

        }

        /* 保存函数调用信息 */
        void write_cgraph_info(const std::string& graph_file, function* fun) {

            std::ofstream dotfile(graph_file, std::ofstream::app);
            
            // 记录当前函数的信息
            std::string funcName = get_function_full_name(*fun);
            dotfile << "NEWFUN Node" << '{' << funcName << '}' << '\n';

            // 遍历函数内部基本块及指令，记录GIMPLE_CALL语句
            basic_block bb;

            FOR_ALL_BB_FN(bb, fun) {

                gimple_stmt_iterator gsi;

                for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {

                    gimple* stmt = gsi_stmt(gsi);

                    // 获取gimple的位置信息
                    location_t l = gimple_location(stmt);
                    std::string filename = LOCATION_FILE(l) ? LOCATION_FILE(l) : "";
                    std::uint32_t line = LOCATION_LINE(l) ? LOCATION_LINE(l) : 0;

                    // 获取gimple语句类型
                    enum gimple_code stmt_code = gimple_code(stmt);

                    // 如果是GIMPLE_CALL语句
                    if (stmt_code == GIMPLE_CALL) {

                        std::string called_funName;

                        tree current_fn_decl = gimple_call_fndecl(stmt);

                        if (current_fn_decl) {

                            std::string  funNameA(get_name(current_fn_decl));
                            std::string funNameB(function_name(DECL_STRUCT_FUNCTION(current_fn_decl)));
                    

                            if (funNameB != "(nofn)")
                                called_funName.append(funNameB);
                            else
                                called_funName.append(funNameA);

                            // 获取函数后缀，由所在文件名、函数参数列表组成，用于区分函数重载
                            std::string funName_suffix("");
                            funName_suffix.append(get_called_arg_types(*stmt));

                            dotfile << funcName << " -> " << called_funName  << funName_suffix << '\n';

                        }
                    

                    }

                }

            }

            dotfile.close();

        }

        std::string get_called_arg_types(const gimple& stmt) {

            enum gimple_code stmt_code = gimple_code(&stmt);

            std::string suffix(":");

            if (stmt_code != GIMPLE_CALL)
                return suffix;

            unsigned nargs = gimple_call_num_args(&stmt);

            for (unsigned i = 0; i < nargs; i++) {

                tree arg = gimple_call_arg(&stmt, i);
                tree arg_type = TREE_TYPE(arg);

                const char *arg_name = get_tree_code_name(TREE_CODE(arg_type));

                suffix.append(arg_name);
                suffix.append(":");

            }

            return suffix;
            
        }

        /* 检查文件是否存在 */
        bool fileExists(const std::string& filename) {

            std::ifstream file(filename);
            return file.good();

        }

        std::string my_get_function_name(gimple* stmt) {

            

        }

    }; // struct preprocessing_pass

} // anymous namespace

/* 插桩pass */
namespace {

    const pass_data instrument_pass_data = {

        GIMPLE_PASS,        // type
        "aflgo_instrument_pass",    // name
        OPTGROUP_NONE,      // optinfo_flags
        TV_NONE,            // tv_id
        PROP_gimple_any,    // properties_required
        0,                  // properties_provided
        0,                  // properties_destroyed
        0,                  // todo_flags_start
        0,                  // todo_flags_finish

    };

    struct instrument_pass : gimple_opt_pass {

        instrument_pass(gcc::context* ctx) 
            : gimple_opt_pass(instrument_pass_data, ctx),
              inst_blocks(0) {

        }

        unsigned int inst_blocks;

        virtual unsigned int execute(function* fun) override {
            std::cerr << "execute " << execute_time << " in instrument pass， " << "\n";
            execute_time++;

            // 根据机器位数确定存储距离的共享内存偏移位置
#ifdef __x86_64__
            int distance_unit_width = 8;
#else    
            int distance_unit_width = 4;
#endif  

            // 获取当前处理函数名称
            std::string funcName = get_function_full_name(*fun);
            
            //std::cerr << "funcName: " << funcName << "\n";
            /*if (!ifInstrument(funcName)){
                std::cerr << "return\n";
                 return 1;

            }
            std::cerr << "not exit" << std::endl;*/

            // 跳过黑名单函数
            if (isBlacklisted(funcName.c_str())) 
                return 1;

            /* 遍历基本块，进行插桩 */

            int distance = -1;

            basic_block bb;

            int blocks = 0;

            /* These are temporaries used by inline instrumentation only, that
            are live throughout the function.  */
            tree ploc = NULL, indx = NULL, map = NULL, map_ptr = NULL, ntry = NULL,
                cntr = NULL, xaddc = NULL, xincr = NULL, dists_idx = NULL, dists_mem = NULL;


            FOR_EACH_BB_FN(bb, fun) {
                
                distance = -1;

                if (is_aflgo) {
                    
                    /* 插桩前，需要找到当前基本块对应的距离 */

                    std::string bb_name("");

                    // 获取基本块名称
                    gimple_stmt_iterator gsi;

                    for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
                        
                        std::string filename;
                        unsigned line;

                        // 获取gimple
                        gimple* stmt = gsi_stmt(gsi);

                        // 获取gimple的位置信息
                        location_t l = gimple_location(stmt);
                        filename = LOCATION_FILE(l) ? LOCATION_FILE(l) : "";
                        line = LOCATION_LINE(l) ? LOCATION_LINE(l) : 0;

                        if (filename.empty() || line == 0) 
                            continue;
                        
                        std::size_t found = filename.find_last_of("/\\");
                        if (found != std::string::npos)
                            filename = filename.substr(found + 1);

                        bb_name = filename + ":" + std::to_string(line); 

                        break;
            
                    } // end : 遍历指令

                    // 找到当前基本块的距离
                    if (!bb_name.empty()) {

                        if (find(basic_blocks.begin(), basic_blocks.end(), bb_name) == basic_blocks.end()) {

                            //if (is_selective) 
                                continue;

                        }
                        else {
                            
                            // 概率插桩，须补齐条件AFL_R(100) < dinst_ratio
                            if (AFL_R(100) < dinst_ratio) {

                                std::map<std::string, int>::iterator it;

                                for (it = bb_to_dis.begin(); it != bb_to_dis.end(); ++it) {

                                    if (it->first.compare(bb_name) == 0) {

                                        distance = it->second;

                                    }

                                }
                            }
                        }   
                    } // end : 查找基本块距离

                } // end : is_aflgo

                /* 开始给当前基本块插桩 */
                //if (AFL_R(100) >= inst_ratio) continue;
                
                /* Make up cur_loc */
                // 随机出当前基本块的编号cur_loc，并创建语法树节点
                unsigned bid  = AFL_R(MAP_SIZE);                                                        // 随机出当前基本块的编号cur_loc
                tree     bidt = build_int_cst(sizetype, bid);

                gimple_seq seq = NULL;

                static tree afl_prev_loc = get_afl_prev_loc_decl();                                     // __afl_prev_loc
                static tree afl_area_ptr = get_afl_area_ptr_decl();                                     // __afl_area_ptr
                
                //std::cout << "tree: ";
                //print_generic_expr(stdout, afl_prev_loc, TDF_SLIM);
                
                /* Load __afl_prev_loc to a temporary ploc.  */                       
                //std::cout << blocks << std::endl;
            if (blocks == 0)                                                                            // 遍历第一个基本块时创建临时变量，后续基本块直接使用（后同）
                    ploc = create_tmp_var(TREE_TYPE(afl_prev_loc), ".afl_prev_loc");                    // .afl_prev_loc = __afl_prev_loc;
                auto load_loc = gimple_build_assign(ploc, afl_prev_loc);
                gimple_seq_add_stmt(&seq, load_loc);

                /* Compute the index into the map referenced by area_ptr
                that we're to update: indx = (sizetype) ploc ^ bid.  */
                if (blocks == 0) indx = create_tmp_var(TREE_TYPE(bidt), ".afl_index");
                auto conv_ploc =                                                                        // .afl_index = (sizetype).afl_prev_loc
                    gimple_build_assign(indx, fold_convert(TREE_TYPE(indx), ploc));
                gimple_seq_add_stmt(&seq, conv_ploc);
                auto xor_loc = gimple_build_assign(indx, BIT_XOR_EXPR, indx, bidt);                     // .afl_index = .afl_index ^ bid
                gimple_seq_add_stmt(&seq, xor_loc);

                /* Compute the address of that map element.  */
                if (blocks == 0) {

                    map = afl_area_ptr;
                    map_ptr = create_tmp_var(TREE_TYPE(afl_area_ptr), ".afl_map_ptr");
                    ntry = create_tmp_var(TREE_TYPE(afl_area_ptr), ".afl_map_entry");

                }

                /* .map_ptr is initialized at the function entry point, if we
                instrument any blocks, see below.  */

                /* .entry = &map_ptr[.index]; */
                auto idx_map =
                    gimple_build_assign(ntry, POINTER_PLUS_EXPR, map_ptr, indx);                        // .afl_map_entry = .afl_map_ptr + .afl_index
                gimple_seq_add_stmt(&seq, idx_map);

                /* Prepare to add constant 1 to it.  */
                tree incrv = build_one_cst(TREE_TYPE(TREE_TYPE(ntry)));


                /* Use a serialized memory model.  */
                tree memmod = build_int_cst(integer_type_node, MEMMODEL_SEQ_CST);

                tree fadd = builtin_decl_explicit(BUILT_IN_ATOMIC_FETCH_ADD_1);
                auto incr_cntr = gimple_build_call(fadd, 3, ntry, incrv, memmod);                       // .afl_map_entry = .afl_map_entry + 1
                gimple_seq_add_stmt(&seq, incr_cntr);


                /* Store bid >> 1 in __afl_prev_loc.  */    
                auto shift_loc =                                                                        // .afl_prev_loc = bid >> 1
                    gimple_build_assign(ploc, build_int_cst(TREE_TYPE(ploc), bid >> 1));            
                gimple_seq_add_stmt(&seq, shift_loc);
                auto store_loc = gimple_build_assign(afl_prev_loc, ploc);                               // __afl_prev_loc = .afl_prev_loc
                gimple_seq_add_stmt(&seq, store_loc);

                /* 完成AFL插桩 */

                /* AFLGO插桩 */
                if (blocks == 0) {
                    dists_idx = create_tmp_var(sizetype, ".aflgo_distance_index");
                    dists_mem = create_tmp_var(sizetype, ".aflgo_distance_mem_unit");
                }

                if (distance >= 0) {
                    
                    //printf("distance : %d\n", distance);        
                    
                    /* Add distance to shm[MAP_SIZE] */

                    tree distance_index = build_int_cst(TREE_TYPE(dists_idx), MAP_SIZE);                                    // .aflgo_distance_index = MAP_SIZE
                    auto init_distance_index = gimple_build_assign(dists_idx, distance_index);
                    gimple_seq_add_stmt(&seq, init_distance_index);
                    
                    auto tracebits_distance_stmt = gimple_build_assign(ntry, POINTER_PLUS_EXPR, map_ptr, dists_idx);        // .aflgo_distance_entry = .afl_map_ptr + .aflgo_distance_index
                    gimple_seq_add_stmt(&seq, tracebits_distance_stmt);
                    
                    tree memref = build2(MEM_REF, TREE_TYPE(TREE_TYPE(ntry)), ntry, build_zero_cst(TREE_TYPE(ntry)));       // .aflgo_distance_value = *.aflgo_distance_entry
                    auto load_dis_mem_stmt = gimple_build_assign(dists_mem, memref);
                    gimple_seq_add_stmt(&seq, load_dis_mem_stmt);
                    
                    tree distance_value = build_int_cst(TREE_TYPE(dists_mem), distance);                                    // .aflgo_distance_value = .aflgo_distance_value + distance
                    auto add_distance_stmt = gimple_build_assign(dists_mem, PLUS_EXPR, dists_mem, distance_value);
                    gimple_seq_add_stmt(&seq, add_distance_stmt);
                    
                    auto store_dis_mem_stmt = gimple_build_assign(unshare_expr(memref), dists_mem);                         // *.aflgo_distance_entry = .aflgo_distance_value
                    gimple_seq_add_stmt(&seq, store_dis_mem_stmt);


                    /* Increase count at shm[MAP_SIZE + (4 or 8)] */

                    // .aflgo_distance_index = MAP_SIZE + 4 or 8
                    tree cnt_index = build_int_cst(TREE_TYPE(dists_idx), MAP_SIZE + distance_unit_width);
                    auto init_cnt_index = gimple_build_assign(dists_idx, cnt_index);
                    gimple_seq_add_stmt(&seq, init_cnt_index);

                    // .aflgo_distance_entry = .afl_map_ptr + .aflgo_distance_index
                    auto tracebits_cnt_stmt = gimple_build_assign(ntry, POINTER_PLUS_EXPR, map_ptr, dists_idx);
                    gimple_seq_add_stmt(&seq, tracebits_cnt_stmt);

                    // .aflgo_distance_value = *.aflgo_distance_entry
                    tree cnt_memref = build2(MEM_REF, TREE_TYPE(TREE_TYPE(ntry)), ntry, build_zero_cst(TREE_TYPE(ntry)));
                    auto load_cnt_mem_stmt = gimple_build_assign(dists_mem, cnt_memref);
                    gimple_seq_add_stmt(&seq, load_cnt_mem_stmt);

                    // .aflgo_distance_value = .aflgo_distance_value + 1
                    tree cnt_value = build_int_cst(TREE_TYPE(dists_mem), 1);
                    auto add_cnt_stmt = gimple_build_assign(dists_mem, PLUS_EXPR, dists_mem, cnt_value);
                    gimple_seq_add_stmt(&seq, add_cnt_stmt);

                    // *.aflgo_distance_entry = .aflgo_distance_value
                    auto store_cnt_mem_stmt = gimple_build_assign(unshare_expr(cnt_memref), dists_mem);
                    gimple_seq_add_stmt(&seq, store_cnt_mem_stmt);
                }

                /* Insert the generated sequence.  */
                gimple_stmt_iterator insp = gsi_after_labels(bb);
                gsi_insert_seq_before(&insp, seq, GSI_SAME_STMT);
                
                /*
                printf("Basic Block %d:\n", bb->index);

                gimple_stmt_iterator gsi;
                for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi))
                {
                    gimple* stmt = gsi_stmt(gsi);
                    debug_gimple_stmt(stmt); // 打印Gimple语句
                }
                printf("\n");
                */

                /* 基本块计数加1 */
                blocks++;

            } // end : 遍历基本块

            inst_blocks += blocks;

            if (blocks) {


                gimple_seq seq = NULL;

                /* Load afl_area_ptr into map_ptr.  We want to do this only
                    once per function.  */
                auto load_ptr = gimple_build_assign(map_ptr, map);
                gimple_seq_add_stmt(&seq, load_ptr);

                /* Insert it in the edge to the entry block.  We don't want to
                    insert it in the first block, since there might be a loop
                    or a goto back to it.  Insert in the edge, which may create
                    another block.  */
                edge e = single_succ_edge(ENTRY_BLOCK_PTR_FOR_FN(fun));
                gsi_insert_seq_on_edge_immediate(e, seq);

            }

            

            return 0;

        }

        static inline tree get_afl_prev_loc_decl() {

            tree decl = build_decl(BUILTINS_LOCATION, VAR_DECL,
                                get_identifier("__afl_prev_loc"), uint32_type_node);
            TREE_PUBLIC(decl) = 1;
            DECL_EXTERNAL(decl) = 1;
            DECL_ARTIFICIAL(decl) = 1;
            TREE_STATIC(decl) = 1;
        #if !defined(__ANDROID__) && !defined(__HAIKU__)
            set_decl_tls_model(
                decl, (flag_pic ? TLS_MODEL_INITIAL_EXEC : TLS_MODEL_LOCAL_EXEC));
        #endif
            return decl;

        }

        /* Create and return a declaration for the __afl_prev_loc
            thread-local variable.  */
        static inline tree get_afl_area_ptr_decl() {

            /* If type changes, the size N in FETCH_ADD_<N> must be adjusted
            in builtin calls above.  */
            tree type = build_pointer_type(unsigned_char_type_node);
            tree decl = build_decl(BUILTINS_LOCATION, VAR_DECL,
                                get_identifier("__afl_area_ptr"), type);
            TREE_PUBLIC(decl) = 1;
            DECL_EXTERNAL(decl) = 1;
            DECL_ARTIFICIAL(decl) = 1;
            TREE_STATIC(decl) = 1;

            return decl;

        }

        static inline bool ifInstrument(std::string funcName) {

            std::string function_list_file = DistanceFile;
            
            std::size_t pos = function_list_file.find_last_of("/");
            function_list_file = function_list_file.substr(0, pos + 1) + "Fnames.txt";


            std::ifstream cf(function_list_file);

            if (cf.is_open()) {

                std::string line;

                while (getline(cf, line)) {
                    if (line == funcName) {

                        cf.close();
                        return true;

                    }

                }

                cf.close();
                return false;


            } 
            else {

                std::cerr << "Unable to find " << function_list_file << std::endl;
                return false;

            }


            return false;
        }

        virtual instrument_pass* clone() override {

            return this;

        }

        static void plugin_finalize(void *, void *p) {

            opt_pass *op = (opt_pass *)p;
            instrument_pass &self = (instrument_pass &)*op;

            SAYF(cLGN "[+] " cRST "Instrumented %u locations.\n", self.inst_blocks);

        }


    }; // struct instrument_pass

} // anymous namespace


int plugin_init (struct plugin_name_args *plugin_info, struct plugin_gcc_version *version) {

	if (!plugin_default_version_check (version, &gcc_version))
    {
        std::cerr << "This GCC plugin is for version " << GCCPLUGIN_VERSION_MAJOR << "." << GCCPLUGIN_VERSION_MINOR << "\n";
		return 1;
    }

    register_callback(plugin_info->base_name,       // 
                      PLUGIN_INFO,                  // 事件
                      NULL,                         // 回调函数
                      &aflgo_gcc_plugin_info);      // user_data

    /* 获取命令行参数 */
    for (int i = 0; i < plugin_info->argc; i++) {

        //std::cout << plugin_info->argv[i].key << " : " << plugin_info->argv[i].value << std::endl;
        
        // 获取targets
        if (strcmp(plugin_info->argv[i].key, "targets") == 0) 
            TargetsFile = plugin_info->argv[i].value;

        // 获取outdir
        if (strcmp(plugin_info->argv[i].key, "outdir") == 0) 
            OutDirectory = plugin_info->argv[i].value;

        // 获取distance
        if (strcmp(plugin_info->argv[i].key, "distance") == 0) 
            DistanceFile = plugin_info->argv[i].value;
        
    }

    // 目标点文件和距离文件不能同时存在
    if (!TargetsFile.empty() && !DistanceFile.empty()) {
        std::cerr << "Cannot specify both '-targets' and '-distance'!" << "\n";
        return 1;
    }

    /* 根据是否通过命令行传入了参数targets来判断是第几次编译 */
    if (!TargetsFile.empty()) {
        
        // 第一次编译
        
        if (OutDirectory.empty()) {

            std::cerr << "Provide output directory '-outdir <directory>'" << "\n";
            return 1;

        }

        // 读取目标点
        std::ifstream targetsfile(TargetsFile);
        std::string line;

        while (std::getline(targetsfile, line)) {

            if (line.compare(0, 4, "fun:") == 0) {
                line = line.substr(4);
                if (line.length() > 0) {
                    allowListFunctions.push_back(line);
                }
            }
            else if (line.compare(0, 4, "src:") == 0){
                line = line.substr(4);
                if (line.length() > 0) {
                    allowListFiles.push_back(line);
                }
            }
            else if (line.compare(0, 4, "line:") == 0){
                line = line.substr(5);
                if (line.length() > 0) {
                    targets.push_back(line);
                }
            }
            else{
                if (line.find(":") != std::string::npos) {
                    if (line.length() > 0) {
                        targets.push_back(line);
                    }
                }
                else if(line.length() > 0) {
                    allowListFiles.push_back(line);
                }
            }

        }

        targetsfile.close();

        // 将预处理标志位置为1
        is_aflgo_preprocessing = 1;

    }
    else if (!DistanceFile.empty()) {

        // 第二次编译
        
        // 读取CFG距离，将每个基本块及其对应的距离保存在全局变量bb_to_dis中
        // 同时将所有基本块的名字保存在全局变量basic_blocks中
        std::ifstream cf(DistanceFile);

        if (cf.is_open()) {

            std::string line;

            while (getline(cf, line)) {

                std::size_t pos = line.find(",");
                std::string bb_name = line.substr(0, pos);
                int bb_dis = (int) (100.0 * atof(line.substr(pos + 1, line.length()).c_str()));

                bb_to_dis.emplace(bb_name, bb_dis);
                basic_blocks.push_back(bb_name);

            }

            cf.close();

            // 将is_aflgo置为1
            is_aflgo = true;

        } 
        else {

            std::cerr << "Unable to find " << DistanceFile << std::endl;
            return false;

        }

        

    }

    /* show a banner */
    if (is_aflgo || is_aflgo_preprocessing)
      SAYF(cCYA "abgo-gcc-pass (yeah!) " cBRI VERSION cRST " (%s mode)\n",
           (is_aflgo_preprocessing ? "preprocessing" : "distance instrumentation"));
    else
      SAYF(cCYA "abgo-gcc-pass " cBRI VERSION cRST " by <lszekeres@google.com>\n");

    /* 确定插桩概率，待完善 */
    char* inst_ratio_str = getenv("AFL_INST_RATIO");
    unsigned int inst_ratio = 100;

    // ...

    /* pass work */
    /* 对aflgo进行优化，无需每次加载插件重新读取文件 */
    /* 预处理阶段 */
    if (is_aflgo_preprocessing) {

        /* 创建用于存放dot文件的文件夹 */
        dotfiles = OutDirectory + "/dot-files";
        if (mkdir(dotfiles.c_str(), 0777) == 0) {
            std::cerr << "Could not create directory " << dotfiles.c_str() << "\n";
        }

        /* 注册预处理pass */

        struct register_pass_info preprocessing_pass_info;

        preprocessing_pass_info.pass = new preprocessing_pass(g);
        preprocessing_pass_info.reference_pass_name = "cfg";
        preprocessing_pass_info.ref_pass_instance_number = 1;
        preprocessing_pass_info.pos_op = PASS_POS_INSERT_AFTER;

        register_callback (plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &preprocessing_pass_info);    

    }
    /* 插桩阶段 */
    else {

        /* 注册插桩pass */

        instrument_pass* instru_pass = new instrument_pass(g);

        struct register_pass_info instrument_pass_info;
        
        instrument_pass_info.pass = instru_pass;
        instrument_pass_info.reference_pass_name = "cfg";
        instrument_pass_info.ref_pass_instance_number = 1;
        instrument_pass_info.pos_op = PASS_POS_INSERT_AFTER;

        register_callback (plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &instrument_pass_info);   
        //register_callback(plugin_info->base_name, PLUGIN_FINISH, instrument_pass::plugin_finalize, instrument_pass_info.pass); 

    }






    return 0;
}
