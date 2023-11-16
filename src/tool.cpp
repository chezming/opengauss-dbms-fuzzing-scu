#include "../include/instantiate.h"

using namespace std;

//-----------------------------Column-----------------------------
Column::Column(name_t name, ColumnType type): identifierName_(name), columnType_(type) {}
Column::Column(Column* column) {
	notNull_ = column->notNull_;
	hasDefault_ = column->hasDefault_;
	isVisible_ = column->isVisible_;
	identifierName_ = column->identifierName_;
	duplicated_ = column->duplicated_;
	columnType_ = column->columnType_;

	return;
}

void Column::instantiate() {
	return;
}

//-----------------------------Index-----------------------------
Index::Index(name_t name): identifierName_(name) {}

//-----------------------------Constraint-----------------------------
Constraint::Constraint(name_t name, ConstraintType type): identifierName_(name), constraintType_(type) {}

//-----------------------------Table-----------------------------
Table::Table(name_t name, IdentifierType identifierType, SelectStmtInfo* inSelectStmtInfo) : identifierName_(name), inSelectStmtInfo_(inSelectStmtInfo), identifierType_(identifierType) {
	for (int i = 0; i < RESERVEDCOLUMNNAME; i++) {
		acceColumnNames_.push_back(i);
	}

	random_device rd;
	mt19937 g(rd());

	shuffle(acceColumnNames_.begin(), acceColumnNames_.end(), g);

	return;
}
Table::Table(Table* otherTable, SelectStmtInfo* inSelectStmtInfo) : identifierName_(otherTable->identifierName_), inSelectStmtInfo_(inSelectStmtInfo), identifierType_(otherTable->identifierType_) {
	acceColumnNames_ = otherTable->acceColumnNames_;

	for (Column* c : otherTable->columns_) {
		add_column(new Column(c));
	}

	return;
}
Table::~Table() {
	for (Column* c : columns_) {
		delete c;
	}

	for (Index* i : indexes_) {
		delete i;
	}

	for (Constraint* c : constraints_) {
		delete c;
	}

	return;
}

bool Table::check_drop_table() {
	if (!refFromOtherColumns_.empty()) {
		return false;
	}

	return false;
}
bool Table::exist_duplicated_column() {
	for (Column* c : columns_) {
		if (c->duplicated_ == ColumnStatus::csDuplication) {
			return true;
		}
	}

	return false;
}
void Table::update_duplicationInfo(vector<int>& duplicationInfo) {
	assert(duplicationInfo.size() == COLUMNNAMEMAX);
	int size = columns_.size();

	for (int i = 0; i < size; i++) {
		duplicationInfo[columns_[i]->identifierName_]++;
	}

	return;
}
void Table::update_duplicatedColumns(vector<int>& duplicationInfo) {
	assert(duplicationInfo.size() == COLUMNNAMEMAX);

	for (Column* c : columns_) {
		assert(c->identifierName_ < COLUMNNAMEMAX);
		if (duplicationInfo[c->identifierName_] > 1) {
			c->duplicated_ = ColumnStatus::csDuplication;
		}
		else if (duplicationInfo[c->identifierName_] == 1) {
			c->duplicated_ = ColumnStatus::csNormal;
		}
	}

	return;
}

