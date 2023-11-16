#ifndef __AST_H__
#define __AST_H__
#include <vector>
#include <string>
#include "define.h"
#include <iostream>

using namespace std;

enum IRTYPE{
#define DECLARE_TYPE(v)  \
	v,
ALLTYPE(DECLARE_TYPE)
#undef DECLARE_TYPE
};

enum TERMINAL {
#define DECLARE_TERMINAL(v)  \
	v,
ALLTERMINAL(DECLARE_TERMINAL)
#undef DECLARE_TERMINAL
};

#define DECLARE_CLASS(v) \
	class v ; 
ALLCLASS(DECLARE_CLASS);
#undef DECLARE_CLASS

enum CASEIDX{
	CASE0, CASE1, CASE2, CASE3, CASE4, CASE5, CASE6, CASE7, CASE8, 
	CASE9, CASE10, CASE11, CASE12, CASE13, CASE14, CASE15, CASE16,
	CASE17, CASE18, CASE19, CASE20, CASE21, CASE22, CASE23, CASE24,
	CASE25, CASE26, CASE27, CASE28, CASE29, CASE30, CASE31, CASE32, 
	CASE33, CASE34, CASE35, CASE36, CASE37, CASE38, CASE39, CASE40, 
	CASE41, CASE42, CASE43, CASE44, CASE45, CASE46, CASE47, CASE48,
	CASE49, CASE50, CASE51, CASE52, CASE53, CASE54, CASE55, CASE56, 
	CASE57, CASE58, CASE59, CASE60, CASE61, CASE62, CASE63
};

class IROperator{
public:
    IROperator(TERMINAL prefix, TERMINAL middle, TERMINAL suffix):
        prefix_(prefix), middle_(middle), suffix_(suffix) {}

	TERMINAL prefix_;
	TERMINAL middle_;
	TERMINAL suffix_;
};

class IR{
public:
	IR(IRTYPE type,  IROperator * op, IR * left, IR* right): type_(type), op_(op), left_(left), right_(right) {}

	IR(const IR* ir, IR* left, IR* right){
		this->type_ = ir->type_;

		if(ir->op_ != NULL)
			this->op_ = OP3(ir->op_->prefix_, ir->op_->middle_, ir->op_->suffix_);
		else
			this->op_ = OP3(tEmpty, tEmpty, tEmpty);

		this->left_ = left;
		this->right_ = right;

		this->str_val_ = ir->str_val_;
		this->long_val_ = ir->long_val_;
	}

	union{
		int int_val_;
		long long_val_;
		double float_val_;
		bool bool_val_;
	};

	IRTYPE type_;

	string str_val_;

	IROperator* op_;
	IR* left_;
	IR* right_;

	string to_string();
	string to_string_core();
	void trim_string(string&);
};

class Node{
public:
	Node(){};
	~Node(){};

	char length_;
	unsigned long int vector_;

	unsigned int case_idx_;

	virtual IR* translate() {};
	virtual void deep_delete() {};
};

IR * deep_copy(const IR * root);
void deep_delete(IR * root);

class ParseToplevel: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Stmtmulti* stmtmulti_;
};

class Stmtmulti: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Stmtmulti* stmtmulti_;
	Stmt* stmt_;
};

class Stmt: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	CreateTableStmt* create_table_stmt_;
	CreateViewStmt* create_view_stmt_;
	CreateIndexStmt* create_index_stmt_;
	SelectStmt* select_stmt_;
	DropIndexStmt* drop_index_stmt_;
	DropTableStmt* drop_table_stmt_;
	DropViewStmt* drop_view_stmt_;
	DeleteStmt* delete_stmt_;
	UpdateStmt* update_stmt_;
	InsertStmt* insert_stmt_;
	AlterTableStmt* alter_table_stmt_;
};

class DeleteStmt: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptSimpleLimit* opt_simple_limit_;
	OptOrderClause* opt_order_clause_;
	OptWhereClause* opt_where_clause_;
	OptAlias* opt_alias_;
	TableName* table_name_;
	OptDeleteOptions* opt_delete_options_;
	OptWithClause* opt_with_clause_;
	TableReferenceList* table_reference_list_;
	TableList* table_list_;
};

class OptDeleteOptions: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	DeleteOptions* delete_options_;
};

class DeleteOptions: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	DeleteOption* delete_option_;
	DeleteOptions* delete_options_;
};

class DeleteOption: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class AlterTableStmt: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	AlterList* alter_list_;
	TableName* table_name_;
};

