#ifndef __INSTANTIATE_H__
#define __INSTANTIATE_H__

#include <stdio.h>
#include <ctype.h>
#include <stack>
#include <cassert>
#include <vector>
#include <stdint.h>
#include <random>
#include <string>
#include <fstream>
#include <algorithm>
#include <climits>
#include <map>
#include <chrono>
#include <cmath>

#include "ast.h"
#include "mutate.h"

using namespace std;

typedef uint8_t name_t;

#define UNREFERENCE static_cast<name_t>(-1)
#define COLUMNNAMEMAX 64
#define RESERVEDCOLUMNNAME (COLUMNNAMEMAX - 1)

#define LIMITMIN 1
#define LIMITMAX 128
#define OFFSETMIN 0
#define OFFSETMAX 16

#define DEFAULTROWNUMBER -1
#define DEFAULTCOLUMNNUMBER -1

#define MINSTRINGLENGTH 0
#define MAXSTRINGLENGTH 64

#define CAPACITYBASE 80
#define MAXCAPACITY 120
#define CAPACITYGROWTHRATE 1.02
#define TIMETHRESHOLD (3600 * 1000)

#ifdef DEBUG
#define NT_check(ir, type) (assert(ir->type_ == type), ir)
#else
#define NT_check(ir, type) ir
#endif

enum class IdentifierType {
	iTable,
	iColumn,
	iWindow,
	iIndex,
	iConstraint,
	iView
};
enum class ColumnStatus {
	csNormal,
	csDuplication
};
enum class ColumnType {
	ctNull, ctInt, ctTinyInt, ctSmallInt, ctMediumInt, ctBigInt,
	ctReal, ctDouble, ctFloat, ctDecimal, ctNumeric, ctFixed,
	ctBool, ctBoolean,
	ctChar, ctBinary, ctVarchar, ctVarbinary, ctLongVarbinary, 
	ctTinyBlob, ctMediumBlob, ctLongBlob, ctBlob,
	ctTinyText, ctMediumText, ctLongText, ctText
};
enum class TypeCompatibility {
	tcNoCompatibility,
	tcWeakCompatibility,
	tcFullyCompatibility
};
enum class IndexType {
	itNormal,
	itFunctional
};
enum class ConstraintType {
	ctNull,
	ctUnique,
	ctPrimaryKey,
	ctCheck,
	ctForeignKey
};
enum class StmtLocation {
	slNotSubquery,
	slSubquery,
	slCTE,
	slRecursiveCTE,
	slRecursivePart1,
	slRecursivePart2,
	slTableReference,
	slWhereClause,
	slGroupClause,
	slTargetList,	
	slHavingClause,
	slWindowClause,
	slOrderClause
};
enum class WindowStatus {
	wsNoInherit,
	wsInherit,
	wsCantBeInherited,
	wsWithOrder
};
enum class ColumnBasicType {
	ctString,
	ctNumber,
	ctBool
};

class Table;
class SelectStmtInfo;
class Instantiator;
class Constraint;

class Column {
public:
	Column(name_t name, ColumnType type);
	Column(Column* column);

	bool notNull_ = false;
	bool hasDefault_ = false;
	bool isVisible_ = true;
	name_t identifierName_;
	ColumnStatus duplicated_ = ColumnStatus::csNormal;
	ColumnType columnType_;
	Table* parentTable_ = nullptr;

	static vector<vector<TypeCompatibility>> compatibility_;
	static map<TERMINAL, ColumnType> typeTranslation_;
	static map<ColumnType, ColumnBasicType> typeToBasicType_;

	static void instantiate();
};

class Index {
public:
	Index(name_t name);

	name_t identifierName_;
	Constraint* constraint_ = nullptr;
	map<Column*, IndexType> columns_;
};

class Constraint {
public:
	Constraint(name_t name, ConstraintType type);

