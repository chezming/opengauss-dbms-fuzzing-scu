#include "../include/mutate.h"
#include "../include/ast.h"
#include "../include/define.h"

#include "../parser/bison_parser.h"
#include "../parser/flex_lexer.h"

#include <iostream>
#include <assert.h>
#include <fstream>
#include <cstdio>
#include <climits>
#include <cfloat>
#include <algorithm>
#include <deque>

#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

double get_random_double(int min, int max) {
	random_device rd;
	mt19937 r_eng(rd());
	uniform_real_distribution<double> dis(min, max);

	return dis(r_eng);
}

//-----------------------------Mutator-----------------------------

vector<IRTYPE> Mutator::stmtIRType_ = {
	IRTYPE::kCreateTableStmt,
	IRTYPE::kCreateViewStmt,
	IRTYPE::kCreateIndexStmt,
	IRTYPE::kSelectStmt,
	IRTYPE::kDropIndexStmt,
	IRTYPE::kDropTableStmt,
	IRTYPE::kDropViewStmt,
	IRTYPE::kDeleteStmt,
	IRTYPE::kUpdateStmt,
	IRTYPE::kInsertStmt,
	IRTYPE::kAlterTableStmt
};
vector<IRTYPE> Mutator::clauseIRType_ = {
	IRTYPE::kAlterListItem,
	IRTYPE::kValues,
	IRTYPE::kUpdateElem,
	IRTYPE::kDefaultAttribute,
	IRTYPE::kCheckConstraint,
	IRTYPE::kKeyPartWithExpression,
	IRTYPE::kOptWindowClause,
	IRTYPE::kOptHavingClause,
	IRTYPE::kWhereClause,
	IRTYPE::kFromClause,
	IRTYPE::kSelectItem
};

Mutator::Mutator() {
	random_device rd;
	srand(rd());

	startTime = chrono::steady_clock::now();

	not_mutatable_types_.insert({ IRTYPE::kParseToplevel, IRTYPE::kStmtmulti, IRTYPE::kStmt });

	return;
}
Mutator::~Mutator() {
	free_IR_library();
	free_IR_config_info();

	return;
}

bool Mutator::init_IR_library(string dirPath) {
	vector<string> IRFileNames = get_all_files_in_dir(dirPath.c_str());
	if (IRFileNames.empty()) {
		fprintf(stderr, "ERROR: No IR library File in dirPath!!!\n");
		return false;
	}		

	for (string fileName : IRFileNames) {
		ifstream inputFile(fileName);
		string sql;

		while (getline(inputFile, sql)) {
			if (sql.empty())
				continue;

			IR* p = IR_parser(sql.c_str());
			if (p == nullptr)
				continue;

			add_ir_to_library(p);
			deep_delete(p);
		}
	}

	return true;
}

void Mutator::free_IR_library() {
	for (IR* p : IR_library_[IRTYPE::kParseToplevel]) {
		if (p)
			deep_delete(p);
	}

	return;
}

bool Mutator::init_IR_config_info(string filePath) {
	char flag = 0;
	IRTYPE type = IRTYPE::kNone;
	int count = 0;

	FILE* fd = fopen(filePath.c_str(), "r");
	if (!fd){
		fprintf(stderr, "IR_config file is not exists!!\n");
		return false;
	}

	while (!feof(fd)) {
		if (!flag) {
			int tmpType = 0;
			fscanf(fd, "%d %d\n", &tmpType, &count);

			type = static_cast<IRTYPE>(tmpType);

			if (IRInfo_.find(static_cast<IRTYPE>(type)) != IRInfo_.end()) {
				fprintf(stderr, "ERROR: IR_config file has duplication type: %d!!\n", type);
				return false;
			}

			IRInfo_[type] = new IRInfoManger();
			IRInfo_[type]->curIRType_ = type;

			OpInfo_[type] = new OperatorManger();
			OpInfo_[type]->curIRType_ = type;

			IRTypeCount_[type] = { 0, 0 };

			flag = 1;
		}
		else {
			int prefix = 0;
			int left = 0;
			int middle = 0;
			int right = 0;
			int suffix = 0;

			for (int i = 0; i < count; i++) {
				fscanf(fd, "%d %d %d %d %d\n", &prefix, &left, &middle, &right, &suffix);

				IRInfomation* tmp = new IRInfomation(prefix, left, middle, right, suffix);
				IRInfo_[type]->add_IRInfomation(tmp);
			}

			flag = 0;
			type = IRTYPE::kNone;
			count = 0;
		}
	}

	fclose(fd);

	return true;
}

void Mutator::free_IR_config_info() {
	for (auto& it : IRInfo_) {
		delete it.second;
	}

	for (auto& it : OpInfo_) {
		delete it.second;
	}

	return;
}