class AlterList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	AlterListItem* alter_list_item_;
	AlterList* alter_list_;
};

class AlterListItem: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptPlace* opt_place_;
	FieldDef* field_def_;
	ColumnName* column_name_1_;
	ColumnName* column_name_2_;
	TableElementList* table_element_list_;
	TableConstraintDef* table_constraint_def_;
	ConstraintName* constraint_name_;
	IndexName* index_name_1_;
	IndexName* index_name_2_;
	ExprRoot* expr_root_;
	Visibility* visibility_;
	TableName* table_name_;
};

class OptPlace: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ColumnName* column_name_;
};

class DropIndexStmt: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	TableName* table_name_;
	IndexName* index_name_;
};

class DropTableStmt: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	TableList* table_list_;
	IfExists* if_exists_;
};

class IfExists: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class TableList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	TableName* table_name_;
	TableList* table_list_;
};

class DropViewStmt: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	TableList* table_list_;
	IfExists* if_exists_;
};

class UpdateStmt: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptSimpleLimit* opt_simple_limit_;
	OptOrderClause* opt_order_clause_;
	OptWhereClause* opt_where_clause_;
	UpdateList* update_list_;
	TableReferenceList* table_reference_list_;
	OptIgnore* opt_ignore_;
	OptLowPriority* opt_low_priority_;
	OptWithClause* opt_with_clause_;
};

class OptSimpleLimit: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Iconst* iconst_;
};

class OptWithClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WithClause* with_clause_;
};

class OptLowPriority: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class InsertStmt: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptInsertUpdateList* opt_insert_update_list_;
	OptValuesReference* opt_values_reference_;
	InsertFromConstructor* insert_from_constructor_;
	TableName* table_name_;
	OptIgnore* opt_ignore_;
	InsertLockOption* insert_lock_option_;
	UpdateList* update_list_;
	InsertQueryExpression* insert_query_expression_;
};

class InsertQueryExpression: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	QueryExpression* query_expression_;
	ColumnList* column_list_;
};

class InsertFromConstructor: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ValuesList* values_list_;
	ColumnList* column_list_;
};

class ValuesList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	RowValue* row_value_;
	ValuesList* values_list_;
};

class RowValue: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptValues* opt_values_;
};

class OptValues: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Values* values_;
};

class Values: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ExprOrDefault* expr_or_default_;
	Values* values_;
};

class ExprOrDefault: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ExprRoot* expr_root_;
};

class OptValuesReference: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptColumnList* opt_column_list_;
	TableName* table_name_;
};

class OptInsertUpdateList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	UpdateList* update_list_;
};

class UpdateList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	UpdateElem* update_elem_;
	UpdateList* update_list_;
};

class UpdateElem: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ExprOrDefault* expr_or_default_;
	Columnref* columnref_;
};

class InsertLockOption: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptIgnore: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class CreateIndexStmt: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	KeyListWithExpression* key_list_with_expression_;
	TableName* table_name_;
	IndexName* index_name_;
	OptUnique* opt_unique_;
};

class OptUnique: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class CreateViewStmt: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	QueryExpression* query_expression_;
	OptColumnList* opt_column_list_;
	ViewName* view_name_;
	OptViewAlgorithm* opt_view_algorithm_;
	OptOrReplace* opt_or_replace_;
};

class OptOrReplace: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptViewAlgorithm: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class CreateTableStmt: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptDuplicateAsQe* opt_duplicate_as_qe_;
	TableElementList* table_element_list_;
	TableName* table_name_;
	OptIfNotExists* opt_if_not_exists_;
	OptTemporary* opt_temporary_;
	DuplicateAsQe* duplicate_as_qe_;
};

class OptTemporary: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptIfNotExists: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptDuplicateAsQe: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	DuplicateAsQe* duplicate_as_qe_;
};

class DuplicateAsQe: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	QueryExpression* query_expression_;
	Duplicate* duplicate_;
};

class Duplicate: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class TableElementList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	TableElement* table_element_;
	TableElementList* table_element_list_;
};

class TableElement: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ColumnDef* column_def_;
	TableConstraintDef* table_constraint_def_;
};

class ColumnDef: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	FieldDef* field_def_;
	ColumnName* column_name_;
};

class FieldDef: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptColumnAttributeList* opt_column_attribute_list_;
	DataType* data_type_;
};