bool Table::column_drop_check(Column* column) {
	vector<Index*> indexes = get_index(column);
	vector<Constraint*> constraints = get_constraint(column);

	if (find(columns_.begin(), columns_.end(), column) == columns_.end()) {
		return false;
	}

	for (Index* index : indexes) {
		if (index->columns_[column] == IndexType::itFunctional) {
			return false;
		}
	}

	for (Constraint* constraint : constraints) {
		if (constraint->columns_[column] == IndexType::itFunctional || constraint->constraintType_ == ConstraintType::ctForeignKey || constraint->constraintType_ == ConstraintType::ctCheck) {
			return false;
		}
	}

	return true;
}
Column* Table::get_column() {
	if (columns_.empty()) {
		fprintf(stderr, "ERROR: Empty Table!!!\n");
		assert(0);
	}

	return columns_[rand() % columns_.size()];
}
Column* Table::get_column(ColumnType type) {
	vector<Column*> tmp;

	for (Column* c : columns_) {
		if (c->columnType_ == type) {
			tmp.push_back(c);
		}
	}

	if (tmp.empty()) {
		return nullptr;
	}

	return tmp[rand() % tmp.size()];
}
name_t Table::get_acce_column_name() {
	if (acceColumnNames_.empty()) {
		return 0;
	}

	return acceColumnNames_[0];
}
vector<Column*> Table::get_column_seq() {
	int size = columns_.size();
	vector<Column*> tmp;

	for (int i = 0; i < size; i++) {
		tmp.push_back(columns_[i]);
	}

	random_device rd;
	mt19937 g(rd());

	shuffle(tmp.begin(), tmp.end(), g);

	return tmp;
}
void Table::add_column_first(Column* column) {
	assert(column != nullptr);
	remove_column_name(column->identifierName_);
	column->parentTable_ = this;

	columns_.insert(columns_.begin(), column);

	return;
}
bool Table::add_column(Column* column, Column* afterColumn) {
	assert(column != nullptr);
	remove_column_name(column->identifierName_);
	column->parentTable_ = this;

	return move_column(column, afterColumn);
}
void Table::move_column_first(Column* column) {
	auto it = find(columns_.begin(), columns_.end(), column);
	assert(it != columns_.end());

	columns_.erase(it);
	columns_.insert(columns_.begin(), column);

	return;
}
bool Table::move_column(Column* column, Column* afterColumn) {
	assert(column != afterColumn);

	auto it1 = find(columns_.begin(), columns_.end(), column);
	auto it2 = find(columns_.begin(), columns_.end(), afterColumn);

	assert(afterColumn == nullptr || it2 != columns_.end());

	if (it1 != columns_.end()) {
		columns_.erase(it1);
	}

	if (afterColumn == nullptr) {
		columns_.push_back(column);
	}
	else {
		auto it2 = find(columns_.begin(), columns_.end(), afterColumn);
		columns_.insert(it2 + 1, column);
	}

	return true;
}
bool Table::drop_column(Column* column) {
	if (!column_drop_check(column)) {
		return false;
	}

	auto it = find(columns_.begin(), columns_.end(), column);

	add_column_name(column->identifierName_);
	
	drop_index(column);
	drop_constraint(column);

	delete column;
	columns_.erase(it);

	return true;
}

void Table::column_rename(Column* column, name_t oldName, name_t newName) {
	column->identifierName_ = newName;

	remove_column_name(newName);
	add_column_name(oldName);

	return;
}

Index* Table::get_index() {
	if (indexes_.empty()) {
		return nullptr;
	}

	return indexes_[rand() % indexes_.size()];
}
vector<Index*> Table::get_index(Column* column) {
	vector<Index*> tmp;

	for (Index* index : indexes_) {
		if (index->columns_.find(column) != index->columns_.end()) {
			tmp.push_back(index);
		}
	}

	return tmp;
}
bool Table::add_index(Index* index) {
	if (find(indexes_.begin(), indexes_.end(), index) != indexes_.end()) {
		return false;
	}

	indexes_.push_back(index);
	return true;
}
bool Table::drop_index(Index* index) {
	if (!index_drop_check(index)) {
		return false;
	}

	auto it = find(indexes_.begin(), indexes_.end(), index);

	if (index->constraint_ != nullptr) {
		assert(index->constraint_->index_ == index);
		index->constraint_->index_ = nullptr;
		drop_constraint(index->constraint_);
	}

	indexes_.erase(it);
	delete index;	

	return true;
}

