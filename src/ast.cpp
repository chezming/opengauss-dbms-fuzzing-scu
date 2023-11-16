#include "../include/ast.h"
#include "../include/define.h"
#include <cassert>

using namespace std;

//TRIM**SUBSTRING**BIT_AND**BIT_OR**BIT_XOR**COUNT**MIN**MAX**SUM**STD**STDDEV_SAMP**VARIANCE**VAR_SAMP**GROUP_CONCAT**

const char* convertToTerminal[] = {
	";",
	"FROM",
	"DELETE",
	"",
	"QUICK",
	"LOW_PRIORITY",
	"IGNORE",
	"ALTER TABLE",
	",",
	"ADD COLUMN",
	")",
	"ADD COLUMN (",
	"ADD",
	"CHANGE COLUMN",
	"MODIFY COLUMN",
	"DROP COLUMN",
	"DROP FOREIGN KEY",
	"DROP PRIMARY KEY",
	"DROP INDEX",
	"DROP CHECK",
	"DROP CONSTRAINT",
	"SET DEFAULT (",
	"ALTER COLUMN",
	"DROP DEFAULT",
	"SET",
	"ALTER INDEX",
	"RENAME TO",
	"TO",
	"RENAME INDEX",
	"RENAME COLUMN",
	"AFTER",
	"FIRST",
	"ON",
	"DROP TABLE",
	"IF EXISTS",
	"DROP VIEW",
	"UPDATE",
	"LIMIT",
	"INTO",
	"INSERT",
	"(",
	"VALUES",
	") VALUES",
	"DEFAULT",
	"AS",
	"ON DUPLICATE KEY UPDATE",
	"=",
	"DELAYED",
	"HIGH_PRIORITY",
	"INDEX",
	"CREATE",
	"UNIQUE",
	"VIEW",
	"OR REPLACE",
	"ALGORITHM = UNDEFINED",
	"ALGORITHM = MERGE",
	"ALGORITHM = TEMPTABLE",
	"TABLE",
	"TEMPORARY",
	"IF NOT EXISTS",
	"REPLACE",
	"CHAR",
	"BINARY",
	"VARCHAR",
	"VARBINARY",
	"LONG VARBINARY",
	"TINYBLOB",
	"MEDIUMBLOB",
	"LONGBLOB",
	"BLOB",
	"TINYTEXT",
	"MEDIUMTEXT",
	"LONGTEXT",
	"TEXT",
	"BOOL",
	"BOOLEAN",
	"FLOAT",
	"DECIMAL",
	"NUMERIC",
	"FIXED",
	"REAL",
	"DOUBLE",
	"INT",
	"TINYINT",
	"SMALLINT",
	"MEDIUMINT",
	"BIGINT",
	"SIGNED",
	"UNSIGNED",
	"ZEROFILL",
	"DEFAULT (",
	"NOT NULL",
	"VISIBLE",
	"INVISIBLE",
	"CONSTRAINT",
	"CHECK (",
	"FOREIGN KEY",
	"REFERENCES",
	"ON UPDATE",
	"ON DELETE",
	"CASCADE",
	"SET NULL",
	"NO ACTION",
	"SET DEFAULT",
	"PRIMARY KEY",
	"UNION",
	"EXCEPT",
	"INTERSECT",
	"SELECT",
	"WINDOW",
	"PARTITION BY",
	"ORDER BY",
	"ASC",
	"DESC",
	"ROWS",
	"RANGE",
	"UNBOUNDED PRECEDING",
	"PRECEDING",
	"CURRENT ROW",
	"AND",
	"BETWEEN",
	"UNBOUNDED FOLLOWING",
	"FOLLOWING",
	"HAVING",
	"GROUP BY",
	"WITH ROLLUP",
	"WHERE",
	"LATERAL",
	"USING (",
	"JOIN",
	"INNER JOIN",
	"CROSS JOIN",
	"STRAIGHT_JOIN",
	"NATURAL",
	"NATURAL RIGHT",
	"NATURAL LEFT",
	"INNER",
	"OUTER",
	"LEFT",
	"RIGHT",
	"*",
	". *",
	"SQL_NO_CACHE",
	"DISTINCT",
	"SQL_SMALL_RESULT",
	"SQL_BIG_RESULT",
	"SQL_BUFFER_RESULT",
	"SQL_CALC_FOUND_ROWS",
	"ALL",
	"WITH",
	"WITH RECURSIVE",
	"OR",
	"XOR",
	"NOT",
	"IS TRUE",
	"IS NOT TRUE",
	"IS FALSE",
	"IS NOT FALSE",
	"IS UNKNOWN",
	"IS NOT UNKNOWN",
	"IS NULL",
	"IS NOT NULL",
	"IN (",
	"NOT IN (",
	"NOT BETWEEN",
	"SOUNDS LIKE",
	"LIKE",
	"NOT LIKE",
	"REGEXP",
	"NOT REGEXP",
	"|",
	"&",
	"<<",
	">>",
	"+",
	"-",
	"/",
	"%",
	"^",
	"~",
	"EXISTS",
	"ASCII (",
	"BIN (",
	"BIT_LENGTH (",
	"CHAR_LENGTH (",
	"LENGTH (",
	"LOWER (",
	"LTRIM (",
	"OCT (",
	"ORD (",
	"QUOTE (",
	"REVERSE (",
	"RTRIM (",
	"SPACE (",
	"TRIM(",
	"TRIM( LEADING FROM",
	"TRIM( TRAILING FROM",
	"UNHEX (",
	"UPPER (",
	"FIND_IN_SET (",
	"INSTR (",
	"LEFT (",
	"LOCATE (",
	"REPEAT (",
	"RIGHT (",
	"SUBSTRING(",
	"TRIM( LEADING",
	"TRIM( TRAILING",
	"EXPORT_SET (",
	"LPAD (",
	"REPLACE (",
	"RPAD (",
	"SUBSTRING_INDEX (",
	"INSERT (",
	"CHAR (",
	"CONCAT (",
	"CONCAT_WS (",
	"ELT (",
	"FIELD (",
	"MAKE_SET (",
	"PI ( )",
	"RAND ( )",
	"ABS (",
	"ACOS (",
	"ASIN (",
	"ATAN (",
	"CEILING (",
	"COS (",
	"COT (",
	"DEGREES (",
	"EXP (",
	"FLOOR (",
	"HEX (",
	"LN (",
	"RADIANS (",
	"RAND (",
	"ROUND (",
	"SIGN (",
	"SIN (",
	"SQRT (",
	"TAN (",
	"FORMAT (",
	"LOG (",
	"MOD (",
	"POW (",
	"TRUNCATE (",
	"CONV (",
	"ROW_NUMBER ( )",
	"RANK ( )",
	"DENSE_RANK ( )",
	"CUME_DIST ( )",
	"PERCENT_RANK ( )",
	"NTILE (",
	"LEAD (",
	"LAG (",
	"FIRST_VALUE (",
	"LAST_VALUE (",
	"NTH_VALUE (",
	"RESPECT NULLS",
	"IGNORE NULLS",
	"FROM FIRST",
	"FROM LAST",
	"AVG (",
	"BIT_AND(",
	"BIT_OR(",
	"BIT_XOR(",
	"COUNT( * )",
	"COUNT(",
	"COUNT( DISTINCT",
	"MIN(",
	"MAX(",
	"SUM(",
	"STD(",
	"STDDEV_SAMP(",
	"VARIANCE(",
	"VAR_SAMP(",
	"GROUP_CONCAT(",
	"SEPARATOR",
	"OVER",
	"IF (",
	"IFNULL (",
	"NULLIF (",
	"END",
	"CASE",
	"THEN",
	"WHEN",
	"ELSE",
	"CAST (",
	"CONVERT (",
	"NCHAR",
	"NULL",
	"FALSE",
	"TRUE",
	".",
	">=",
	">",
	"<=",
	"<",
	"!=",
	"ANY",
	"SOME",
	"IN",
	"NOT IN",
	"ICONST",
	"FCONST",
	"SCONST",
	"IDENT",
};

void deep_delete(IR * root){
	if (!root)
		return;

	if(root->left_) deep_delete(root->left_);
	if(root->right_) deep_delete(root->right_);
    
	if(root->op_) delete root->op_;

	delete root;
}

IR* deep_copy(const IR* root) {
	IR* left = NULL, * right = NULL, * copy_res;

	if (root == nullptr) return nullptr;

	if (root->left_) left = deep_copy(root->left_);
	if (root->right_) right = deep_copy(root->right_);

	copy_res = new IR(root, left, right);

	return copy_res;
}

string IR::to_string(){
    	auto res = to_string_core();
    	trim_string(res);
    	return res;
}

string IR::to_string_core(){
	switch (type_) {
		case kIconst: 
			return std::to_string(long_val_) + " ";
		case kFconst: 
			return std::to_string(float_val_) + " ";
		case kSconst: 
			return str_val_ + " ";
		case kIdent: 
			return str_val_ + " ";
	}

	string res;

	if (op_->prefix_ != tEmpty)
		res += string(convertToTerminal[op_->prefix_]) + " ";
	if (left_ != NULL)
		res += left_->to_string_core();
	if (op_->middle_ != tEmpty)
		res += string(convertToTerminal[op_->middle_]) + " ";
	if (right_ != NULL)
		res += right_->to_string_core();
	if (op_->suffix_ != tEmpty)
		res += string(convertToTerminal[op_->suffix_]) + " ";

	return res;
}

void IR::trim_string(string& res) {
   	char preChar = 0;
   	int count = 0;

	for (int i = 0; i < res.size(); i++) {
		if ((res[i] == ' ') && (preChar == ';'))
			res[i] = '\n';

		preChar = res[i];
	}

	return;
}