class DataType: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	NumberType* number_type_;
	BoolType* bool_type_;
	StringType* string_type_;
};

class StringType: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptFieldLength* opt_field_length_;
	FieldLength* field_length_;
};

class BoolType: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class NumberType: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptFieldOptions* opt_field_options_;
	IntType* int_type_;
	OptPrecision* opt_precision_;
	RealType* real_type_;
	FloatOptions* float_options_;
	NumericType* numeric_type_;
};

class NumericType: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class RealType: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptPrecision: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Precision* precision_;
};

class IntType: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptFieldOptions: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	FieldOptionList* field_option_list_;
};

class FieldOptionList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	FieldOption* field_option_;
	FieldOptionList* field_option_list_;
};

class FieldOption: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptColumnAttributeList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ColumnAttributeList* column_attribute_list_;
};

class ColumnAttributeList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ColumnAttribute* column_attribute_;
	ColumnAttributeList* column_attribute_list_;
};

class ColumnAttribute: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	NotNull* not_null_;
	DefaultAttribute* default_attribute_;
	Visibility* visibility_;
};

class DefaultAttribute: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ExprRoot* expr_root_;
};

class NotNull: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class Visibility: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptConstraintName: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ConstraintName* constraint_name_;
};

class CheckConstraint: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ExprRoot* expr_root_;
};

class TableConstraintDef: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	KeyListWithExpression* key_list_with_expression_;
	OptIndexName* opt_index_name_;
	ConstraintKeyType* constraint_key_type_;
	OptConstraintName* opt_constraint_name_;
	CheckConstraint* check_constraint_;
	References* references_;
	KeyList* key_list_;
};

class References: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptOnUpdateDelete* opt_on_update_delete_;
	ColumnList* column_list_;
	TableName* table_name_;
};

class OptOnUpdateDelete: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OnUpdateOption* on_update_option_1_;
	OnUpdateOption* on_update_option_2_;
};

class OnUpdateOption: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class KeyList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	KeyPart* key_part_;
	KeyList* key_list_;
};

class KeyListWithExpression: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	KeyPartWithExpression* key_part_with_expression_;
	KeyListWithExpression* key_list_with_expression_;
};

class KeyPartWithExpression: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	KeyPart* key_part_;
	OptOrderingDirection* opt_ordering_direction_;
	ExprRoot* expr_root_;
};

class KeyPart: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptOrderingDirection* opt_ordering_direction_;
	ColumnName* column_name_;
	Iconst* iconst_;
};

class ConstraintKeyType: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptIndexName: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	IndexName* index_name_;
};

class SelectStmt: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	QueryExpression* query_expression_;
};

class QueryExpression: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptLimitClause* opt_limit_clause_;
	OptOrderClause* opt_order_clause_;
	QueryExpressionBody* query_expression_body_;
	WithClause* with_clause_;
};

class QueryExpressionBody: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	QueryPrimary* query_primary_;
	QueryExpressionParens* query_expression_parens_;
	QueryExpressionBody* query_expression_body_1_;
	QueryExpressionBody* query_expression_body_2_;
	OptUnionOption* opt_union_option_;
};

class QueryPrimary: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	QuerySpecification* query_specification_;
};

class QuerySpecification: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptWindowClause* opt_window_clause_;
	OptHavingClause* opt_having_clause_;
	OptGroupClause* opt_group_clause_;
	OptWhereClause* opt_where_clause_;
	OptFromClause* opt_from_clause_;
	SelectItemList* select_item_list_;
	OptSelectOptions* opt_select_options_;
};

class OptWindowClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WindowDefinitionList* window_definition_list_;
};

class WindowDefinitionList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WindowDefinition* window_definition_;
	WindowDefinitionList* window_definition_list_;
};

class WindowDefinition: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WindowSpec* window_spec_;
	WindowName* window_name_;
};

class WindowSpec: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WindowSpecDetails* window_spec_details_;
};

class WindowSpecDetails: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptWindowFrameClause* opt_window_frame_clause_;
	OptWindowOrderByClause* opt_window_order_by_clause_;
	OptPartitionClause* opt_partition_clause_;
	OptExistingWindowName* opt_existing_window_name_;
};

class OptExistingWindowName: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WindowName* window_name_;
};

class OptPartitionClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ExprRootList* expr_root_list_;
};

class OptWindowOrderByClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OrderList* order_list_;
};

class OrderList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OrderExpr* order_expr_;
	OrderList* order_list_;
};

