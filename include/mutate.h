#ifndef __MUTATOR_H__
#define __MUTATOR_H__


#include "ast.h"
#include "define.h"
#include "instantiate.h"

#include <set>
#include <map>
#include <vector>
#include <stdint.h>
#include <random>
#include <unordered_map>
#include <climits>
#include <chrono>

#define TOPNUMBER 10

#ifdef CLOSEMETHOD
#define IRTYPEUPDATETHRESHOLD UINT_MAX
#define IRINFOUPDATETHRESHOLD UINT_MAX
#else
#define IRTYPEUPDATETHRESHOLD 100
#define IRINFOUPDATETHRESHOLD 100
#endif

#define REWARDINCREMENT 10
#define REWARDNEWCOVERAGE 10

#define TIMETHRESHOLD (3600 * 1000)

#ifdef BALANCE
#define MUTATENUMBER 2 
#endif

using namespace std;

#define ERRORINDEX USHRT_MAX

enum class MutateOperator {
    moReplace,
    moInsert,
    moDelete
};

double get_random_double(int min, int max);

typedef struct {
    unsigned char stmt_;
    unsigned char inSubquery_;
    unsigned short int clause_;
    unsigned short int parentIRType_;
    unsigned short int parentIRItem_;
} IRContextS;

class IRInfomation {
public:
    IRInfomation(int prefix, int left, int middle, int right, int suffix);
    IRInfomation(IR* root);

    bool operator!=(IRInfomation* info);

    TERMINAL prefix_;
    IRTYPE left_;
    TERMINAL middle_;
    IRTYPE right_;
    TERMINAL suffix_;
};

class IRInfoManger {
public:
    IRInfoManger();
    ~IRInfoManger();

    IRTYPE curIRType_;

    vector<unsigned long int> IRInfoChosenCount_;

    void create_new_context(IRContextS* context);

    void update_count(IRContextS* context, unsigned short int item, bool hasNewCoverage, unsigned int reward);

    void add_IRInfomation(IRInfomation* info);
    IRInfomation* get_IRInfomation(unsigned short int index);
    vector<unsigned short int> get_IRInfo_index(IRContextS* context, double heat);

    unsigned short int get_index_of_IRInfo(IR* root);

    string print_IRInformation_status();

private:
    vector<IRInfomation*> IRInfomation_;
    unordered_map<uint64_t, vector<pair<unsigned int, unsigned int>>> contextDependentCount_;
};

class OperatorManger {
public:
    OperatorManger();
    ~OperatorManger();

    IRTYPE curIRType_;
    vector<unsigned long> OpInfoChosenCount_;
    vector<unsigned long> OpInfoFailedCount_;

    void create_new_context(IRContextS* context);
    vector<unsigned short int> get_OpInfo_index(IRContextS* context, double heat);
    MutateOperator get_operator(unsigned short int index);

    void update_count(IRContextS* context, unsigned short int item, bool hasNewCoverage, unsigned int reward);

    string print_OperatorInformation_status();

private:
    vector<MutateOperator> OperatorInfomation_;
    map<uint64_t, vector<pair<unsigned int, unsigned int>>> contextDependentCount_;
};

class InputInfo {
public:
    InputInfo();
    ~InputInfo();

    IRTYPE MutateIRType_;
    unsigned short int MutateIRInfoItem_;
    unsigned short int MutateOperatorItem_;

    IRContextS MutateContext_;
    IR* input_;
};

class Mutator {
public:
    Mutator();

    ~Mutator();

    unsigned int failedMutate_ = 0;
    unsigned int totalMutate_ = 0;

    IR* IR_parser(const char* sql);

    vector<InputInfo*> mutate_all(const char* sql);

    void update_status(InputInfo* input, unsigned int execStatus, bool isValid);

    bool init_IR_library(string filePath);
    bool init_IR_config_info(string filePath);

    void print_information(string outputPath);

private:
    static vector<IRTYPE> stmtIRType_;
    static vector<IRTYPE> clauseIRType_;

    chrono::time_point<chrono::steady_clock> startTime;

    unsigned int seedCount_ = 0;
    vector<unsigned short> mutateNumber_;

    map<IRTYPE, IRInfoManger*> IRInfo_;
    map<IRTYPE, OperatorManger*> OpInfo_;
    map<IRTYPE, pair<unsigned int, unsigned int>> IRTypeCount_;

    set<IRTYPE> not_mutatable_types_;
    set<uint64_t> sql_hash_;

    map<IRTYPE, vector<IR*>> IR_library_;
    map<IRTYPE, set<uint64_t>> IR_library_hash_;

    void free_IR_library();
    void free_IR_config_info();

    uint64_t hash(string& sql);
    vector<string> get_all_files_in_dir(const char* dir_name);

    ParseToplevel* parser(const char* sql);

    bool add_ir_to_library(IR* root);
    bool add_ir_to_library_no_deepcopy(IR* root);

    IR* get_ir_from_library(IRTYPE type);

    void extract_struct(IR* root);

    vector<InputInfo*> mutate(IR* root, IRContextS context, double heat);
    IR* get_new_tree(IR* oldIRRoot, IR* curIR, IR* newIR);

    unsigned int get_duration_ms();
    double get_cur_heat();
    unsigned int get_cur_reward();

    void print_IRType_information(string outputPath);
    void print_IRInfo_information(string outputPath);
    void print_Mutate_number(string outputPath);
    void print_OpInfo_information(string outputPath);

    vector<InputInfo*> original_mutate(IR* root, IRContextS context, double heat);

    IR* strategy_delete(IR* root);
    IR* strategy_insert(IR* root);
    IR* strategy_replace(IR* root);

    IR* check_sql(string& sql);
};

#endif