Constraint* Table::get_constraint() {
	if (constraints_.empty()) {
		return nullptr;
	}

	return constraints_[rand() % constraints_.size()];
}
vector<Constraint*> Table::get_constraint(Column* column) {
	vector<Constraint*> tmp;

	for (Constraint* constraint : constraints_) {
		if (constraint->columns_.find(column) != constraint->columns_.end()) {
			tmp.push_back(constraint);
		}
	}

	return tmp;
}
bool Table::add_constraint(Constraint* constraint, Index* index) {
	if (find(constraints_.begin(), constraints_.end(), constraint) != constraints_.end()) {
		return false;
	}

	constraint->index_ = index;
	constraints_.push_back(constraint);

	if (index != nullptr) {
		index->constraint_ = constraint;
		add_index(index);
	}

	if (constraint->constraintType_ == ConstraintType::ctForeignKey) {
		for (auto& it : constraint->refToOtherColumns_) {
			Column* otherTableColumn = it.first;
			for (Column* curTableColumn : it.second) {
				curTableColumn->parentTable_->set_references(curTableColumn, otherTableColumn);
			}
		}
	}

	if (constraint->constraintType_ == ConstraintType::ctPrimaryKey) {
		hasPrimaryKey_ = true;
	}

	return true;
}
bool Table::drop_constraint(Constraint* constraint) {
	if (!constraint_drop_check(constraint)) {
		return false;
	}

	auto it = find(constraints_.begin(), constraints_.end(), constraint);

	if (constraint->index_ != nullptr) {
		assert(constraint->index_->constraint_ == constraint);
		constraint->index_->constraint_ = nullptr;
		drop_index(constraint->index_);
	}

	if (constraint->constraintType_ == ConstraintType::ctForeignKey) {
		for (auto& it : constraint->refToOtherColumns_) {
			Column* otherTableColumn = it.first;
			for (Column* curTableColumn : it.second) {
				curTableColumn->parentTable_->remove_references(curTableColumn, otherTableColumn);
			}
		}
	}

	if (constraint->constraintType_ == ConstraintType::ctPrimaryKey) {
		hasPrimaryKey_ = false;
	}

	delete constraint;
	constraints_.erase(it);

	return true;
}
bool Table::set_references(Column* curTableColumn, Column* otherTableColumn) {
	if (find(columns_.begin(), columns_.end(), curTableColumn) == columns_.end()) {
		return false;
	}

	if (refFromOtherColumns_.find(curTableColumn) != refFromOtherColumns_.end() && refFromOtherColumns_[curTableColumn].find(otherTableColumn) != refFromOtherColumns_[curTableColumn].end()) {
		refFromOtherColumns_[curTableColumn][otherTableColumn] += 1;
	}
	else {
		refFromOtherColumns_[curTableColumn][otherTableColumn] = 1;
	}

	return true;
}
bool Table::remove_references(Column* curTableColumn, Column* otherTableColumn) {
	if (find(columns_.begin(), columns_.end(), curTableColumn) == columns_.end()) {
		return false;
	}

	if (refFromOtherColumns_.find(curTableColumn) != refFromOtherColumns_.end() && refFromOtherColumns_[curTableColumn].find(otherTableColumn) != refFromOtherColumns_[curTableColumn].end()) {
		refFromOtherColumns_[curTableColumn][otherTableColumn] -= 1;

		if (refFromOtherColumns_[curTableColumn][otherTableColumn] <= 0) {
			auto it1 = refFromOtherColumns_[curTableColumn].find(otherTableColumn);
			refFromOtherColumns_[curTableColumn].erase(it1);

			if (refFromOtherColumns_[curTableColumn].empty()) {
				auto it2 = refFromOtherColumns_.find(curTableColumn);
				refFromOtherColumns_.erase(it2);
			}
		}
	}
	else {
		return false;
	}

	return true;
}


bool Table::remove_column_name(name_t name) {
	auto it = find(acceColumnNames_.begin(), acceColumnNames_.end(), name);

	if (it == acceColumnNames_.end()) {
		return false;
	}

	acceColumnNames_.erase(it);

	return true;
}
bool Table::add_column_name(name_t name) {
	auto it = find(acceColumnNames_.begin(), acceColumnNames_.end(), name);
	if (it == acceColumnNames_.end()) {
		acceColumnNames_.push_back(name);
		return true;
	}

	return false;
}
void Table::drop_index(Column* column) {
	vector<Index*> waitForDrop;
	vector<Index*> indexes = get_index(column);

	for (Index* index : indexes) {
		auto it = index->columns_.find(column);
		index->columns_.erase(it);

		if (index->columns_.empty()) {
			waitForDrop.push_back(index);
		}
	}

	for (Index* index : waitForDrop) {
		drop_index(index);
	}

	return;
}
void Table::drop_constraint(Column* column) {
	vector<Constraint*> waitForDrop;
	vector<Constraint*> constraints = get_constraint(column);

	for (Constraint* constraint : constraints) {
		auto it = constraint->columns_.find(column);
		constraint->columns_.erase(column);

		if (constraint->columns_.empty()) {
			waitForDrop.push_back(constraint);
		}
	}

	for (Constraint* constraint : constraints_) {
		drop_constraint(constraint);
	}

	return;
}

bool Table::index_drop_check(Index* index) {
	if (find(indexes_.begin(), indexes_.end(), index) == indexes_.end()) {
		return false;
	}

	for (auto& it : index->columns_) {
		if (refFromOtherColumns_.find(it.first) != refFromOtherColumns_.end()) {
			return false;
		}
	}

	return true;
}

bool Table::constraint_drop_check(Constraint* constraint) {
	if (find(constraints_.begin(), constraints_.end(), constraint) == constraints_.end()) {
		return false;
	}

	return true;
}

//-----------------------------SelectStmtInfo-----------------------------
SelectStmtInfo::SelectStmtInfo(StmtLocation location, int rowNumber, int columnNumber) : location_(location), rowNumber_(rowNumber), columnNumber_(columnNumber) {}
SelectStmtInfo::~SelectStmtInfo() {
	clear_local_info();
	clear_cte();

	return;
}

bool SelectStmtInfo::is_duplicated_table(Table* table) {
	for (Table* t : usedTables_) {
		if (t->identifierName_ == table->identifierName_) {
			return true;
		}
	}

	return false;
}
bool SelectStmtInfo::exist_duplicated_column(vector<Column*>& v1, vector<Column*>& v2) {
	vector<int> bitmap = vector<int>(COLUMNNAMEMAX, 0);

	for (Column* c : v1) {
		bitmap[c->identifierName_]++;
	}

	for (Column* c : v2) {
		bitmap[c->identifierName_]++;
	}

	for (int i : bitmap) {
		if (i > 1) {
			return true;
		}
	}

	return false;
}
bool SelectStmtInfo::has_column_limit() {
	return columnNumber_ != DEFAULTCOLUMNNUMBER;
}
bool SelectStmtInfo::has_row_limit() {
	return rowNumber_ != DEFAULTROWNUMBER;
}