class OrderExpr: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptOrderingDirection* opt_ordering_direction_;
	ExprRoot* expr_root_;
};

class OptOrderingDirection: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptWindowFrameClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WindowFrameExtent* window_frame_extent_;
	WindowFrameUnits* window_frame_units_;
};

class WindowFrameUnits: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class WindowFrameExtent: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WindowFrameStart* window_frame_start_;
	WindowFrameBetween* window_frame_between_;
};

class WindowFrameStart: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Iconst* iconst_;
};

class WindowFrameBetween: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WindowFrameBound* window_frame_bound_;
	WindowFrameStart* window_frame_start_;
};

class WindowFrameBound: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WindowFrameStart* window_frame_start_;
	Iconst* iconst_;
};

class OptHavingClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ExprRoot* expr_root_;
};

class OptGroupClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OlapOpt* olap_opt_;
	ExprRootList* expr_root_list_;
};

class OlapOpt: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptWhereClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WhereClause* where_clause_;
};

class WhereClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ExprRoot* expr_root_;
};

class OptFromClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	FromClause* from_clause_;
};

class FromClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	TableReferenceList* table_reference_list_;
};

class TableReferenceList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	TableReference* table_reference_;
	TableReferenceList* table_reference_list_;
};

class TableReference: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	TableFactor* table_factor_;
	JoinedTable* joined_table_;
};

class TableFactor: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	SingleTable* single_table_;
	DerivedTable* derived_table_;
	JoinedTableParens* joined_table_parens_;
	TableReferenceListParens* table_reference_list_parens_;
};

class TableReferenceListParens: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	TableReferenceList* table_reference_list_;
	TableReference* table_reference_;
};

class JoinedTableParens: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	JoinedTable* joined_table_;
};

class DerivedTable: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptColumnList* opt_column_list_;
	Alias* alias_;
	Subquery* subquery_;
};

class OptColumnList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ColumnList* column_list_;
};

class ColumnList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ColumnName* column_name_;
	ColumnList* column_list_;
};

class Subquery: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	QueryExpressionParens* query_expression_parens_;
};

class SingleTable: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptAlias* opt_alias_;
	TableName* table_name_;
};

class OptAlias: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Alias* alias_;
};

class JoinedTable: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ExprRoot* expr_root_;
	TableReference* table_reference_1_;
	TableReference* table_reference_2_;
	InnerJoinType* inner_join_type_;
	ColumnList* column_list_;
	OuterJoinType* outer_join_type_;
	TableFactor* table_factor_;
	NaturalJoinType* natural_join_type_;
};

class InnerJoinType: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class NaturalJoinType: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptInner* opt_inner_;
	OptOuter* opt_outer_;
};

class OptInner: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptOuter: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OuterJoinType: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptOuter* opt_outer_;
};

class SelectItemList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	SelectItem* select_item_;
	SelectItemList* select_item_list_;
};

class SelectItem: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	TableWild* table_wild_;
	OptAlias* opt_alias_;
	ExprRoot* expr_root_;
};

class TableWild: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	TableName* table_name_;
};

class OptSelectOptions: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	SelectOptionList* select_option_list_;
};

class SelectOptionList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	SelectOption* select_option_;
	SelectOptionList* select_option_list_;
};

class SelectOption: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class QueryExpressionParens: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	QueryExpressionParens* query_expression_parens_;
	QueryExpression* query_expression_;
};

class OptUnionOption: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptOrderClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OrderClause* order_clause_;
};

class OrderClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OrderList* order_list_;
};

class OptLimitClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	LimitClause* limit_clause_;
};

class LimitClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	LimitOptions* limit_options_;
};

class LimitOptions: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Iconst* iconst_1_;
	Iconst* iconst_2_;
};

class WithClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WithList* with_list_;
};

class WithList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	CommonTableExpr* common_table_expr_;
	WithList* with_list_;
};

class CommonTableExpr: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Subquery* subquery_;
	OptColumnList* opt_column_list_;
	TableName* table_name_;
};

class ExprRootList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ExprRoot* expr_root_;
	ExprRootList* expr_root_list_;
};

class ExprRoot: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Expr* expr_;
};

class Expr: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Expr* expr_1_;
	Expr* expr_2_;
	BoolPri* bool_pri_;
};

class BoolPri: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	BoolPri* bool_pri_;
	Predicate* predicate_;
	CompOp* comp_op_;
	AllSubquery* all_subquery_;
};