	name_t identifierName_;
	ConstraintType constraintType_;
	Index* index_ = nullptr;
	map<Column*, IndexType> columns_;
	map<Column*, vector<Column*>> refToOtherColumns_;
};

class Table {
public:
	Table(name_t name, IdentifierType identifierType, SelectStmtInfo* inSelectStmtInfo);
	Table(Table* otherTable, SelectStmtInfo* inSelectStmtInfo);
	~Table();

	bool isSubQuery_ = false;
	bool isTemporary_ = false;
	bool hasPrimaryKey_ = false;
	name_t identifierName_;
	IdentifierType identifierType_;
	SelectStmtInfo* inSelectStmtInfo_ = nullptr;

	vector<name_t> acceColumnNames_;
	vector<Column*> columns_;

	vector<Index*> indexes_;

	vector<Constraint*> constraints_;
	map<Column*, map<Column*, int>> refFromOtherColumns_;

	bool check_drop_table();
	bool exist_duplicated_column();
	void update_duplicationInfo(vector<int>& duplicationInfo);
	void update_duplicatedColumns(vector<int>& duplicationInfo);

	bool column_drop_check(Column* column);
	Column* get_column();
	Column* get_column(ColumnType type);
	name_t get_acce_column_name();
	vector<Column*> get_column_seq();
	void add_column_first(Column* column);
	bool add_column(Column* column, Column* afterColumn = nullptr);
	void move_column_first(Column* column);
	bool move_column(Column* column, Column* afterColumn);
	bool drop_column(Column* column);

	void column_rename(Column* column, name_t oldName, name_t newName);

	Index* get_index();
	vector<Index*> get_index(Column* column);
	bool add_index(Index* index);
	bool drop_index(Index* index);

	Constraint* get_constraint();
	vector<Constraint*> get_constraint(Column* column);
	bool add_constraint(Constraint* constraint, Index* index = nullptr);
	bool drop_constraint(Constraint* constraint);
	bool set_references(Column* curTableColumn, Column* otherTableColumn);
	bool remove_references(Column* curTableColumn, Column* otherTableColumn);

	bool remove_column_name(name_t name);	
	bool add_column_name(name_t name);
	void drop_index(Column* column);
	void drop_constraint(Column* column);

	bool index_drop_check(Index* index);

	bool constraint_drop_check(Constraint* constraint);
};

class SelectStmtInfo {
public:
	SelectStmtInfo(StmtLocation location, int rowNumber, int columnNumber);
	~SelectStmtInfo();

	bool hasAggregate = false;

	int columnNumber_;
	int rowNumber_;

	name_t recursiveCteName_ = UNREFERENCE;
	Table* recursiveCte_ = nullptr;

	StmtLocation location_;

	vector<Table*> innerCteTables_;
	vector<Table*> outCteTables_;

	vector<Column*> outReferences_;

	vector<Column*> usedOutColumns_;
	vector<Column*> usedInnerColumns_;

	vector<Table*> usedTables_;
	vector<Column*> acceColumns_;	

	map<name_t, WindowStatus> windowStatusRecord_;
	
	bool is_duplicated_table(Table* table);
	bool exist_duplicated_column(vector<Column*>& v1, vector<Column*>& v2);
	bool has_column_limit();
	bool has_row_limit();

	void add_used_table(Table* table);
	void clear_local_info();
	void clear_cte();

	void update_acceColumns();
	void update_acceColumns(vector<Table*>& usedTables, vector<int>& duplicationInfo);

	void add_usedInnerColumns(Column* column);
	void add_usedOutColumns(Column* column);
	void add_out_usedColumns(vector<Column*>& usedColumns);
	void add_all_usedColumns(vector<Column*>& usedColumns);

private:
	vector<int> duplicationInfo_ = vector<int>(COLUMNNAMEMAX, 0);
};

class GlobalStatusManger {
public:
	~GlobalStatusManger();			

	stack<SelectStmtInfo*> selectInfoStack_;