void SelectStmtInfo::add_used_table(Table* table) {
	table->update_duplicationInfo(duplicationInfo_);
	usedTables_.push_back(table);

	return;
}
void SelectStmtInfo::clear_local_info() {
	for (Table* t : usedTables_) {
		delete t;
	}

	for (int i = 0; i < COLUMNNAMEMAX; i++) {
		duplicationInfo_[i] = 0;
	}

	hasAggregate = false;

	usedTables_.clear();	
	usedInnerColumns_.clear();
	acceColumns_.clear();
	windowStatusRecord_.clear();

	return;
}
void SelectStmtInfo::clear_cte() {
	for (auto it = innerCteTables_.begin(); it != innerCteTables_.end(); it++) {
		Table* cte = *it;
		delete cte;
	}

	innerCteTables_.clear();

	return;
}

void SelectStmtInfo::update_acceColumns() {
	acceColumns_.clear();
	map<name_t, vector<Column*>> tmp;

	for (Table* t : usedTables_) {
		t->update_duplicatedColumns(duplicationInfo_);

		for (Column* c : t->columns_) {
			tmp[c->identifierName_].push_back(c);
		}
	}

	for (Column* c : outReferences_) {
		if (tmp.find(c->identifierName_) == tmp.end()) {
			tmp[c->identifierName_].push_back(c);
		}
		else {
			Column* cc = tmp[c->identifierName_][0];

			if (cc->parentTable_->inSelectStmtInfo_ != this) {
				tmp[c->identifierName_].push_back(c);
			}
		}
	}

	for (auto& it : tmp) {
		for (Column* c : it.second) {
			acceColumns_.push_back(c);
		}
	}

	return;
}
void SelectStmtInfo::update_acceColumns(vector<Table*>& usedTables, vector<int>& duplicationInfo) {
	acceColumns_.clear();
	map<name_t, vector<Column*>> tmp;

	for (Table* t : usedTables) {
		t->update_duplicatedColumns(duplicationInfo);

		for (Column* c: t->columns_) {
			tmp[c->identifierName_].push_back(c);
		}
	}

	for (Column* c : outReferences_) {
		if (tmp.find(c->identifierName_) == tmp.end()) {
			tmp[c->identifierName_].push_back(c);
		}
		else {
			Column* cc = tmp[c->identifierName_][0];

			if (cc->parentTable_->inSelectStmtInfo_ != this) {
				tmp[c->identifierName_].push_back(c);
			}
		}
	}

	for (auto& it : tmp) {
		for (Column* c : it.second) {
			acceColumns_.push_back(c);
		}
	}

	return;
}

void SelectStmtInfo::add_usedInnerColumns(Column* column) {
	if (find(usedInnerColumns_.begin(), usedInnerColumns_.end(), column) == usedInnerColumns_.end()) {
		usedInnerColumns_.push_back(column);
	}

	return;
}
void SelectStmtInfo::add_usedOutColumns(Column* column) {
	if (find(usedOutColumns_.begin(), usedOutColumns_.end(), column) == usedOutColumns_.end()) {
		usedOutColumns_.push_back(column);
	}

	return;
}
void SelectStmtInfo::add_out_usedColumns(vector<Column*>& usedColumns) {
	for (Column* c : usedColumns) {
		if (c->parentTable_->inSelectStmtInfo_ != this) {
			add_usedOutColumns(c);
		}
	}

	return;
}
void SelectStmtInfo::add_all_usedColumns(vector<Column*>& usedColumns) {
	for (Column* c : usedColumns) {
		if (c->parentTable_->inSelectStmtInfo_ == this) {
			add_usedInnerColumns(c);
		}
		else {
			add_usedOutColumns(c);
		}
	}

	return;
}

//-----------------------------GlobalStatusManger-----------------------------
GlobalStatusManger::~GlobalStatusManger() {
	for (Table* t : globalTables_) {
		delete t;
	}

	return;
}

void GlobalStatusManger::reset_status() {
	totalTableName_ = 0;
	totalViewName_ = 0;
	totalWindowName_ = 0;
	totalIndexName_ = 0;
	totalConstraintName_ = 0;

	assert(selectInfoStack_.empty());

	for (Table* t : globalTables_) {
		delete t;
	}

	globalTables_.clear();

	return;
}