uint64_t Mutator::hash(string& sql) {
	const void* key = static_cast<const void*>(sql.c_str());
	int len = sql.size();

	const uint64_t m = 0xc6a4a7935bd1e995;
	const int r = 47;
	uint64_t h = 0xdeadbeefdeadbeef ^ (len * m);

	const uint64_t* data = static_cast<const uint64_t*>(key);
	const uint64_t* end = data + (len / 8);

	while (data != end)
	{
		uint64_t k = *data++;

		k *= m;
		k ^= k >> r;
		k *= m;

		h ^= k;
		h *= m;
	}

	const unsigned char* data2 = (const unsigned char*)data;

	switch (len & 7)
	{
	case 7: h ^= uint64_t(data2[6]) << 48;
	case 6: h ^= uint64_t(data2[5]) << 40;
	case 5: h ^= uint64_t(data2[4]) << 32;
	case 4: h ^= uint64_t(data2[3]) << 24;
	case 3: h ^= uint64_t(data2[2]) << 16;
	case 2: h ^= uint64_t(data2[1]) << 8;
	case 1: h ^= uint64_t(data2[0]);
		h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}

vector<string> Mutator::get_all_files_in_dir(const char* dirPath) {
	vector<string> res;

	fs::path dir = dirPath;
	if (!fs::exists(dir)) {
		return res;
	}

	for (const fs::directory_entry& entry : fs::directory_iterator(dir)) {
		if (entry.is_regular_file()) {
			res.push_back(entry.path().string());
		}
	}

	return res;
}

ParseToplevel* Mutator::parser(const char* sql) {

	yyscan_t scanner;
	YY_BUFFER_STATE state;
	ParseToplevel* p = new ParseToplevel();

	if (ff_lex_init(&scanner)) {
		return NULL;
	}

	state = ff__scan_string(sql, scanner);

	int ret = ff_parse(p, scanner);

	ff__delete_buffer(state, scanner);
	ff_lex_destroy(scanner);

	if (ret != 0) {
		p->deep_delete();
		return NULL;
	}

	return p;
}

IR* Mutator::IR_parser(const char* sql) {
	ParseToplevel* p = parser(sql);
	if (p == nullptr)
		return nullptr;

	IR* res = p->translate();
	p->deep_delete();

	return res;
}

bool Mutator::add_ir_to_library(IR* root) {
	extract_struct(root);
	IR* newRoot = deep_copy(root);

	if (!add_ir_to_library_no_deepcopy(newRoot)) {
		deep_delete(newRoot);
		return false;
	}

	return true;
}

bool Mutator::add_ir_to_library_no_deepcopy(IR* root) {
	bool flag = false;

	if (root->left_) flag |= add_ir_to_library_no_deepcopy(root->left_);

	if (root->right_) flag |= add_ir_to_library_no_deepcopy(root->right_);

	IRTYPE type = root->type_;

	string tmpStr = root->to_string();
	uint64_t h = hash(tmpStr);
	if (find(IR_library_hash_[type].begin(), IR_library_hash_[type].end(), h) != IR_library_hash_[type].end()) {
		if ((type == IRTYPE::kParseToplevel) && flag) {
			IR_library_[type].push_back(root);
		}

		return false | flag;
	}

	IR_library_[type].push_back(root);
	IR_library_hash_[type].insert(h);

	return true;
}

void Mutator::extract_struct(IR* root) {
	IRTYPE type = root->type_;

	if (root->left_) extract_struct(root->left_);
	if (root->right_) extract_struct(root->right_);

	if (root->left_ || root->right_) return;

	switch (root->type_) {
		case IRTYPE::kIconst:
			root->long_val_ = 1;
			break;
		case IRTYPE::kFconst:
			root->float_val_ = 1.0;
			break;
		case IRTYPE::kSconst:
			root->str_val_ = "'x'";
			break;
		case IRTYPE::kIdent:
			root->str_val_ = "x";
			break;
	}

	return;
}

void Mutator::update_status(InputInfo* input, unsigned int execStatus, bool isValid) {
	unsigned int reward = get_cur_reward();
	bool hasNewCoverage = (execStatus == 1 || execStatus == 2);

	IRInfoManger* manger = IRInfo_[input->MutateIRType_];
	manger->update_count(&input->MutateContext_, input->MutateIRInfoItem_, hasNewCoverage, reward);

#ifdef ORIGINAL
	OperatorManger* OpManger = OpInfo_[input->MutateIRType_];
	OpManger->update_count(&input->MutateContext_, input->MutateOperatorItem_, hasNewCoverage, reward);
#endif

	if (hasNewCoverage == true){	
		if (execStatus == 1) {
			IRTypeCount_[input->MutateIRType_].first += reward;
		}
		else{
			IRTypeCount_[input->MutateIRType_].first += reward * REWARDNEWCOVERAGE;
		}

		if (isValid == true) {
			add_ir_to_library(input->input_);
		}		
	}
	else {
		IRTypeCount_[input->MutateIRType_].second += reward;
	}

	return;
}

vector<InputInfo*> Mutator::mutate_all(const char* sql) {
	vector<InputInfo*> res;
	deque<pair<IR*, IRContextS>> q;

	vector<IRTYPE> IRTypeRank;
	vector<double> probabilityRank;

	map<IRTYPE, vector<pair<IR*, IRContextS>>> indexes;

	IR* originRoot = IR_parser(sql);
	if (originRoot == nullptr)
		return res;

	IRContextS tmp = { 0 };
	q.push_back({originRoot, tmp});

	while (!q.empty()) {
		IR* curIR = q.front().first;
		IRContextS curContext = q.front().second;
		q.pop_front();

		if (curIR == nullptr) {
			continue;
		}

		IRContextS nextContext = { 0 };

		if (find(stmtIRType_.begin(), stmtIRType_.end(), curIR->type_) != stmtIRType_.end()) {
			nextContext.stmt_ = static_cast<unsigned short int>(curIR->type_);
		}
		else {
			nextContext.stmt_ = curContext.stmt_;
		}

		if (find(clauseIRType_.begin(), clauseIRType_.end(), curIR->type_) != clauseIRType_.end()) {
			nextContext.clause_ = static_cast<unsigned short int>(curIR->type_);
		}
		else {
			nextContext.clause_ = curContext.clause_;
		}

		if (curIR->type_ == IRTYPE::kSubquery) {
			nextContext.inSubquery_ = 1;
		}
		else {
			nextContext.inSubquery_ = curContext.inSubquery_;
		}

		nextContext.parentIRType_ = curIR->type_;
		nextContext.parentIRItem_ = IRInfo_[curIR->type_]->get_index_of_IRInfo(curIR);

		q.push_back({curIR->left_, nextContext });
		q.push_back({ curIR->right_ , nextContext });

		if (not_mutatable_types_.find(curIR->type_) != not_mutatable_types_.end()) {
			continue;
		}			

		unsigned int chosenNumber = IRTypeCount_[curIR->type_].first + IRTypeCount_[curIR->type_].second;
		if (find(IRTypeRank.begin(), IRTypeRank.end(), curIR->type_) == IRTypeRank.end() && chosenNumber > IRTYPEUPDATETHRESHOLD) {
			if (IRTypeRank.size() < TOPNUMBER) {
				IRTypeRank.push_back(curIR->type_);
			}
			else {
				IRTYPE tLeft = IRTypeRank[IRTypeRank.size() - 1];

				double pLeft = static_cast<double>(IRTypeCount_[tLeft].first) / (IRTypeCount_[tLeft].first + IRTypeCount_[tLeft].second);
				double pRight = static_cast<double>(IRTypeCount_[curIR->type_].first) / chosenNumber;

				if (pLeft < pRight) {
					IRTypeRank[IRTypeRank.size() - 1] = curIR->type_;
				}
			}
		}

		for (int i = IRTypeRank.size() - 1; i > 0; i--) {
			IRTYPE tLeft = IRTypeRank[i - 1];
			IRTYPE tRight = IRTypeRank[i];

			double pLeft = static_cast<double>(IRTypeCount_[tLeft].first) / (IRTypeCount_[tLeft].first + IRTypeCount_[tLeft].second);
			double pRight = static_cast<double>(IRTypeCount_[tRight].first) / (IRTypeCount_[tRight].first + IRTypeCount_[tRight].second);

			if (pLeft > pRight) {
				break;
			}

			IRTYPE tmp = IRTypeRank[i];
			IRTypeRank[i] = IRTypeRank[i - 1];
			IRTypeRank[i - 1] = tmp;
		}

		indexes[curIR->type_].push_back({ curIR, curContext });
	}

	double expectSum = 0;
	for (IRTYPE t : IRTypeRank) {
		expectSum += static_cast<double>(IRTypeCount_[t].first) / (IRTypeCount_[t].first + IRTypeCount_[t].second);
	}

	for (int i = 0; i < IRTypeRank.size(); i++) {
		IRTYPE t = IRTypeRank[i];
		double p = static_cast<double>(IRTypeCount_[t].first) / (IRTypeCount_[t].first + IRTypeCount_[t].second);

		probabilityRank.push_back(p / expectSum);
	}

	double heat = get_cur_heat();
	for (auto& it : indexes) {
		IRTYPE curType = it.first;
		unsigned int chosenNumber = IRTypeCount_[curType].first + IRTypeCount_[curType].second;

		for (pair<IR*, IRContextS>& node : it.second) {
			IR* curNode = nullptr;
			IRContextS curContext = { 0 };

			double randomNumber = get_random_double(0, 1);

			if (chosenNumber < IRTYPEUPDATETHRESHOLD || randomNumber < heat) {
				curNode = node.first;
				curContext = node.second;
			}
			else {
				double tmpRand = get_random_double(0, 1);
				IRTYPE tmpType = IRTYPE::kNone;

				for (int i = 0; i < probabilityRank.size(); i++) {
					if (tmpRand >= probabilityRank[i]) {
						tmpRand -= probabilityRank[i];
						continue;
					}

					tmpType = IRTypeRank[i];
					break;
				}

				if (tmpType == IRTYPE::kNone) {
					curNode = node.first;
					curContext = node.second;
				}
				else {
					pair<IR*, IRContextS>& tmp = indexes[tmpType][rand() % indexes[tmpType].size()];
					curNode = tmp.first;
					curContext = tmp.second;
				}
			}

#ifdef ORIGINAL
			vector<InputInfo*> newInputInfos = original_mutate(curNode, curContext, heat);
#else
			vector<InputInfo*> newInputInfos = mutate(curNode, curContext, heat);
#endif	

			for (InputInfo* newInputInfo : newInputInfos) {
				newInputInfo->input_ = get_new_tree(originRoot, curNode, newInputInfo->input_);

				extract_struct(newInputInfo->input_);

				string tmpStr = newInputInfo->input_->to_string();

				uint64_t h = hash(tmpStr);
				if (sql_hash_.find(h) != sql_hash_.end()) {
					delete newInputInfo;
					continue;
				}
				sql_hash_.insert(h);

				IR* checkSql = check_sql(tmpStr);
				if (checkSql == nullptr) {
					delete newInputInfo;
					continue;
				}
				else {
					deep_delete(newInputInfo->input_);
					newInputInfo->input_ = checkSql;
				}

				res.push_back(newInputInfo);
			}
		}		
	}

	deep_delete(originRoot);

	mutateNumber_.push_back(res.size());
	seedCount_++;

	return res;
}

vector<InputInfo*> Mutator::mutate(IR* root, IRContextS context, double heat) {
	vector<InputInfo*> res;

	IRInfoManger* manger = IRInfo_[root->type_];
	vector<unsigned short int> indexes = manger->get_IRInfo_index(&context, heat);

	for (unsigned short int index : indexes) {
		char flag = (rand() % 3) + 1;

		IR* left = nullptr;
		IR* right = nullptr;

		IRInfomation* tmpIRInformation = manger->get_IRInfomation(index);

		InputInfo* newInputInfo = new InputInfo();
		newInputInfo->MutateIRType_ = root->type_;
		newInputInfo->MutateIRInfoItem_ = index;
		newInputInfo->MutateContext_ = context;

		if ((flag & 1) || ((root->left_ ? root->left_->type_ : IRTYPE::kNone) != tmpIRInformation->left_)) {
			IR* tmp = get_ir_from_library(tmpIRInformation->left_);
			left = deep_copy(tmp);
		}
		else {
			left = deep_copy(root->left_);
		}

		if ((flag & 2) || ((root->right_ ? root->right_->type_ : IRTYPE::kNone) != tmpIRInformation->right_)) {
			IR* tmp = get_ir_from_library(tmpIRInformation->right_);
			right = deep_copy(tmp);
		}
		else {
			right = deep_copy(root->right_);
		}

		IR* newIRTree = new IR(root, left, right);

		newIRTree->op_->prefix_ = tmpIRInformation->prefix_;
		newIRTree->op_->middle_ = tmpIRInformation->middle_;
		newIRTree->op_->suffix_ = tmpIRInformation->suffix_;

		newInputInfo->input_ = newIRTree;

		res.push_back(newInputInfo);
	}

	return res;
}

IR* Mutator::get_ir_from_library(IRTYPE type) {
	if (type == IRTYPE::kNone)
		return nullptr;

	if (IR_library_[type].empty()) {
		fprintf(stderr, "ERROR: Lack IRTYPE : %d!!!\n", static_cast<int>(type));
		assert(0);
	}

	return IR_library_[type][rand() % (IR_library_[type].size())];
}

IR* Mutator::get_new_tree(IR* oldIRRoot, IR* curIR, IR* newIR) {
	assert(oldIRRoot && curIR && newIR);

	IR* res = nullptr;
	IR* left = nullptr;
	IR* right = nullptr;

	if (oldIRRoot->left_ != nullptr) {
		if (oldIRRoot->left_ == curIR)
			left = newIR;
		else
			left = get_new_tree(oldIRRoot->left_, curIR, newIR);
	}

	if (oldIRRoot->right_ != nullptr) {
		if (oldIRRoot->right_ == curIR)
			right = newIR;
		else
			right = get_new_tree(oldIRRoot->right_, curIR, newIR);
	}

	res = new IR(oldIRRoot, left, right);

	return res;
}

void Mutator::print_information(string outputPath) {
	print_IRType_information(outputPath);
	print_IRInfo_information(outputPath);
	print_Mutate_number(outputPath);

#ifdef ORIGINAL
	print_OpInfo_information(outputPath);
#endif

	return;
}
void Mutator::print_IRType_information(string outputPath) {
	ofstream outFileIRType(outputPath + "IRType_Information", ios::trunc);

	if (outFileIRType) {
		for (auto& it : IRTypeCount_) {
			IRTYPE curType = it.first;
			pair<unsigned int, unsigned int>& count = it.second;

			outFileIRType << "Current IRTYPE: " << to_string(curType) << endl;
			outFileIRType << "\tSuccesses: " << to_string(count.first) << ", Failed: " << to_string(count.second) << ", Total: " << to_string(count.first + count.second) << endl << endl;
		}

		outFileIRType.close();
	}
	else {
		cout << "ERROR: Can't Write IRType_Information File!!!" << endl;
	}

	return;
}
void Mutator::print_IRInfo_information(string outputPath) {
	ofstream outFileIRInfo(outputPath + "IRInfo_information", ios::trunc);

	if (outFileIRInfo) {
		for (auto& it : IRInfo_) {
			string info = it.second->print_IRInformation_status();
			outFileIRInfo << info << endl;
		}
	}
	else {
		cout << "ERROR: Can't Write IRInfo_information File!!!" << endl;
	}

	return;
}
void Mutator::print_Mutate_number(string outputPath) {
	ofstream outFileMutate(outputPath + "Mutate_number", ios::trunc);

	if (outFileMutate) {
		for (unsigned short n : mutateNumber_) {
			outFileMutate << n << endl;
		}
	}
	else {
		cout << "ERROR: Can't Write Mutate_number File!!!" << endl;
	}

	return;
}
void Mutator::print_OpInfo_information(string outputPath) {
	ofstream outFileIRInfo(outputPath + "OpInfo_information", ios::trunc);

	if (outFileIRInfo) {
		pair<unsigned long int, unsigned long int> OpReplace = { 0, 0 };
		pair<unsigned long int, unsigned long int> OpInsert = { 0, 0 };
		pair<unsigned long int, unsigned long int> OpDelete = { 0, 0 };

		for (auto& it : OpInfo_) {
			vector<unsigned long int>& chosenCounts = it.second->OpInfoChosenCount_;
			vector<unsigned long int>& failedCounts = it.second->OpInfoFailedCount_;

			OpReplace.first += chosenCounts[0];
			OpReplace.second += failedCounts[0];

			OpInsert.first += chosenCounts[1];
			OpInsert.second += failedCounts[1];

			OpDelete.first += chosenCounts[2];
			OpDelete.second += failedCounts[2];

			string info = it.second->print_OperatorInformation_status();
			outFileIRInfo << info << endl;
		}

		outFileIRInfo << "Replace Operator\n\tTotal Chosen Number: " << to_string(OpReplace.first) << ", Total Failed Number: " << to_string(OpReplace.second) << ", Accuracy Rate: " << to_string(1 - static_cast<double>(OpReplace.second) / OpReplace.first) << "%" << endl;
		outFileIRInfo << "Insert Operator\n\tTotal Chosen Number: " << to_string(OpInsert.first) << ", Total Failed Number: " << to_string(OpInsert.second) << ", Accuracy Rate: " << to_string(1 - static_cast<double>(OpInsert.second) / OpInsert.first) << "%" << endl;
		outFileIRInfo << "Delete Operator\n\tTotal Chosen Number: " << to_string(OpDelete.first) << ", Total Failed Number: " << to_string(OpDelete.second) << ", Accuracy Rate: " << to_string(1 - static_cast<double>(OpDelete.second) / OpDelete.first) << "%" << endl;
	}
	else {
		cout << "ERROR: Can't Write OpInfo_information File!!!" << endl;
	}

	return;
}

unsigned int Mutator::get_duration_ms() {
	chrono::time_point<chrono::steady_clock> curTime = chrono::steady_clock::now();

	return chrono::duration_cast<chrono::milliseconds>(curTime - startTime).count();
}
double Mutator::get_cur_heat() {
	unsigned int duration = get_duration_ms();

	return static_cast<double>(1) / (static_cast<unsigned int>(duration / TIMETHRESHOLD) + 1);
}
unsigned int Mutator::get_cur_reward() {
	unsigned int duration = get_duration_ms();

	return 1 + (duration / TIMETHRESHOLD) * REWARDINCREMENT;
}

vector<InputInfo*> Mutator::original_mutate(IR* root, IRContextS context, double heat) {
	vector<InputInfo*> res;

	OperatorManger* OpManger = OpInfo_[root->type_];
	vector<unsigned short int> indexes = OpManger->get_OpInfo_index(&context, heat);

	for (unsigned short int index : indexes) {
		MutateOperator op = OpManger->get_operator(index);
		IR* tmp = nullptr;

		switch (op) {
			case MutateOperator::moReplace: {
				tmp = strategy_replace(root);
				break;
			}
			case MutateOperator::moInsert: {
				tmp = strategy_insert(root);
				break;
			}
			case MutateOperator::moDelete: {
				tmp = strategy_delete(root);
				break;
			}
		}

		if (tmp != nullptr) {
			IRInfoManger* manger = IRInfo_[tmp->type_];
			manger->get_IRInfo_index(&context, get_cur_heat());

			InputInfo* newInputInfo = new InputInfo();

			newInputInfo->MutateIRType_ = root->type_;
			newInputInfo->MutateIRInfoItem_ = manger->get_index_of_IRInfo(tmp);
			newInputInfo->MutateContext_ = context;
			newInputInfo->MutateOperatorItem_ = index;

			if (newInputInfo->MutateIRInfoItem_ != ERRORINDEX) {
				manger->IRInfoChosenCount_[newInputInfo->MutateIRInfoItem_]++;
			}

			newInputInfo->input_ = tmp;

			res.push_back(newInputInfo);
		}
		else {
			OpManger->OpInfoFailedCount_[index]++;
			OpManger->update_count(&context, index, false, get_cur_reward());
		}
	}

	return res;
}

IR* Mutator::strategy_delete(IR* root) {
	IR* res = nullptr;
	int flag = 0;

	if (root->left_ != nullptr && root->right_ != nullptr) {
		flag = (rand() % 3) + 1;
	}
	else if (root->left_ != nullptr) {
		flag = 1;
	}
	else if (root->right_ != nullptr) {
		flag = 2;
	}
	else {
		return nullptr;
	}

	res = deep_copy(root);

	if ((flag & 1) == 1) {
		deep_delete(res->left_);
		res->left_ = nullptr;
	}

	if ((flag & 2) == 2) {
		deep_delete(res->right_);
		res->right_ = nullptr;
	}

	return res;
}
IR* Mutator::strategy_insert(IR* root) {
	IRTYPE curType = root->type_;

	if (root->right_ == nullptr && root->left_ != nullptr) {
		IRTYPE leftNodeType = root->left_->type_;

		for (int i = 0; i < 4; i++) {
			IR* tmp = get_ir_from_library(curType);

			if (tmp != nullptr && tmp->left_ != nullptr && tmp->left_->type_ == leftNodeType && tmp->right_ != nullptr) {
				IR* res = deep_copy(root);
				res->right_ = deep_copy(tmp->right_);

				res->op_->prefix_ = tmp->op_->prefix_;
				res->op_->middle_ = tmp->op_->middle_;
				res->op_->suffix_ = tmp->op_->suffix_;

				return res;
			}
		}
	}
	else if (root->right_ != nullptr && root->left_ == nullptr) {
		IRTYPE rightNodeType = root->right_->type_;

		for (int i = 0; i < 4; i++) {
			IR* tmp = get_ir_from_library(curType);

			if (tmp != nullptr && tmp->right_ != nullptr && tmp->right_->type_ == rightNodeType && tmp->left_ != nullptr) {
				IR* res = deep_copy(root);
				res->left_ = deep_copy(tmp->left_);

				res->op_->prefix_ = tmp->op_->prefix_;
				res->op_->middle_ = tmp->op_->middle_;
				res->op_->suffix_ = tmp->op_->suffix_;

				return res;
			}
		}
	}
	else if (root->right_ == nullptr && root->left_ == nullptr) {
		for (int i = 0; i < 4; i++) {
			IR* tmp = get_ir_from_library(curType);

			if (tmp != nullptr && (tmp->left_ != nullptr || tmp->right_ != nullptr)) {
				IR* res = deep_copy(root);
				res->left_ = deep_copy(tmp->left_);
				res->right_ = deep_copy(tmp->right_);

				res->op_->prefix_ = tmp->op_->prefix_;
				res->op_->middle_ = tmp->op_->middle_;
				res->op_->suffix_ = tmp->op_->suffix_;

				return res;
			}
		}
	}

	return nullptr;
}
IR* Mutator::strategy_replace(IR* root) {
	IR* res = nullptr;
	int flag = 0;

	if (root->left_ != nullptr && root->right_ != nullptr) {
		flag = (rand() % 3) + 1;
	}
	else if (root->left_ != nullptr) {
		flag = 1;
	}
	else if (root->right_ != nullptr) {
		flag = 2;
	}
	else {
		return nullptr;
	}

	res = deep_copy(root);

	if ((flag & 1) == 1) {
		IR* tmpLeft = get_ir_from_library(res->left_->type_);

		if (tmpLeft != nullptr) {
			deep_delete(res->left_);
			res->left_ = deep_copy(tmpLeft);
		}
	}

	if ((flag & 2) == 2) {
		IR* tmpRight = get_ir_from_library(res->right_->type_);

		if (tmpRight != nullptr) {
			deep_delete(res->right_);
			res->right_ = deep_copy(tmpRight);
		}
	}

	return res;
}

IR* Mutator::check_sql(string& sql) {
	IR* res = IR_parser(sql.c_str());

	totalMutate_++;
	if (res == nullptr) {
		failedMutate_++;
	}
	
	return res;
}

//-----------------------------IRInfomation-----------------------------
IRInfomation::IRInfomation(int prefix, int left, int middle, int right, int suffix) {
	prefix_ = static_cast<TERMINAL>(prefix);
	left_ = static_cast<IRTYPE>(left);
	middle_ = static_cast<TERMINAL>(middle);
	right_ = static_cast<IRTYPE>(right);
	suffix_ = static_cast<TERMINAL>(suffix);

	return;
}
IRInfomation::IRInfomation(IR* root) {
	prefix_ = root->op_->prefix_;
	left_ = root->left_ != nullptr ? root->left_->type_ : IRTYPE::kNone;
	middle_ = root->op_->middle_;
	right_ = root->right_ != nullptr ? root->right_->type_ : IRTYPE::kNone;
	suffix_ = root->op_->suffix_;

	return;
}

bool IRInfomation::operator!=(IRInfomation* info) {
	if (info->prefix_ != prefix_) {
		return true;
	}

	if (info->left_ != left_) {
		return true;
	}

	if (info->middle_ != middle_) {
		return true;
	}

	if (info->right_ != right_) {
		return true;
	}

	if (info->suffix_ != suffix_) {
		return true;
	}

	return false;
}

//-----------------------------IRInfoManger-----------------------------
IRInfoManger::IRInfoManger() {}
IRInfoManger::~IRInfoManger() {
	for (IRInfomation* info : IRInfomation_) {
		delete info;
	}

	return;
}

void IRInfoManger::create_new_context(IRContextS* context) {
	uint64_t index = *reinterpret_cast<uint64_t*>(context);

	assert(contextDependentCount_.find(index) == contextDependentCount_.end());

	for (int i = 0; i < IRInfomation_.size(); i++) {
		contextDependentCount_[index].push_back({0, 0});
	}

	return;
}

void IRInfoManger::update_count(IRContextS* context, unsigned short int item, bool hasNewCoverage, unsigned int reward) {
	uint64_t index = *reinterpret_cast<uint64_t*>(context);

	assert(contextDependentCount_.find(index) != contextDependentCount_.end());

	if (item == ERRORINDEX) {
		return;
	}

	if (hasNewCoverage == true) {
		contextDependentCount_[index][item].first += reward;
	}
	else {
		contextDependentCount_[index][item].second += reward;
	}

	return;
}

void IRInfoManger::add_IRInfomation(IRInfomation* info) {
	IRInfomation_.push_back(info);
	IRInfoChosenCount_.push_back(0);
	return;
}
IRInfomation* IRInfoManger::get_IRInfomation(unsigned short int index) {
	assert(index < IRInfomation_.size() && index >= 0);
	IRInfoChosenCount_[index]++;
	return IRInfomation_[index];
}

#ifdef BALANCE
vector<unsigned short int> IRInfoManger::get_IRInfo_index(IRContextS* context, double heat) {
	vector<unsigned short int> res;
	uint64_t index = *reinterpret_cast<uint64_t*>(context);

	if (contextDependentCount_.find(index) == contextDependentCount_.end()) {
		create_new_context(context);
	}

	for (int i = 0; i < MUTATENUMBER; i++) {
		double randomNumber = get_random_double(0, 1);
		if (randomNumber < heat) {
			res.push_back(rand() % IRInfomation_.size());
		}
		else {
			int maxIndex = ERRORINDEX;
			double maxExpect = 0.0;
			vector<pair<unsigned int, unsigned int>>& counts = contextDependentCount_[index];

			for (int i = 0; i < counts.size(); i++) {
				unsigned int totalNumber = counts[i].first + counts[i].second;
				if (totalNumber <= IRINFOUPDATETHRESHOLD) {
					continue;
				}

				double expect = counts[i].first / totalNumber;
				if (expect > maxExpect) {
					maxIndex = i;
					maxExpect = expect;
				}
			}

			if (maxIndex != ERRORINDEX) {
				res.push_back(maxIndex);
			}
			else {
				res.push_back(rand() % IRInfomation_.size());
			}
	}
	}

	return res;
}
#else
vector<unsigned short int> IRInfoManger::get_IRInfo_index(IRContextS* context, double heat) {
	vector<unsigned short int> res;
	vector<unsigned short int> skip;
	uint64_t index = *reinterpret_cast<uint64_t*>(context);

	if (contextDependentCount_.find(index) == contextDependentCount_.end()) {
		create_new_context(context);
	}

	unsigned short int maxExpectIndex = ERRORINDEX;
	double maxExpect = 0;

	vector<pair<unsigned int, unsigned int>>& curContextCount = contextDependentCount_[index];
	int mutateNumber = curContextCount.size();

	for (int i = 0; i < curContextCount.size(); i++) {
		pair<unsigned int, unsigned int>& count = curContextCount[i];

		unsigned int chosenNumber = count.first + count.second;
		if (chosenNumber > IRINFOUPDATETHRESHOLD) {
			double expect = static_cast<double>(count.first) / chosenNumber;

			if (expect > maxExpect) {
				maxExpectIndex = i;
				maxExpect = expect;
			}
		}

		double tmp = get_random_double(0, 1);
		if (chosenNumber <= IRINFOUPDATETHRESHOLD || tmp < heat) {
			res.push_back(i);
			mutateNumber--;
		}
		else {
			skip.push_back(i);
		}
	}

	if (maxExpectIndex == ERRORINDEX) {
		for (unsigned short int i : skip) {
			res.push_back(i);
		}
	}
	else {
		while (mutateNumber > 0) {
			res.push_back(maxExpectIndex);
			mutateNumber--;
		}
	}

	return res;
}
#endif

unsigned short int IRInfoManger::get_index_of_IRInfo(IR* root) {
	int res = ERRORINDEX;
	IRInfomation curIRInfo(root);

	for (int i = 0; i < IRInfomation_.size(); i++) {
		if (*IRInfomation_[i] != &curIRInfo) {
			continue;
		}

		res = i;
		break;
	}

	return res;
}

string IRInfoManger::print_IRInformation_status() {
	unsigned long totalSuccessCount = 0;
	unsigned long totalFailedCount = 0;

	string res = "Current IRTYPE: " + to_string(curIRType_) + "\n";

	for (int i = 0; i < IRInfomation_.size(); i++) {
		unsigned long successCount = 0;
		unsigned long failedCount = 0;
		IRInfomation* tmp = IRInfomation_[i];

		res += "--IRInfo " + to_string(i) + ": " + to_string(tmp->prefix_) + " " + to_string(tmp->left_) + " " + to_string(tmp->middle_) + " " + to_string(tmp->right_) + " " + to_string(tmp->suffix_) + ":\n";
		res += "--Total Chosen Number: " + to_string(IRInfoChosenCount_[i]) + "\n";

		for (auto& it : contextDependentCount_) {
			uint64_t context = it.first;
			pair<unsigned int, unsigned int>& count = it.second[i];

			successCount += count.first;
			totalSuccessCount += count.first;
			failedCount += count.second;
			totalFailedCount += count.second;

			res += "\tCurrent Context: " + to_string(context) + ", Successes: " + to_string(count.first) + ", Failed: " + to_string(count.second) + ", Total: " + to_string(count.first + count.second) + "\n";
		}

		res += "--Successes: " + to_string(successCount) + ", Failed: " + to_string(failedCount) + ", Total: " + to_string(successCount + failedCount) + "\n";
	}

	res += "Total Successes: " + to_string(totalSuccessCount) + ", Total Failed: " + to_string(totalFailedCount) + ", Total: " + to_string(totalSuccessCount + totalFailedCount) + "\n\n";

	return res;
}

//-----------------------------InputInfo-----------------------------
InputInfo::InputInfo() {}
InputInfo::~InputInfo() {
	deep_delete(input_);
	return;
}

//-----------------------------OperatorManger-----------------------------
OperatorManger::OperatorManger() {
	OperatorInfomation_ = { MutateOperator::moReplace, MutateOperator::moInsert, MutateOperator::moDelete };
	OpInfoChosenCount_ = { 0, 0, 0 };
	OpInfoFailedCount_ = { 0, 0, 0 };

	return;
}
OperatorManger::~OperatorManger() {}

void OperatorManger::create_new_context(IRContextS* context) {
	uint64_t index = *reinterpret_cast<uint64_t*>(context);

	assert(contextDependentCount_.find(index) == contextDependentCount_.end());

	for (int i = 0; i < OperatorInfomation_.size(); i++) {
		contextDependentCount_[index].push_back({ 0, 0 });
	}

	return;
}
vector<unsigned short int> OperatorManger::get_OpInfo_index(IRContextS* context, double heat) {
	vector<unsigned short int> res;
	vector<unsigned short int> skip;

	IRContextS OpContext = *context;
	OpContext.parentIRItem_ = ERRORINDEX;

	uint64_t index = *reinterpret_cast<uint64_t*>(&OpContext);

	if (contextDependentCount_.find(index) == contextDependentCount_.end()) {
		create_new_context(&OpContext);
	}

	unsigned short int maxExpectIndex = ERRORINDEX;
	double maxExpect = 0;

	vector<pair<unsigned int, unsigned int>>& curContextCount = contextDependentCount_[index];
	int mutateNumber = curContextCount.size();

	for (int i = 0; i < curContextCount.size(); i++) {
		pair<unsigned int, unsigned int>& count = curContextCount[i];

		unsigned int chosenNumber = count.first + count.second;
		if (chosenNumber > IRINFOUPDATETHRESHOLD) {
			double expect = static_cast<double>(count.first) / (count.first + count.second);

			if (expect > maxExpect) {
				maxExpectIndex = i;
				maxExpect = expect;
			}
		}

		double tmp = get_random_double(0, 1);
		if (chosenNumber < IRINFOUPDATETHRESHOLD || tmp < heat) {
			res.push_back(i);
			mutateNumber--;
		}
		else {
			skip.push_back(i);
		}
	}

	if (maxExpectIndex == ERRORINDEX) {
		for (unsigned short int i : skip) {
			res.push_back(i);
		}
	}
	else {
		while (mutateNumber > 0) {
			res.push_back(maxExpectIndex);
			mutateNumber--;
		}
	}

	return res;
}
MutateOperator OperatorManger::get_operator(unsigned short int index) {
	assert(index < OperatorInfomation_.size() && index >= 0);
	OpInfoChosenCount_[index]++;
	return OperatorInfomation_[index];
}

void OperatorManger::update_count(IRContextS* context, unsigned short int item, bool hasNewCoverage, unsigned int reward) {
	IRContextS OpContext = *context;
	OpContext.parentIRItem_ = ERRORINDEX;

	uint64_t index = *reinterpret_cast<uint64_t*>(&OpContext);

	assert(contextDependentCount_.find(index) != contextDependentCount_.end());

	if (item == ERRORINDEX) {
		return;
	}

	if (hasNewCoverage == true) {
		contextDependentCount_[index][item].first += reward;
	}
	else {
		contextDependentCount_[index][item].second += reward;
	}

	return;
}

string OperatorManger::print_OperatorInformation_status() {
	unsigned long totalSuccessCount = 0;
	unsigned long totalFailedCount = 0;

	string res = "Current IRTYPE: " + to_string(curIRType_) + "\n";

	for (int i = 0; i < OperatorInfomation_.size(); i++) {
		unsigned long successCount = 0;
		unsigned long failedCount = 0;
		MutateOperator op = OperatorInfomation_[i];

		string OpStr = "";
		switch (op) {
			case MutateOperator::moReplace: {
				OpStr += "REPLACE";
				break;
			}
			case MutateOperator::moInsert: {
				OpStr += "INSERT";
				break;
			}
			case MutateOperator::moDelete: {
				OpStr += "DELETE";
				break;
			}
		}

		res += "--Operator " + OpStr + ":\n";
		res += "--Total Chosen Number: " + to_string(OpInfoChosenCount_[i]) + ", " + "Mutate Failed: " + to_string(OpInfoFailedCount_[i]) + "\n";

		for (auto& it : contextDependentCount_) {
			uint64_t context = it.first;
			pair<unsigned int, unsigned int>& count = it.second[i];

			successCount += count.first;
			totalSuccessCount += count.first;
			failedCount += count.second;
			totalFailedCount += count.second;

			res += "\tCurrent Context: " + to_string(context) + ", Successes: " + to_string(count.first) + ", Failed: " + to_string(count.second) + ", Total: " + to_string(count.first + count.second) + "\n";
		}

		res += "--Successes: " + to_string(successCount) + ", Failed: " + to_string(failedCount) + ", Total: " + to_string(successCount + failedCount) + "\n";
	}

	res += "Total Successes: " + to_string(totalSuccessCount) + ", Total Failed: " + to_string(totalFailedCount) + ", Total: " + to_string(totalSuccessCount + totalFailedCount) + "\n\n";

	return res;
}