IR* ParseToplevel::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(stmtmulti_);
	res = new IR(IRTYPE::kParseToplevel, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void ParseToplevel::deep_delete() {
	SAFEDELETE(stmtmulti_);
	delete this;
}

IR* Stmtmulti::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(stmt_);
			auto tmp2 = SAFETRANSLATE(stmtmulti_);
			res = new IR(IRTYPE::kStmtmulti, OP3(TERMINAL::tEmpty, TERMINAL::tOpSemi, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(stmt_);
			res = new IR(IRTYPE::kStmtmulti, OP3(TERMINAL::tEmpty, TERMINAL::tOpSemi, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void Stmtmulti::deep_delete() {
	SAFEDELETE(stmtmulti_);
	SAFEDELETE(stmt_);
	delete this;
}

IR* Stmt::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(create_table_stmt_);
			res = new IR(IRTYPE::kStmt, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(create_view_stmt_);
			res = new IR(IRTYPE::kStmt, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(create_index_stmt_);
			res = new IR(IRTYPE::kStmt, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(select_stmt_);
			res = new IR(IRTYPE::kStmt, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(drop_index_stmt_);
			res = new IR(IRTYPE::kStmt, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(5)
			auto tmp1 = SAFETRANSLATE(drop_table_stmt_);
			res = new IR(IRTYPE::kStmt, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(6)
			auto tmp1 = SAFETRANSLATE(drop_view_stmt_);
			res = new IR(IRTYPE::kStmt, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(7)
			auto tmp1 = SAFETRANSLATE(delete_stmt_);
			res = new IR(IRTYPE::kStmt, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(8)
			auto tmp1 = SAFETRANSLATE(update_stmt_);
			res = new IR(IRTYPE::kStmt, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(9)
			auto tmp1 = SAFETRANSLATE(insert_stmt_);
			res = new IR(IRTYPE::kStmt, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(10)
			auto tmp1 = SAFETRANSLATE(alter_table_stmt_);
			res = new IR(IRTYPE::kStmt, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void Stmt::deep_delete() {
	SAFEDELETE(create_table_stmt_);
	SAFEDELETE(create_view_stmt_);
	SAFEDELETE(create_index_stmt_);
	SAFEDELETE(select_stmt_);
	SAFEDELETE(drop_index_stmt_);
	SAFEDELETE(drop_table_stmt_);
	SAFEDELETE(drop_view_stmt_);
	SAFEDELETE(delete_stmt_);
	SAFEDELETE(update_stmt_);
	SAFEDELETE(insert_stmt_);
	SAFEDELETE(alter_table_stmt_);
	delete this;
}

IR* DeleteStmt::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(opt_with_clause_);
			auto tmp2 = SAFETRANSLATE(opt_delete_options_);
			auto tmp3 = SAFETRANSLATE(table_name_);
			auto tmp4 = SAFETRANSLATE(opt_alias_);
			auto tmp5 = SAFETRANSLATE(opt_where_clause_);
			auto tmp6 = SAFETRANSLATE(opt_order_clause_);
			auto tmp7 = SAFETRANSLATE(opt_simple_limit_);
			auto tmp8 = new IR(IRTYPE::kDeleteStmtTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp6, tmp7);
			auto tmp9 = new IR(IRTYPE::kDeleteStmtTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp5, tmp8);
			auto tmp10 = new IR(IRTYPE::kDeleteStmtTmp3, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp4, tmp9);
			auto tmp11 = new IR(IRTYPE::kDeleteStmtTmp4, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp3, tmp10);
			auto tmp12 = new IR(IRTYPE::kDeleteStmtTmp5, OP3(TERMINAL::tEmpty, TERMINAL::tFrom, TERMINAL::tEmpty), tmp2, tmp11);
			res = new IR(IRTYPE::kDeleteStmt, OP3(TERMINAL::tEmpty, TERMINAL::tDelete, TERMINAL::tEmpty), tmp1, tmp12);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(opt_with_clause_);
			auto tmp2 = SAFETRANSLATE(opt_delete_options_);
			auto tmp3 = SAFETRANSLATE(table_list_);
			auto tmp4 = SAFETRANSLATE(table_reference_list_);
			auto tmp5 = SAFETRANSLATE(opt_where_clause_);
			auto tmp6 = new IR(IRTYPE::kDeleteStmtTmp6, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp4, tmp5);
			auto tmp7 = new IR(IRTYPE::kDeleteStmtTmp7, OP3(TERMINAL::tEmpty, TERMINAL::tFrom, TERMINAL::tEmpty), tmp3, tmp6);
			auto tmp8 = new IR(IRTYPE::kDeleteStmtTmp8, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp7);
			res = new IR(IRTYPE::kDeleteStmt, OP3(TERMINAL::tEmpty, TERMINAL::tDelete, TERMINAL::tEmpty), tmp1, tmp8);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void DeleteStmt::deep_delete() {
	SAFEDELETE(opt_simple_limit_);
	SAFEDELETE(opt_order_clause_);
	SAFEDELETE(opt_where_clause_);
	SAFEDELETE(opt_alias_);
	SAFEDELETE(table_name_);
	SAFEDELETE(opt_delete_options_);
	SAFEDELETE(opt_with_clause_);
	SAFEDELETE(table_reference_list_);
	SAFEDELETE(table_list_);
	delete this;
}

IR* OptDeleteOptions::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptDeleteOptions, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(delete_options_);
			res = new IR(IRTYPE::kOptDeleteOptions, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptDeleteOptions::deep_delete() {
	SAFEDELETE(delete_options_);
	delete this;
}

IR* DeleteOptions::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(delete_option_);
			res = new IR(IRTYPE::kDeleteOptions, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(delete_option_);
			auto tmp2 = SAFETRANSLATE(delete_options_);
			res = new IR(IRTYPE::kDeleteOptions, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void DeleteOptions::deep_delete() {
	SAFEDELETE(delete_option_);
	SAFEDELETE(delete_options_);
	delete this;
}

IR* DeleteOption::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kDeleteOption, OP3(TERMINAL::tQuick, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kDeleteOption, OP3(TERMINAL::tLowPriority, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kDeleteOption, OP3(TERMINAL::tIgnore, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void DeleteOption::deep_delete() {
	delete this;
}

IR* AlterTableStmt::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(table_name_);
	auto tmp2 = SAFETRANSLATE(alter_list_);
	res = new IR(IRTYPE::kAlterTableStmt, OP3(TERMINAL::tAlterTable, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);

	TRANSLATEEND
}

void AlterTableStmt::deep_delete() {
	SAFEDELETE(alter_list_);
	SAFEDELETE(table_name_);
	delete this;
}

IR* AlterList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(alter_list_item_);
			res = new IR(IRTYPE::kAlterList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(alter_list_item_);
			auto tmp2 = SAFETRANSLATE(alter_list_);
			res = new IR(IRTYPE::kAlterList, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void AlterList::deep_delete() {
	SAFEDELETE(alter_list_item_);
	SAFEDELETE(alter_list_);
	delete this;
}

IR* AlterListItem::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(column_name_1_);
			auto tmp2 = SAFETRANSLATE(field_def_);
			auto tmp3 = SAFETRANSLATE(opt_place_);
			auto tmp4 = new IR(IRTYPE::kAlterListItemTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tAddColumn, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(table_element_list_);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tAddColumnOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(table_constraint_def_);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tAdd, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(column_name_1_);
			auto tmp2 = SAFETRANSLATE(column_name_2_);
			auto tmp3 = SAFETRANSLATE(field_def_);
			auto tmp4 = SAFETRANSLATE(opt_place_);
			auto tmp5 = new IR(IRTYPE::kAlterListItemTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp3, tmp4);
			auto tmp6 = new IR(IRTYPE::kAlterListItemTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp5);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tChangeColumn, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp6);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(column_name_1_);
			auto tmp2 = SAFETRANSLATE(field_def_);
			auto tmp3 = SAFETRANSLATE(opt_place_);
			auto tmp4 = new IR(IRTYPE::kAlterListItemTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tModifyColumn, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(5)
			auto tmp1 = SAFETRANSLATE(column_name_1_);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tDropColumn, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(6)
			auto tmp1 = SAFETRANSLATE(constraint_name_);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tDropForeignKey, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(7)
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tDropPrimaryKey, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(8)
			auto tmp1 = SAFETRANSLATE(index_name_1_);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tDropIndex, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(9)
			auto tmp1 = SAFETRANSLATE(constraint_name_);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tDropCheck, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(10)
			auto tmp1 = SAFETRANSLATE(constraint_name_);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tDropConstraint, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(11)
			auto tmp1 = SAFETRANSLATE(column_name_1_);
			auto tmp2 = SAFETRANSLATE(expr_root_);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tAlterColumn, TERMINAL::tSetDefaultOpLp, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(12)
			auto tmp1 = SAFETRANSLATE(column_name_1_);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tAlterColumn, TERMINAL::tDropDefault, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(13)
			auto tmp1 = SAFETRANSLATE(column_name_1_);
			auto tmp2 = SAFETRANSLATE(visibility_);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tAlterColumn, TERMINAL::tSet, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(14)
			auto tmp1 = SAFETRANSLATE(index_name_1_);
			auto tmp2 = SAFETRANSLATE(visibility_);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tAlterIndex, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(15)
			auto tmp1 = SAFETRANSLATE(table_name_);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tRenameTo, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(16)
			auto tmp1 = SAFETRANSLATE(index_name_1_);
			auto tmp2 = SAFETRANSLATE(index_name_2_);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tRenameIndex, TERMINAL::tTo, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(17)
			auto tmp1 = SAFETRANSLATE(column_name_1_);
			auto tmp2 = SAFETRANSLATE(column_name_2_);
			res = new IR(IRTYPE::kAlterListItem, OP3(TERMINAL::tRenameColumn, TERMINAL::tTo, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void AlterListItem::deep_delete() {
	SAFEDELETE(opt_place_);
	SAFEDELETE(field_def_);
	SAFEDELETE(column_name_1_);
	SAFEDELETE(column_name_2_);
	SAFEDELETE(table_element_list_);
	SAFEDELETE(table_constraint_def_);
	SAFEDELETE(constraint_name_);
	SAFEDELETE(index_name_1_);
	SAFEDELETE(index_name_2_);
	SAFEDELETE(expr_root_);
	SAFEDELETE(visibility_);
	SAFEDELETE(table_name_);
	delete this;
}

IR* OptPlace::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptPlace, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(column_name_);
			res = new IR(IRTYPE::kOptPlace, OP3(TERMINAL::tAfter, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kOptPlace, OP3(TERMINAL::tFirst, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptPlace::deep_delete() {
	SAFEDELETE(column_name_);
	delete this;
}

IR* DropIndexStmt::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(index_name_);
	auto tmp2 = SAFETRANSLATE(table_name_);
	res = new IR(IRTYPE::kDropIndexStmt, OP3(TERMINAL::tDropIndex, TERMINAL::tOn, TERMINAL::tEmpty), tmp1, tmp2);

	TRANSLATEEND
}

void DropIndexStmt::deep_delete() {
	SAFEDELETE(table_name_);
	SAFEDELETE(index_name_);
	delete this;
}

IR* DropTableStmt::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(if_exists_);
	auto tmp2 = SAFETRANSLATE(table_list_);
	res = new IR(IRTYPE::kDropTableStmt, OP3(TERMINAL::tDropTable, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);

	TRANSLATEEND
}

void DropTableStmt::deep_delete() {
	SAFEDELETE(table_list_);
	SAFEDELETE(if_exists_);
	delete this;
}

IR* IfExists::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kIfExists, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kIfExists, OP3(TERMINAL::tIfExists, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void IfExists::deep_delete() {
	delete this;
}

IR* TableList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(table_name_);
			res = new IR(IRTYPE::kTableList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(table_name_);
			auto tmp2 = SAFETRANSLATE(table_list_);
			res = new IR(IRTYPE::kTableList, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void TableList::deep_delete() {
	SAFEDELETE(table_name_);
	SAFEDELETE(table_list_);
	delete this;
}

IR* DropViewStmt::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(if_exists_);
	auto tmp2 = SAFETRANSLATE(table_list_);
	res = new IR(IRTYPE::kDropViewStmt, OP3(TERMINAL::tDropView, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);

	TRANSLATEEND
}

void DropViewStmt::deep_delete() {
	SAFEDELETE(table_list_);
	SAFEDELETE(if_exists_);
	delete this;
}

IR* UpdateStmt::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(opt_with_clause_);
	auto tmp2 = SAFETRANSLATE(opt_low_priority_);
	auto tmp3 = SAFETRANSLATE(opt_ignore_);
	auto tmp4 = SAFETRANSLATE(table_reference_list_);
	auto tmp5 = SAFETRANSLATE(update_list_);
	auto tmp6 = SAFETRANSLATE(opt_where_clause_);
	auto tmp7 = SAFETRANSLATE(opt_order_clause_);
	auto tmp8 = SAFETRANSLATE(opt_simple_limit_);
	auto tmp9 = new IR(IRTYPE::kUpdateStmtTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp7, tmp8);
	auto tmp10 = new IR(IRTYPE::kUpdateStmtTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp6, tmp9);
	auto tmp11 = new IR(IRTYPE::kUpdateStmtTmp3, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp5, tmp10);
	auto tmp12 = new IR(IRTYPE::kUpdateStmtTmp4, OP3(TERMINAL::tEmpty, TERMINAL::tSet, TERMINAL::tEmpty), tmp4, tmp11);
	auto tmp13 = new IR(IRTYPE::kUpdateStmtTmp5, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp3, tmp12);
	auto tmp14 = new IR(IRTYPE::kUpdateStmtTmp6, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp13);
	res = new IR(IRTYPE::kUpdateStmt, OP3(TERMINAL::tEmpty, TERMINAL::tUpdate, TERMINAL::tEmpty), tmp1, tmp14);

	TRANSLATEEND
}

void UpdateStmt::deep_delete() {
	SAFEDELETE(opt_simple_limit_);
	SAFEDELETE(opt_order_clause_);
	SAFEDELETE(opt_where_clause_);
	SAFEDELETE(update_list_);
	SAFEDELETE(table_reference_list_);
	SAFEDELETE(opt_ignore_);
	SAFEDELETE(opt_low_priority_);
	SAFEDELETE(opt_with_clause_);
	delete this;
}

IR* OptSimpleLimit::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptSimpleLimit, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(iconst_);
			res = new IR(IRTYPE::kOptSimpleLimit, OP3(TERMINAL::tLimit, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptSimpleLimit::deep_delete() {
	SAFEDELETE(iconst_);
	delete this;
}

IR* OptWithClause::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptWithClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(with_clause_);
			res = new IR(IRTYPE::kOptWithClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptWithClause::deep_delete() {
	SAFEDELETE(with_clause_);
	delete this;
}

IR* OptLowPriority::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptLowPriority, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOptLowPriority, OP3(TERMINAL::tLowPriority, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptLowPriority::deep_delete() {
	delete this;
}

IR* InsertStmt::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(insert_lock_option_);
			auto tmp2 = SAFETRANSLATE(opt_ignore_);
			auto tmp3 = SAFETRANSLATE(table_name_);
			auto tmp4 = SAFETRANSLATE(insert_from_constructor_);
			auto tmp5 = SAFETRANSLATE(opt_values_reference_);
			auto tmp6 = SAFETRANSLATE(opt_insert_update_list_);
			auto tmp7 = new IR(IRTYPE::kInsertStmtTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp5, tmp6);
			auto tmp8 = new IR(IRTYPE::kInsertStmtTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp4, tmp7);
			auto tmp9 = new IR(IRTYPE::kInsertStmtTmp3, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp3, tmp8);
			auto tmp10 = new IR(IRTYPE::kInsertStmtTmp4, OP3(TERMINAL::tEmpty, TERMINAL::tInto, TERMINAL::tEmpty), tmp2, tmp9);
			res = new IR(IRTYPE::kInsertStmt, OP3(TERMINAL::tInsert, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp10);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(insert_lock_option_);
			auto tmp2 = SAFETRANSLATE(opt_ignore_);
			auto tmp3 = SAFETRANSLATE(table_name_);
			auto tmp4 = SAFETRANSLATE(update_list_);
			auto tmp5 = SAFETRANSLATE(opt_values_reference_);
			auto tmp6 = SAFETRANSLATE(opt_insert_update_list_);
			auto tmp7 = new IR(IRTYPE::kInsertStmtTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp5, tmp6);
			auto tmp8 = new IR(IRTYPE::kInsertStmtTmp5, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp4, tmp7);
			auto tmp9 = new IR(IRTYPE::kInsertStmtTmp6, OP3(TERMINAL::tEmpty, TERMINAL::tSet, TERMINAL::tEmpty), tmp3, tmp8);
			auto tmp10 = new IR(IRTYPE::kInsertStmtTmp7, OP3(TERMINAL::tEmpty, TERMINAL::tInto, TERMINAL::tEmpty), tmp2, tmp9);
			res = new IR(IRTYPE::kInsertStmt, OP3(TERMINAL::tInsert, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp10);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(insert_lock_option_);
			auto tmp2 = SAFETRANSLATE(opt_ignore_);
			auto tmp3 = SAFETRANSLATE(table_name_);
			auto tmp4 = SAFETRANSLATE(insert_query_expression_);
			auto tmp5 = SAFETRANSLATE(opt_insert_update_list_);
			auto tmp6 = new IR(IRTYPE::kInsertStmtTmp8, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp4, tmp5);
			auto tmp7 = new IR(IRTYPE::kInsertStmtTmp9, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp3, tmp6);
			auto tmp8 = new IR(IRTYPE::kInsertStmtTmp10, OP3(TERMINAL::tEmpty, TERMINAL::tInto, TERMINAL::tEmpty), tmp2, tmp7);
			res = new IR(IRTYPE::kInsertStmt, OP3(TERMINAL::tInsert, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp8);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void InsertStmt::deep_delete() {
	SAFEDELETE(opt_insert_update_list_);
	SAFEDELETE(opt_values_reference_);
	SAFEDELETE(insert_from_constructor_);
	SAFEDELETE(table_name_);
	SAFEDELETE(opt_ignore_);
	SAFEDELETE(insert_lock_option_);
	SAFEDELETE(update_list_);
	SAFEDELETE(insert_query_expression_);
	delete this;
}

IR* InsertQueryExpression::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(query_expression_);
			res = new IR(IRTYPE::kInsertQueryExpression, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(column_list_);
			auto tmp2 = SAFETRANSLATE(query_expression_);
			res = new IR(IRTYPE::kInsertQueryExpression, OP3(TERMINAL::tOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void InsertQueryExpression::deep_delete() {
	SAFEDELETE(query_expression_);
	SAFEDELETE(column_list_);
	delete this;
}

IR* InsertFromConstructor::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(values_list_);
			res = new IR(IRTYPE::kInsertFromConstructor, OP3(TERMINAL::tValues, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(column_list_);
			auto tmp2 = SAFETRANSLATE(values_list_);
			res = new IR(IRTYPE::kInsertFromConstructor, OP3(TERMINAL::tOpLp, TERMINAL::tOpRpValues, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void InsertFromConstructor::deep_delete() {
	SAFEDELETE(values_list_);
	SAFEDELETE(column_list_);
	delete this;
}

IR* ValuesList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(row_value_);
			res = new IR(IRTYPE::kValuesList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(row_value_);
			auto tmp2 = SAFETRANSLATE(values_list_);
			res = new IR(IRTYPE::kValuesList, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void ValuesList::deep_delete() {
	SAFEDELETE(row_value_);
	SAFEDELETE(values_list_);
	delete this;
}

IR* RowValue::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(opt_values_);
	res = new IR(IRTYPE::kRowValue, OP3(TERMINAL::tOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void RowValue::deep_delete() {
	SAFEDELETE(opt_values_);
	delete this;
}

IR* OptValues::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptValues, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(values_);
			res = new IR(IRTYPE::kOptValues, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptValues::deep_delete() {
	SAFEDELETE(values_);
	delete this;
}

IR* Values::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(expr_or_default_);
			res = new IR(IRTYPE::kValues, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_or_default_);
			auto tmp2 = SAFETRANSLATE(values_);
			res = new IR(IRTYPE::kValues, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void Values::deep_delete() {
	SAFEDELETE(expr_or_default_);
	SAFEDELETE(values_);
	delete this;
}

IR* ExprOrDefault::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(expr_root_);
			res = new IR(IRTYPE::kExprOrDefault, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kExprOrDefault, OP3(TERMINAL::tDefault, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void ExprOrDefault::deep_delete() {
	SAFEDELETE(expr_root_);
	delete this;
}

IR* OptValuesReference::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptValuesReference, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(table_name_);
			auto tmp2 = SAFETRANSLATE(opt_column_list_);
			res = new IR(IRTYPE::kOptValuesReference, OP3(TERMINAL::tAs, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptValuesReference::deep_delete() {
	SAFEDELETE(opt_column_list_);
	SAFEDELETE(table_name_);
	delete this;
}

IR* OptInsertUpdateList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptInsertUpdateList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(update_list_);
			res = new IR(IRTYPE::kOptInsertUpdateList, OP3(TERMINAL::tOnDuplicateKeyUpdate, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptInsertUpdateList::deep_delete() {
	SAFEDELETE(update_list_);
	delete this;
}

IR* UpdateList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(update_elem_);
			res = new IR(IRTYPE::kUpdateList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(update_elem_);
			auto tmp2 = SAFETRANSLATE(update_list_);
			res = new IR(IRTYPE::kUpdateList, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void UpdateList::deep_delete() {
	SAFEDELETE(update_elem_);
	SAFEDELETE(update_list_);
	delete this;
}

IR* UpdateElem::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(columnref_);
	auto tmp2 = SAFETRANSLATE(expr_or_default_);
	res = new IR(IRTYPE::kUpdateElem, OP3(TERMINAL::tEmpty, TERMINAL::tOpEqual, TERMINAL::tEmpty), tmp1, tmp2);

	TRANSLATEEND
}

void UpdateElem::deep_delete() {
	SAFEDELETE(expr_or_default_);
	SAFEDELETE(columnref_);
	delete this;
}

IR* InsertLockOption::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kInsertLockOption, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kInsertLockOption, OP3(TERMINAL::tLowPriority, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kInsertLockOption, OP3(TERMINAL::tDelayed, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(3)
			res = new IR(IRTYPE::kInsertLockOption, OP3(TERMINAL::tHighPriority, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void InsertLockOption::deep_delete() {
	delete this;
}

IR* OptIgnore::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptIgnore, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOptIgnore, OP3(TERMINAL::tIgnore, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptIgnore::deep_delete() {
	delete this;
}

IR* CreateIndexStmt::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(opt_unique_);
	auto tmp2 = SAFETRANSLATE(index_name_);
	auto tmp3 = SAFETRANSLATE(table_name_);
	auto tmp4 = SAFETRANSLATE(key_list_with_expression_);
	auto tmp5 = new IR(IRTYPE::kCreateIndexStmtTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpLp, TERMINAL::tOpRp), tmp3, tmp4);
	auto tmp6 = new IR(IRTYPE::kCreateIndexStmtTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tOn, TERMINAL::tEmpty), tmp2, tmp5);
	res = new IR(IRTYPE::kCreateIndexStmt, OP3(TERMINAL::tCreate, TERMINAL::tIndex, TERMINAL::tEmpty), tmp1, tmp6);

	TRANSLATEEND
}

void CreateIndexStmt::deep_delete() {
	SAFEDELETE(key_list_with_expression_);
	SAFEDELETE(table_name_);
	SAFEDELETE(index_name_);
	SAFEDELETE(opt_unique_);
	delete this;
}

IR* OptUnique::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptUnique, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOptUnique, OP3(TERMINAL::tUnique, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptUnique::deep_delete() {
	delete this;
}

IR* CreateViewStmt::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(opt_or_replace_);
	auto tmp2 = SAFETRANSLATE(opt_view_algorithm_);
	auto tmp3 = SAFETRANSLATE(view_name_);
	auto tmp4 = SAFETRANSLATE(opt_column_list_);
	auto tmp5 = SAFETRANSLATE(query_expression_);
	auto tmp6 = new IR(IRTYPE::kCreateViewStmtTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tAs, TERMINAL::tEmpty), tmp4, tmp5);
	auto tmp7 = new IR(IRTYPE::kCreateViewStmtTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp3, tmp6);
	auto tmp8 = new IR(IRTYPE::kCreateViewStmtTmp3, OP3(TERMINAL::tEmpty, TERMINAL::tView, TERMINAL::tEmpty), tmp2, tmp7);
	res = new IR(IRTYPE::kCreateViewStmt, OP3(TERMINAL::tCreate, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp8);

	TRANSLATEEND
}

void CreateViewStmt::deep_delete() {
	SAFEDELETE(query_expression_);
	SAFEDELETE(opt_column_list_);
	SAFEDELETE(view_name_);
	SAFEDELETE(opt_view_algorithm_);
	SAFEDELETE(opt_or_replace_);
	delete this;
}

IR* OptOrReplace::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptOrReplace, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOptOrReplace, OP3(TERMINAL::tOrReplace, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptOrReplace::deep_delete() {
	delete this;
}

IR* OptViewAlgorithm::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptViewAlgorithm, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOptViewAlgorithm, OP3(TERMINAL::tAlgorithmOpEqualUndefined, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kOptViewAlgorithm, OP3(TERMINAL::tAlgorithmOpEqualMerge, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(3)
			res = new IR(IRTYPE::kOptViewAlgorithm, OP3(TERMINAL::tAlgorithmOpEqualTemptable, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptViewAlgorithm::deep_delete() {
	delete this;
}

IR* CreateTableStmt::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(opt_temporary_);
			auto tmp2 = SAFETRANSLATE(opt_if_not_exists_);
			auto tmp3 = SAFETRANSLATE(table_name_);
			auto tmp4 = SAFETRANSLATE(table_element_list_);
			auto tmp5 = SAFETRANSLATE(opt_duplicate_as_qe_);
			auto tmp6 = new IR(IRTYPE::kCreateTableStmtTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp4, tmp5);
			auto tmp7 = new IR(IRTYPE::kCreateTableStmtTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tOpLp, TERMINAL::tEmpty), tmp3, tmp6);
			auto tmp8 = new IR(IRTYPE::kCreateTableStmtTmp3, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp7);
			res = new IR(IRTYPE::kCreateTableStmt, OP3(TERMINAL::tCreate, TERMINAL::tTable, TERMINAL::tEmpty), tmp1, tmp8);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(opt_temporary_);
			auto tmp2 = SAFETRANSLATE(opt_if_not_exists_);
			auto tmp3 = SAFETRANSLATE(table_name_);
			auto tmp4 = SAFETRANSLATE(duplicate_as_qe_);
			auto tmp5 = new IR(IRTYPE::kCreateTableStmtTmp4, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp3, tmp4);
			auto tmp6 = new IR(IRTYPE::kCreateTableStmtTmp5, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp5);
			res = new IR(IRTYPE::kCreateTableStmt, OP3(TERMINAL::tCreate, TERMINAL::tTable, TERMINAL::tEmpty), tmp1, tmp6);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void CreateTableStmt::deep_delete() {
	SAFEDELETE(opt_duplicate_as_qe_);
	SAFEDELETE(table_element_list_);
	SAFEDELETE(table_name_);
	SAFEDELETE(opt_if_not_exists_);
	SAFEDELETE(opt_temporary_);
	SAFEDELETE(duplicate_as_qe_);
	delete this;
}

IR* OptTemporary::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptTemporary, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOptTemporary, OP3(TERMINAL::tTemporary, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptTemporary::deep_delete() {
	delete this;
}

IR* OptIfNotExists::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptIfNotExists, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOptIfNotExists, OP3(TERMINAL::tIfNotExists, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptIfNotExists::deep_delete() {
	delete this;
}

IR* OptDuplicateAsQe::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptDuplicateAsQe, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(duplicate_as_qe_);
			res = new IR(IRTYPE::kOptDuplicateAsQe, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptDuplicateAsQe::deep_delete() {
	SAFEDELETE(duplicate_as_qe_);
	delete this;
}

IR* DuplicateAsQe::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(duplicate_);
			auto tmp2 = SAFETRANSLATE(query_expression_);
			res = new IR(IRTYPE::kDuplicateAsQe, OP3(TERMINAL::tEmpty, TERMINAL::tAs, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(query_expression_);
			res = new IR(IRTYPE::kDuplicateAsQe, OP3(TERMINAL::tAs, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void DuplicateAsQe::deep_delete() {
	SAFEDELETE(query_expression_);
	SAFEDELETE(duplicate_);
	delete this;
}

IR* Duplicate::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kDuplicate, OP3(TERMINAL::tReplace, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kDuplicate, OP3(TERMINAL::tIgnore, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void Duplicate::deep_delete() {
	delete this;
}

IR* TableElementList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(table_element_);
			res = new IR(IRTYPE::kTableElementList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(table_element_);
			auto tmp2 = SAFETRANSLATE(table_element_list_);
			res = new IR(IRTYPE::kTableElementList, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void TableElementList::deep_delete() {
	SAFEDELETE(table_element_);
	SAFEDELETE(table_element_list_);
	delete this;
}

IR* TableElement::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(column_def_);
			res = new IR(IRTYPE::kTableElement, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(table_constraint_def_);
			res = new IR(IRTYPE::kTableElement, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void TableElement::deep_delete() {
	SAFEDELETE(column_def_);
	SAFEDELETE(table_constraint_def_);
	delete this;
}

IR* ColumnDef::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(column_name_);
	auto tmp2 = SAFETRANSLATE(field_def_);
	res = new IR(IRTYPE::kColumnDef, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);

	TRANSLATEEND
}

void ColumnDef::deep_delete() {
	SAFEDELETE(field_def_);
	SAFEDELETE(column_name_);
	delete this;
}

IR* FieldDef::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(data_type_);
	auto tmp2 = SAFETRANSLATE(opt_column_attribute_list_);
	res = new IR(IRTYPE::kFieldDef, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);

	TRANSLATEEND
}

void FieldDef::deep_delete() {
	SAFEDELETE(opt_column_attribute_list_);
	SAFEDELETE(data_type_);
	delete this;
}

IR* DataType::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(number_type_);
			res = new IR(IRTYPE::kDataType, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(bool_type_);
			res = new IR(IRTYPE::kDataType, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(string_type_);
			res = new IR(IRTYPE::kDataType, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void DataType::deep_delete() {
	SAFEDELETE(number_type_);
	SAFEDELETE(bool_type_);
	SAFEDELETE(string_type_);
	delete this;
}

IR* StringType::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(opt_field_length_);
			res = new IR(IRTYPE::kStringType, OP3(TERMINAL::tChar, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(opt_field_length_);
			res = new IR(IRTYPE::kStringType, OP3(TERMINAL::tBinary, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(field_length_);
			res = new IR(IRTYPE::kStringType, OP3(TERMINAL::tVarchar, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(field_length_);
			res = new IR(IRTYPE::kStringType, OP3(TERMINAL::tVarbinary, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(4)
			res = new IR(IRTYPE::kStringType, OP3(TERMINAL::tLongVarbinary, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(5)
			res = new IR(IRTYPE::kStringType, OP3(TERMINAL::tTinyblob, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(6)
			res = new IR(IRTYPE::kStringType, OP3(TERMINAL::tMediumblob, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(7)
			res = new IR(IRTYPE::kStringType, OP3(TERMINAL::tLongblob, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(8)
			auto tmp1 = SAFETRANSLATE(opt_field_length_);
			res = new IR(IRTYPE::kStringType, OP3(TERMINAL::tBlob, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(9)
			res = new IR(IRTYPE::kStringType, OP3(TERMINAL::tTinytext, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(10)
			res = new IR(IRTYPE::kStringType, OP3(TERMINAL::tMediumtext, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(11)
			res = new IR(IRTYPE::kStringType, OP3(TERMINAL::tLongtext, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(12)
			res = new IR(IRTYPE::kStringType, OP3(TERMINAL::tText, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void StringType::deep_delete() {
	SAFEDELETE(opt_field_length_);
	SAFEDELETE(field_length_);
	delete this;
}

IR* BoolType::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kBoolType, OP3(TERMINAL::tBool, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kBoolType, OP3(TERMINAL::tBoolean, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void BoolType::deep_delete() {
	delete this;
}

IR* NumberType::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(int_type_);
			auto tmp2 = SAFETRANSLATE(opt_field_options_);
			res = new IR(IRTYPE::kNumberType, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(real_type_);
			auto tmp2 = SAFETRANSLATE(opt_precision_);
			auto tmp3 = SAFETRANSLATE(opt_field_options_);
			auto tmp4 = new IR(IRTYPE::kNumberTypeTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kNumberType, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(numeric_type_);
			auto tmp2 = SAFETRANSLATE(float_options_);
			auto tmp3 = SAFETRANSLATE(opt_field_options_);
			auto tmp4 = new IR(IRTYPE::kNumberTypeTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kNumberType, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void NumberType::deep_delete() {
	SAFEDELETE(opt_field_options_);
	SAFEDELETE(int_type_);
	SAFEDELETE(opt_precision_);
	SAFEDELETE(real_type_);
	SAFEDELETE(float_options_);
	SAFEDELETE(numeric_type_);
	delete this;
}

IR* NumericType::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kNumericType, OP3(TERMINAL::tFloat, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kNumericType, OP3(TERMINAL::tDecimal, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kNumericType, OP3(TERMINAL::tNumeric, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(3)
			res = new IR(IRTYPE::kNumericType, OP3(TERMINAL::tFixed, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void NumericType::deep_delete() {
	delete this;
}

IR* RealType::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kRealType, OP3(TERMINAL::tReal, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kRealType, OP3(TERMINAL::tDouble, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void RealType::deep_delete() {
	delete this;
}

IR* OptPrecision::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptPrecision, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(precision_);
			res = new IR(IRTYPE::kOptPrecision, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptPrecision::deep_delete() {
	SAFEDELETE(precision_);
	delete this;
}

IR* IntType::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kIntType, OP3(TERMINAL::tInt, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kIntType, OP3(TERMINAL::tTinyint, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kIntType, OP3(TERMINAL::tSmallint, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(3)
			res = new IR(IRTYPE::kIntType, OP3(TERMINAL::tMediumint, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(4)
			res = new IR(IRTYPE::kIntType, OP3(TERMINAL::tBigint, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void IntType::deep_delete() {
	delete this;
}

IR* OptFieldOptions::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptFieldOptions, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(field_option_list_);
			res = new IR(IRTYPE::kOptFieldOptions, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptFieldOptions::deep_delete() {
	SAFEDELETE(field_option_list_);
	delete this;
}

IR* FieldOptionList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(field_option_);
			res = new IR(IRTYPE::kFieldOptionList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(field_option_);
			auto tmp2 = SAFETRANSLATE(field_option_list_);
			res = new IR(IRTYPE::kFieldOptionList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void FieldOptionList::deep_delete() {
	SAFEDELETE(field_option_);
	SAFEDELETE(field_option_list_);
	delete this;
}

IR* FieldOption::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kFieldOption, OP3(TERMINAL::tSigned, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kFieldOption, OP3(TERMINAL::tUnsigned, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kFieldOption, OP3(TERMINAL::tZerofill, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void FieldOption::deep_delete() {
	delete this;
}

IR* OptColumnAttributeList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptColumnAttributeList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(column_attribute_list_);
			res = new IR(IRTYPE::kOptColumnAttributeList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptColumnAttributeList::deep_delete() {
	SAFEDELETE(column_attribute_list_);
	delete this;
}

IR* ColumnAttributeList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(column_attribute_);
			res = new IR(IRTYPE::kColumnAttributeList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(column_attribute_);
			auto tmp2 = SAFETRANSLATE(column_attribute_list_);
			res = new IR(IRTYPE::kColumnAttributeList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void ColumnAttributeList::deep_delete() {
	SAFEDELETE(column_attribute_);
	SAFEDELETE(column_attribute_list_);
	delete this;
}

IR* ColumnAttribute::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(not_null_);
			res = new IR(IRTYPE::kColumnAttribute, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(default_attribute_);
			res = new IR(IRTYPE::kColumnAttribute, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(visibility_);
			res = new IR(IRTYPE::kColumnAttribute, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void ColumnAttribute::deep_delete() {
	SAFEDELETE(not_null_);
	SAFEDELETE(default_attribute_);
	SAFEDELETE(visibility_);
	delete this;
}

IR* DefaultAttribute::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(expr_root_);
	res = new IR(IRTYPE::kDefaultAttribute, OP3(TERMINAL::tDefaultOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void DefaultAttribute::deep_delete() {
	SAFEDELETE(expr_root_);
	delete this;
}

IR* NotNull::translate() {
	TRANSLATESTART

	res = new IR(IRTYPE::kNotNull, OP3(TERMINAL::tNotNull, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);

	TRANSLATEEND
}

void NotNull::deep_delete() {
	delete this;
}

IR* Visibility::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kVisibility, OP3(TERMINAL::tVisible, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kVisibility, OP3(TERMINAL::tInvisible, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void Visibility::deep_delete() {
	delete this;
}

IR* OptConstraintName::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptConstraintName, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(constraint_name_);
			res = new IR(IRTYPE::kOptConstraintName, OP3(TERMINAL::tConstraint, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptConstraintName::deep_delete() {
	SAFEDELETE(constraint_name_);
	delete this;
}

IR* CheckConstraint::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(expr_root_);
	res = new IR(IRTYPE::kCheckConstraint, OP3(TERMINAL::tCheckOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void CheckConstraint::deep_delete() {
	SAFEDELETE(expr_root_);
	delete this;
}

IR* TableConstraintDef::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(opt_index_name_);
			auto tmp2 = SAFETRANSLATE(key_list_with_expression_);
			res = new IR(IRTYPE::kTableConstraintDef, OP3(TERMINAL::tIndex, TERMINAL::tOpLp, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(opt_constraint_name_);
			auto tmp2 = SAFETRANSLATE(constraint_key_type_);
			auto tmp3 = SAFETRANSLATE(opt_index_name_);
			auto tmp4 = SAFETRANSLATE(key_list_with_expression_);
			auto tmp5 = new IR(IRTYPE::kTableConstraintDefTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpLp, TERMINAL::tOpRp), tmp3, tmp4);
			auto tmp6 = new IR(IRTYPE::kTableConstraintDefTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp5);
			res = new IR(IRTYPE::kTableConstraintDef, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp6);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(opt_constraint_name_);
			auto tmp2 = SAFETRANSLATE(check_constraint_);
			res = new IR(IRTYPE::kTableConstraintDef, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(opt_constraint_name_);
			auto tmp2 = SAFETRANSLATE(opt_index_name_);
			auto tmp3 = SAFETRANSLATE(key_list_);
			auto tmp4 = SAFETRANSLATE(references_);
			auto tmp5 = new IR(IRTYPE::kTableConstraintDefTmp3, OP3(TERMINAL::tEmpty, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp3, tmp4);
			auto tmp6 = new IR(IRTYPE::kTableConstraintDefTmp4, OP3(TERMINAL::tEmpty, TERMINAL::tOpLp, TERMINAL::tEmpty), tmp2, tmp5);
			res = new IR(IRTYPE::kTableConstraintDef, OP3(TERMINAL::tEmpty, TERMINAL::tForeignKey, TERMINAL::tEmpty), tmp1, tmp6);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void TableConstraintDef::deep_delete() {
	SAFEDELETE(key_list_with_expression_);
	SAFEDELETE(opt_index_name_);
	SAFEDELETE(constraint_key_type_);
	SAFEDELETE(opt_constraint_name_);
	SAFEDELETE(check_constraint_);
	SAFEDELETE(references_);
	SAFEDELETE(key_list_);
	delete this;
}

IR* References::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(table_name_);
	auto tmp2 = SAFETRANSLATE(column_list_);
	auto tmp3 = SAFETRANSLATE(opt_on_update_delete_);
	auto tmp4 = new IR(IRTYPE::kReferencesTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp2, tmp3);
	res = new IR(IRTYPE::kReferences, OP3(TERMINAL::tReferences, TERMINAL::tOpLp, TERMINAL::tEmpty), tmp1, tmp4);

	TRANSLATEEND
}

void References::deep_delete() {
	SAFEDELETE(opt_on_update_delete_);
	SAFEDELETE(column_list_);
	SAFEDELETE(table_name_);
	delete this;
}

IR* OptOnUpdateDelete::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptOnUpdateDelete, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(on_update_option_1_);
			res = new IR(IRTYPE::kOptOnUpdateDelete, OP3(TERMINAL::tOnUpdate, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(on_update_option_1_);
			res = new IR(IRTYPE::kOptOnUpdateDelete, OP3(TERMINAL::tOnDelete, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(on_update_option_1_);
			auto tmp2 = SAFETRANSLATE(on_update_option_2_);
			res = new IR(IRTYPE::kOptOnUpdateDelete, OP3(TERMINAL::tOnUpdate, TERMINAL::tOnDelete, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(on_update_option_1_);
			auto tmp2 = SAFETRANSLATE(on_update_option_2_);
			res = new IR(IRTYPE::kOptOnUpdateDelete, OP3(TERMINAL::tOnDelete, TERMINAL::tOnUpdate, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptOnUpdateDelete::deep_delete() {
	SAFEDELETE(on_update_option_1_);
	SAFEDELETE(on_update_option_2_);
	delete this;
}

IR* OnUpdateOption::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOnUpdateOption, OP3(TERMINAL::tCascade, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOnUpdateOption, OP3(TERMINAL::tSetNull, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kOnUpdateOption, OP3(TERMINAL::tNoAction, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(3)
			res = new IR(IRTYPE::kOnUpdateOption, OP3(TERMINAL::tSetDefault, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OnUpdateOption::deep_delete() {
	delete this;
}

IR* KeyList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(key_part_);
			res = new IR(IRTYPE::kKeyList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(key_part_);
			auto tmp2 = SAFETRANSLATE(key_list_);
			res = new IR(IRTYPE::kKeyList, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void KeyList::deep_delete() {
	SAFEDELETE(key_part_);
	SAFEDELETE(key_list_);
	delete this;
}

IR* KeyListWithExpression::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(key_part_with_expression_);
			res = new IR(IRTYPE::kKeyListWithExpression, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(key_part_with_expression_);
			auto tmp2 = SAFETRANSLATE(key_list_with_expression_);
			res = new IR(IRTYPE::kKeyListWithExpression, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void KeyListWithExpression::deep_delete() {
	SAFEDELETE(key_part_with_expression_);
	SAFEDELETE(key_list_with_expression_);
	delete this;
}

IR* KeyPartWithExpression::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(key_part_);
			res = new IR(IRTYPE::kKeyPartWithExpression, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_root_);
			auto tmp2 = SAFETRANSLATE(opt_ordering_direction_);
			res = new IR(IRTYPE::kKeyPartWithExpression, OP3(TERMINAL::tOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void KeyPartWithExpression::deep_delete() {
	SAFEDELETE(key_part_);
	SAFEDELETE(opt_ordering_direction_);
	SAFEDELETE(expr_root_);
	delete this;
}

IR* KeyPart::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(column_name_);
			auto tmp2 = SAFETRANSLATE(opt_ordering_direction_);
			res = new IR(IRTYPE::kKeyPart, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(column_name_);
			auto tmp2 = SAFETRANSLATE(iconst_);
			auto tmp3 = SAFETRANSLATE(opt_ordering_direction_);
			auto tmp4 = new IR(IRTYPE::kKeyPartTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kKeyPart, OP3(TERMINAL::tEmpty, TERMINAL::tOpLp, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void KeyPart::deep_delete() {
	SAFEDELETE(opt_ordering_direction_);
	SAFEDELETE(column_name_);
	SAFEDELETE(iconst_);
	delete this;
}

IR* ConstraintKeyType::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kConstraintKeyType, OP3(TERMINAL::tPrimaryKey, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kConstraintKeyType, OP3(TERMINAL::tUnique, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void ConstraintKeyType::deep_delete() {
	delete this;
}

IR* OptIndexName::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptIndexName, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(index_name_);
			res = new IR(IRTYPE::kOptIndexName, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptIndexName::deep_delete() {
	SAFEDELETE(index_name_);
	delete this;
}

IR* SelectStmt::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(query_expression_);
	res = new IR(IRTYPE::kSelectStmt, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void SelectStmt::deep_delete() {
	SAFEDELETE(query_expression_);
	delete this;
}

IR* QueryExpression::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(query_expression_body_);
			auto tmp2 = SAFETRANSLATE(opt_order_clause_);
			auto tmp3 = SAFETRANSLATE(opt_limit_clause_);
			auto tmp4 = new IR(IRTYPE::kQueryExpressionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kQueryExpression, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(with_clause_);
			auto tmp2 = SAFETRANSLATE(query_expression_body_);
			auto tmp3 = SAFETRANSLATE(opt_order_clause_);
			auto tmp4 = SAFETRANSLATE(opt_limit_clause_);
			auto tmp5 = new IR(IRTYPE::kQueryExpressionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp3, tmp4);
			auto tmp6 = new IR(IRTYPE::kQueryExpressionTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp5);
			res = new IR(IRTYPE::kQueryExpression, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp6);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void QueryExpression::deep_delete() {
	SAFEDELETE(opt_limit_clause_);
	SAFEDELETE(opt_order_clause_);
	SAFEDELETE(query_expression_body_);
	SAFEDELETE(with_clause_);
	delete this;
}

IR* QueryExpressionBody::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(query_primary_);
			res = new IR(IRTYPE::kQueryExpressionBody, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(query_expression_parens_);
			res = new IR(IRTYPE::kQueryExpressionBody, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(query_expression_body_1_);
			auto tmp2 = SAFETRANSLATE(opt_union_option_);
			auto tmp3 = SAFETRANSLATE(query_expression_body_2_);
			auto tmp4 = new IR(IRTYPE::kQueryExpressionBodyTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kQueryExpressionBody, OP3(TERMINAL::tEmpty, TERMINAL::tUnion, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(query_expression_body_1_);
			auto tmp2 = SAFETRANSLATE(opt_union_option_);
			auto tmp3 = SAFETRANSLATE(query_expression_body_2_);
			auto tmp4 = new IR(IRTYPE::kQueryExpressionBodyTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kQueryExpressionBody, OP3(TERMINAL::tEmpty, TERMINAL::tExcept, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(query_expression_body_1_);
			auto tmp2 = SAFETRANSLATE(opt_union_option_);
			auto tmp3 = SAFETRANSLATE(query_expression_body_2_);
			auto tmp4 = new IR(IRTYPE::kQueryExpressionBodyTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kQueryExpressionBody, OP3(TERMINAL::tEmpty, TERMINAL::tIntersect, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void QueryExpressionBody::deep_delete() {
	SAFEDELETE(query_primary_);
	SAFEDELETE(query_expression_parens_);
	SAFEDELETE(query_expression_body_1_);
	SAFEDELETE(query_expression_body_2_);
	SAFEDELETE(opt_union_option_);
	delete this;
}

IR* QueryPrimary::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(query_specification_);
	res = new IR(IRTYPE::kQueryPrimary, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void QueryPrimary::deep_delete() {
	SAFEDELETE(query_specification_);
	delete this;
}

IR* QuerySpecification::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(opt_select_options_);
	auto tmp2 = SAFETRANSLATE(select_item_list_);
	auto tmp3 = SAFETRANSLATE(opt_from_clause_);
	auto tmp4 = SAFETRANSLATE(opt_where_clause_);
	auto tmp5 = SAFETRANSLATE(opt_group_clause_);
	auto tmp6 = SAFETRANSLATE(opt_having_clause_);
	auto tmp7 = SAFETRANSLATE(opt_window_clause_);
	auto tmp8 = new IR(IRTYPE::kQuerySpecificationTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp6, tmp7);
	auto tmp9 = new IR(IRTYPE::kQuerySpecificationTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp5, tmp8);
	auto tmp10 = new IR(IRTYPE::kQuerySpecificationTmp3, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp4, tmp9);
	auto tmp11 = new IR(IRTYPE::kQuerySpecificationTmp4, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp3, tmp10);
	auto tmp12 = new IR(IRTYPE::kQuerySpecificationTmp5, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp11);
	res = new IR(IRTYPE::kQuerySpecification, OP3(TERMINAL::tSelect, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp12);

	TRANSLATEEND
}

void QuerySpecification::deep_delete() {
	SAFEDELETE(opt_window_clause_);
	SAFEDELETE(opt_having_clause_);
	SAFEDELETE(opt_group_clause_);
	SAFEDELETE(opt_where_clause_);
	SAFEDELETE(opt_from_clause_);
	SAFEDELETE(select_item_list_);
	SAFEDELETE(opt_select_options_);
	delete this;
}

IR* OptWindowClause::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptWindowClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(window_definition_list_);
			res = new IR(IRTYPE::kOptWindowClause, OP3(TERMINAL::tWindow, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptWindowClause::deep_delete() {
	SAFEDELETE(window_definition_list_);
	delete this;
}

IR* WindowDefinitionList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(window_definition_);
			res = new IR(IRTYPE::kWindowDefinitionList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(window_definition_);
			auto tmp2 = SAFETRANSLATE(window_definition_list_);
			res = new IR(IRTYPE::kWindowDefinitionList, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void WindowDefinitionList::deep_delete() {
	SAFEDELETE(window_definition_);
	SAFEDELETE(window_definition_list_);
	delete this;
}

IR* WindowDefinition::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(window_name_);
	auto tmp2 = SAFETRANSLATE(window_spec_);
	res = new IR(IRTYPE::kWindowDefinition, OP3(TERMINAL::tEmpty, TERMINAL::tAs, TERMINAL::tEmpty), tmp1, tmp2);

	TRANSLATEEND
}

void WindowDefinition::deep_delete() {
	SAFEDELETE(window_spec_);
	SAFEDELETE(window_name_);
	delete this;
}

IR* WindowSpec::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(window_spec_details_);
	res = new IR(IRTYPE::kWindowSpec, OP3(TERMINAL::tOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void WindowSpec::deep_delete() {
	SAFEDELETE(window_spec_details_);
	delete this;
}

IR* WindowSpecDetails::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(opt_existing_window_name_);
	auto tmp2 = SAFETRANSLATE(opt_partition_clause_);
	auto tmp3 = SAFETRANSLATE(opt_window_order_by_clause_);
	auto tmp4 = SAFETRANSLATE(opt_window_frame_clause_);
	auto tmp5 = new IR(IRTYPE::kWindowSpecDetailsTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp3, tmp4);
	auto tmp6 = new IR(IRTYPE::kWindowSpecDetailsTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp5);
	res = new IR(IRTYPE::kWindowSpecDetails, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp6);

	TRANSLATEEND
}

void WindowSpecDetails::deep_delete() {
	SAFEDELETE(opt_window_frame_clause_);
	SAFEDELETE(opt_window_order_by_clause_);
	SAFEDELETE(opt_partition_clause_);
	SAFEDELETE(opt_existing_window_name_);
	delete this;
}

IR* OptExistingWindowName::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptExistingWindowName, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(window_name_);
			res = new IR(IRTYPE::kOptExistingWindowName, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptExistingWindowName::deep_delete() {
	SAFEDELETE(window_name_);
	delete this;
}

IR* OptPartitionClause::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptPartitionClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_root_list_);
			res = new IR(IRTYPE::kOptPartitionClause, OP3(TERMINAL::tPartitionBy, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptPartitionClause::deep_delete() {
	SAFEDELETE(expr_root_list_);
	delete this;
}

IR* OptWindowOrderByClause::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptWindowOrderByClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(order_list_);
			res = new IR(IRTYPE::kOptWindowOrderByClause, OP3(TERMINAL::tOrderBy, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptWindowOrderByClause::deep_delete() {
	SAFEDELETE(order_list_);
	delete this;
}

IR* OrderList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(order_expr_);
			res = new IR(IRTYPE::kOrderList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(order_expr_);
			auto tmp2 = SAFETRANSLATE(order_list_);
			res = new IR(IRTYPE::kOrderList, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OrderList::deep_delete() {
	SAFEDELETE(order_expr_);
	SAFEDELETE(order_list_);
	delete this;
}

IR* OrderExpr::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(expr_root_);
	auto tmp2 = SAFETRANSLATE(opt_ordering_direction_);
	res = new IR(IRTYPE::kOrderExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);

	TRANSLATEEND
}

void OrderExpr::deep_delete() {
	SAFEDELETE(opt_ordering_direction_);
	SAFEDELETE(expr_root_);
	delete this;
}

IR* OptOrderingDirection::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptOrderingDirection, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOptOrderingDirection, OP3(TERMINAL::tAsc, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kOptOrderingDirection, OP3(TERMINAL::tDesc, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptOrderingDirection::deep_delete() {
	delete this;
}

IR* OptWindowFrameClause::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptWindowFrameClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(window_frame_units_);
			auto tmp2 = SAFETRANSLATE(window_frame_extent_);
			res = new IR(IRTYPE::kOptWindowFrameClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptWindowFrameClause::deep_delete() {
	SAFEDELETE(window_frame_extent_);
	SAFEDELETE(window_frame_units_);
	delete this;
}

IR* WindowFrameUnits::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kWindowFrameUnits, OP3(TERMINAL::tRows, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kWindowFrameUnits, OP3(TERMINAL::tRange, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void WindowFrameUnits::deep_delete() {
	delete this;
}

IR* WindowFrameExtent::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(window_frame_start_);
			res = new IR(IRTYPE::kWindowFrameExtent, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(window_frame_between_);
			res = new IR(IRTYPE::kWindowFrameExtent, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void WindowFrameExtent::deep_delete() {
	SAFEDELETE(window_frame_start_);
	SAFEDELETE(window_frame_between_);
	delete this;
}

IR* WindowFrameStart::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kWindowFrameStart, OP3(TERMINAL::tUnboundedPreceding, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(iconst_);
			res = new IR(IRTYPE::kWindowFrameStart, OP3(TERMINAL::tEmpty, TERMINAL::tPreceding, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kWindowFrameStart, OP3(TERMINAL::tCurrentRow, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void WindowFrameStart::deep_delete() {
	SAFEDELETE(iconst_);
	delete this;
}

IR* WindowFrameBetween::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(window_frame_start_);
	auto tmp2 = SAFETRANSLATE(window_frame_bound_);
	res = new IR(IRTYPE::kWindowFrameBetween, OP3(TERMINAL::tBetween, TERMINAL::tAnd, TERMINAL::tEmpty), tmp1, tmp2);

	TRANSLATEEND
}

void WindowFrameBetween::deep_delete() {
	SAFEDELETE(window_frame_bound_);
	SAFEDELETE(window_frame_start_);
	delete this;
}

IR* WindowFrameBound::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(window_frame_start_);
			res = new IR(IRTYPE::kWindowFrameBound, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kWindowFrameBound, OP3(TERMINAL::tUnboundedFollowing, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(iconst_);
			res = new IR(IRTYPE::kWindowFrameBound, OP3(TERMINAL::tEmpty, TERMINAL::tFollowing, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void WindowFrameBound::deep_delete() {
	SAFEDELETE(window_frame_start_);
	SAFEDELETE(iconst_);
	delete this;
}

IR* OptHavingClause::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptHavingClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_root_);
			res = new IR(IRTYPE::kOptHavingClause, OP3(TERMINAL::tHaving, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptHavingClause::deep_delete() {
	SAFEDELETE(expr_root_);
	delete this;
}

IR* OptGroupClause::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptGroupClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_root_list_);
			auto tmp2 = SAFETRANSLATE(olap_opt_);
			res = new IR(IRTYPE::kOptGroupClause, OP3(TERMINAL::tGroupBy, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptGroupClause::deep_delete() {
	SAFEDELETE(olap_opt_);
	SAFEDELETE(expr_root_list_);
	delete this;
}

IR* OlapOpt::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOlapOpt, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOlapOpt, OP3(TERMINAL::tWithRollup, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OlapOpt::deep_delete() {
	delete this;
}

IR* OptWhereClause::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptWhereClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(where_clause_);
			res = new IR(IRTYPE::kOptWhereClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptWhereClause::deep_delete() {
	SAFEDELETE(where_clause_);
	delete this;
}

IR* WhereClause::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(expr_root_);
	res = new IR(IRTYPE::kWhereClause, OP3(TERMINAL::tWhere, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void WhereClause::deep_delete() {
	SAFEDELETE(expr_root_);
	delete this;
}

IR* OptFromClause::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptFromClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(from_clause_);
			res = new IR(IRTYPE::kOptFromClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptFromClause::deep_delete() {
	SAFEDELETE(from_clause_);
	delete this;
}

IR* FromClause::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(table_reference_list_);
	res = new IR(IRTYPE::kFromClause, OP3(TERMINAL::tFrom, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void FromClause::deep_delete() {
	SAFEDELETE(table_reference_list_);
	delete this;
}

IR* TableReferenceList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(table_reference_);
			res = new IR(IRTYPE::kTableReferenceList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(table_reference_);
			auto tmp2 = SAFETRANSLATE(table_reference_list_);
			res = new IR(IRTYPE::kTableReferenceList, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void TableReferenceList::deep_delete() {
	SAFEDELETE(table_reference_);
	SAFEDELETE(table_reference_list_);
	delete this;
}

IR* TableReference::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(table_factor_);
			res = new IR(IRTYPE::kTableReference, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(joined_table_);
			res = new IR(IRTYPE::kTableReference, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void TableReference::deep_delete() {
	SAFEDELETE(table_factor_);
	SAFEDELETE(joined_table_);
	delete this;
}

IR* TableFactor::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(single_table_);
			res = new IR(IRTYPE::kTableFactor, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(derived_table_);
			res = new IR(IRTYPE::kTableFactor, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(joined_table_parens_);
			res = new IR(IRTYPE::kTableFactor, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(table_reference_list_parens_);
			res = new IR(IRTYPE::kTableFactor, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void TableFactor::deep_delete() {
	SAFEDELETE(single_table_);
	SAFEDELETE(derived_table_);
	SAFEDELETE(joined_table_parens_);
	SAFEDELETE(table_reference_list_parens_);
	delete this;
}

IR* TableReferenceListParens::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(table_reference_);
	auto tmp2 = SAFETRANSLATE(table_reference_list_);
	res = new IR(IRTYPE::kTableReferenceListParens, OP3(TERMINAL::tOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);

	TRANSLATEEND
}

void TableReferenceListParens::deep_delete() {
	SAFEDELETE(table_reference_list_);
	SAFEDELETE(table_reference_);
	delete this;
}

IR* JoinedTableParens::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(joined_table_);
	res = new IR(IRTYPE::kJoinedTableParens, OP3(TERMINAL::tOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void JoinedTableParens::deep_delete() {
	SAFEDELETE(joined_table_);
	delete this;
}

IR* DerivedTable::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(subquery_);
			auto tmp2 = SAFETRANSLATE(alias_);
			auto tmp3 = SAFETRANSLATE(opt_column_list_);
			auto tmp4 = new IR(IRTYPE::kDerivedTableTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kDerivedTable, OP3(TERMINAL::tEmpty, TERMINAL::tAs, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(subquery_);
			auto tmp2 = SAFETRANSLATE(alias_);
			auto tmp3 = SAFETRANSLATE(opt_column_list_);
			auto tmp4 = new IR(IRTYPE::kDerivedTableTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kDerivedTable, OP3(TERMINAL::tLateral, TERMINAL::tAs, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void DerivedTable::deep_delete() {
	SAFEDELETE(opt_column_list_);
	SAFEDELETE(alias_);
	SAFEDELETE(subquery_);
	delete this;
}

IR* OptColumnList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptColumnList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(column_list_);
			res = new IR(IRTYPE::kOptColumnList, OP3(TERMINAL::tOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptColumnList::deep_delete() {
	SAFEDELETE(column_list_);
	delete this;
}

IR* ColumnList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(column_name_);
			res = new IR(IRTYPE::kColumnList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(column_name_);
			auto tmp2 = SAFETRANSLATE(column_list_);
			res = new IR(IRTYPE::kColumnList, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void ColumnList::deep_delete() {
	SAFEDELETE(column_name_);
	SAFEDELETE(column_list_);
	delete this;
}

IR* Subquery::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(query_expression_parens_);
	res = new IR(IRTYPE::kSubquery, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void Subquery::deep_delete() {
	SAFEDELETE(query_expression_parens_);
	delete this;
}

IR* SingleTable::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(table_name_);
	auto tmp2 = SAFETRANSLATE(opt_alias_);
	res = new IR(IRTYPE::kSingleTable, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);

	TRANSLATEEND
}

void SingleTable::deep_delete() {
	SAFEDELETE(opt_alias_);
	SAFEDELETE(table_name_);
	delete this;
}

IR* OptAlias::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptAlias, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(alias_);
			res = new IR(IRTYPE::kOptAlias, OP3(TERMINAL::tAs, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptAlias::deep_delete() {
	SAFEDELETE(alias_);
	delete this;
}

IR* JoinedTable::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(table_reference_1_);
			auto tmp2 = SAFETRANSLATE(inner_join_type_);
			auto tmp3 = SAFETRANSLATE(table_reference_2_);
			auto tmp4 = SAFETRANSLATE(expr_root_);
			auto tmp5 = new IR(IRTYPE::kJoinedTableTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOn, TERMINAL::tEmpty), tmp3, tmp4);
			auto tmp6 = new IR(IRTYPE::kJoinedTableTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp5);
			res = new IR(IRTYPE::kJoinedTable, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp6);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(table_reference_1_);
			auto tmp2 = SAFETRANSLATE(inner_join_type_);
			auto tmp3 = SAFETRANSLATE(table_reference_2_);
			auto tmp4 = SAFETRANSLATE(column_list_);
			auto tmp5 = new IR(IRTYPE::kJoinedTableTmp3, OP3(TERMINAL::tEmpty, TERMINAL::tUsingOpLp, TERMINAL::tOpRp), tmp3, tmp4);
			auto tmp6 = new IR(IRTYPE::kJoinedTableTmp4, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp5);
			res = new IR(IRTYPE::kJoinedTable, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp6);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(table_reference_1_);
			auto tmp2 = SAFETRANSLATE(outer_join_type_);
			auto tmp3 = SAFETRANSLATE(table_reference_2_);
			auto tmp4 = SAFETRANSLATE(expr_root_);
			auto tmp5 = new IR(IRTYPE::kJoinedTableTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOn, TERMINAL::tEmpty), tmp3, tmp4);
			auto tmp6 = new IR(IRTYPE::kJoinedTableTmp5, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp5);
			res = new IR(IRTYPE::kJoinedTable, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp6);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(table_reference_1_);
			auto tmp2 = SAFETRANSLATE(outer_join_type_);
			auto tmp3 = SAFETRANSLATE(table_reference_2_);
			auto tmp4 = SAFETRANSLATE(column_list_);
			auto tmp5 = new IR(IRTYPE::kJoinedTableTmp3, OP3(TERMINAL::tEmpty, TERMINAL::tUsingOpLp, TERMINAL::tOpRp), tmp3, tmp4);
			auto tmp6 = new IR(IRTYPE::kJoinedTableTmp6, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp5);
			res = new IR(IRTYPE::kJoinedTable, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp6);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(table_reference_1_);
			auto tmp2 = SAFETRANSLATE(inner_join_type_);
			auto tmp3 = SAFETRANSLATE(table_reference_2_);
			auto tmp4 = new IR(IRTYPE::kJoinedTableTmp7, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kJoinedTable, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(5)
			auto tmp1 = SAFETRANSLATE(table_reference_1_);
			auto tmp2 = SAFETRANSLATE(natural_join_type_);
			auto tmp3 = SAFETRANSLATE(table_factor_);
			auto tmp4 = new IR(IRTYPE::kJoinedTableTmp8, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kJoinedTable, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void JoinedTable::deep_delete() {
	SAFEDELETE(expr_root_);
	SAFEDELETE(table_reference_1_);
	SAFEDELETE(table_reference_2_);
	SAFEDELETE(inner_join_type_);
	SAFEDELETE(column_list_);
	SAFEDELETE(outer_join_type_);
	SAFEDELETE(table_factor_);
	SAFEDELETE(natural_join_type_);
	delete this;
}

IR* InnerJoinType::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kInnerJoinType, OP3(TERMINAL::tJoin, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kInnerJoinType, OP3(TERMINAL::tInnerJoin, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kInnerJoinType, OP3(TERMINAL::tCrossJoin, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(3)
			res = new IR(IRTYPE::kInnerJoinType, OP3(TERMINAL::tStraightJoin, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void InnerJoinType::deep_delete() {
	delete this;
}

IR* NaturalJoinType::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(opt_inner_);
			res = new IR(IRTYPE::kNaturalJoinType, OP3(TERMINAL::tNatural, TERMINAL::tJoin, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(opt_outer_);
			res = new IR(IRTYPE::kNaturalJoinType, OP3(TERMINAL::tNaturalRight, TERMINAL::tJoin, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(opt_outer_);
			res = new IR(IRTYPE::kNaturalJoinType, OP3(TERMINAL::tNaturalLeft, TERMINAL::tJoin, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void NaturalJoinType::deep_delete() {
	SAFEDELETE(opt_inner_);
	SAFEDELETE(opt_outer_);
	delete this;
}

IR* OptInner::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptInner, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOptInner, OP3(TERMINAL::tInner, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptInner::deep_delete() {
	delete this;
}

IR* OptOuter::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptOuter, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOptOuter, OP3(TERMINAL::tOuter, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptOuter::deep_delete() {
	delete this;
}

IR* OuterJoinType::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(opt_outer_);
			res = new IR(IRTYPE::kOuterJoinType, OP3(TERMINAL::tLeft, TERMINAL::tJoin, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(opt_outer_);
			res = new IR(IRTYPE::kOuterJoinType, OP3(TERMINAL::tRight, TERMINAL::tJoin, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OuterJoinType::deep_delete() {
	SAFEDELETE(opt_outer_);
	delete this;
}

IR* SelectItemList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(select_item_);
			res = new IR(IRTYPE::kSelectItemList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(select_item_);
			auto tmp2 = SAFETRANSLATE(select_item_list_);
			res = new IR(IRTYPE::kSelectItemList, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void SelectItemList::deep_delete() {
	SAFEDELETE(select_item_);
	SAFEDELETE(select_item_list_);
	delete this;
}

IR* SelectItem::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(table_wild_);
			res = new IR(IRTYPE::kSelectItem, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_root_);
			auto tmp2 = SAFETRANSLATE(opt_alias_);
			res = new IR(IRTYPE::kSelectItem, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void SelectItem::deep_delete() {
	SAFEDELETE(table_wild_);
	SAFEDELETE(opt_alias_);
	SAFEDELETE(expr_root_);
	delete this;
}

IR* TableWild::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kTableWild, OP3(TERMINAL::tOpMul, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(table_name_);
			res = new IR(IRTYPE::kTableWild, OP3(TERMINAL::tEmpty, TERMINAL::tOpDotOpMul, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void TableWild::deep_delete() {
	SAFEDELETE(table_name_);
	delete this;
}

IR* OptSelectOptions::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptSelectOptions, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(select_option_list_);
			res = new IR(IRTYPE::kOptSelectOptions, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptSelectOptions::deep_delete() {
	SAFEDELETE(select_option_list_);
	delete this;
}

IR* SelectOptionList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(select_option_);
			res = new IR(IRTYPE::kSelectOptionList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(select_option_);
			auto tmp2 = SAFETRANSLATE(select_option_list_);
			res = new IR(IRTYPE::kSelectOptionList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void SelectOptionList::deep_delete() {
	SAFEDELETE(select_option_);
	SAFEDELETE(select_option_list_);
	delete this;
}

IR* SelectOption::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kSelectOption, OP3(TERMINAL::tSqlNoCache, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kSelectOption, OP3(TERMINAL::tStraightJoin, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kSelectOption, OP3(TERMINAL::tHighPriority, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(3)
			res = new IR(IRTYPE::kSelectOption, OP3(TERMINAL::tDistinct, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(4)
			res = new IR(IRTYPE::kSelectOption, OP3(TERMINAL::tSqlSmallResult, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(5)
			res = new IR(IRTYPE::kSelectOption, OP3(TERMINAL::tSqlBigResult, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(6)
			res = new IR(IRTYPE::kSelectOption, OP3(TERMINAL::tSqlBufferResult, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(7)
			res = new IR(IRTYPE::kSelectOption, OP3(TERMINAL::tSqlCalcFoundRows, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(8)
			res = new IR(IRTYPE::kSelectOption, OP3(TERMINAL::tAll, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void SelectOption::deep_delete() {
	delete this;
}

IR* QueryExpressionParens::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(query_expression_parens_);
			res = new IR(IRTYPE::kQueryExpressionParens, OP3(TERMINAL::tOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(query_expression_);
			res = new IR(IRTYPE::kQueryExpressionParens, OP3(TERMINAL::tOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void QueryExpressionParens::deep_delete() {
	SAFEDELETE(query_expression_parens_);
	SAFEDELETE(query_expression_);
	delete this;
}

IR* OptUnionOption::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptUnionOption, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOptUnionOption, OP3(TERMINAL::tDistinct, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kOptUnionOption, OP3(TERMINAL::tAll, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptUnionOption::deep_delete() {
	delete this;
}

IR* OptOrderClause::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptOrderClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(order_clause_);
			res = new IR(IRTYPE::kOptOrderClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptOrderClause::deep_delete() {
	SAFEDELETE(order_clause_);
	delete this;
}

IR* OrderClause::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(order_list_);
	res = new IR(IRTYPE::kOrderClause, OP3(TERMINAL::tOrderBy, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void OrderClause::deep_delete() {
	SAFEDELETE(order_list_);
	delete this;
}

IR* OptLimitClause::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptLimitClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(limit_clause_);
			res = new IR(IRTYPE::kOptLimitClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptLimitClause::deep_delete() {
	SAFEDELETE(limit_clause_);
	delete this;
}

IR* LimitClause::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(limit_options_);
	res = new IR(IRTYPE::kLimitClause, OP3(TERMINAL::tLimit, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void LimitClause::deep_delete() {
	SAFEDELETE(limit_options_);
	delete this;
}

IR* LimitOptions::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(iconst_1_);
			res = new IR(IRTYPE::kLimitOptions, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(iconst_1_);
			auto tmp2 = SAFETRANSLATE(iconst_2_);
			res = new IR(IRTYPE::kLimitOptions, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void LimitOptions::deep_delete() {
	SAFEDELETE(iconst_1_);
	SAFEDELETE(iconst_2_);
	delete this;
}

IR* WithClause::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(with_list_);
			res = new IR(IRTYPE::kWithClause, OP3(TERMINAL::tWith, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(with_list_);
			res = new IR(IRTYPE::kWithClause, OP3(TERMINAL::tWithRecursive, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void WithClause::deep_delete() {
	SAFEDELETE(with_list_);
	delete this;
}

IR* WithList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(common_table_expr_);
			res = new IR(IRTYPE::kWithList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(common_table_expr_);
			auto tmp2 = SAFETRANSLATE(with_list_);
			res = new IR(IRTYPE::kWithList, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void WithList::deep_delete() {
	SAFEDELETE(common_table_expr_);
	SAFEDELETE(with_list_);
	delete this;
}

IR* CommonTableExpr::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(table_name_);
	auto tmp2 = SAFETRANSLATE(opt_column_list_);
	auto tmp3 = SAFETRANSLATE(subquery_);
	auto tmp4 = new IR(IRTYPE::kCommonTableExprTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tAs, TERMINAL::tEmpty), tmp2, tmp3);
	res = new IR(IRTYPE::kCommonTableExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);

	TRANSLATEEND
}

void CommonTableExpr::deep_delete() {
	SAFEDELETE(subquery_);
	SAFEDELETE(opt_column_list_);
	SAFEDELETE(table_name_);
	delete this;
}

IR* ExprRootList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(expr_root_);
			res = new IR(IRTYPE::kExprRootList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_root_);
			auto tmp2 = SAFETRANSLATE(expr_root_list_);
			res = new IR(IRTYPE::kExprRootList, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void ExprRootList::deep_delete() {
	SAFEDELETE(expr_root_);
	SAFEDELETE(expr_root_list_);
	delete this;
}

IR* ExprRoot::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(expr_);
	res = new IR(IRTYPE::kExprRoot, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void ExprRoot::deep_delete() {
	SAFEDELETE(expr_);
	delete this;
}

IR* Expr::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kExpr, OP3(TERMINAL::tEmpty, TERMINAL::tOr, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kExpr, OP3(TERMINAL::tEmpty, TERMINAL::tXor, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kExpr, OP3(TERMINAL::tEmpty, TERMINAL::tAnd, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kExpr, OP3(TERMINAL::tNot, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(bool_pri_);
			res = new IR(IRTYPE::kExpr, OP3(TERMINAL::tEmpty, TERMINAL::tIsTrue, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(5)
			auto tmp1 = SAFETRANSLATE(bool_pri_);
			res = new IR(IRTYPE::kExpr, OP3(TERMINAL::tEmpty, TERMINAL::tIsNotTrue, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(6)
			auto tmp1 = SAFETRANSLATE(bool_pri_);
			res = new IR(IRTYPE::kExpr, OP3(TERMINAL::tEmpty, TERMINAL::tIsFalse, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(7)
			auto tmp1 = SAFETRANSLATE(bool_pri_);
			res = new IR(IRTYPE::kExpr, OP3(TERMINAL::tEmpty, TERMINAL::tIsNotFalse, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(8)
			auto tmp1 = SAFETRANSLATE(bool_pri_);
			res = new IR(IRTYPE::kExpr, OP3(TERMINAL::tEmpty, TERMINAL::tIsUnknown, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(9)
			auto tmp1 = SAFETRANSLATE(bool_pri_);
			res = new IR(IRTYPE::kExpr, OP3(TERMINAL::tEmpty, TERMINAL::tIsNotUnknown, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(10)
			auto tmp1 = SAFETRANSLATE(bool_pri_);
			res = new IR(IRTYPE::kExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void Expr::deep_delete() {
	SAFEDELETE(expr_1_);
	SAFEDELETE(expr_2_);
	SAFEDELETE(bool_pri_);
	delete this;
}

IR* BoolPri::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(bool_pri_);
			res = new IR(IRTYPE::kBoolPri, OP3(TERMINAL::tEmpty, TERMINAL::tIsNull, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(bool_pri_);
			res = new IR(IRTYPE::kBoolPri, OP3(TERMINAL::tEmpty, TERMINAL::tIsNotNull, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(bool_pri_);
			auto tmp2 = SAFETRANSLATE(comp_op_);
			auto tmp3 = SAFETRANSLATE(predicate_);
			auto tmp4 = new IR(IRTYPE::kBoolPriTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kBoolPri, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(bool_pri_);
			auto tmp2 = SAFETRANSLATE(comp_op_);
			auto tmp3 = SAFETRANSLATE(all_subquery_);
			auto tmp4 = new IR(IRTYPE::kBoolPriTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kBoolPri, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(predicate_);
			res = new IR(IRTYPE::kBoolPri, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void BoolPri::deep_delete() {
	SAFEDELETE(bool_pri_);
	SAFEDELETE(predicate_);
	SAFEDELETE(comp_op_);
	SAFEDELETE(all_subquery_);
	delete this;
}

IR* Predicate::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(in_subquery_);
			res = new IR(IRTYPE::kPredicate, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_);
			res = new IR(IRTYPE::kPredicate, OP3(TERMINAL::tEmpty, TERMINAL::tInOpLp, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_);
			auto tmp3 = SAFETRANSLATE(expr_list_);
			auto tmp4 = new IR(IRTYPE::kPredicateTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp2, tmp3);
			res = new IR(IRTYPE::kPredicate, OP3(TERMINAL::tEmpty, TERMINAL::tInOpLp, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_);
			res = new IR(IRTYPE::kPredicate, OP3(TERMINAL::tEmpty, TERMINAL::tNotInOpLp, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_);
			auto tmp3 = SAFETRANSLATE(expr_list_);
			auto tmp4 = new IR(IRTYPE::kPredicateTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp2, tmp3);
			res = new IR(IRTYPE::kPredicate, OP3(TERMINAL::tEmpty, TERMINAL::tNotInOpLp, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(5)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			auto tmp3 = SAFETRANSLATE(predicate_);
			auto tmp4 = new IR(IRTYPE::kPredicateTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tAnd, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kPredicate, OP3(TERMINAL::tEmpty, TERMINAL::tBetween, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(6)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			auto tmp3 = SAFETRANSLATE(predicate_);
			auto tmp4 = new IR(IRTYPE::kPredicateTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tAnd, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kPredicate, OP3(TERMINAL::tEmpty, TERMINAL::tNotBetween, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(7)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			res = new IR(IRTYPE::kPredicate, OP3(TERMINAL::tEmpty, TERMINAL::tSoundsLike, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(8)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(simple_expr_);
			res = new IR(IRTYPE::kPredicate, OP3(TERMINAL::tEmpty, TERMINAL::tLike, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(9)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(simple_expr_);
			res = new IR(IRTYPE::kPredicate, OP3(TERMINAL::tEmpty, TERMINAL::tNotLike, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(10)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			res = new IR(IRTYPE::kPredicate, OP3(TERMINAL::tEmpty, TERMINAL::tRegexp, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(11)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			res = new IR(IRTYPE::kPredicate, OP3(TERMINAL::tEmpty, TERMINAL::tNotRegexp, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(12)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			res = new IR(IRTYPE::kPredicate, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void Predicate::deep_delete() {
	SAFEDELETE(in_subquery_);
	SAFEDELETE(bit_expr_1_);
	SAFEDELETE(bit_expr_2_);
	SAFEDELETE(expr_);
	SAFEDELETE(expr_list_);
	SAFEDELETE(predicate_);
	SAFEDELETE(simple_expr_);
	delete this;
}

IR* BitExpr::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			res = new IR(IRTYPE::kBitExpr, OP3(TERMINAL::tEmpty, TERMINAL::tOpOr, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			res = new IR(IRTYPE::kBitExpr, OP3(TERMINAL::tEmpty, TERMINAL::tOpAnd, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			res = new IR(IRTYPE::kBitExpr, OP3(TERMINAL::tEmpty, TERMINAL::tOpShl, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			res = new IR(IRTYPE::kBitExpr, OP3(TERMINAL::tEmpty, TERMINAL::tOpShr, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			res = new IR(IRTYPE::kBitExpr, OP3(TERMINAL::tEmpty, TERMINAL::tOpAdd, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(5)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			res = new IR(IRTYPE::kBitExpr, OP3(TERMINAL::tEmpty, TERMINAL::tOpSub, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(6)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			res = new IR(IRTYPE::kBitExpr, OP3(TERMINAL::tEmpty, TERMINAL::tOpMul, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(7)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			res = new IR(IRTYPE::kBitExpr, OP3(TERMINAL::tEmpty, TERMINAL::tOpDivide, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(8)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			res = new IR(IRTYPE::kBitExpr, OP3(TERMINAL::tEmpty, TERMINAL::tOpMod, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(9)
			auto tmp1 = SAFETRANSLATE(bit_expr_1_);
			auto tmp2 = SAFETRANSLATE(bit_expr_2_);
			res = new IR(IRTYPE::kBitExpr, OP3(TERMINAL::tEmpty, TERMINAL::tOpXor, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(10)
			auto tmp1 = SAFETRANSLATE(simple_expr_);
			res = new IR(IRTYPE::kBitExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void BitExpr::deep_delete() {
	SAFEDELETE(bit_expr_1_);
	SAFEDELETE(bit_expr_2_);
	SAFEDELETE(simple_expr_);
	delete this;
}

IR* SimpleExpr::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(columnref_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(literal_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(iconst_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(fconst_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(sconst_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(5)
			auto tmp1 = SAFETRANSLATE(simple_expr_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tOpAdd, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(6)
			auto tmp1 = SAFETRANSLATE(simple_expr_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tOpSub, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(7)
			auto tmp1 = SAFETRANSLATE(simple_expr_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tOpNot, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(8)
			auto tmp1 = SAFETRANSLATE(subquery_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(9)
			auto tmp1 = SAFETRANSLATE(subquery_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tExists, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(10)
			auto tmp1 = SAFETRANSLATE(type_cast_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(11)
			auto tmp1 = SAFETRANSLATE(case_expr_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(12)
			auto tmp1 = SAFETRANSLATE(columnref_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tDefaultOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(13)
			auto tmp1 = SAFETRANSLATE(function_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(14)
			auto tmp1 = SAFETRANSLATE(expr_);
			res = new IR(IRTYPE::kSimpleExpr, OP3(TERMINAL::tOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void SimpleExpr::deep_delete() {
	SAFEDELETE(columnref_);
	SAFEDELETE(literal_);
	SAFEDELETE(iconst_);
	SAFEDELETE(fconst_);
	SAFEDELETE(sconst_);
	SAFEDELETE(simple_expr_);
	SAFEDELETE(subquery_);
	SAFEDELETE(type_cast_);
	SAFEDELETE(case_expr_);
	SAFEDELETE(function_);
	SAFEDELETE(expr_);
	delete this;
}

IR* Function::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(control_function_);
			res = new IR(IRTYPE::kFunction, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(math_function_);
			res = new IR(IRTYPE::kFunction, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(string_function_);
			res = new IR(IRTYPE::kFunction, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(aggregate_function_);
			res = new IR(IRTYPE::kFunction, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(window_function_);
			res = new IR(IRTYPE::kFunction, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void Function::deep_delete() {
	SAFEDELETE(control_function_);
	SAFEDELETE(math_function_);
	SAFEDELETE(string_function_);
	SAFEDELETE(aggregate_function_);
	SAFEDELETE(window_function_);
	delete this;
}

IR* StringFunction::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tAsciiOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tBinOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tBitLengthOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tCharLengthOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tLengthOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(5)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tLowerOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(6)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tLtrimOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(7)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tOctOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(8)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tOrdOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(9)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tQuoteOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(10)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tReverseOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(11)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tRtrimOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(12)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tSpaceOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(13)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tTrimOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(14)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tTrimOpLpLeadingFrom, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(15)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tTrimOpLpTrailingFrom, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(16)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tUnhexOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(17)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tUpperOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(18)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tFindInSetOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(19)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tInstrOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(20)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tLeftOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(21)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tLocateOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(22)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tRepeatOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(23)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tRightOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(24)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tSubstringOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(25)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tTrimOpLpLeading, TERMINAL::tFrom, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(26)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tTrimOpLpTrailing, TERMINAL::tFrom, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(27)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tTrimOpLp, TERMINAL::tFrom, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(28)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			auto tmp3 = SAFETRANSLATE(expr_3_);
			auto tmp4 = new IR(IRTYPE::kStringFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp2, tmp3);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tExportSetOpLp, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(29)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			auto tmp3 = SAFETRANSLATE(expr_3_);
			auto tmp4 = new IR(IRTYPE::kStringFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp2, tmp3);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tLocateOpLp, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(30)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			auto tmp3 = SAFETRANSLATE(expr_3_);
			auto tmp4 = new IR(IRTYPE::kStringFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp2, tmp3);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tLpadOpLp, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(31)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			auto tmp3 = SAFETRANSLATE(expr_3_);
			auto tmp4 = new IR(IRTYPE::kStringFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp2, tmp3);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tReplaceOpLp, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(32)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			auto tmp3 = SAFETRANSLATE(expr_3_);
			auto tmp4 = new IR(IRTYPE::kStringFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp2, tmp3);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tRpadOpLp, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(33)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			auto tmp3 = SAFETRANSLATE(expr_3_);
			auto tmp4 = new IR(IRTYPE::kStringFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp2, tmp3);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tSubstringOpLp, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(34)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			auto tmp3 = SAFETRANSLATE(expr_3_);
			auto tmp4 = new IR(IRTYPE::kStringFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp2, tmp3);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tSubstringIndexOpLp, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(35)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			auto tmp3 = SAFETRANSLATE(expr_3_);
			auto tmp4 = SAFETRANSLATE(expr_4_);
			auto tmp5 = new IR(IRTYPE::kStringFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp3, tmp4);
			auto tmp6 = new IR(IRTYPE::kStringFunctionTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp2, tmp5);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tExportSetOpLp, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp6);
		CASEEND
		CASESTART(36)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			auto tmp3 = SAFETRANSLATE(expr_3_);
			auto tmp4 = SAFETRANSLATE(expr_4_);
			auto tmp5 = new IR(IRTYPE::kStringFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp3, tmp4);
			auto tmp6 = new IR(IRTYPE::kStringFunctionTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp2, tmp5);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tInsertOpLp, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp6);
		CASEEND
		CASESTART(37)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			auto tmp3 = SAFETRANSLATE(expr_3_);
			auto tmp4 = SAFETRANSLATE(expr_4_);
			auto tmp5 = SAFETRANSLATE(expr_5_);
			auto tmp6 = new IR(IRTYPE::kStringFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp4, tmp5);
			auto tmp7 = new IR(IRTYPE::kStringFunctionTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp3, tmp6);
			auto tmp8 = new IR(IRTYPE::kStringFunctionTmp3, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp2, tmp7);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tExportSetOpLp, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp8);
		CASEEND
		CASESTART(38)
			auto tmp1 = SAFETRANSLATE(expr_list_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tCharOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(39)
			auto tmp1 = SAFETRANSLATE(expr_list_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tConcatOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(40)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_list_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tConcatWsOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(41)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_list_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tEltOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(42)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_list_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tFieldOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(43)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_list_);
			res = new IR(IRTYPE::kStringFunction, OP3(TERMINAL::tMakeSetOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void StringFunction::deep_delete() {
	SAFEDELETE(expr_1_);
	SAFEDELETE(expr_2_);
	SAFEDELETE(expr_3_);
	SAFEDELETE(expr_4_);
	SAFEDELETE(expr_5_);
	SAFEDELETE(expr_list_);
	delete this;
}

IR* MathFunction::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tPiOpLpOpRp, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tRandOpLpOpRp, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tAbsOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tAcosOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tAsinOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(5)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tAtanOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(6)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tCeilingOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(7)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tCosOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(8)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tCotOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(9)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tDegreesOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(10)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tExpOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(11)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tFloorOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(12)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tHexOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(13)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tLnOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(14)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tRadiansOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(15)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tRandOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(16)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tRoundOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(17)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tSignOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(18)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tSinOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(19)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tSqrtOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(20)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tTanOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(21)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tAtanOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(22)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tFormatOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(23)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tLogOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(24)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tModOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(25)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tPowOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(26)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tRoundOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(27)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tTruncateOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(28)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			auto tmp3 = SAFETRANSLATE(expr_3_);
			auto tmp4 = new IR(IRTYPE::kMathFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp2, tmp3);
			res = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tConvOpLp, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void MathFunction::deep_delete() {
	SAFEDELETE(expr_1_);
	SAFEDELETE(expr_2_);
	SAFEDELETE(expr_3_);
	delete this;
}

IR* WindowFunction::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(windowing_clause_);
			res = new IR(IRTYPE::kWindowFunction, OP3(TERMINAL::tRowNumberOpLpOpRp, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(windowing_clause_);
			res = new IR(IRTYPE::kWindowFunction, OP3(TERMINAL::tRankOpLpOpRp, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(windowing_clause_);
			res = new IR(IRTYPE::kWindowFunction, OP3(TERMINAL::tDenseRankOpLpOpRp, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(windowing_clause_);
			res = new IR(IRTYPE::kWindowFunction, OP3(TERMINAL::tCumeDistOpLpOpRp, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(windowing_clause_);
			res = new IR(IRTYPE::kWindowFunction, OP3(TERMINAL::tPercentRankOpLpOpRp, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(5)
			auto tmp1 = SAFETRANSLATE(iconst_);
			auto tmp2 = SAFETRANSLATE(windowing_clause_);
			res = new IR(IRTYPE::kWindowFunction, OP3(TERMINAL::tNtileOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(6)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(opt_lead_lag_info_);
			auto tmp3 = SAFETRANSLATE(opt_null_treatment_);
			auto tmp4 = SAFETRANSLATE(windowing_clause_);
			auto tmp5 = new IR(IRTYPE::kWindowFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp3, tmp4);
			auto tmp6 = new IR(IRTYPE::kWindowFunctionTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp2, tmp5);
			res = new IR(IRTYPE::kWindowFunction, OP3(TERMINAL::tLeadOpLp, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp6);
		CASEEND
		CASESTART(7)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(opt_lead_lag_info_);
			auto tmp3 = SAFETRANSLATE(opt_null_treatment_);
			auto tmp4 = SAFETRANSLATE(windowing_clause_);
			auto tmp5 = new IR(IRTYPE::kWindowFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp3, tmp4);
			auto tmp6 = new IR(IRTYPE::kWindowFunctionTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp2, tmp5);
			res = new IR(IRTYPE::kWindowFunction, OP3(TERMINAL::tLagOpLp, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp6);
		CASEEND
		CASESTART(8)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(opt_null_treatment_);
			auto tmp3 = SAFETRANSLATE(windowing_clause_);
			auto tmp4 = new IR(IRTYPE::kWindowFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kWindowFunction, OP3(TERMINAL::tFirstValueOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(9)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(opt_null_treatment_);
			auto tmp3 = SAFETRANSLATE(windowing_clause_);
			auto tmp4 = new IR(IRTYPE::kWindowFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kWindowFunction, OP3(TERMINAL::tLastValueOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(10)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(iconst_);
			auto tmp3 = SAFETRANSLATE(opt_from_first_last_);
			auto tmp4 = SAFETRANSLATE(opt_null_treatment_);
			auto tmp5 = SAFETRANSLATE(windowing_clause_);
			auto tmp6 = new IR(IRTYPE::kWindowFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp4, tmp5);
			auto tmp7 = new IR(IRTYPE::kWindowFunctionTmp3, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp3, tmp6);
			auto tmp8 = new IR(IRTYPE::kWindowFunctionTmp4, OP3(TERMINAL::tEmpty, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp2, tmp7);
			res = new IR(IRTYPE::kWindowFunction, OP3(TERMINAL::tNthValueOpLp, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp8);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void WindowFunction::deep_delete() {
	SAFEDELETE(windowing_clause_);
	SAFEDELETE(iconst_);
	SAFEDELETE(opt_null_treatment_);
	SAFEDELETE(opt_lead_lag_info_);
	SAFEDELETE(expr_);
	SAFEDELETE(opt_from_first_last_);
	delete this;
}

IR* OptNullTreatment::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptNullTreatment, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOptNullTreatment, OP3(TERMINAL::tRespectNulls, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kOptNullTreatment, OP3(TERMINAL::tIgnoreNulls, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptNullTreatment::deep_delete() {
	delete this;
}

IR* OptFromFirstLast::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptFromFirstLast, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOptFromFirstLast, OP3(TERMINAL::tFromFirst, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kOptFromFirstLast, OP3(TERMINAL::tFromLast, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptFromFirstLast::deep_delete() {
	delete this;
}

IR* OptLeadLagInfo::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptLeadLagInfo, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(lead_lag_info_);
			res = new IR(IRTYPE::kOptLeadLagInfo, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptLeadLagInfo::deep_delete() {
	SAFEDELETE(lead_lag_info_);
	delete this;
}

IR* LeadLagInfo::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(iconst_);
			res = new IR(IRTYPE::kLeadLagInfo, OP3(TERMINAL::tOpComma, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(iconst_);
			auto tmp2 = SAFETRANSLATE(expr_);
			res = new IR(IRTYPE::kLeadLagInfo, OP3(TERMINAL::tOpComma, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void LeadLagInfo::deep_delete() {
	SAFEDELETE(iconst_);
	SAFEDELETE(expr_);
	delete this;
}

IR* AggregateFunction::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(opt_distinct_);
			auto tmp2 = SAFETRANSLATE(expr_);
			auto tmp3 = SAFETRANSLATE(opt_windowing_clause_);
			auto tmp4 = new IR(IRTYPE::kAggregateFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tAvgOpLp, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(opt_windowing_clause_);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tBitAndOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(opt_windowing_clause_);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tBitOrOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(3)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(opt_windowing_clause_);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tBitXorOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(4)
			auto tmp1 = SAFETRANSLATE(opt_windowing_clause_);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tCountOpLpOpMulOpRp, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(5)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(opt_windowing_clause_);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tCountOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(6)
			auto tmp1 = SAFETRANSLATE(expr_list_);
			auto tmp2 = SAFETRANSLATE(opt_windowing_clause_);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tCountOpLpDistinct, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(7)
			auto tmp1 = SAFETRANSLATE(opt_distinct_);
			auto tmp2 = SAFETRANSLATE(expr_);
			auto tmp3 = SAFETRANSLATE(opt_windowing_clause_);
			auto tmp4 = new IR(IRTYPE::kAggregateFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tMinOpLp, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(8)
			auto tmp1 = SAFETRANSLATE(opt_distinct_);
			auto tmp2 = SAFETRANSLATE(expr_);
			auto tmp3 = SAFETRANSLATE(opt_windowing_clause_);
			auto tmp4 = new IR(IRTYPE::kAggregateFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tMaxOpLp, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(9)
			auto tmp1 = SAFETRANSLATE(opt_distinct_);
			auto tmp2 = SAFETRANSLATE(expr_);
			auto tmp3 = SAFETRANSLATE(opt_windowing_clause_);
			auto tmp4 = new IR(IRTYPE::kAggregateFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp2, tmp3);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tSumOpLp, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(10)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(opt_windowing_clause_);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tStdOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(11)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(opt_windowing_clause_);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tStddevSampOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(12)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(opt_windowing_clause_);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tVarianceOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(13)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(opt_windowing_clause_);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tVarSampOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
		CASESTART(14)
			auto tmp1 = SAFETRANSLATE(opt_distinct_);
			auto tmp2 = SAFETRANSLATE(expr_list_);
			auto tmp3 = SAFETRANSLATE(opt_order_clause_);
			auto tmp4 = SAFETRANSLATE(opt_gconcat_separator_);
			auto tmp5 = SAFETRANSLATE(opt_windowing_clause_);
			auto tmp6 = new IR(IRTYPE::kAggregateFunctionTmp2, OP3(TERMINAL::tEmpty, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp4, tmp5);
			auto tmp7 = new IR(IRTYPE::kAggregateFunctionTmp3, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp3, tmp6);
			auto tmp8 = new IR(IRTYPE::kAggregateFunctionTmp4, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp2, tmp7);
			res = new IR(IRTYPE::kAggregateFunction, OP3(TERMINAL::tGroupConcatOpLp, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp8);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void AggregateFunction::deep_delete() {
	SAFEDELETE(opt_windowing_clause_);
	SAFEDELETE(expr_);
	SAFEDELETE(opt_distinct_);
	SAFEDELETE(expr_list_);
	SAFEDELETE(opt_gconcat_separator_);
	SAFEDELETE(opt_order_clause_);
	delete this;
}

IR* OptGconcatSeparator::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptGconcatSeparator, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(sconst_);
			res = new IR(IRTYPE::kOptGconcatSeparator, OP3(TERMINAL::tSeparator, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptGconcatSeparator::deep_delete() {
	SAFEDELETE(sconst_);
	delete this;
}

IR* OptDistinct::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptDistinct, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kOptDistinct, OP3(TERMINAL::tDistinct, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptDistinct::deep_delete() {
	delete this;
}

IR* OptWindowingClause::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptWindowingClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(windowing_clause_);
			res = new IR(IRTYPE::kOptWindowingClause, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptWindowingClause::deep_delete() {
	SAFEDELETE(windowing_clause_);
	delete this;
}

IR* WindowingClause::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(window_name_or_spec_);
	res = new IR(IRTYPE::kWindowingClause, OP3(TERMINAL::tOver, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void WindowingClause::deep_delete() {
	SAFEDELETE(window_name_or_spec_);
	delete this;
}

IR* WindowNameOrSpec::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(window_name_);
			res = new IR(IRTYPE::kWindowNameOrSpec, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(window_spec_);
			res = new IR(IRTYPE::kWindowNameOrSpec, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void WindowNameOrSpec::deep_delete() {
	SAFEDELETE(window_name_);
	SAFEDELETE(window_spec_);
	delete this;
}

IR* ControlFunction::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			auto tmp3 = SAFETRANSLATE(expr_3_);
			auto tmp4 = new IR(IRTYPE::kControlFunctionTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp2, tmp3);
			res = new IR(IRTYPE::kControlFunction, OP3(TERMINAL::tIfOpLp, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp4);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kControlFunction, OP3(TERMINAL::tIfnullOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(expr_1_);
			auto tmp2 = SAFETRANSLATE(expr_2_);
			res = new IR(IRTYPE::kControlFunction, OP3(TERMINAL::tNullifOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void ControlFunction::deep_delete() {
	SAFEDELETE(expr_1_);
	SAFEDELETE(expr_2_);
	SAFEDELETE(expr_3_);
	delete this;
}

IR* CaseExpr::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(opt_expr_);
	auto tmp2 = SAFETRANSLATE(when_clasue_list_);
	auto tmp3 = SAFETRANSLATE(opt_else_);
	auto tmp4 = new IR(IRTYPE::kCaseExprTmp1, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEnd), tmp2, tmp3);
	res = new IR(IRTYPE::kCaseExpr, OP3(TERMINAL::tCase, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp4);

	TRANSLATEEND
}

void CaseExpr::deep_delete() {
	SAFEDELETE(opt_else_);
	SAFEDELETE(when_clasue_list_);
	SAFEDELETE(opt_expr_);
	delete this;
}

IR* WhenClasueList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(when_list_);
			res = new IR(IRTYPE::kWhenClasueList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(when_list_);
			auto tmp2 = SAFETRANSLATE(when_clasue_list_);
			res = new IR(IRTYPE::kWhenClasueList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void WhenClasueList::deep_delete() {
	SAFEDELETE(when_list_);
	SAFEDELETE(when_clasue_list_);
	delete this;
}

IR* WhenList::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(expr_1_);
	auto tmp2 = SAFETRANSLATE(expr_2_);
	res = new IR(IRTYPE::kWhenList, OP3(TERMINAL::tWhen, TERMINAL::tThen, TERMINAL::tEmpty), tmp1, tmp2);

	TRANSLATEEND
}

void WhenList::deep_delete() {
	SAFEDELETE(expr_1_);
	SAFEDELETE(expr_2_);
	delete this;
}

IR* OptExpr::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_);
			res = new IR(IRTYPE::kOptExpr, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptExpr::deep_delete() {
	SAFEDELETE(expr_);
	delete this;
}

IR* OptElse::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptElse, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_);
			res = new IR(IRTYPE::kOptElse, OP3(TERMINAL::tElse, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptElse::deep_delete() {
	SAFEDELETE(expr_);
	delete this;
}

IR* TypeCast::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(cast_type_);
			res = new IR(IRTYPE::kTypeCast, OP3(TERMINAL::tCastOpLp, TERMINAL::tAs, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(cast_type_);
			res = new IR(IRTYPE::kTypeCast, OP3(TERMINAL::tConvertOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void TypeCast::deep_delete() {
	SAFEDELETE(cast_type_);
	SAFEDELETE(expr_);
	delete this;
}

IR* CastType::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(opt_field_length_);
			res = new IR(IRTYPE::kCastType, OP3(TERMINAL::tBinary, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(opt_field_length_);
			res = new IR(IRTYPE::kCastType, OP3(TERMINAL::tChar, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(opt_field_length_);
			res = new IR(IRTYPE::kCastType, OP3(TERMINAL::tNchar, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(3)
			res = new IR(IRTYPE::kCastType, OP3(TERMINAL::tSigned, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(4)
			res = new IR(IRTYPE::kCastType, OP3(TERMINAL::tUnsigned, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(5)
			auto tmp1 = SAFETRANSLATE(float_options_);
			res = new IR(IRTYPE::kCastType, OP3(TERMINAL::tDecimal, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(6)
			res = new IR(IRTYPE::kCastType, OP3(TERMINAL::tReal, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(7)
			auto tmp1 = SAFETRANSLATE(opt_field_length_);
			res = new IR(IRTYPE::kCastType, OP3(TERMINAL::tFloat, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void CastType::deep_delete() {
	SAFEDELETE(opt_field_length_);
	SAFEDELETE(float_options_);
	delete this;
}

IR* FloatOptions::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kFloatOptions, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(field_length_);
			res = new IR(IRTYPE::kFloatOptions, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(precision_);
			res = new IR(IRTYPE::kFloatOptions, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void FloatOptions::deep_delete() {
	SAFEDELETE(field_length_);
	SAFEDELETE(precision_);
	delete this;
}

IR* Precision::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(iconst_1_);
	auto tmp2 = SAFETRANSLATE(iconst_2_);
	res = new IR(IRTYPE::kPrecision, OP3(TERMINAL::tOpLp, TERMINAL::tOpComma, TERMINAL::tOpRp), tmp1, tmp2);

	TRANSLATEEND
}

void Precision::deep_delete() {
	SAFEDELETE(iconst_1_);
	SAFEDELETE(iconst_2_);
	delete this;
}

IR* OptFieldLength::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kOptFieldLength, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(field_length_);
			res = new IR(IRTYPE::kOptFieldLength, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void OptFieldLength::deep_delete() {
	SAFEDELETE(field_length_);
	delete this;
}

IR* FieldLength::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(iconst_);
	res = new IR(IRTYPE::kFieldLength, OP3(TERMINAL::tOpLp, TERMINAL::tOpRp, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void FieldLength::deep_delete() {
	SAFEDELETE(iconst_);
	delete this;
}

IR* Literal::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kLiteral, OP3(TERMINAL::tNull, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kLiteral, OP3(TERMINAL::tFalse, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kLiteral, OP3(TERMINAL::tTrue, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void Literal::deep_delete() {
	delete this;
}

IR* Columnref::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(column_name_);
			res = new IR(IRTYPE::kColumnref, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(table_name_);
			auto tmp2 = SAFETRANSLATE(column_name_);
			res = new IR(IRTYPE::kColumnref, OP3(TERMINAL::tEmpty, TERMINAL::tOpDot, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void Columnref::deep_delete() {
	SAFEDELETE(column_name_);
	SAFEDELETE(table_name_);
	delete this;
}

IR* CompOp::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			res = new IR(IRTYPE::kCompOp, OP3(TERMINAL::tOpEqual, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(1)
			res = new IR(IRTYPE::kCompOp, OP3(TERMINAL::tOpGreatereq, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(2)
			res = new IR(IRTYPE::kCompOp, OP3(TERMINAL::tOpGreaterthan, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(3)
			res = new IR(IRTYPE::kCompOp, OP3(TERMINAL::tOpLesseq, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(4)
			res = new IR(IRTYPE::kCompOp, OP3(TERMINAL::tOpLessthan, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
		CASESTART(5)
			res = new IR(IRTYPE::kCompOp, OP3(TERMINAL::tOpNotequal, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void CompOp::deep_delete() {
	delete this;
}

IR* AllSubquery::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(subquery_);
			res = new IR(IRTYPE::kAllSubquery, OP3(TERMINAL::tAll, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(subquery_);
			res = new IR(IRTYPE::kAllSubquery, OP3(TERMINAL::tAny, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(2)
			auto tmp1 = SAFETRANSLATE(subquery_);
			res = new IR(IRTYPE::kAllSubquery, OP3(TERMINAL::tSome, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void AllSubquery::deep_delete() {
	SAFEDELETE(subquery_);
	delete this;
}

IR* InSubquery::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(subquery_);
			res = new IR(IRTYPE::kInSubquery, OP3(TERMINAL::tIn, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(subquery_);
			res = new IR(IRTYPE::kInSubquery, OP3(TERMINAL::tNotIn, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void InSubquery::deep_delete() {
	SAFEDELETE(subquery_);
	delete this;
}

IR* ExprList::translate() {
	TRANSLATESTART

	SWITCHSTART
		CASESTART(0)
			auto tmp1 = SAFETRANSLATE(expr_);
			res = new IR(IRTYPE::kExprList, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);
		CASEEND
		CASESTART(1)
			auto tmp1 = SAFETRANSLATE(expr_);
			auto tmp2 = SAFETRANSLATE(expr_list_);
			res = new IR(IRTYPE::kExprList, OP3(TERMINAL::tEmpty, TERMINAL::tOpComma, TERMINAL::tEmpty), tmp1, tmp2);
		CASEEND
	SWITCHEND

	TRANSLATEEND
}

void ExprList::deep_delete() {
	SAFEDELETE(expr_);
	SAFEDELETE(expr_list_);
	delete this;
}

IR*  Iconst::translate(){
	TRANSLATESTART

	res = new IR(kIconst, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
	res->long_val_ = this->long_val_;

	TRANSLATEEND
}

void Iconst::deep_delete(){
	delete this;
}

IR*  Fconst::translate(){
	TRANSLATESTART

	res = new IR(kFconst, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
	res->float_val_ = this->float_val_;

	TRANSLATEEND
}

void Fconst::deep_delete(){
	delete this;
}

IR*  Sconst::translate(){
	TRANSLATESTART

	res = new IR(kSconst, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
	res->str_val_ = this->str_val_;

	TRANSLATEEND
}

void Sconst::deep_delete(){
	delete this;
}

IR*  Ident::translate(){
	TRANSLATESTART

	res = new IR(kIdent, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);
	res->str_val_ = this->str_val_;

	TRANSLATEEND
}

void Ident::deep_delete(){
	delete this;
}

IR* Alias::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(ident_);
	res = new IR(IRTYPE::kAlias, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void Alias::deep_delete() {
	SAFEDELETE(ident_);
	delete this;
}

IR* ColumnName::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(ident_);
	res = new IR(IRTYPE::kColumnName, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void ColumnName::deep_delete() {
	SAFEDELETE(ident_);
	delete this;
}

IR* WindowName::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(ident_);
	res = new IR(IRTYPE::kWindowName, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void WindowName::deep_delete() {
	SAFEDELETE(ident_);
	delete this;
}

IR* TableName::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(ident_);
	res = new IR(IRTYPE::kTableName, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void TableName::deep_delete() {
	SAFEDELETE(ident_);
	delete this;
}

IR* ConstraintName::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(ident_);
	res = new IR(IRTYPE::kConstraintName, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void ConstraintName::deep_delete() {
	SAFEDELETE(ident_);
	delete this;
}

IR* IndexName::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(ident_);
	res = new IR(IRTYPE::kIndexName, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void IndexName::deep_delete() {
	SAFEDELETE(ident_);
	delete this;
}

IR* ViewName::translate() {
	TRANSLATESTART

	auto tmp1 = SAFETRANSLATE(ident_);
	res = new IR(IRTYPE::kViewName, OP3(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty), tmp1, nullptr);

	TRANSLATEEND
}

void ViewName::deep_delete() {
	SAFEDELETE(ident_);
	delete this;
}