	vector<StmtLocation> ignoreInnerUsedColumns_ = { StmtLocation::slWhereClause, StmtLocation::slTableReference, StmtLocation::slOrderClause };

	void reset_status();

	void push_selectInfo(StmtLocation location, int rowNumber, int columnNumber);
	void pop_selectInfo();

	SelectStmtInfo* get_cur_selectInfo();

	int get_acce_table_number();
	Table* get_acce_table(int index);
	vector<Table*> get_acce_table(IdentifierType type);

	name_t get_new_table_name();
	name_t get_new_view_name();
	name_t get_new_window_name();
	name_t get_new_index_name();
	name_t get_new_constraint_name();

	bool add_table(Table* table);
	bool drop_table(Table* table);
	bool drop_table(name_t name, IdentifierType type);

private:
	name_t totalTableName_ = 0;
	name_t totalViewName_ = 0;
	name_t totalWindowName_ = 0;
	name_t totalIndexName_ = 0;
	name_t totalConstraintName_ = 0;

	vector<Table*> globalTables_;

	void pop_cur_selectInfo();
};

class IRTrim {
public:
	IRTrim(Instantiator* instantiator);
	~IRTrim();

	void instantiate_simple_limit();
	void instantiate_simple_column_list();
	void instantiate_simple_alias();
	void instantiate_simple_expr_root();
	void instantiate_simple_expr_root_true();
	void instantiate_simple_expr_root_list();
	void instantiate_simple_simple_select_item_list();
	void instantiate_simple_window_spec();
	void instantiate_simple_index_name();
	void instantiate_simple_constraint_name();
	void instantiate_simple_key_part();
	void instantiate_simple_key_part_list();
	void instantiate_simple_table_element();
	void instantiate_simple_values();

	IR* get_expr_root_columnref(Column* column);
	void set_recursive_limit(IR* expr_root);

	IR* simple_limit_ = nullptr;
	IR* simple_column_list_ = nullptr;
	IR* simple_alias_ = nullptr;
	IR* simple_expr_root_ = nullptr;
	IR* simple_expr_root_true_ = nullptr;
	IR* simple_expr_root_list_ = nullptr;
	IR* simple_select_item_list_ = nullptr;
	IR* simple_window_spec_ = nullptr;
	IR* simple_index_name_ = nullptr;
	IR* simple_constraint_name_ = nullptr;
	IR* simple_key_part_ = nullptr;
	IR* simple_key_part_list_ = nullptr;
	IR* simple_table_element_ = nullptr;
	IR* simple_values_ = nullptr;

	IR* expr_root_columnref_ = nullptr;

private:
	vector<IR*> simpleIR_;
	Instantiator* instantiator_;
};

class ExprInstantiator {
public:
	ExprInstantiator(Instantiator* m): instantiator_(m){}

	vector<Column*> usedColumnsBuffer_;

	void instantiate_expr_root(IR* expr_root, vector<Column*>& acceColumns, StmtLocation location);
	void instantiate_expr(IR* expr, vector<Column*>& acceColumns, StmtLocation location);
	void instantiate_simple_expr(IR* simple_expr, vector<Column*>& acceColumns, StmtLocation location);
	void instantiate_type_cast(IR* type_cast, vector<Column*>& acceColumns, StmtLocation location);
	void instantiate_function(IR* function, vector<Column*>& acceColumns, StmtLocation location);
	void instantiate_aggregate_function(IR* aggregate_function, vector<Column*>& acceColumns, StmtLocation location);
	void instantiate_window_function(IR* window_function, vector<Column*>& acceColumns, StmtLocation location);

	long int get_random_integer(long int min = LONG_MIN, long int max = LONG_MAX);
	double get_random_float(int min = INT_MIN, int max = INT_MAX);
	string get_random_string(int minLength, int maxLength);

private:
	Instantiator* instantiator_;
	vector<IRTYPE> ignoreSimpleExpr_ = { IRTYPE::kSimpleExpr, IRTYPE::kCaseExpr, IRTYPE::kExpr, IRTYPE::kLiteral };
};