class Predicate: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	InSubquery* in_subquery_;
	BitExpr* bit_expr_1_;
	BitExpr* bit_expr_2_;
	Expr* expr_;
	ExprList* expr_list_;
	Predicate* predicate_;
	SimpleExpr* simple_expr_;
};

class BitExpr: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	BitExpr* bit_expr_1_;
	BitExpr* bit_expr_2_;
	SimpleExpr* simple_expr_;
};

class SimpleExpr: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Columnref* columnref_;
	Literal* literal_;
	Iconst* iconst_;
	Fconst* fconst_;
	Sconst* sconst_;
	SimpleExpr* simple_expr_;
	Subquery* subquery_;
	TypeCast* type_cast_;
	CaseExpr* case_expr_;
	Function* function_;
	Expr* expr_;
};

class Function: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ControlFunction* control_function_;
	MathFunction* math_function_;
	StringFunction* string_function_;
	AggregateFunction* aggregate_function_;
	WindowFunction* window_function_;
};

class StringFunction: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Expr* expr_1_;
	Expr* expr_2_;
	Expr* expr_3_;
	Expr* expr_4_;
	Expr* expr_5_;
	ExprList* expr_list_;
};

class MathFunction: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Expr* expr_1_;
	Expr* expr_2_;
	Expr* expr_3_;
};

class WindowFunction: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WindowingClause* windowing_clause_;
	Iconst* iconst_;
	OptNullTreatment* opt_null_treatment_;
	OptLeadLagInfo* opt_lead_lag_info_;
	Expr* expr_;
	OptFromFirstLast* opt_from_first_last_;
};

class OptNullTreatment: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptFromFirstLast: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptLeadLagInfo: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	LeadLagInfo* lead_lag_info_;
};

class LeadLagInfo: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Iconst* iconst_;
	Expr* expr_;
};

class AggregateFunction: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptWindowingClause* opt_windowing_clause_;
	Expr* expr_;
	OptDistinct* opt_distinct_;
	ExprList* expr_list_;
	OptGconcatSeparator* opt_gconcat_separator_;
	OptOrderClause* opt_order_clause_;
};

class OptGconcatSeparator: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Sconst* sconst_;
};

class OptDistinct: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class OptWindowingClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WindowingClause* windowing_clause_;
};

class WindowingClause: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WindowNameOrSpec* window_name_or_spec_;
};

class WindowNameOrSpec: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WindowName* window_name_;
	WindowSpec* window_spec_;
};

class ControlFunction: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Expr* expr_1_;
	Expr* expr_2_;
	Expr* expr_3_;
};

class CaseExpr: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptElse* opt_else_;
	WhenClasueList* when_clasue_list_;
	OptExpr* opt_expr_;
};

class WhenClasueList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	WhenList* when_list_;
	WhenClasueList* when_clasue_list_;
};

class WhenList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Expr* expr_1_;
	Expr* expr_2_;
};

class OptExpr: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Expr* expr_;
};

class OptElse: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Expr* expr_;
};

class TypeCast: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	CastType* cast_type_;
	Expr* expr_;
};

class CastType: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	OptFieldLength* opt_field_length_;
	FloatOptions* float_options_;
};

class FloatOptions: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	FieldLength* field_length_;
	Precision* precision_;
};

class Precision: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Iconst* iconst_1_;
	Iconst* iconst_2_;
};

class OptFieldLength: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	FieldLength* field_length_;
};

class FieldLength: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Iconst* iconst_;
};

class Literal: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class Columnref: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	ColumnName* column_name_;
	TableName* table_name_;
};

class CompOp: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

};

class AllSubquery: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Subquery* subquery_;
};

class InSubquery: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Subquery* subquery_;
};

class ExprList: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Expr* expr_;
	ExprList* expr_list_;
};

class Iconst: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	long int long_val_;
};

class Fconst: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	double float_val_;
};

class Sconst: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	string str_val_;
};

class Ident: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	string str_val_;
};

class Alias: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Ident* ident_;
};

class ColumnName: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Ident* ident_;
};

class WindowName: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Ident* ident_;
};

class TableName: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Ident* ident_;
};

class ConstraintName: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Ident* ident_;
};

class IndexName: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Ident* ident_;
};

class ViewName: public Node {
public:
	virtual IR* translate();
	virtual void deep_delete();

	Ident* ident_;
};

#endif