void GlobalStatusManger::push_selectInfo(StmtLocation location, int rowNumber, int columnNumber) {
	SelectStmtInfo* info = new SelectStmtInfo(location, rowNumber, columnNumber);
	SelectStmtInfo* preInfo = get_cur_selectInfo();

	if (preInfo != nullptr) {
		info->outCteTables_ = preInfo->outCteTables_;
		for (Table* t : preInfo->innerCteTables_) {
			info->outCteTables_.push_back(t);
		}

		info->outReferences_ = preInfo->acceColumns_;

		if (location == StmtLocation::slRecursiveCTE) {
			info->recursiveCteName_ = preInfo->recursiveCteName_;
		}
	}

	selectInfoStack_.push(info);

	return;
}
void GlobalStatusManger::pop_selectInfo() {
	SelectStmtInfo* info = get_cur_selectInfo();
	pop_cur_selectInfo();
	SelectStmtInfo* preinfo = get_cur_selectInfo();

	if (preinfo != nullptr && info != nullptr) {
		if (find(ignoreInnerUsedColumns_.begin(), ignoreInnerUsedColumns_.end(), info->location_) != ignoreInnerUsedColumns_.end()) {
			preinfo->add_out_usedColumns(info->usedOutColumns_);
		}
		else {
			preinfo->add_all_usedColumns(info->usedOutColumns_);
		}		
	}

	if (info != nullptr) {
		delete info;
	}

	return;
}

SelectStmtInfo* GlobalStatusManger::get_cur_selectInfo() {
	if (selectInfoStack_.empty()) {
		return nullptr;
	}		

	return selectInfoStack_.top();
}
void GlobalStatusManger::pop_cur_selectInfo() {
	if (selectInfoStack_.empty()) {
		fprintf(stderr, "ERROR: Pop Empty Stack\n");
		assert(0);
	}

	selectInfoStack_.pop();

	return;
}

int GlobalStatusManger::get_acce_table_number() {
	int res = 0;
	SelectStmtInfo* info = get_cur_selectInfo();

	res += globalTables_.size() + info->outCteTables_.size() + info->innerCteTables_.size();

	return res;
}
Table* GlobalStatusManger::get_acce_table(int index) {
	Table* res = nullptr;
	SelectStmtInfo* info = get_cur_selectInfo();

	if (index < globalTables_.size()) {
		res = globalTables_[index];
	}
	else if(index - globalTables_.size() < info->outCteTables_.size()) {
		res = info->outCteTables_[index - globalTables_.size()];
	}
	else {
		res = info->innerCteTables_[index - globalTables_.size() - info->outCteTables_.size()];
	}

	assert(res != nullptr);

	return res;
}
vector<Table*> GlobalStatusManger::get_acce_table(IdentifierType type) {
	assert(type == IdentifierType::iTable || type == IdentifierType::iView);

	vector<Table*> res;

	for (Table* table : globalTables_) {
		if (table->identifierType_ == type) {
			res.push_back(table);
		}
	}

	return res;
}

name_t GlobalStatusManger::get_new_table_name() {
	return totalTableName_++;
}
name_t GlobalStatusManger::get_new_view_name() {
	return totalViewName_++;
}
name_t GlobalStatusManger::get_new_window_name() {
	return totalWindowName_++;
}
name_t GlobalStatusManger::get_new_index_name() {
	return totalIndexName_++;
}
name_t GlobalStatusManger::get_new_constraint_name() {
	return totalConstraintName_++;
}

bool GlobalStatusManger::add_table(Table* table) {
	if (table == nullptr || find(globalTables_.begin(), globalTables_.end(), table) != globalTables_.end()) {
		return false;
	}

	globalTables_.push_back(table);

	return true;
}
bool GlobalStatusManger::drop_table(Table* table) {
	auto it = find(globalTables_.begin(), globalTables_.end(), table);

	if (it == globalTables_.end()) {
		return false;
	}

	globalTables_.erase(it);
	delete table;

	return true;
}
bool GlobalStatusManger::drop_table(name_t name, IdentifierType type) {
	Table* tmp = nullptr;

	for (Table* table : globalTables_) {
		if (table->identifierName_ == name && table->identifierType_ == type) {
			tmp = table;
			break;
		}
	}

	if (tmp != nullptr) {
		return drop_table(tmp);
	}

	return true;
}

//-----------------------------IRTrim-----------------------------
IRTrim::IRTrim(Instantiator* instantiator): instantiator_(instantiator) {
	instantiate_simple_limit();
	instantiate_simple_column_list();
	instantiate_simple_alias();
	instantiate_simple_expr_root();
	instantiate_simple_expr_root_true();
	instantiate_simple_expr_root_list();
	instantiate_simple_simple_select_item_list();
	instantiate_simple_window_spec();
	instantiate_simple_index_name();
	instantiate_simple_constraint_name();
	instantiate_simple_key_part();
	instantiate_simple_key_part_list();
	instantiate_simple_table_element();
	instantiate_simple_values();

	return;
}
IRTrim::~IRTrim() {
	for (IR* ir : simpleIR_) {
		if (ir != nullptr) {
			deep_delete(ir);
		}			
	}

	return;
}