class Instantiator {
public:
	Instantiator();
	~Instantiator();
	
	chrono::time_point<chrono::steady_clock> startTime;
	unsigned int get_duration_ms();

	bool instantaite_sql(IR* root);
	int calc_capacity(IR* root);
	int calc_node(IR* root);

	IRTrim* IRTrim_;
	GlobalStatusManger* globalStatusManger_;
	ExprInstantiator* exprInstantiator_;

	void instantiate_parse_toplevel(IR* parse_toplevel);

	void instantiate_alter_table_stmt(IR* alter_table_stmt);
	void instantiate_alter_list_item(IR* alter_list_item, Table* table);

	void instantiate_insert_stmt(IR* insert_stmt);
	void instantiate_values_list(IR* values_list, int chosenColumnNumber);
	void instantiate_values(IR* values, int chosenColumnNumber);

	void instantiate_update_stmt(IR* update_stmt);
	int instantiate_update_list(IR* update_list, Table* table);

	void instantiate_delete_stmt(IR* delete_stmt);

	void instantiate_drop_table_or_view_stmt(IR* drop_table_stmt, IdentifierType type);

	void instantiate_drop_index_stmt(IR* drop_index_stmt);

	void instantiate_create_index_stmt(IR* create_index_stmt);

	void instantiate_create_view_stmt(IR* create_view_stmt);

	void instantiate_create_table_stmt(IR* create_table_stmt);
	void instantiate_table_element_list(IR* table_element_list, Table* table);
	pair<Column*, vector<IR*>> instantiate_column_def(IR* column_def, Table* table);	
	ColumnType instantiate_data_type(IR* data_type);
	void instantiate_column_attribute(IR* column_attribute, Column* column, Table* table);
	map<Column*, IndexType> instantiate_key_list_with_expression(IR* key_list_with_expression, Table* table, ConstraintType type);
	bool instantiate_table_constraint_def_index(IR* table_constraint_def, Table* table);
	bool instantiate_table_constraint_def_key(IR* table_constraint_def, Table* table);
	bool instantiate_table_constraint_def_check(IR* table_constraint_def, Table* table);
	bool instantiate_table_constraint_def_foreign_key(IR* table_constraint_def, Table* table);

	void instantiate_select_stmt(IR* select_stmt);
	Table* instantiate_query_expression(IR* subquery, StmtLocation location, int rowNumber, int columnNumber);
	Table* instantiate_query_expression(IR* query_expression);
	Table* instantiate_query_primary(IR* query_primary);
	Table* instanitate_union(IR* query_expression_body);

	void instantiate_with_clause(IR* with_clause);
	void instantiate_order_clause(IR* order_clause);
	void instantiate_limit_clause(IR* limit_clause);

	Table* instantiate_select_item_list(IR* select_item_list);
	void instantiate_from_clause(IR* from_clause);
	vector<Table*> instantiate_table_reference(IR* table_reference);
	Table* instantiate_single_table(IR* single_table);
	Table* instantiate_derived_table(IR* derived_table);
	vector<Table*> instantiate_joined_table(IR* joined_table);
	void instantiate_where_clause(IR* where_clause);
	void instantiate_group_clause(IR* group_clause);
	void instantiate_having_clause(IR* having_clause);
	void instantiate_window_clause(IR* window_clause);
	void instantiate_window_definition(IR* window_definition);
	WindowStatus instantiate_window_spec(IR* window_spec);

	void instantiate_column_list(IR* column_list, vector<name_t>& columns);
	void instantiate_column_list(IR* column_list, vector<Column*>& columns);
	void instantiate_key_list(IR* key_list, vector<Column*>& columns);
	void instantiate_ident(IR* identifier_name, IdentifierType type, name_t name);

	bool check_foreign_key(Table* table, vector<Column*>& curTableColumn, vector<Column*>& otherTableColumn);
};

#endif