void IRTrim::instantiate_simple_limit() {
	IR* iconst = new IR(IRTYPE::kIconst, OP0(), nullptr, nullptr);
	IR* limit_options = new IR(IRTYPE::kLimitOptions, OP0(), iconst, nullptr);
	IR* limit_clause = new IR(IRTYPE::kLimitClause, OP3(TERMINAL::tLimit, TERMINAL::tEmpty, TERMINAL::tEmpty), limit_options, nullptr);

	simple_limit_ = limit_clause;
	simpleIR_.push_back(simple_limit_);

	return;
}
void IRTrim::instantiate_simple_column_list() {
	IR* ident = new IR(IRTYPE::kIdent, OP0(), nullptr, nullptr);
	IR* column_name = new IR(IRTYPE::kColumnName, OP0(), ident, nullptr);
	IR* column_list = new IR(IRTYPE::kColumnList, OP0(), column_name, nullptr);

	simple_column_list_ = column_list;
	simpleIR_.push_back(simple_column_list_);

	return;
}
void IRTrim::instantiate_simple_alias() {
	IR* ident = new IR(IRTYPE::kIdent, OP0(), nullptr, nullptr);
	IR* alias = new IR(IRTYPE::kAlias, OP0(), ident, nullptr);

	simple_alias_ = alias;
	simpleIR_.push_back(simple_alias_);

	return;
}
void IRTrim::instantiate_simple_expr_root() {
	IR* iconst = new IR(IRTYPE::kIconst, OP0(), nullptr, nullptr);
	IR* simple_expr = new IR(IRTYPE::kSimpleExpr, OP0(), iconst, nullptr);
	IR* bit_expr = new IR(IRTYPE::kBitExpr, OP0(), simple_expr, nullptr);
	IR* predicate = new IR(IRTYPE::kPredicate, OP0(), bit_expr, nullptr);
	IR* bool_pri = new IR(IRTYPE::kBoolPri, OP0(), predicate, nullptr);
	IR* expr = new IR(IRTYPE::kExpr, OP0(), bool_pri, nullptr);
	IR* expr_root = new IR(IRTYPE::kExprRoot, OP0(), expr, nullptr);

	iconst->long_val_ = 1;

	simple_expr_root_ = expr_root;
	simpleIR_.push_back(simple_expr_root_);

	return;
}
void IRTrim::instantiate_simple_expr_root_true() {
	IR* literal = new IR(IRTYPE::kLiteral, OP3(TERMINAL::tTrue, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
	IR* simple_expr = new IR(IRTYPE::kSimpleExpr, OP0(), literal, nullptr);
	IR* bit_expr = new IR(IRTYPE::kBitExpr, OP0(), simple_expr, nullptr);
	IR* predicate = new IR(IRTYPE::kPredicate, OP0(), bit_expr, nullptr);
	IR* bool_pri = new IR(IRTYPE::kBoolPri, OP0(), predicate, nullptr);
	IR* expr = new IR(IRTYPE::kExpr, OP0(), bool_pri, nullptr);
	IR* expr_root = new IR(IRTYPE::kExprRoot, OP0(), expr, nullptr);
	
	simple_expr_root_true_ = expr_root;
	simpleIR_.push_back(simple_expr_root_true_);

	return;
}
void IRTrim::instantiate_simple_expr_root_list() {
	IR* column_ident = new IR(IRTYPE::kIdent, OP0(), nullptr, nullptr);
	IR* column_name = new IR(IRTYPE::kColumnName, OP0(), column_ident, nullptr);
	IR* columnref = new IR(IRTYPE::kColumnref, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), column_name, nullptr);
	IR* simple_expr = new IR(IRTYPE::kSimpleExpr, OP0(), columnref, nullptr);
	IR* bit_expr = new IR(IRTYPE::kBitExpr, OP0(), simple_expr, nullptr);
	IR* predicate = new IR(IRTYPE::kPredicate, OP0(), bit_expr, nullptr);
	IR* bool_pri = new IR(IRTYPE::kBoolPri, OP0(), predicate, nullptr);
	IR* expr = new IR(IRTYPE::kExpr, OP0(), bool_pri, nullptr);
	IR* exor_root = new IR(IRTYPE::kExprRoot, OP0(), expr, nullptr);
	IR* expr_root_list = new IR(IRTYPE::kExprRootList, OP0(), exor_root, nullptr);

	simple_expr_root_list_ = expr_root_list;
	simpleIR_.push_back(simple_expr_root_list_);

	return;
}
void IRTrim::instantiate_simple_simple_select_item_list() {
	IR* expr_root = deep_copy(simple_expr_root_);
	IR* alise = new IR(IRTYPE::kOptAlias, OP3(TERMINAL::tAs, TERMINAL::tEmpty, TERMINAL::tEmpty), deep_copy(simple_alias_), nullptr);
	IR* select_item = new IR(IRTYPE::kSelectItem, OP0(), expr_root, alise);
	IR* select_item_list = new IR(IRTYPE::kSelectItemList, OP0(), select_item, nullptr);

	simple_select_item_list_ = select_item_list;
	simpleIR_.push_back(simple_select_item_list_);

	return;
}
void IRTrim::instantiate_simple_window_spec() {
	IR* opt_existing_window_name = new IR(IRTYPE::kOptExistingWindowName, OP0(), nullptr, nullptr);
	IR* opt_partition_clause = new IR(IRTYPE::kOptPartitionClause, OP0(), nullptr, nullptr);
	IR* opt_window_order_by_clause = new IR(IRTYPE::kOptWindowOrderByClause, OP0(), nullptr, nullptr);
	IR* opt_window_frame_clause = new IR(IRTYPE::kOptWindowFrameClause, OP0(), nullptr, nullptr);

	IR* tmp1 = new IR(IRTYPE::kWindowSpecDetailsTmp1, OP0(), opt_window_order_by_clause, opt_window_frame_clause);
	IR* tmp2 = new IR(IRTYPE::kWindowSpecDetailsTmp2, OP0(), opt_partition_clause, tmp1);
	IR* window_spec_details = new IR(IRTYPE::kWindowSpecDetails, OP0(), opt_existing_window_name, tmp2);

	IR* window_spec = new IR(IRTYPE::kWindowSpec, OP3(TERMINAL::tOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), window_spec_details, nullptr);

	simple_window_spec_ = window_spec;
	simpleIR_.push_back(simple_window_spec_);

	return;
}
void IRTrim::instantiate_simple_index_name() {
	IR* ident = new IR(IRTYPE::kIdent, OP0(), nullptr, nullptr);
	IR* index_name = new IR(IRTYPE::kIndexName, OP0(), ident, nullptr);

	simple_index_name_ = index_name;
	simpleIR_.push_back(simple_index_name_);

	return;
}
void IRTrim::instantiate_simple_constraint_name() {
	IR* ident = new IR(IRTYPE::kIdent, OP0(), nullptr, nullptr);
	IR* constraint_name = new IR(IRTYPE::kConstraintName, OP0(), ident, nullptr);

	simple_constraint_name_ = constraint_name;
	simpleIR_.push_back(simple_constraint_name_);

	return;
}
void IRTrim::instantiate_simple_key_part() {
	IR* ident = new IR(IRTYPE::kIdent, OP0(), nullptr, nullptr);
	IR* column_name = new IR(IRTYPE::kColumnName, OP0(), ident, nullptr);
	IR* opt_ordering_direction = new IR(IRTYPE::kOptOrderingDirection, OP0(), nullptr, nullptr);
	IR* key_part = new IR(IRTYPE::kKeyPart, OP0(), column_name, opt_ordering_direction);

	simple_key_part_ = key_part;
	simpleIR_.push_back(simple_key_part_);

	return;
}
void IRTrim::instantiate_simple_key_part_list() {
	IR* key_part = deep_copy(simple_key_part_);
	IR* key_list = new IR(IRTYPE::kKeyList, OP0(), key_part, nullptr);

	simple_key_part_list_ = key_list;
	simpleIR_.push_back(simple_key_part_list_);

	return;
}
void IRTrim::instantiate_simple_table_element() {
	IR* int_type = new IR(IRTYPE::kIntType, OP3(TERMINAL::tInt, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
	IR* opt_field_options = new IR(IRTYPE::kOptFieldOptions, OP0(), nullptr, nullptr);
	IR* number_type = new IR(IRTYPE::kNumberType, OP0(), int_type, opt_field_options);
	IR* data_type = new IR(IRTYPE::kDataType, OP0(), number_type, nullptr);
	IR* opt_column_attribute_list = new IR(IRTYPE::kOptColumnAttributeList, OP0(), nullptr, nullptr);
	IR* field_def = new IR(IRTYPE::kFieldDef, OP0(), data_type, opt_column_attribute_list);

	IR* ident = new IR(IRTYPE::kIdent, OP0(), nullptr, nullptr);
	IR* column_name = new IR(IRTYPE::kColumnName, OP0(), ident, nullptr);
	IR* column_def = new IR(IRTYPE::kColumnDef, OP0(), column_name, field_def);
	
	IR* table_element = new IR(IRTYPE::kTableElement, OP0(), column_def, nullptr);

	simple_table_element_ = table_element;
	simpleIR_.push_back(simple_table_element_);

	return;
}
void IRTrim::instantiate_simple_values() {
	IR* expr_or_default = new IR(IRTYPE::kExprOrDefault, OP3(TERMINAL::tDefault, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
	IR* values = new IR(IRTYPE::kValues, OP0(), expr_or_default, nullptr);

	simple_values_ = values;
	simpleIR_.push_back(simple_values_);

	return;
}

IR* IRTrim::get_expr_root_columnref(Column* column) {
	IR* tmp1 = nullptr;

	if (column->duplicated_ == ColumnStatus::csDuplication) {
		IR* table_ident = new IR(IRTYPE::kIdent, OP0(), nullptr, nullptr);
		IR* table_name = new IR(IRTYPE::kTableName, OP0(), table_ident, nullptr);

		instantiator_->instantiate_ident(table_name, IdentifierType::iTable, column->parentTable_->identifierName_);

		IR* column_ident = new IR(IRTYPE::kIdent, OP0(), nullptr, nullptr);
		IR* column_name = new IR(IRTYPE::kColumnName, OP0(), column_ident, nullptr);

		instantiator_->instantiate_ident(column_name, IdentifierType::iColumn, column->identifierName_);

		tmp1 = new IR(IRTYPE::kColumnref, OP3(TERMINAL::tEmpty, TERMINAL::tOpDot, TERMINAL::tEmpty), table_name, column_name);
	}
	else {
		IR* column_ident = new IR(IRTYPE::kIdent, OP0(), nullptr, nullptr);
		IR* column_name = new IR(IRTYPE::kColumnName, OP0(), column_ident, nullptr);

		instantiator_->instantiate_ident(column_name, IdentifierType::iColumn, column->identifierName_);

		tmp1 = new IR(IRTYPE::kColumnref, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), column_name, nullptr);
	}

	IR* tmp2 = new IR(IRTYPE::kSimpleExpr, OP0(), tmp1, nullptr);
	IR* tmp3 = new IR(IRTYPE::kBitExpr, OP0(), tmp2, nullptr);
	IR* tmp4 = new IR(IRTYPE::kPredicate, OP0(), tmp3, nullptr);
	IR* tmp5 = new IR(IRTYPE::kBoolPri, OP0(), tmp4, nullptr);
	IR* tmp6 = new IR(IRTYPE::kExpr, OP0(), tmp5, nullptr);
	IR* tmp7 = new IR(IRTYPE::kExprRoot, OP0(), tmp6, nullptr);

	return tmp7;
}
void IRTrim::set_recursive_limit(IR* expr_root) {
	IR* number_tmp1 = new IR(IRTYPE::kIconst, OP0(), nullptr, nullptr);
	number_tmp1->long_val_ = rand() % LIMITMAX;

	IR* number_tmp2 = new IR(IRTYPE::kSimpleExpr, OP0(), number_tmp1, nullptr);
	IR* number_tmp3 = new IR(IRTYPE::kBitExpr, OP0(), number_tmp2, nullptr);
	IR* number_tmp4 = new IR(IRTYPE::kPredicate, OP0(), number_tmp3, nullptr);


	IR* column_ident = new IR(IRTYPE::kIdent, OP0(), nullptr, nullptr);
	IR* column_name = new IR(IRTYPE::kColumnName, OP0(), column_ident, nullptr);

	instantiator_->instantiate_ident(column_name, IdentifierType::iColumn, RESERVEDCOLUMNNAME);

	IR* tmp1 = new IR(IRTYPE::kColumnref, OP0(), column_name, nullptr);
	IR* tmp2 = new IR(IRTYPE::kSimpleExpr, OP0(), tmp1, nullptr);
	IR* tmp3 = new IR(IRTYPE::kBitExpr, OP0(), tmp2, nullptr);
	IR* tmp4 = new IR(IRTYPE::kPredicate, OP0(), tmp3, nullptr);
	IR* tmp5 = new IR(IRTYPE::kBoolPri, OP0(), tmp4, nullptr);

	IR* comp_op = new IR(IRTYPE::kCompOp, OP3(TERMINAL::tOpLessthan, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);

	IR* bool_pri_tmp1 = new IR(IRTYPE::kBoolPriTmp1, OP0(), comp_op, number_tmp4);
	IR* bool_pri = new IR(IRTYPE::kBoolPri, OP0(), tmp5, bool_pri_tmp1);

	IR* expr_right = new IR(IRTYPE::kExpr, OP0(), bool_pri, nullptr);
	IR* expr_left = NT_check(expr_root->left_, IRTYPE::kExpr);

	IR* new_expr = new IR(IRTYPE::kExpr, OP3(TERMINAL::tEmpty, TERMINAL::tAnd, TERMINAL::tEmpty), expr_left, expr_right);

	expr_root->left_ = new_expr;

	return;
}