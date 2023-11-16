%{
#include "bison_parser.h"
#include "flex_lexer.h"
#include <stdio.h>
#include <string.h>
int yyerror(YYLTYPE* llocp, ParseToplevel * result, yyscan_t scanner, const char *msg) { /*printf("%s\n", msg);*/ return 0; }
%}
%code requires {
#include "../include/ast.h"
#include "../include/instantiate.h"
#include "parser_typedef.h"
}
%define api.prefix	{ff_}
%define parse.error	verbose
%define api.pure	full
%define api.token.prefix	{SQL_}
%locations

%initial-action {
    @$.first_column = 0;
    @$.last_column = 0;
    @$.first_line = 0;
    @$.last_line = 0;
    @$.total_column = 0;
    @$.string_length = 0;
};

%lex-param { yyscan_t scanner }
%parse-param { ParseToplevel* result }
%parse-param { yyscan_t scanner }

%union FF_STYPE{
	long		long_value;
	char*		charp_value;
	double		double_value;

	ParseToplevel*		parse_toplevel_t;
	Stmtmulti*		stmtmulti_t;
	Stmt*		stmt_t;
	DeleteStmt*		delete_stmt_t;
	OptDeleteOptions*		opt_delete_options_t;
	DeleteOptions*		delete_options_t;
	DeleteOption*		delete_option_t;
	AlterTableStmt*		alter_table_stmt_t;
	AlterList*		alter_list_t;
	AlterListItem*		alter_list_item_t;
	OptPlace*		opt_place_t;
	DropIndexStmt*		drop_index_stmt_t;
	DropTableStmt*		drop_table_stmt_t;
	IfExists*		if_exists_t;
	TableList*		table_list_t;
	DropViewStmt*		drop_view_stmt_t;
	UpdateStmt*		update_stmt_t;
	OptSimpleLimit*		opt_simple_limit_t;
	OptWithClause*		opt_with_clause_t;
	OptLowPriority*		opt_low_priority_t;
	InsertStmt*		insert_stmt_t;
	InsertQueryExpression*		insert_query_expression_t;
	InsertFromConstructor*		insert_from_constructor_t;
	ValuesList*		values_list_t;
	RowValue*		row_value_t;
	OptValues*		opt_values_t;
	Values*		values_t;
	ExprOrDefault*		expr_or_default_t;
	OptValuesReference*		opt_values_reference_t;
	OptInsertUpdateList*		opt_insert_update_list_t;
	UpdateList*		update_list_t;
	UpdateElem*		update_elem_t;
	InsertLockOption*		insert_lock_option_t;
	OptIgnore*		opt_ignore_t;
	CreateIndexStmt*		create_index_stmt_t;
	OptUnique*		opt_unique_t;
	CreateViewStmt*		create_view_stmt_t;
	OptOrReplace*		opt_or_replace_t;
	OptViewAlgorithm*		opt_view_algorithm_t;
	CreateTableStmt*		create_table_stmt_t;
	OptTemporary*		opt_temporary_t;
	OptIfNotExists*		opt_if_not_exists_t;
	OptDuplicateAsQe*		opt_duplicate_as_qe_t;
	DuplicateAsQe*		duplicate_as_qe_t;
	Duplicate*		duplicate_t;
	TableElementList*		table_element_list_t;
	TableElement*		table_element_t;
	ColumnDef*		column_def_t;
	FieldDef*		field_def_t;
	DataType*		data_type_t;
	StringType*		string_type_t;
	BoolType*		bool_type_t;
	NumberType*		number_type_t;
	NumericType*		numeric_type_t;
	RealType*		real_type_t;
	OptPrecision*		opt_precision_t;
	IntType*		int_type_t;
	OptFieldOptions*		opt_field_options_t;
	FieldOptionList*		field_option_list_t;
	FieldOption*		field_option_t;
	OptColumnAttributeList*		opt_column_attribute_list_t;
	ColumnAttributeList*		column_attribute_list_t;
	ColumnAttribute*		column_attribute_t;
	DefaultAttribute*		default_attribute_t;
	NotNull*		not_null_t;
	Visibility*		visibility_t;
	OptConstraintName*		opt_constraint_name_t;
	CheckConstraint*		check_constraint_t;
	TableConstraintDef*		table_constraint_def_t;
	References*		references_t;
	OptOnUpdateDelete*		opt_on_update_delete_t;
	OnUpdateOption*		on_update_option_t;
	KeyList*		key_list_t;
	KeyListWithExpression*		key_list_with_expression_t;
	KeyPartWithExpression*		key_part_with_expression_t;
	KeyPart*		key_part_t;
	ConstraintKeyType*		constraint_key_type_t;
	OptIndexName*		opt_index_name_t;
	SelectStmt*		select_stmt_t;
	QueryExpression*		query_expression_t;
	QueryExpressionBody*		query_expression_body_t;
	QueryPrimary*		query_primary_t;
	QuerySpecification*		query_specification_t;
	OptWindowClause*		opt_window_clause_t;
	WindowDefinitionList*		window_definition_list_t;
	WindowDefinition*		window_definition_t;
	WindowSpec*		window_spec_t;
	WindowSpecDetails*		window_spec_details_t;
	OptExistingWindowName*		opt_existing_window_name_t;
	OptPartitionClause*		opt_partition_clause_t;
	OptWindowOrderByClause*		opt_window_order_by_clause_t;
	OrderList*		order_list_t;
	OrderExpr*		order_expr_t;
	OptOrderingDirection*		opt_ordering_direction_t;
	OptWindowFrameClause*		opt_window_frame_clause_t;
	WindowFrameUnits*		window_frame_units_t;
	WindowFrameExtent*		window_frame_extent_t;
	WindowFrameStart*		window_frame_start_t;
	WindowFrameBetween*		window_frame_between_t;
	WindowFrameBound*		window_frame_bound_t;
	OptHavingClause*		opt_having_clause_t;
	OptGroupClause*		opt_group_clause_t;
	OlapOpt*		olap_opt_t;
	OptWhereClause*		opt_where_clause_t;
	WhereClause*		where_clause_t;
	OptFromClause*		opt_from_clause_t;
	FromClause*		from_clause_t;
	TableReferenceList*		table_reference_list_t;
	TableReference*		table_reference_t;
	TableFactor*		table_factor_t;
	TableReferenceListParens*		table_reference_list_parens_t;
	JoinedTableParens*		joined_table_parens_t;
	DerivedTable*		derived_table_t;
	OptColumnList*		opt_column_list_t;
	ColumnList*		column_list_t;
	Subquery*		subquery_t;
	SingleTable*		single_table_t;
	OptAlias*		opt_alias_t;
	JoinedTable*		joined_table_t;
	InnerJoinType*		inner_join_type_t;
	NaturalJoinType*		natural_join_type_t;
	OptInner*		opt_inner_t;
	OptOuter*		opt_outer_t;
	OuterJoinType*		outer_join_type_t;
	SelectItemList*		select_item_list_t;
	SelectItem*		select_item_t;
	TableWild*		table_wild_t;
	OptSelectOptions*		opt_select_options_t;
	SelectOptionList*		select_option_list_t;
	SelectOption*		select_option_t;
	QueryExpressionParens*		query_expression_parens_t;
	OptUnionOption*		opt_union_option_t;
	OptOrderClause*		opt_order_clause_t;
	OrderClause*		order_clause_t;
	OptLimitClause*		opt_limit_clause_t;
	LimitClause*		limit_clause_t;
	LimitOptions*		limit_options_t;
	WithClause*		with_clause_t;
	WithList*		with_list_t;
	CommonTableExpr*		common_table_expr_t;
	ExprRootList*		expr_root_list_t;
	ExprRoot*		expr_root_t;
	Expr*		expr_t;
	BoolPri*		bool_pri_t;
	Predicate*		predicate_t;
	BitExpr*		bit_expr_t;
	SimpleExpr*		simple_expr_t;
	Function*		function_t;
	StringFunction*		string_function_t;
	MathFunction*		math_function_t;
	WindowFunction*		window_function_t;
	OptNullTreatment*		opt_null_treatment_t;
	OptFromFirstLast*		opt_from_first_last_t;
	OptLeadLagInfo*		opt_lead_lag_info_t;
	LeadLagInfo*		lead_lag_info_t;
	AggregateFunction*		aggregate_function_t;
	OptGconcatSeparator*		opt_gconcat_separator_t;
	OptDistinct*		opt_distinct_t;
	OptWindowingClause*		opt_windowing_clause_t;
	WindowingClause*		windowing_clause_t;
	WindowNameOrSpec*		window_name_or_spec_t;
	ControlFunction*		control_function_t;
	CaseExpr*		case_expr_t;
	WhenClasueList*		when_clasue_list_t;
	WhenList*		when_list_t;
	OptExpr*		opt_expr_t;
	OptElse*		opt_else_t;
	TypeCast*		type_cast_t;
	CastType*		cast_type_t;
	FloatOptions*		float_options_t;
	Precision*		precision_t;
	OptFieldLength*		opt_field_length_t;
	FieldLength*		field_length_t;
	Literal*		literal_t;
	Columnref*		columnref_t;
	CompOp*		comp_op_t;
	AllSubquery*		all_subquery_t;
	InSubquery*		in_subquery_t;
	ExprList*		expr_list_t;
	Iconst*		iconst_t;
	Fconst*		fconst_t;
	Sconst*		sconst_t;
	Ident*		ident_t;
	Alias*		alias_t;
	ColumnName*		column_name_t;
	WindowName*		window_name_t;
	TableName*		table_name_t;
	ConstraintName*		constraint_name_t;
	IndexName*		index_name_t;
	ViewName*		view_name_t;
}

%token OP_SEMI FROM DELETE  QUICK LOW_PRIORITY IGNORE ALTER TABLE OP_COMMA 
%token ADD COLUMN OP_RP OP_LP CHANGE MODIFY DROP FOREIGN KEY PRIMARY 
%token INDEX CHECK CONSTRAINT SET DEFAULT RENAME TO AFTER FIRST ON 
%token IF EXISTS VIEW UPDATE LIMIT INTO INSERT VALUES AS DUPLICATE 
%token OP_EQUAL DELAYED HIGH_PRIORITY CREATE UNIQUE OR REPLACE ALGORITHM UNDEFINED MERGE 
%token TEMPTABLE TEMPORARY NOT CHAR BINARY VARCHAR VARBINARY LONG TINYBLOB MEDIUMBLOB 
%token LONGBLOB BLOB TINYTEXT MEDIUMTEXT LONGTEXT TEXT BOOL BOOLEAN FLOAT DECIMAL 
%token NUMERIC FIXED REAL DOUBLE INT TINYINT SMALLINT MEDIUMINT BIGINT SIGNED 
%token UNSIGNED ZEROFILL NULL VISIBLE INVISIBLE REFERENCES CASCADE NO ACTION UNION 
%token EXCEPT INTERSECT SELECT WINDOW PARTITION BY ORDER ASC DESC ROWS 
%token RANGE UNBOUNDED PRECEDING CURRENT ROW AND BETWEEN FOLLOWING HAVING GROUP 
%token WITH ROLLUP WHERE LATERAL USING JOIN INNER CROSS STRAIGHT_JOIN NATURAL 
%token RIGHT LEFT OUTER OP_MUL OP_DOT SQL_NO_CACHE DISTINCT SQL_SMALL_RESULT SQL_BIG_RESULT SQL_BUFFER_RESULT 
%token SQL_CALC_FOUND_ROWS ALL RECURSIVE XOR IS TRUE FALSE UNKNOWN IN SOUNDS 
%token LIKE REGEXP OP_OR OP_AND OP_SHL OP_SHR OP_ADD OP_SUB OP_DIVIDE OP_MOD 
%token OP_XOR OP_NOT ASCII BIN BIT_LENGTH CHAR_LENGTH LENGTH LOWER LTRIM OCT 
%token ORD QUOTE REVERSE RTRIM SPACE TRIM LEADING TRAILING UNHEX UPPER 
%token FIND_IN_SET INSTR LOCATE REPEAT SUBSTRING EXPORT_SET LPAD RPAD SUBSTRING_INDEX CONCAT 
%token CONCAT_WS ELT FIELD MAKE_SET PI RAND ABS ACOS ASIN ATAN 
%token CEILING COS COT DEGREES EXP FLOOR HEX LN RADIANS ROUND 
%token SIGN SIN SQRT TAN FORMAT LOG MOD POW TRUNCATE CONV 
%token ROW_NUMBER RANK DENSE_RANK CUME_DIST PERCENT_RANK NTILE LEAD LAG FIRST_VALUE LAST_VALUE 
%token NTH_VALUE RESPECT NULLS LAST AVG BIT_AND BIT_OR BIT_XOR COUNT MIN 
%token MAX SUM STD STDDEV_SAMP VARIANCE VAR_SAMP GROUP_CONCAT SEPARATOR OVER IFNULL 
%token NULLIF END CASE THEN WHEN ELSE CAST CONVERT NCHAR OP_GREATEREQ 
%token OP_GREATERTHAN OP_LESSEQ OP_LESSTHAN OP_NOTEQUAL ANY SOME 

%token <charp_value> IDENT
%token <long_value> ICONST
%token <double_value> FCONST
%token <charp_value> SCONST

%type <parse_toplevel_t> parse_toplevel
%type <stmtmulti_t> stmtmulti
%type <stmt_t> stmt
%type <delete_stmt_t> delete_stmt
%type <opt_delete_options_t> opt_delete_options
%type <delete_options_t> delete_options
%type <delete_option_t> delete_option
%type <alter_table_stmt_t> alter_table_stmt
%type <alter_list_t> alter_list
%type <alter_list_item_t> alter_list_item
%type <opt_place_t> opt_place
%type <drop_index_stmt_t> drop_index_stmt
%type <drop_table_stmt_t> drop_table_stmt
%type <if_exists_t> if_exists
%type <table_list_t> table_list
%type <drop_view_stmt_t> drop_view_stmt
%type <update_stmt_t> update_stmt
%type <opt_simple_limit_t> opt_simple_limit
%type <opt_with_clause_t> opt_with_clause
%type <opt_low_priority_t> opt_low_priority
%type <insert_stmt_t> insert_stmt
%type <insert_query_expression_t> insert_query_expression
%type <insert_from_constructor_t> insert_from_constructor
%type <values_list_t> values_list
%type <row_value_t> row_value
%type <opt_values_t> opt_values
%type <values_t> values
%type <expr_or_default_t> expr_or_default
%type <opt_values_reference_t> opt_values_reference
%type <opt_insert_update_list_t> opt_insert_update_list
%type <update_list_t> update_list
%type <update_elem_t> update_elem
%type <insert_lock_option_t> insert_lock_option
%type <opt_ignore_t> opt_ignore
%type <create_index_stmt_t> create_index_stmt
%type <opt_unique_t> opt_unique
%type <create_view_stmt_t> create_view_stmt
%type <opt_or_replace_t> opt_or_replace
%type <opt_view_algorithm_t> opt_view_algorithm
%type <create_table_stmt_t> create_table_stmt
%type <opt_temporary_t> opt_temporary
%type <opt_if_not_exists_t> opt_if_not_exists
%type <opt_duplicate_as_qe_t> opt_duplicate_as_qe
%type <duplicate_as_qe_t> duplicate_as_qe
%type <duplicate_t> duplicate
%type <table_element_list_t> table_element_list
%type <table_element_t> table_element
%type <column_def_t> column_def
%type <field_def_t> field_def
%type <data_type_t> data_type
%type <string_type_t> string_type
%type <bool_type_t> bool_type
%type <number_type_t> number_type
%type <numeric_type_t> numeric_type
%type <real_type_t> real_type
%type <opt_precision_t> opt_precision
%type <int_type_t> int_type
%type <opt_field_options_t> opt_field_options
%type <field_option_list_t> field_option_list
%type <field_option_t> field_option
%type <opt_column_attribute_list_t> opt_column_attribute_list
%type <column_attribute_list_t> column_attribute_list
%type <column_attribute_t> column_attribute
%type <default_attribute_t> default_attribute
%type <not_null_t> not_null
%type <visibility_t> visibility
%type <opt_constraint_name_t> opt_constraint_name
%type <check_constraint_t> check_constraint
%type <table_constraint_def_t> table_constraint_def
%type <references_t> references
%type <opt_on_update_delete_t> opt_on_update_delete
%type <on_update_option_t> on_update_option
%type <key_list_t> key_list
%type <key_list_with_expression_t> key_list_with_expression
%type <key_part_with_expression_t> key_part_with_expression
%type <key_part_t> key_part
%type <constraint_key_type_t> constraint_key_type
%type <opt_index_name_t> opt_index_name
%type <select_stmt_t> select_stmt
%type <query_expression_t> query_expression
%type <query_expression_body_t> query_expression_body
%type <query_primary_t> query_primary
%type <query_specification_t> query_specification
%type <opt_window_clause_t> opt_window_clause
%type <window_definition_list_t> window_definition_list
%type <window_definition_t> window_definition
%type <window_spec_t> window_spec
%type <window_spec_details_t> window_spec_details
%type <opt_existing_window_name_t> opt_existing_window_name
%type <opt_partition_clause_t> opt_partition_clause
%type <opt_window_order_by_clause_t> opt_window_order_by_clause
%type <order_list_t> order_list
%type <order_expr_t> order_expr
%type <opt_ordering_direction_t> opt_ordering_direction
%type <opt_window_frame_clause_t> opt_window_frame_clause
%type <window_frame_units_t> window_frame_units
%type <window_frame_extent_t> window_frame_extent
%type <window_frame_start_t> window_frame_start
%type <window_frame_between_t> window_frame_between
%type <window_frame_bound_t> window_frame_bound
%type <opt_having_clause_t> opt_having_clause
%type <opt_group_clause_t> opt_group_clause
%type <olap_opt_t> olap_opt
%type <opt_where_clause_t> opt_where_clause
%type <where_clause_t> where_clause
%type <opt_from_clause_t> opt_from_clause
%type <from_clause_t> from_clause
%type <table_reference_list_t> table_reference_list
%type <table_reference_t> table_reference
%type <table_factor_t> table_factor
%type <table_reference_list_parens_t> table_reference_list_parens
%type <joined_table_parens_t> joined_table_parens
%type <derived_table_t> derived_table
%type <opt_column_list_t> opt_column_list
%type <column_list_t> column_list
%type <subquery_t> subquery
%type <single_table_t> single_table
%type <opt_alias_t> opt_alias
%type <joined_table_t> joined_table
%type <inner_join_type_t> inner_join_type
%type <natural_join_type_t> natural_join_type
%type <opt_inner_t> opt_inner
%type <opt_outer_t> opt_outer
%type <outer_join_type_t> outer_join_type
%type <select_item_list_t> select_item_list
%type <select_item_t> select_item
%type <table_wild_t> table_wild
%type <opt_select_options_t> opt_select_options
%type <select_option_list_t> select_option_list
%type <select_option_t> select_option
%type <query_expression_parens_t> query_expression_parens
%type <opt_union_option_t> opt_union_option
%type <opt_order_clause_t> opt_order_clause
%type <order_clause_t> order_clause
%type <opt_limit_clause_t> opt_limit_clause
%type <limit_clause_t> limit_clause
%type <limit_options_t> limit_options
%type <with_clause_t> with_clause
%type <with_list_t> with_list
%type <common_table_expr_t> common_table_expr
%type <expr_root_list_t> expr_root_list
%type <expr_root_t> expr_root
%type <expr_t> expr
%type <bool_pri_t> bool_pri
%type <predicate_t> predicate
%type <bit_expr_t> bit_expr
%type <simple_expr_t> simple_expr
%type <function_t> function
%type <string_function_t> string_function
%type <math_function_t> math_function
%type <window_function_t> window_function
%type <opt_null_treatment_t> opt_null_treatment
%type <opt_from_first_last_t> opt_from_first_last
%type <opt_lead_lag_info_t> opt_lead_lag_info
%type <lead_lag_info_t> lead_lag_info
%type <aggregate_function_t> aggregate_function
%type <opt_gconcat_separator_t> opt_gconcat_separator
%type <opt_distinct_t> opt_distinct
%type <opt_windowing_clause_t> opt_windowing_clause
%type <windowing_clause_t> windowing_clause
%type <window_name_or_spec_t> window_name_or_spec
%type <control_function_t> control_function
%type <case_expr_t> case_expr
%type <when_clasue_list_t> when_clasue_list
%type <when_list_t> when_list
%type <opt_expr_t> opt_expr
%type <opt_else_t> opt_else
%type <type_cast_t> type_cast
%type <cast_type_t> cast_type
%type <float_options_t> float_options
%type <precision_t> precision
%type <opt_field_length_t> opt_field_length
%type <field_length_t> field_length
%type <literal_t> literal
%type <columnref_t> columnref
%type <comp_op_t> comp_op
%type <all_subquery_t> all_subquery
%type <in_subquery_t> in_subquery
%type <expr_list_t> expr_list
%type <iconst_t> iconst
%type <fconst_t> fconst
%type <sconst_t> sconst
%type <ident_t> ident
%type <alias_t> alias
%type <column_name_t> column_name
%type <window_name_t> window_name
%type <table_name_t> table_name
%type <constraint_name_t> constraint_name
%type <index_name_t> index_name
%type <view_name_t> view_name

%left  UNION EXCEPT
%left INTERSECT
%left CONDITIONLESS_JOIN
%left  JOIN INNER CROSS STRAIGHT_JOIN NATURAL LEFT RIGHT ON USING
%left  OR
%left  XOR
%left  AND
%left   BETWEEN CASE WHEN THEN ELSE
%left   OP_GREATERTHAN OP_LESSTHAN OP_GREATEREQ OP_LESSEQ OP_NOTEQUAL OP_EQUAL IS LIKE REGEXP IN
%left   OP_OR
%left   OP_AND
%left   OP_SHL OP_SHR 
%left  OP_ADD OP_SUB
%left  OP_MUL OP_MOD OP_DIVIDE
%left   OP_XOR
%left   UMINUS OP_NOT
%right  NOT
%left SUBQUERY_AS_EXPR
%left  OP_RP OP_LP

%destructor{
	free( ($$) );
}  <charp_value>

%destructor{
	 
}  <double_value> <long_value> 

%destructor { if($$!=NULL)$$->deep_delete(); } <*>

%start parse_toplevel

%%

parse_toplevel:
	 stmtmulti {
		$$ = result;
		$$->case_idx_ = CASE0;

		$$->stmtmulti_ = $1;

		$$ = NULL; 
	 }
	;

stmtmulti:
	 stmt OP_SEMI stmtmulti {
		$$ = new Stmtmulti();
		$$->case_idx_ = CASE0;

		$$->stmt_ = $1;
		$$->stmtmulti_ = $3; 
	 }
	|stmt OP_SEMI {
		$$ = new Stmtmulti();
		$$->case_idx_ = CASE1;

		$$->stmt_ = $1; 
	 }
	;

stmt:
	 create_table_stmt {
		$$ = new Stmt();
		$$->case_idx_ = CASE0;

		$$->create_table_stmt_ = $1; 
	 }
	|create_view_stmt {
		$$ = new Stmt();
		$$->case_idx_ = CASE1;

		$$->create_view_stmt_ = $1; 
	 }
	|create_index_stmt {
		$$ = new Stmt();
		$$->case_idx_ = CASE2;

		$$->create_index_stmt_ = $1; 
	 }
	|select_stmt {
		$$ = new Stmt();
		$$->case_idx_ = CASE3;

		$$->select_stmt_ = $1; 
	 }
	|drop_index_stmt {
		$$ = new Stmt();
		$$->case_idx_ = CASE4;

		$$->drop_index_stmt_ = $1; 
	 }
	|drop_table_stmt {
		$$ = new Stmt();
		$$->case_idx_ = CASE5;

		$$->drop_table_stmt_ = $1; 
	 }
	|drop_view_stmt {
		$$ = new Stmt();
		$$->case_idx_ = CASE6;

		$$->drop_view_stmt_ = $1; 
	 }
	|delete_stmt {
		$$ = new Stmt();
		$$->case_idx_ = CASE7;

		$$->delete_stmt_ = $1; 
	 }
	|update_stmt {
		$$ = new Stmt();
		$$->case_idx_ = CASE8;

		$$->update_stmt_ = $1; 
	 }
	|insert_stmt {
		$$ = new Stmt();
		$$->case_idx_ = CASE9;

		$$->insert_stmt_ = $1; 
	 }
	|alter_table_stmt {
		$$ = new Stmt();
		$$->case_idx_ = CASE10;

		$$->alter_table_stmt_ = $1; 
	 }
	;

delete_stmt:
	 opt_with_clause DELETE opt_delete_options FROM table_name opt_alias opt_where_clause opt_order_clause opt_simple_limit {
		$$ = new DeleteStmt();
		$$->case_idx_ = CASE0;

		$$->opt_with_clause_ = $1;
		$$->opt_delete_options_ = $3;
		$$->table_name_ = $5;
		$$->opt_alias_ = $6;
		$$->opt_where_clause_ = $7;
		$$->opt_order_clause_ = $8;
		$$->opt_simple_limit_ = $9; 
	 }
	|opt_with_clause DELETE opt_delete_options table_list FROM table_reference_list opt_where_clause {
		$$ = new DeleteStmt();
		$$->case_idx_ = CASE1;

		$$->opt_with_clause_ = $1;
		$$->opt_delete_options_ = $3;
		$$->table_list_ = $4;
		$$->table_reference_list_ = $6;
		$$->opt_where_clause_ = $7; 
	 }
	;

opt_delete_options:
	 	 {
		$$ = new OptDeleteOptions();
		$$->case_idx_ = CASE0; 
	 }
	|delete_options {
		$$ = new OptDeleteOptions();
		$$->case_idx_ = CASE1;

		$$->delete_options_ = $1; 
	 }
	;

delete_options:
	 delete_option {
		$$ = new DeleteOptions();
		$$->case_idx_ = CASE0;

		$$->delete_option_ = $1; 
	 }
	|delete_option delete_options {
		$$ = new DeleteOptions();
		$$->case_idx_ = CASE1;

		$$->delete_option_ = $1;
		$$->delete_options_ = $2; 
	 }
	;

delete_option:
	 QUICK {
		$$ = new DeleteOption();
		$$->case_idx_ = CASE0; 
	 }
	|LOW_PRIORITY {
		$$ = new DeleteOption();
		$$->case_idx_ = CASE1; 
	 }
	|IGNORE {
		$$ = new DeleteOption();
		$$->case_idx_ = CASE2; 
	 }
	;

alter_table_stmt:
	 ALTER TABLE table_name alter_list {
		$$ = new AlterTableStmt();
		$$->case_idx_ = CASE0;

		$$->table_name_ = $3;
		$$->alter_list_ = $4; 
	 }
	;

alter_list:
	 alter_list_item {
		$$ = new AlterList();
		$$->case_idx_ = CASE0;

		$$->alter_list_item_ = $1; 
	 }
	|alter_list_item OP_COMMA alter_list {
		$$ = new AlterList();
		$$->case_idx_ = CASE1;

		$$->alter_list_item_ = $1;
		$$->alter_list_ = $3; 
	 }
	;

alter_list_item:
	 ADD COLUMN column_name field_def opt_place {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE0;

		$$->column_name_1_ = $3;
		$$->field_def_ = $4;
		$$->opt_place_ = $5; 
	 }
	|ADD COLUMN OP_LP table_element_list OP_RP {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE1;

		$$->table_element_list_ = $4; 
	 }
	|ADD table_constraint_def {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE2;

		$$->table_constraint_def_ = $2; 
	 }
	|CHANGE COLUMN column_name column_name field_def opt_place {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE3;

		$$->column_name_1_ = $3;
		$$->column_name_2_ = $4;
		$$->field_def_ = $5;
		$$->opt_place_ = $6; 
	 }
	|MODIFY COLUMN column_name field_def opt_place {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE4;

		$$->column_name_1_ = $3;
		$$->field_def_ = $4;
		$$->opt_place_ = $5; 
	 }
	|DROP COLUMN column_name {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE5;

		$$->column_name_1_ = $3; 
	 }
	|DROP FOREIGN KEY constraint_name {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE6;

		$$->constraint_name_ = $4; 
	 }
	|DROP PRIMARY KEY {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE7; 
	 }
	|DROP INDEX index_name {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE8;

		$$->index_name_1_ = $3; 
	 }
	|DROP CHECK constraint_name {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE9;

		$$->constraint_name_ = $3; 
	 }
	|DROP CONSTRAINT constraint_name {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE10;

		$$->constraint_name_ = $3; 
	 }
	|ALTER COLUMN column_name SET DEFAULT OP_LP expr_root OP_RP {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE11;

		$$->column_name_1_ = $3;
		$$->expr_root_ = $7; 
	 }
	|ALTER COLUMN column_name DROP DEFAULT {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE12;

		$$->column_name_1_ = $3; 
	 }
	|ALTER COLUMN column_name SET visibility {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE13;

		$$->column_name_1_ = $3;
		$$->visibility_ = $5; 
	 }
	|ALTER INDEX index_name visibility {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE14;

		$$->index_name_1_ = $3;
		$$->visibility_ = $4; 
	 }
	|RENAME TO table_name {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE15;

		$$->table_name_ = $3; 
	 }
	|RENAME INDEX index_name TO index_name {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE16;

		$$->index_name_1_ = $3;
		$$->index_name_2_ = $5; 
	 }
	|RENAME COLUMN column_name TO column_name {
		$$ = new AlterListItem();
		$$->case_idx_ = CASE17;

		$$->column_name_1_ = $3;
		$$->column_name_2_ = $5; 
	 }
	;

opt_place:
	 	 {
		$$ = new OptPlace();
		$$->case_idx_ = CASE0; 
	 }
	|AFTER column_name {
		$$ = new OptPlace();
		$$->case_idx_ = CASE1;

		$$->column_name_ = $2; 
	 }
	|FIRST {
		$$ = new OptPlace();
		$$->case_idx_ = CASE2; 
	 }
	;

drop_index_stmt:
	 DROP INDEX index_name ON table_name {
		$$ = new DropIndexStmt();
		$$->case_idx_ = CASE0;

		$$->index_name_ = $3;
		$$->table_name_ = $5; 
	 }
	;

drop_table_stmt:
	 DROP TABLE if_exists table_list {
		$$ = new DropTableStmt();
		$$->case_idx_ = CASE0;

		$$->if_exists_ = $3;
		$$->table_list_ = $4; 
	 }
	;

if_exists:
	 	 {
		$$ = new IfExists();
		$$->case_idx_ = CASE0; 
	 }
	|IF EXISTS {
		$$ = new IfExists();
		$$->case_idx_ = CASE1; 
	 }
	;

table_list:
	 table_name {
		$$ = new TableList();
		$$->case_idx_ = CASE0;

		$$->table_name_ = $1; 
	 }
	|table_name OP_COMMA table_list {
		$$ = new TableList();
		$$->case_idx_ = CASE1;

		$$->table_name_ = $1;
		$$->table_list_ = $3; 
	 }
	;

drop_view_stmt:
	 DROP VIEW if_exists table_list {
		$$ = new DropViewStmt();
		$$->case_idx_ = CASE0;

		$$->if_exists_ = $3;
		$$->table_list_ = $4; 
	 }
	;

update_stmt:
	 opt_with_clause UPDATE opt_low_priority opt_ignore table_reference_list SET update_list opt_where_clause opt_order_clause opt_simple_limit {
		$$ = new UpdateStmt();
		$$->case_idx_ = CASE0;

		$$->opt_with_clause_ = $1;
		$$->opt_low_priority_ = $3;
		$$->opt_ignore_ = $4;
		$$->table_reference_list_ = $5;
		$$->update_list_ = $7;
		$$->opt_where_clause_ = $8;
		$$->opt_order_clause_ = $9;
		$$->opt_simple_limit_ = $10; 
	 }
	;

opt_simple_limit:
	 	 {
		$$ = new OptSimpleLimit();
		$$->case_idx_ = CASE0; 
	 }
	|LIMIT iconst {
		$$ = new OptSimpleLimit();
		$$->case_idx_ = CASE1;

		$$->iconst_ = $2; 
	 }
	;

opt_with_clause:
	 	 {
		$$ = new OptWithClause();
		$$->case_idx_ = CASE0; 
	 }
	|with_clause {
		$$ = new OptWithClause();
		$$->case_idx_ = CASE1;

		$$->with_clause_ = $1; 
	 }
	;

opt_low_priority:
	 	 {
		$$ = new OptLowPriority();
		$$->case_idx_ = CASE0; 
	 }
	|LOW_PRIORITY {
		$$ = new OptLowPriority();
		$$->case_idx_ = CASE1; 
	 }
	;

insert_stmt:
	 INSERT insert_lock_option opt_ignore INTO table_name insert_from_constructor opt_values_reference opt_insert_update_list {
		$$ = new InsertStmt();
		$$->case_idx_ = CASE0;

		$$->insert_lock_option_ = $2;
		$$->opt_ignore_ = $3;
		$$->table_name_ = $5;
		$$->insert_from_constructor_ = $6;
		$$->opt_values_reference_ = $7;
		$$->opt_insert_update_list_ = $8; 
	 }
	|INSERT insert_lock_option opt_ignore INTO table_name SET update_list opt_values_reference opt_insert_update_list {
		$$ = new InsertStmt();
		$$->case_idx_ = CASE1;

		$$->insert_lock_option_ = $2;
		$$->opt_ignore_ = $3;
		$$->table_name_ = $5;
		$$->update_list_ = $7;
		$$->opt_values_reference_ = $8;
		$$->opt_insert_update_list_ = $9; 
	 }
	|INSERT insert_lock_option opt_ignore INTO table_name insert_query_expression opt_insert_update_list {
		$$ = new InsertStmt();
		$$->case_idx_ = CASE2;

		$$->insert_lock_option_ = $2;
		$$->opt_ignore_ = $3;
		$$->table_name_ = $5;
		$$->insert_query_expression_ = $6;
		$$->opt_insert_update_list_ = $7; 
	 }
	;

insert_query_expression:
	 query_expression {
		$$ = new InsertQueryExpression();
		$$->case_idx_ = CASE0;

		$$->query_expression_ = $1; 
	 }
	|OP_LP column_list OP_RP query_expression {
		$$ = new InsertQueryExpression();
		$$->case_idx_ = CASE1;

		$$->column_list_ = $2;
		$$->query_expression_ = $4; 
	 }
	;

insert_from_constructor:
	 VALUES values_list {
		$$ = new InsertFromConstructor();
		$$->case_idx_ = CASE0;

		$$->values_list_ = $2; 
	 }
	|OP_LP column_list OP_RP VALUES values_list {
		$$ = new InsertFromConstructor();
		$$->case_idx_ = CASE1;

		$$->column_list_ = $2;
		$$->values_list_ = $5; 
	 }
	;

values_list:
	 row_value {
		$$ = new ValuesList();
		$$->case_idx_ = CASE0;

		$$->row_value_ = $1; 
	 }
	|row_value OP_COMMA values_list {
		$$ = new ValuesList();
		$$->case_idx_ = CASE1;

		$$->row_value_ = $1;
		$$->values_list_ = $3; 
	 }
	;

row_value:
	 OP_LP opt_values OP_RP {
		$$ = new RowValue();
		$$->case_idx_ = CASE0;

		$$->opt_values_ = $2; 
	 }
	;

opt_values:
	 	 {
		$$ = new OptValues();
		$$->case_idx_ = CASE0; 
	 }
	|values {
		$$ = new OptValues();
		$$->case_idx_ = CASE1;

		$$->values_ = $1; 
	 }
	;

values:
	 expr_or_default {
		$$ = new Values();
		$$->case_idx_ = CASE0;

		$$->expr_or_default_ = $1; 
	 }
	|expr_or_default OP_COMMA values {
		$$ = new Values();
		$$->case_idx_ = CASE1;

		$$->expr_or_default_ = $1;
		$$->values_ = $3; 
	 }
	;

expr_or_default:
	 expr_root {
		$$ = new ExprOrDefault();
		$$->case_idx_ = CASE0;

		$$->expr_root_ = $1; 
	 }
	|DEFAULT {
		$$ = new ExprOrDefault();
		$$->case_idx_ = CASE1; 
	 }
	;

opt_values_reference:
	 	 {
		$$ = new OptValuesReference();
		$$->case_idx_ = CASE0; 
	 }
	|AS table_name opt_column_list {
		$$ = new OptValuesReference();
		$$->case_idx_ = CASE1;

		$$->table_name_ = $2;
		$$->opt_column_list_ = $3; 
	 }
	;

opt_insert_update_list:
	 	 {
		$$ = new OptInsertUpdateList();
		$$->case_idx_ = CASE0; 
	 }
	|ON DUPLICATE KEY UPDATE update_list {
		$$ = new OptInsertUpdateList();
		$$->case_idx_ = CASE1;

		$$->update_list_ = $5; 
	 }
	;

update_list:
	 update_elem {
		$$ = new UpdateList();
		$$->case_idx_ = CASE0;

		$$->update_elem_ = $1; 
	 }
	|update_elem OP_COMMA update_list {
		$$ = new UpdateList();
		$$->case_idx_ = CASE1;

		$$->update_elem_ = $1;
		$$->update_list_ = $3; 
	 }
	;

update_elem:
	 columnref OP_EQUAL expr_or_default {
		$$ = new UpdateElem();
		$$->case_idx_ = CASE0;

		$$->columnref_ = $1;
		$$->expr_or_default_ = $3; 
	 }
	;

insert_lock_option:
	 	 {
		$$ = new InsertLockOption();
		$$->case_idx_ = CASE0; 
	 }
	|LOW_PRIORITY {
		$$ = new InsertLockOption();
		$$->case_idx_ = CASE1; 
	 }
	|DELAYED {
		$$ = new InsertLockOption();
		$$->case_idx_ = CASE2; 
	 }
	|HIGH_PRIORITY {
		$$ = new InsertLockOption();
		$$->case_idx_ = CASE3; 
	 }
	;

opt_ignore:
	 	 {
		$$ = new OptIgnore();
		$$->case_idx_ = CASE0; 
	 }
	|IGNORE {
		$$ = new OptIgnore();
		$$->case_idx_ = CASE1; 
	 }
	;

create_index_stmt:
	 CREATE opt_unique INDEX index_name ON table_name OP_LP key_list_with_expression OP_RP {
		$$ = new CreateIndexStmt();
		$$->case_idx_ = CASE0;

		$$->opt_unique_ = $2;
		$$->index_name_ = $4;
		$$->table_name_ = $6;
		$$->key_list_with_expression_ = $8; 
	 }
	;

opt_unique:
	 	 {
		$$ = new OptUnique();
		$$->case_idx_ = CASE0; 
	 }
	|UNIQUE {
		$$ = new OptUnique();
		$$->case_idx_ = CASE1; 
	 }
	;

create_view_stmt:
	 CREATE opt_or_replace opt_view_algorithm VIEW view_name opt_column_list AS query_expression {
		$$ = new CreateViewStmt();
		$$->case_idx_ = CASE0;

		$$->opt_or_replace_ = $2;
		$$->opt_view_algorithm_ = $3;
		$$->view_name_ = $5;
		$$->opt_column_list_ = $6;
		$$->query_expression_ = $8; 
	 }
	;

opt_or_replace:
	 	 {
		$$ = new OptOrReplace();
		$$->case_idx_ = CASE0; 
	 }
	|OR REPLACE {
		$$ = new OptOrReplace();
		$$->case_idx_ = CASE1; 
	 }
	;

opt_view_algorithm:
	 	 {
		$$ = new OptViewAlgorithm();
		$$->case_idx_ = CASE0; 
	 }
	|ALGORITHM OP_EQUAL UNDEFINED {
		$$ = new OptViewAlgorithm();
		$$->case_idx_ = CASE1; 
	 }
	|ALGORITHM OP_EQUAL MERGE {
		$$ = new OptViewAlgorithm();
		$$->case_idx_ = CASE2; 
	 }
	|ALGORITHM OP_EQUAL TEMPTABLE {
		$$ = new OptViewAlgorithm();
		$$->case_idx_ = CASE3; 
	 }
	;

create_table_stmt:
	 CREATE opt_temporary TABLE opt_if_not_exists table_name OP_LP table_element_list OP_RP opt_duplicate_as_qe {
		$$ = new CreateTableStmt();
		$$->case_idx_ = CASE0;

		$$->opt_temporary_ = $2;
		$$->opt_if_not_exists_ = $4;
		$$->table_name_ = $5;
		$$->table_element_list_ = $7;
		$$->opt_duplicate_as_qe_ = $9; 
	 }
	|CREATE opt_temporary TABLE opt_if_not_exists table_name duplicate_as_qe {
		$$ = new CreateTableStmt();
		$$->case_idx_ = CASE1;

		$$->opt_temporary_ = $2;
		$$->opt_if_not_exists_ = $4;
		$$->table_name_ = $5;
		$$->duplicate_as_qe_ = $6; 
	 }
	;

opt_temporary:
	 	 {
		$$ = new OptTemporary();
		$$->case_idx_ = CASE0; 
	 }
	|TEMPORARY {
		$$ = new OptTemporary();
		$$->case_idx_ = CASE1; 
	 }
	;

opt_if_not_exists:
	 	 {
		$$ = new OptIfNotExists();
		$$->case_idx_ = CASE0; 
	 }
	|IF NOT EXISTS {
		$$ = new OptIfNotExists();
		$$->case_idx_ = CASE1; 
	 }
	;

opt_duplicate_as_qe:
	 	 {
		$$ = new OptDuplicateAsQe();
		$$->case_idx_ = CASE0; 
	 }
	|duplicate_as_qe {
		$$ = new OptDuplicateAsQe();
		$$->case_idx_ = CASE1;

		$$->duplicate_as_qe_ = $1; 
	 }
	;

duplicate_as_qe:
	 duplicate AS query_expression {
		$$ = new DuplicateAsQe();
		$$->case_idx_ = CASE0;

		$$->duplicate_ = $1;
		$$->query_expression_ = $3; 
	 }
	|AS query_expression {
		$$ = new DuplicateAsQe();
		$$->case_idx_ = CASE1;

		$$->query_expression_ = $2; 
	 }
	;

duplicate:
	 REPLACE {
		$$ = new Duplicate();
		$$->case_idx_ = CASE0; 
	 }
	|IGNORE {
		$$ = new Duplicate();
		$$->case_idx_ = CASE1; 
	 }
	;

table_element_list:
	 table_element {
		$$ = new TableElementList();
		$$->case_idx_ = CASE0;

		$$->table_element_ = $1; 
	 }
	|table_element OP_COMMA table_element_list {
		$$ = new TableElementList();
		$$->case_idx_ = CASE1;

		$$->table_element_ = $1;
		$$->table_element_list_ = $3; 
	 }
	;

table_element:
	 column_def {
		$$ = new TableElement();
		$$->case_idx_ = CASE0;

		$$->column_def_ = $1; 
	 }
	|table_constraint_def {
		$$ = new TableElement();
		$$->case_idx_ = CASE1;

		$$->table_constraint_def_ = $1; 
	 }
	;

column_def:
	 column_name field_def {
		$$ = new ColumnDef();
		$$->case_idx_ = CASE0;

		$$->column_name_ = $1;
		$$->field_def_ = $2; 
	 }
	;

field_def:
	 data_type opt_column_attribute_list {
		$$ = new FieldDef();
		$$->case_idx_ = CASE0;

		$$->data_type_ = $1;
		$$->opt_column_attribute_list_ = $2; 
	 }
	;

data_type:
	 number_type {
		$$ = new DataType();
		$$->case_idx_ = CASE0;

		$$->number_type_ = $1; 
	 }
	|bool_type {
		$$ = new DataType();
		$$->case_idx_ = CASE1;

		$$->bool_type_ = $1; 
	 }
	|string_type {
		$$ = new DataType();
		$$->case_idx_ = CASE2;

		$$->string_type_ = $1; 
	 }
	;

string_type:
	 CHAR opt_field_length {
		$$ = new StringType();
		$$->case_idx_ = CASE0;

		$$->opt_field_length_ = $2; 
	 }
	|BINARY opt_field_length {
		$$ = new StringType();
		$$->case_idx_ = CASE1;

		$$->opt_field_length_ = $2; 
	 }
	|VARCHAR field_length {
		$$ = new StringType();
		$$->case_idx_ = CASE2;

		$$->field_length_ = $2; 
	 }
	|VARBINARY field_length {
		$$ = new StringType();
		$$->case_idx_ = CASE3;

		$$->field_length_ = $2; 
	 }
	|LONG VARBINARY {
		$$ = new StringType();
		$$->case_idx_ = CASE4; 
	 }
	|TINYBLOB {
		$$ = new StringType();
		$$->case_idx_ = CASE5; 
	 }
	|MEDIUMBLOB {
		$$ = new StringType();
		$$->case_idx_ = CASE6; 
	 }
	|LONGBLOB {
		$$ = new StringType();
		$$->case_idx_ = CASE7; 
	 }
	|BLOB opt_field_length {
		$$ = new StringType();
		$$->case_idx_ = CASE8;

		$$->opt_field_length_ = $2; 
	 }
	|TINYTEXT {
		$$ = new StringType();
		$$->case_idx_ = CASE9; 
	 }
	|MEDIUMTEXT {
		$$ = new StringType();
		$$->case_idx_ = CASE10; 
	 }
	|LONGTEXT {
		$$ = new StringType();
		$$->case_idx_ = CASE11; 
	 }
	|TEXT {
		$$ = new StringType();
		$$->case_idx_ = CASE12; 
	 }
	;

bool_type:
	 BOOL {
		$$ = new BoolType();
		$$->case_idx_ = CASE0; 
	 }
	|BOOLEAN {
		$$ = new BoolType();
		$$->case_idx_ = CASE1; 
	 }
	;

number_type:
	 int_type opt_field_options {
		$$ = new NumberType();
		$$->case_idx_ = CASE0;

		$$->int_type_ = $1;
		$$->opt_field_options_ = $2; 
	 }
	|real_type opt_precision opt_field_options {
		$$ = new NumberType();
		$$->case_idx_ = CASE1;

		$$->real_type_ = $1;
		$$->opt_precision_ = $2;
		$$->opt_field_options_ = $3; 
	 }
	|numeric_type float_options opt_field_options {
		$$ = new NumberType();
		$$->case_idx_ = CASE2;

		$$->numeric_type_ = $1;
		$$->float_options_ = $2;
		$$->opt_field_options_ = $3; 
	 }
	;

numeric_type:
	 FLOAT {
		$$ = new NumericType();
		$$->case_idx_ = CASE0; 
	 }
	|DECIMAL {
		$$ = new NumericType();
		$$->case_idx_ = CASE1; 
	 }
	|NUMERIC {
		$$ = new NumericType();
		$$->case_idx_ = CASE2; 
	 }
	|FIXED {
		$$ = new NumericType();
		$$->case_idx_ = CASE3; 
	 }
	;

real_type:
	 REAL {
		$$ = new RealType();
		$$->case_idx_ = CASE0; 
	 }
	|DOUBLE {
		$$ = new RealType();
		$$->case_idx_ = CASE1; 
	 }
	;

opt_precision:
	 	 {
		$$ = new OptPrecision();
		$$->case_idx_ = CASE0; 
	 }
	|precision {
		$$ = new OptPrecision();
		$$->case_idx_ = CASE1;

		$$->precision_ = $1; 
	 }
	;

int_type:
	 INT {
		$$ = new IntType();
		$$->case_idx_ = CASE0; 
	 }
	|TINYINT {
		$$ = new IntType();
		$$->case_idx_ = CASE1; 
	 }
	|SMALLINT {
		$$ = new IntType();
		$$->case_idx_ = CASE2; 
	 }
	|MEDIUMINT {
		$$ = new IntType();
		$$->case_idx_ = CASE3; 
	 }
	|BIGINT {
		$$ = new IntType();
		$$->case_idx_ = CASE4; 
	 }
	;

opt_field_options:
	 	 {
		$$ = new OptFieldOptions();
		$$->case_idx_ = CASE0; 
	 }
	|field_option_list {
		$$ = new OptFieldOptions();
		$$->case_idx_ = CASE1;

		$$->field_option_list_ = $1; 
	 }
	;

field_option_list:
	 field_option {
		$$ = new FieldOptionList();
		$$->case_idx_ = CASE0;

		$$->field_option_ = $1; 
	 }
	|field_option field_option_list {
		$$ = new FieldOptionList();
		$$->case_idx_ = CASE1;

		$$->field_option_ = $1;
		$$->field_option_list_ = $2; 
	 }
	;

field_option:
	 SIGNED {
		$$ = new FieldOption();
		$$->case_idx_ = CASE0; 
	 }
	|UNSIGNED {
		$$ = new FieldOption();
		$$->case_idx_ = CASE1; 
	 }
	|ZEROFILL {
		$$ = new FieldOption();
		$$->case_idx_ = CASE2; 
	 }
	;

opt_column_attribute_list:
	 	 {
		$$ = new OptColumnAttributeList();
		$$->case_idx_ = CASE0; 
	 }
	|column_attribute_list {
		$$ = new OptColumnAttributeList();
		$$->case_idx_ = CASE1;

		$$->column_attribute_list_ = $1; 
	 }
	;

column_attribute_list:
	 column_attribute {
		$$ = new ColumnAttributeList();
		$$->case_idx_ = CASE0;

		$$->column_attribute_ = $1; 
	 }
	|column_attribute column_attribute_list {
		$$ = new ColumnAttributeList();
		$$->case_idx_ = CASE1;

		$$->column_attribute_ = $1;
		$$->column_attribute_list_ = $2; 
	 }
	;

column_attribute:
	 not_null {
		$$ = new ColumnAttribute();
		$$->case_idx_ = CASE0;

		$$->not_null_ = $1; 
	 }
	|default_attribute {
		$$ = new ColumnAttribute();
		$$->case_idx_ = CASE1;

		$$->default_attribute_ = $1; 
	 }
	|visibility {
		$$ = new ColumnAttribute();
		$$->case_idx_ = CASE2;

		$$->visibility_ = $1; 
	 }
	;

default_attribute:
	 DEFAULT OP_LP expr_root OP_RP {
		$$ = new DefaultAttribute();
		$$->case_idx_ = CASE0;

		$$->expr_root_ = $3; 
	 }
	;

not_null:
	 NOT NULL {
		$$ = new NotNull();
		$$->case_idx_ = CASE0; 
	 }
	;

visibility:
	 VISIBLE {
		$$ = new Visibility();
		$$->case_idx_ = CASE0; 
	 }
	|INVISIBLE {
		$$ = new Visibility();
		$$->case_idx_ = CASE1; 
	 }
	;

opt_constraint_name:
	 	 {
		$$ = new OptConstraintName();
		$$->case_idx_ = CASE0; 
	 }
	|CONSTRAINT constraint_name {
		$$ = new OptConstraintName();
		$$->case_idx_ = CASE1;

		$$->constraint_name_ = $2; 
	 }
	;

check_constraint:
	 CHECK OP_LP expr_root OP_RP {
		$$ = new CheckConstraint();
		$$->case_idx_ = CASE0;

		$$->expr_root_ = $3; 
	 }
	;

table_constraint_def:
	 INDEX opt_index_name OP_LP key_list_with_expression OP_RP {
		$$ = new TableConstraintDef();
		$$->case_idx_ = CASE0;

		$$->opt_index_name_ = $2;
		$$->key_list_with_expression_ = $4; 
	 }
	|opt_constraint_name constraint_key_type opt_index_name OP_LP key_list_with_expression OP_RP {
		$$ = new TableConstraintDef();
		$$->case_idx_ = CASE1;

		$$->opt_constraint_name_ = $1;
		$$->constraint_key_type_ = $2;
		$$->opt_index_name_ = $3;
		$$->key_list_with_expression_ = $5; 
	 }
	|opt_constraint_name check_constraint {
		$$ = new TableConstraintDef();
		$$->case_idx_ = CASE2;

		$$->opt_constraint_name_ = $1;
		$$->check_constraint_ = $2; 
	 }
	|opt_constraint_name FOREIGN KEY opt_index_name OP_LP key_list OP_RP references {
		$$ = new TableConstraintDef();
		$$->case_idx_ = CASE3;

		$$->opt_constraint_name_ = $1;
		$$->opt_index_name_ = $4;
		$$->key_list_ = $6;
		$$->references_ = $8; 
	 }
	;

references:
	 REFERENCES table_name OP_LP column_list OP_RP opt_on_update_delete {
		$$ = new References();
		$$->case_idx_ = CASE0;

		$$->table_name_ = $2;
		$$->column_list_ = $4;
		$$->opt_on_update_delete_ = $6; 
	 }
	;

opt_on_update_delete:
	 	 {
		$$ = new OptOnUpdateDelete();
		$$->case_idx_ = CASE0; 
	 }
	|ON UPDATE on_update_option {
		$$ = new OptOnUpdateDelete();
		$$->case_idx_ = CASE1;

		$$->on_update_option_1_ = $3; 
	 }
	|ON DELETE on_update_option {
		$$ = new OptOnUpdateDelete();
		$$->case_idx_ = CASE2;

		$$->on_update_option_1_ = $3; 
	 }
	|ON UPDATE on_update_option ON DELETE on_update_option {
		$$ = new OptOnUpdateDelete();
		$$->case_idx_ = CASE3;

		$$->on_update_option_1_ = $3;
		$$->on_update_option_2_ = $6; 
	 }
	|ON DELETE on_update_option ON UPDATE on_update_option {
		$$ = new OptOnUpdateDelete();
		$$->case_idx_ = CASE4;

		$$->on_update_option_1_ = $3;
		$$->on_update_option_2_ = $6; 
	 }
	;

on_update_option:
	 CASCADE {
		$$ = new OnUpdateOption();
		$$->case_idx_ = CASE0; 
	 }
	|SET NULL {
		$$ = new OnUpdateOption();
		$$->case_idx_ = CASE1; 
	 }
	|NO ACTION {
		$$ = new OnUpdateOption();
		$$->case_idx_ = CASE2; 
	 }
	|SET DEFAULT {
		$$ = new OnUpdateOption();
		$$->case_idx_ = CASE3; 
	 }
	;

key_list:
	 key_part {
		$$ = new KeyList();
		$$->case_idx_ = CASE0;

		$$->key_part_ = $1; 
	 }
	|key_part OP_COMMA key_list {
		$$ = new KeyList();
		$$->case_idx_ = CASE1;

		$$->key_part_ = $1;
		$$->key_list_ = $3; 
	 }
	;

key_list_with_expression:
	 key_part_with_expression {
		$$ = new KeyListWithExpression();
		$$->case_idx_ = CASE0;

		$$->key_part_with_expression_ = $1; 
	 }
	|key_part_with_expression OP_COMMA key_list_with_expression {
		$$ = new KeyListWithExpression();
		$$->case_idx_ = CASE1;

		$$->key_part_with_expression_ = $1;
		$$->key_list_with_expression_ = $3; 
	 }
	;

key_part_with_expression:
	 key_part {
		$$ = new KeyPartWithExpression();
		$$->case_idx_ = CASE0;

		$$->key_part_ = $1; 
	 }
	|OP_LP expr_root OP_RP opt_ordering_direction {
		$$ = new KeyPartWithExpression();
		$$->case_idx_ = CASE1;

		$$->expr_root_ = $2;
		$$->opt_ordering_direction_ = $4; 
	 }
	;

key_part:
	 column_name opt_ordering_direction {
		$$ = new KeyPart();
		$$->case_idx_ = CASE0;

		$$->column_name_ = $1;
		$$->opt_ordering_direction_ = $2; 
	 }
	|column_name OP_LP iconst OP_RP opt_ordering_direction {
		$$ = new KeyPart();
		$$->case_idx_ = CASE1;

		$$->column_name_ = $1;
		$$->iconst_ = $3;
		$$->opt_ordering_direction_ = $5; 
	 }
	;

constraint_key_type:
	 PRIMARY KEY {
		$$ = new ConstraintKeyType();
		$$->case_idx_ = CASE0; 
	 }
	|UNIQUE {
		$$ = new ConstraintKeyType();
		$$->case_idx_ = CASE1; 
	 }
	;

opt_index_name:
	 	 {
		$$ = new OptIndexName();
		$$->case_idx_ = CASE0; 
	 }
	|index_name {
		$$ = new OptIndexName();
		$$->case_idx_ = CASE1;

		$$->index_name_ = $1; 
	 }
	;

select_stmt:
	 query_expression {
		$$ = new SelectStmt();
		$$->case_idx_ = CASE0;

		$$->query_expression_ = $1; 
	 }
	;

query_expression:
	 query_expression_body opt_order_clause opt_limit_clause {
		$$ = new QueryExpression();
		$$->case_idx_ = CASE0;

		$$->query_expression_body_ = $1;
		$$->opt_order_clause_ = $2;
		$$->opt_limit_clause_ = $3; 
	 }
	|with_clause query_expression_body opt_order_clause opt_limit_clause {
		$$ = new QueryExpression();
		$$->case_idx_ = CASE1;

		$$->with_clause_ = $1;
		$$->query_expression_body_ = $2;
		$$->opt_order_clause_ = $3;
		$$->opt_limit_clause_ = $4; 
	 }
	;

query_expression_body:
	 query_primary {
		$$ = new QueryExpressionBody();
		$$->case_idx_ = CASE0;

		$$->query_primary_ = $1; 
	 }
	|query_expression_parens %prec SUBQUERY_AS_EXPR {
		$$ = new QueryExpressionBody();
		$$->case_idx_ = CASE1;

		$$->query_expression_parens_ = $1; 
	 }
	|query_expression_body UNION opt_union_option query_expression_body {
		$$ = new QueryExpressionBody();
		$$->case_idx_ = CASE2;

		$$->query_expression_body_1_ = $1;
		$$->opt_union_option_ = $3;
		$$->query_expression_body_2_ = $4; 
	 }
	|query_expression_body EXCEPT opt_union_option query_expression_body {
		$$ = new QueryExpressionBody();
		$$->case_idx_ = CASE3;

		$$->query_expression_body_1_ = $1;
		$$->opt_union_option_ = $3;
		$$->query_expression_body_2_ = $4; 
	 }
	|query_expression_body INTERSECT opt_union_option query_expression_body {
		$$ = new QueryExpressionBody();
		$$->case_idx_ = CASE4;

		$$->query_expression_body_1_ = $1;
		$$->opt_union_option_ = $3;
		$$->query_expression_body_2_ = $4; 
	 }
	;

query_primary:
	 query_specification {
		$$ = new QueryPrimary();
		$$->case_idx_ = CASE0;

		$$->query_specification_ = $1; 
	 }
	;

query_specification:
	 SELECT opt_select_options select_item_list opt_from_clause opt_where_clause opt_group_clause opt_having_clause opt_window_clause {
		$$ = new QuerySpecification();
		$$->case_idx_ = CASE0;

		$$->opt_select_options_ = $2;
		$$->select_item_list_ = $3;
		$$->opt_from_clause_ = $4;
		$$->opt_where_clause_ = $5;
		$$->opt_group_clause_ = $6;
		$$->opt_having_clause_ = $7;
		$$->opt_window_clause_ = $8; 
	 }
	;

opt_window_clause:
	 	 {
		$$ = new OptWindowClause();
		$$->case_idx_ = CASE0; 
	 }
	|WINDOW window_definition_list {
		$$ = new OptWindowClause();
		$$->case_idx_ = CASE1;

		$$->window_definition_list_ = $2; 
	 }
	;

window_definition_list:
	 window_definition {
		$$ = new WindowDefinitionList();
		$$->case_idx_ = CASE0;

		$$->window_definition_ = $1; 
	 }
	|window_definition OP_COMMA window_definition_list {
		$$ = new WindowDefinitionList();
		$$->case_idx_ = CASE1;

		$$->window_definition_ = $1;
		$$->window_definition_list_ = $3; 
	 }
	;

window_definition:
	 window_name AS window_spec {
		$$ = new WindowDefinition();
		$$->case_idx_ = CASE0;

		$$->window_name_ = $1;
		$$->window_spec_ = $3; 
	 }
	;

window_spec:
	 OP_LP window_spec_details OP_RP {
		$$ = new WindowSpec();
		$$->case_idx_ = CASE0;

		$$->window_spec_details_ = $2; 
	 }
	;

window_spec_details:
	 opt_existing_window_name opt_partition_clause opt_window_order_by_clause opt_window_frame_clause {
		$$ = new WindowSpecDetails();
		$$->case_idx_ = CASE0;

		$$->opt_existing_window_name_ = $1;
		$$->opt_partition_clause_ = $2;
		$$->opt_window_order_by_clause_ = $3;
		$$->opt_window_frame_clause_ = $4; 
	 }
	;

opt_existing_window_name:
	 	 {
		$$ = new OptExistingWindowName();
		$$->case_idx_ = CASE0; 
	 }
	|window_name {
		$$ = new OptExistingWindowName();
		$$->case_idx_ = CASE1;

		$$->window_name_ = $1; 
	 }
	;

opt_partition_clause:
	 	 {
		$$ = new OptPartitionClause();
		$$->case_idx_ = CASE0; 
	 }
	|PARTITION BY expr_root_list {
		$$ = new OptPartitionClause();
		$$->case_idx_ = CASE1;

		$$->expr_root_list_ = $3; 
	 }
	;

opt_window_order_by_clause:
	 	 {
		$$ = new OptWindowOrderByClause();
		$$->case_idx_ = CASE0; 
	 }
	|ORDER BY order_list {
		$$ = new OptWindowOrderByClause();
		$$->case_idx_ = CASE1;

		$$->order_list_ = $3; 
	 }
	;

order_list:
	 order_expr {
		$$ = new OrderList();
		$$->case_idx_ = CASE0;

		$$->order_expr_ = $1; 
	 }
	|order_expr OP_COMMA order_list {
		$$ = new OrderList();
		$$->case_idx_ = CASE1;

		$$->order_expr_ = $1;
		$$->order_list_ = $3; 
	 }
	;

order_expr:
	 expr_root opt_ordering_direction {
		$$ = new OrderExpr();
		$$->case_idx_ = CASE0;

		$$->expr_root_ = $1;
		$$->opt_ordering_direction_ = $2; 
	 }
	;

opt_ordering_direction:
	 	 {
		$$ = new OptOrderingDirection();
		$$->case_idx_ = CASE0; 
	 }
	|ASC {
		$$ = new OptOrderingDirection();
		$$->case_idx_ = CASE1; 
	 }
	|DESC {
		$$ = new OptOrderingDirection();
		$$->case_idx_ = CASE2; 
	 }
	;

opt_window_frame_clause:
	 	 {
		$$ = new OptWindowFrameClause();
		$$->case_idx_ = CASE0; 
	 }
	|window_frame_units window_frame_extent {
		$$ = new OptWindowFrameClause();
		$$->case_idx_ = CASE1;

		$$->window_frame_units_ = $1;
		$$->window_frame_extent_ = $2; 
	 }
	;

window_frame_units:
	 ROWS {
		$$ = new WindowFrameUnits();
		$$->case_idx_ = CASE0; 
	 }
	|RANGE {
		$$ = new WindowFrameUnits();
		$$->case_idx_ = CASE1; 
	 }
	;

window_frame_extent:
	 window_frame_start {
		$$ = new WindowFrameExtent();
		$$->case_idx_ = CASE0;

		$$->window_frame_start_ = $1; 
	 }
	|window_frame_between {
		$$ = new WindowFrameExtent();
		$$->case_idx_ = CASE1;

		$$->window_frame_between_ = $1; 
	 }
	;

window_frame_start:
	 UNBOUNDED PRECEDING {
		$$ = new WindowFrameStart();
		$$->case_idx_ = CASE0; 
	 }
	|iconst PRECEDING {
		$$ = new WindowFrameStart();
		$$->case_idx_ = CASE1;

		$$->iconst_ = $1; 
	 }
	|CURRENT ROW {
		$$ = new WindowFrameStart();
		$$->case_idx_ = CASE2; 
	 }
	;

window_frame_between:
	 BETWEEN window_frame_start AND window_frame_bound {
		$$ = new WindowFrameBetween();
		$$->case_idx_ = CASE0;

		$$->window_frame_start_ = $2;
		$$->window_frame_bound_ = $4; 
	 }
	;

window_frame_bound:
	 window_frame_start {
		$$ = new WindowFrameBound();
		$$->case_idx_ = CASE0;

		$$->window_frame_start_ = $1; 
	 }
	|UNBOUNDED FOLLOWING {
		$$ = new WindowFrameBound();
		$$->case_idx_ = CASE1; 
	 }
	|iconst FOLLOWING {
		$$ = new WindowFrameBound();
		$$->case_idx_ = CASE2;

		$$->iconst_ = $1; 
	 }
	;

opt_having_clause:
	 	 {
		$$ = new OptHavingClause();
		$$->case_idx_ = CASE0; 
	 }
	|HAVING expr_root {
		$$ = new OptHavingClause();
		$$->case_idx_ = CASE1;

		$$->expr_root_ = $2; 
	 }
	;

opt_group_clause:
	 	 {
		$$ = new OptGroupClause();
		$$->case_idx_ = CASE0; 
	 }
	|GROUP BY expr_root_list olap_opt {
		$$ = new OptGroupClause();
		$$->case_idx_ = CASE1;

		$$->expr_root_list_ = $3;
		$$->olap_opt_ = $4; 
	 }
	;

olap_opt:
	 	 {
		$$ = new OlapOpt();
		$$->case_idx_ = CASE0; 
	 }
	|WITH ROLLUP {
		$$ = new OlapOpt();
		$$->case_idx_ = CASE1; 
	 }
	;

opt_where_clause:
	 	 {
		$$ = new OptWhereClause();
		$$->case_idx_ = CASE0; 
	 }
	|where_clause {
		$$ = new OptWhereClause();
		$$->case_idx_ = CASE1;

		$$->where_clause_ = $1; 
	 }
	;

where_clause:
	 WHERE expr_root {
		$$ = new WhereClause();
		$$->case_idx_ = CASE0;

		$$->expr_root_ = $2; 
	 }
	;

opt_from_clause:
	 	 {
		$$ = new OptFromClause();
		$$->case_idx_ = CASE0; 
	 }
	|from_clause {
		$$ = new OptFromClause();
		$$->case_idx_ = CASE1;

		$$->from_clause_ = $1; 
	 }
	;

from_clause:
	 FROM table_reference_list {
		$$ = new FromClause();
		$$->case_idx_ = CASE0;

		$$->table_reference_list_ = $2; 
	 }
	;

table_reference_list:
	 table_reference {
		$$ = new TableReferenceList();
		$$->case_idx_ = CASE0;

		$$->table_reference_ = $1; 
	 }
	|table_reference OP_COMMA table_reference_list {
		$$ = new TableReferenceList();
		$$->case_idx_ = CASE1;

		$$->table_reference_ = $1;
		$$->table_reference_list_ = $3; 
	 }
	;

table_reference:
	 table_factor {
		$$ = new TableReference();
		$$->case_idx_ = CASE0;

		$$->table_factor_ = $1; 
	 }
	|joined_table {
		$$ = new TableReference();
		$$->case_idx_ = CASE1;

		$$->joined_table_ = $1; 
	 }
	;

table_factor:
	 single_table {
		$$ = new TableFactor();
		$$->case_idx_ = CASE0;

		$$->single_table_ = $1; 
	 }
	|derived_table {
		$$ = new TableFactor();
		$$->case_idx_ = CASE1;

		$$->derived_table_ = $1; 
	 }
	|joined_table_parens {
		$$ = new TableFactor();
		$$->case_idx_ = CASE2;

		$$->joined_table_parens_ = $1; 
	 }
	|table_reference_list_parens {
		$$ = new TableFactor();
		$$->case_idx_ = CASE3;

		$$->table_reference_list_parens_ = $1; 
	 }
	;

table_reference_list_parens:
	 OP_LP table_reference OP_COMMA table_reference_list OP_RP {
		$$ = new TableReferenceListParens();
		$$->case_idx_ = CASE0;

		$$->table_reference_ = $2;
		$$->table_reference_list_ = $4; 
	 }
	;

joined_table_parens:
	 OP_LP joined_table OP_RP {
		$$ = new JoinedTableParens();
		$$->case_idx_ = CASE0;

		$$->joined_table_ = $2; 
	 }
	;

derived_table:
	 subquery AS alias opt_column_list {
		$$ = new DerivedTable();
		$$->case_idx_ = CASE0;

		$$->subquery_ = $1;
		$$->alias_ = $3;
		$$->opt_column_list_ = $4; 
	 }
	|LATERAL subquery AS alias opt_column_list {
		$$ = new DerivedTable();
		$$->case_idx_ = CASE1;

		$$->subquery_ = $2;
		$$->alias_ = $4;
		$$->opt_column_list_ = $5; 
	 }
	;

opt_column_list:
	 	 {
		$$ = new OptColumnList();
		$$->case_idx_ = CASE0; 
	 }
	|OP_LP column_list OP_RP {
		$$ = new OptColumnList();
		$$->case_idx_ = CASE1;

		$$->column_list_ = $2; 
	 }
	;

column_list:
	 column_name {
		$$ = new ColumnList();
		$$->case_idx_ = CASE0;

		$$->column_name_ = $1; 
	 }
	|column_name OP_COMMA column_list {
		$$ = new ColumnList();
		$$->case_idx_ = CASE1;

		$$->column_name_ = $1;
		$$->column_list_ = $3; 
	 }
	;

subquery:
	 query_expression_parens %prec SUBQUERY_AS_EXPR {
		$$ = new Subquery();
		$$->case_idx_ = CASE0;

		$$->query_expression_parens_ = $1; 
	 }
	;

single_table:
	 table_name opt_alias {
		$$ = new SingleTable();
		$$->case_idx_ = CASE0;

		$$->table_name_ = $1;
		$$->opt_alias_ = $2; 
	 }
	;

opt_alias:
	 	 {
		$$ = new OptAlias();
		$$->case_idx_ = CASE0; 
	 }
	|AS alias {
		$$ = new OptAlias();
		$$->case_idx_ = CASE1;

		$$->alias_ = $2; 
	 }
	;

joined_table:
	 table_reference inner_join_type table_reference ON expr_root {
		$$ = new JoinedTable();
		$$->case_idx_ = CASE0;

		$$->table_reference_1_ = $1;
		$$->inner_join_type_ = $2;
		$$->table_reference_2_ = $3;
		$$->expr_root_ = $5; 
	 }
	|table_reference inner_join_type table_reference USING OP_LP column_list OP_RP {
		$$ = new JoinedTable();
		$$->case_idx_ = CASE1;

		$$->table_reference_1_ = $1;
		$$->inner_join_type_ = $2;
		$$->table_reference_2_ = $3;
		$$->column_list_ = $6; 
	 }
	|table_reference outer_join_type table_reference ON expr_root {
		$$ = new JoinedTable();
		$$->case_idx_ = CASE2;

		$$->table_reference_1_ = $1;
		$$->outer_join_type_ = $2;
		$$->table_reference_2_ = $3;
		$$->expr_root_ = $5; 
	 }
	|table_reference outer_join_type table_reference USING OP_LP column_list OP_RP {
		$$ = new JoinedTable();
		$$->case_idx_ = CASE3;

		$$->table_reference_1_ = $1;
		$$->outer_join_type_ = $2;
		$$->table_reference_2_ = $3;
		$$->column_list_ = $6; 
	 }
	|table_reference inner_join_type table_reference %prec CONDITIONLESS_JOIN {
		$$ = new JoinedTable();
		$$->case_idx_ = CASE4;

		$$->table_reference_1_ = $1;
		$$->inner_join_type_ = $2;
		$$->table_reference_2_ = $3; 
	 }
	|table_reference natural_join_type table_factor {
		$$ = new JoinedTable();
		$$->case_idx_ = CASE5;

		$$->table_reference_1_ = $1;
		$$->natural_join_type_ = $2;
		$$->table_factor_ = $3; 
	 }
	;

inner_join_type:
	 JOIN {
		$$ = new InnerJoinType();
		$$->case_idx_ = CASE0; 
	 }
	|INNER JOIN {
		$$ = new InnerJoinType();
		$$->case_idx_ = CASE1; 
	 }
	|CROSS JOIN {
		$$ = new InnerJoinType();
		$$->case_idx_ = CASE2; 
	 }
	|STRAIGHT_JOIN {
		$$ = new InnerJoinType();
		$$->case_idx_ = CASE3; 
	 }
	;

natural_join_type:
	 NATURAL opt_inner JOIN {
		$$ = new NaturalJoinType();
		$$->case_idx_ = CASE0;

		$$->opt_inner_ = $2; 
	 }
	|NATURAL RIGHT opt_outer JOIN {
		$$ = new NaturalJoinType();
		$$->case_idx_ = CASE1;

		$$->opt_outer_ = $3; 
	 }
	|NATURAL LEFT opt_outer JOIN {
		$$ = new NaturalJoinType();
		$$->case_idx_ = CASE2;

		$$->opt_outer_ = $3; 
	 }
	;

opt_inner:
	 	 {
		$$ = new OptInner();
		$$->case_idx_ = CASE0; 
	 }
	|INNER {
		$$ = new OptInner();
		$$->case_idx_ = CASE1; 
	 }
	;

opt_outer:
	 	 {
		$$ = new OptOuter();
		$$->case_idx_ = CASE0; 
	 }
	|OUTER {
		$$ = new OptOuter();
		$$->case_idx_ = CASE1; 
	 }
	;

outer_join_type:
	 LEFT opt_outer JOIN {
		$$ = new OuterJoinType();
		$$->case_idx_ = CASE0;

		$$->opt_outer_ = $2; 
	 }
	|RIGHT opt_outer JOIN {
		$$ = new OuterJoinType();
		$$->case_idx_ = CASE1;

		$$->opt_outer_ = $2; 
	 }
	;

select_item_list:
	 select_item {
		$$ = new SelectItemList();
		$$->case_idx_ = CASE0;

		$$->select_item_ = $1; 
	 }
	|select_item OP_COMMA select_item_list {
		$$ = new SelectItemList();
		$$->case_idx_ = CASE1;

		$$->select_item_ = $1;
		$$->select_item_list_ = $3; 
	 }
	;

select_item:
	 table_wild {
		$$ = new SelectItem();
		$$->case_idx_ = CASE0;

		$$->table_wild_ = $1; 
	 }
	|expr_root opt_alias {
		$$ = new SelectItem();
		$$->case_idx_ = CASE1;

		$$->expr_root_ = $1;
		$$->opt_alias_ = $2; 
	 }
	;

table_wild:
	 OP_MUL {
		$$ = new TableWild();
		$$->case_idx_ = CASE0; 
	 }
	|table_name OP_DOT OP_MUL {
		$$ = new TableWild();
		$$->case_idx_ = CASE1;

		$$->table_name_ = $1; 
	 }
	;

opt_select_options:
	 	 {
		$$ = new OptSelectOptions();
		$$->case_idx_ = CASE0; 
	 }
	|select_option_list {
		$$ = new OptSelectOptions();
		$$->case_idx_ = CASE1;

		$$->select_option_list_ = $1; 
	 }
	;

select_option_list:
	 select_option {
		$$ = new SelectOptionList();
		$$->case_idx_ = CASE0;

		$$->select_option_ = $1; 
	 }
	|select_option select_option_list {
		$$ = new SelectOptionList();
		$$->case_idx_ = CASE1;

		$$->select_option_ = $1;
		$$->select_option_list_ = $2; 
	 }
	;

select_option:
	 SQL_NO_CACHE {
		$$ = new SelectOption();
		$$->case_idx_ = CASE0; 
	 }
	|STRAIGHT_JOIN {
		$$ = new SelectOption();
		$$->case_idx_ = CASE1; 
	 }
	|HIGH_PRIORITY {
		$$ = new SelectOption();
		$$->case_idx_ = CASE2; 
	 }
	|DISTINCT {
		$$ = new SelectOption();
		$$->case_idx_ = CASE3; 
	 }
	|SQL_SMALL_RESULT {
		$$ = new SelectOption();
		$$->case_idx_ = CASE4; 
	 }
	|SQL_BIG_RESULT {
		$$ = new SelectOption();
		$$->case_idx_ = CASE5; 
	 }
	|SQL_BUFFER_RESULT {
		$$ = new SelectOption();
		$$->case_idx_ = CASE6; 
	 }
	|SQL_CALC_FOUND_ROWS {
		$$ = new SelectOption();
		$$->case_idx_ = CASE7; 
	 }
	|ALL {
		$$ = new SelectOption();
		$$->case_idx_ = CASE8; 
	 }
	;

query_expression_parens:
	 OP_LP query_expression_parens OP_RP {
		$$ = new QueryExpressionParens();
		$$->case_idx_ = CASE0;

		$$->query_expression_parens_ = $2; 
	 }
	|OP_LP query_expression OP_RP {
		$$ = new QueryExpressionParens();
		$$->case_idx_ = CASE1;

		$$->query_expression_ = $2; 
	 }
	;

opt_union_option:
	 	 {
		$$ = new OptUnionOption();
		$$->case_idx_ = CASE0; 
	 }
	|DISTINCT {
		$$ = new OptUnionOption();
		$$->case_idx_ = CASE1; 
	 }
	|ALL {
		$$ = new OptUnionOption();
		$$->case_idx_ = CASE2; 
	 }
	;

opt_order_clause:
	 	 {
		$$ = new OptOrderClause();
		$$->case_idx_ = CASE0; 
	 }
	|order_clause {
		$$ = new OptOrderClause();
		$$->case_idx_ = CASE1;

		$$->order_clause_ = $1; 
	 }
	;

order_clause:
	 ORDER BY order_list {
		$$ = new OrderClause();
		$$->case_idx_ = CASE0;

		$$->order_list_ = $3; 
	 }
	;

opt_limit_clause:
	 	 {
		$$ = new OptLimitClause();
		$$->case_idx_ = CASE0; 
	 }
	|limit_clause {
		$$ = new OptLimitClause();
		$$->case_idx_ = CASE1;

		$$->limit_clause_ = $1; 
	 }
	;

limit_clause:
	 LIMIT limit_options {
		$$ = new LimitClause();
		$$->case_idx_ = CASE0;

		$$->limit_options_ = $2; 
	 }
	;

limit_options:
	 iconst {
		$$ = new LimitOptions();
		$$->case_idx_ = CASE0;

		$$->iconst_1_ = $1; 
	 }
	|iconst OP_COMMA iconst {
		$$ = new LimitOptions();
		$$->case_idx_ = CASE1;

		$$->iconst_1_ = $1;
		$$->iconst_2_ = $3; 
	 }
	;

with_clause:
	 WITH with_list {
		$$ = new WithClause();
		$$->case_idx_ = CASE0;

		$$->with_list_ = $2; 
	 }
	|WITH RECURSIVE with_list {
		$$ = new WithClause();
		$$->case_idx_ = CASE1;

		$$->with_list_ = $3; 
	 }
	;

with_list:
	 common_table_expr {
		$$ = new WithList();
		$$->case_idx_ = CASE0;

		$$->common_table_expr_ = $1; 
	 }
	|common_table_expr OP_COMMA with_list {
		$$ = new WithList();
		$$->case_idx_ = CASE1;

		$$->common_table_expr_ = $1;
		$$->with_list_ = $3; 
	 }
	;

common_table_expr:
	 table_name opt_column_list AS subquery {
		$$ = new CommonTableExpr();
		$$->case_idx_ = CASE0;

		$$->table_name_ = $1;
		$$->opt_column_list_ = $2;
		$$->subquery_ = $4; 
	 }
	;

expr_root_list:
	 expr_root {
		$$ = new ExprRootList();
		$$->case_idx_ = CASE0;

		$$->expr_root_ = $1; 
	 }
	|expr_root OP_COMMA expr_root_list {
		$$ = new ExprRootList();
		$$->case_idx_ = CASE1;

		$$->expr_root_ = $1;
		$$->expr_root_list_ = $3; 
	 }
	;

expr_root:
	 expr {
		$$ = new ExprRoot();
		$$->case_idx_ = CASE0;

		$$->expr_ = $1; 
	 }
	;

expr:
	 expr OR expr {
		$$ = new Expr();
		$$->case_idx_ = CASE0;

		$$->expr_1_ = $1;
		$$->expr_2_ = $3; 
	 }
	|expr XOR expr {
		$$ = new Expr();
		$$->case_idx_ = CASE1;

		$$->expr_1_ = $1;
		$$->expr_2_ = $3; 
	 }
	|expr AND expr {
		$$ = new Expr();
		$$->case_idx_ = CASE2;

		$$->expr_1_ = $1;
		$$->expr_2_ = $3; 
	 }
	|NOT expr {
		$$ = new Expr();
		$$->case_idx_ = CASE3;

		$$->expr_1_ = $2; 
	 }
	|bool_pri IS TRUE {
		$$ = new Expr();
		$$->case_idx_ = CASE4;

		$$->bool_pri_ = $1; 
	 }
	|bool_pri IS NOT TRUE {
		$$ = new Expr();
		$$->case_idx_ = CASE5;

		$$->bool_pri_ = $1; 
	 }
	|bool_pri IS FALSE {
		$$ = new Expr();
		$$->case_idx_ = CASE6;

		$$->bool_pri_ = $1; 
	 }
	|bool_pri IS NOT FALSE {
		$$ = new Expr();
		$$->case_idx_ = CASE7;

		$$->bool_pri_ = $1; 
	 }
	|bool_pri IS UNKNOWN {
		$$ = new Expr();
		$$->case_idx_ = CASE8;

		$$->bool_pri_ = $1; 
	 }
	|bool_pri IS NOT UNKNOWN {
		$$ = new Expr();
		$$->case_idx_ = CASE9;

		$$->bool_pri_ = $1; 
	 }
	|bool_pri {
		$$ = new Expr();
		$$->case_idx_ = CASE10;

		$$->bool_pri_ = $1; 
	 }
	;

bool_pri:
	 bool_pri IS NULL {
		$$ = new BoolPri();
		$$->case_idx_ = CASE0;

		$$->bool_pri_ = $1; 
	 }
	|bool_pri IS NOT NULL {
		$$ = new BoolPri();
		$$->case_idx_ = CASE1;

		$$->bool_pri_ = $1; 
	 }
	|bool_pri comp_op predicate {
		$$ = new BoolPri();
		$$->case_idx_ = CASE2;

		$$->bool_pri_ = $1;
		$$->comp_op_ = $2;
		$$->predicate_ = $3; 
	 }
	|bool_pri comp_op all_subquery {
		$$ = new BoolPri();
		$$->case_idx_ = CASE3;

		$$->bool_pri_ = $1;
		$$->comp_op_ = $2;
		$$->all_subquery_ = $3; 
	 }
	|predicate {
		$$ = new BoolPri();
		$$->case_idx_ = CASE4;

		$$->predicate_ = $1; 
	 }
	;

predicate:
	 bit_expr in_subquery {
		$$ = new Predicate();
		$$->case_idx_ = CASE0;

		$$->bit_expr_1_ = $1;
		$$->in_subquery_ = $2; 
	 }
	|bit_expr IN OP_LP expr OP_RP {
		$$ = new Predicate();
		$$->case_idx_ = CASE1;

		$$->bit_expr_1_ = $1;
		$$->expr_ = $4; 
	 }
	|bit_expr IN OP_LP expr OP_COMMA expr_list OP_RP {
		$$ = new Predicate();
		$$->case_idx_ = CASE2;

		$$->bit_expr_1_ = $1;
		$$->expr_ = $4;
		$$->expr_list_ = $6; 
	 }
	|bit_expr NOT IN OP_LP expr OP_RP {
		$$ = new Predicate();
		$$->case_idx_ = CASE3;

		$$->bit_expr_1_ = $1;
		$$->expr_ = $5; 
	 }
	|bit_expr NOT IN OP_LP expr OP_COMMA expr_list OP_RP {
		$$ = new Predicate();
		$$->case_idx_ = CASE4;

		$$->bit_expr_1_ = $1;
		$$->expr_ = $5;
		$$->expr_list_ = $7; 
	 }
	|bit_expr BETWEEN bit_expr AND predicate {
		$$ = new Predicate();
		$$->case_idx_ = CASE5;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $3;
		$$->predicate_ = $5; 
	 }
	|bit_expr NOT BETWEEN bit_expr AND predicate {
		$$ = new Predicate();
		$$->case_idx_ = CASE6;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $4;
		$$->predicate_ = $6; 
	 }
	|bit_expr SOUNDS LIKE bit_expr {
		$$ = new Predicate();
		$$->case_idx_ = CASE7;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $4; 
	 }
	|bit_expr LIKE simple_expr {
		$$ = new Predicate();
		$$->case_idx_ = CASE8;

		$$->bit_expr_1_ = $1;
		$$->simple_expr_ = $3; 
	 }
	|bit_expr NOT LIKE simple_expr {
		$$ = new Predicate();
		$$->case_idx_ = CASE9;

		$$->bit_expr_1_ = $1;
		$$->simple_expr_ = $4; 
	 }
	|bit_expr REGEXP bit_expr {
		$$ = new Predicate();
		$$->case_idx_ = CASE10;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $3; 
	 }
	|bit_expr NOT REGEXP bit_expr {
		$$ = new Predicate();
		$$->case_idx_ = CASE11;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $4; 
	 }
	|bit_expr {
		$$ = new Predicate();
		$$->case_idx_ = CASE12;

		$$->bit_expr_1_ = $1; 
	 }
	;

bit_expr:
	 bit_expr OP_OR bit_expr {
		$$ = new BitExpr();
		$$->case_idx_ = CASE0;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $3; 
	 }
	|bit_expr OP_AND bit_expr {
		$$ = new BitExpr();
		$$->case_idx_ = CASE1;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $3; 
	 }
	|bit_expr OP_SHL bit_expr {
		$$ = new BitExpr();
		$$->case_idx_ = CASE2;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $3; 
	 }
	|bit_expr OP_SHR bit_expr {
		$$ = new BitExpr();
		$$->case_idx_ = CASE3;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $3; 
	 }
	|bit_expr OP_ADD bit_expr {
		$$ = new BitExpr();
		$$->case_idx_ = CASE4;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $3; 
	 }
	|bit_expr OP_SUB bit_expr {
		$$ = new BitExpr();
		$$->case_idx_ = CASE5;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $3; 
	 }
	|bit_expr OP_MUL bit_expr {
		$$ = new BitExpr();
		$$->case_idx_ = CASE6;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $3; 
	 }
	|bit_expr OP_DIVIDE bit_expr {
		$$ = new BitExpr();
		$$->case_idx_ = CASE7;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $3; 
	 }
	|bit_expr OP_MOD bit_expr {
		$$ = new BitExpr();
		$$->case_idx_ = CASE8;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $3; 
	 }
	|bit_expr OP_XOR bit_expr {
		$$ = new BitExpr();
		$$->case_idx_ = CASE9;

		$$->bit_expr_1_ = $1;
		$$->bit_expr_2_ = $3; 
	 }
	|simple_expr {
		$$ = new BitExpr();
		$$->case_idx_ = CASE10;

		$$->simple_expr_ = $1; 
	 }
	;

simple_expr:
	 columnref {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE0;

		$$->columnref_ = $1; 
	 }
	|literal {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE1;

		$$->literal_ = $1; 
	 }
	|iconst {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE2;

		$$->iconst_ = $1; 
	 }
	|fconst {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE3;

		$$->fconst_ = $1; 
	 }
	|sconst {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE4;

		$$->sconst_ = $1; 
	 }
	|OP_ADD simple_expr %prec UMINUS {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE5;

		$$->simple_expr_ = $2; 
	 }
	|OP_SUB simple_expr %prec UMINUS {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE6;

		$$->simple_expr_ = $2; 
	 }
	|OP_NOT simple_expr %prec UMINUS {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE7;

		$$->simple_expr_ = $2; 
	 }
	|subquery {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE8;

		$$->subquery_ = $1; 
	 }
	|EXISTS subquery {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE9;

		$$->subquery_ = $2; 
	 }
	|type_cast {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE10;

		$$->type_cast_ = $1; 
	 }
	|case_expr {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE11;

		$$->case_expr_ = $1; 
	 }
	|DEFAULT OP_LP columnref OP_RP {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE12;

		$$->columnref_ = $3; 
	 }
	|function {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE13;

		$$->function_ = $1; 
	 }
	|OP_LP expr OP_RP {
		$$ = new SimpleExpr();
		$$->case_idx_ = CASE14;

		$$->expr_ = $2; 
	 }
	;

function:
	 control_function {
		$$ = new Function();
		$$->case_idx_ = CASE0;

		$$->control_function_ = $1; 
	 }
	|math_function {
		$$ = new Function();
		$$->case_idx_ = CASE1;

		$$->math_function_ = $1; 
	 }
	|string_function {
		$$ = new Function();
		$$->case_idx_ = CASE2;

		$$->string_function_ = $1; 
	 }
	|aggregate_function {
		$$ = new Function();
		$$->case_idx_ = CASE3;

		$$->aggregate_function_ = $1; 
	 }
	|window_function {
		$$ = new Function();
		$$->case_idx_ = CASE4;

		$$->window_function_ = $1; 
	 }
	;

string_function:
	 ASCII OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE0;

		$$->expr_1_ = $3; 
	 }
	|BIN OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE1;

		$$->expr_1_ = $3; 
	 }
	|BIT_LENGTH OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE2;

		$$->expr_1_ = $3; 
	 }
	|CHAR_LENGTH OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE3;

		$$->expr_1_ = $3; 
	 }
	|LENGTH OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE4;

		$$->expr_1_ = $3; 
	 }
	|LOWER OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE5;

		$$->expr_1_ = $3; 
	 }
	|LTRIM OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE6;

		$$->expr_1_ = $3; 
	 }
	|OCT OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE7;

		$$->expr_1_ = $3; 
	 }
	|ORD OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE8;

		$$->expr_1_ = $3; 
	 }
	|QUOTE OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE9;

		$$->expr_1_ = $3; 
	 }
	|REVERSE OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE10;

		$$->expr_1_ = $3; 
	 }
	|RTRIM OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE11;

		$$->expr_1_ = $3; 
	 }
	|SPACE OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE12;

		$$->expr_1_ = $3; 
	 }
	|TRIM OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE13;

		$$->expr_1_ = $3; 
	 }
	|TRIM OP_LP LEADING FROM expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE14;

		$$->expr_1_ = $5; 
	 }
	|TRIM OP_LP TRAILING FROM expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE15;

		$$->expr_1_ = $5; 
	 }
	|UNHEX OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE16;

		$$->expr_1_ = $3; 
	 }
	|UPPER OP_LP expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE17;

		$$->expr_1_ = $3; 
	 }
	|FIND_IN_SET OP_LP expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE18;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|INSTR OP_LP expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE19;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|LEFT OP_LP expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE20;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|LOCATE OP_LP expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE21;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|REPEAT OP_LP expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE22;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|RIGHT OP_LP expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE23;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|SUBSTRING OP_LP expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE24;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|TRIM OP_LP LEADING expr FROM expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE25;

		$$->expr_1_ = $4;
		$$->expr_2_ = $6; 
	 }
	|TRIM OP_LP TRAILING expr FROM expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE26;

		$$->expr_1_ = $4;
		$$->expr_2_ = $6; 
	 }
	|TRIM OP_LP expr FROM expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE27;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|EXPORT_SET OP_LP expr OP_COMMA expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE28;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5;
		$$->expr_3_ = $7; 
	 }
	|LOCATE OP_LP expr OP_COMMA expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE29;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5;
		$$->expr_3_ = $7; 
	 }
	|LPAD OP_LP expr OP_COMMA expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE30;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5;
		$$->expr_3_ = $7; 
	 }
	|REPLACE OP_LP expr OP_COMMA expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE31;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5;
		$$->expr_3_ = $7; 
	 }
	|RPAD OP_LP expr OP_COMMA expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE32;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5;
		$$->expr_3_ = $7; 
	 }
	|SUBSTRING OP_LP expr OP_COMMA expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE33;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5;
		$$->expr_3_ = $7; 
	 }
	|SUBSTRING_INDEX OP_LP expr OP_COMMA expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE34;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5;
		$$->expr_3_ = $7; 
	 }
	|EXPORT_SET OP_LP expr OP_COMMA expr OP_COMMA expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE35;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5;
		$$->expr_3_ = $7;
		$$->expr_4_ = $9; 
	 }
	|INSERT OP_LP expr OP_COMMA expr OP_COMMA expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE36;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5;
		$$->expr_3_ = $7;
		$$->expr_4_ = $9; 
	 }
	|EXPORT_SET OP_LP expr OP_COMMA expr OP_COMMA expr OP_COMMA expr OP_COMMA expr OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE37;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5;
		$$->expr_3_ = $7;
		$$->expr_4_ = $9;
		$$->expr_5_ = $11; 
	 }
	|CHAR OP_LP expr_list OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE38;

		$$->expr_list_ = $3; 
	 }
	|CONCAT OP_LP expr_list OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE39;

		$$->expr_list_ = $3; 
	 }
	|CONCAT_WS OP_LP expr OP_COMMA expr_list OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE40;

		$$->expr_1_ = $3;
		$$->expr_list_ = $5; 
	 }
	|ELT OP_LP expr OP_COMMA expr_list OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE41;

		$$->expr_1_ = $3;
		$$->expr_list_ = $5; 
	 }
	|FIELD OP_LP expr OP_COMMA expr_list OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE42;

		$$->expr_1_ = $3;
		$$->expr_list_ = $5; 
	 }
	|MAKE_SET OP_LP expr OP_COMMA expr_list OP_RP {
		$$ = new StringFunction();
		$$->case_idx_ = CASE43;

		$$->expr_1_ = $3;
		$$->expr_list_ = $5; 
	 }
	;

math_function:
	 PI OP_LP OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE0; 
	 }
	|RAND OP_LP OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE1; 
	 }
	|ABS OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE2;

		$$->expr_1_ = $3; 
	 }
	|ACOS OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE3;

		$$->expr_1_ = $3; 
	 }
	|ASIN OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE4;

		$$->expr_1_ = $3; 
	 }
	|ATAN OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE5;

		$$->expr_1_ = $3; 
	 }
	|CEILING OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE6;

		$$->expr_1_ = $3; 
	 }
	|COS OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE7;

		$$->expr_1_ = $3; 
	 }
	|COT OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE8;

		$$->expr_1_ = $3; 
	 }
	|DEGREES OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE9;

		$$->expr_1_ = $3; 
	 }
	|EXP OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE10;

		$$->expr_1_ = $3; 
	 }
	|FLOOR OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE11;

		$$->expr_1_ = $3; 
	 }
	|HEX OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE12;

		$$->expr_1_ = $3; 
	 }
	|LN OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE13;

		$$->expr_1_ = $3; 
	 }
	|RADIANS OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE14;

		$$->expr_1_ = $3; 
	 }
	|RAND OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE15;

		$$->expr_1_ = $3; 
	 }
	|ROUND OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE16;

		$$->expr_1_ = $3; 
	 }
	|SIGN OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE17;

		$$->expr_1_ = $3; 
	 }
	|SIN OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE18;

		$$->expr_1_ = $3; 
	 }
	|SQRT OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE19;

		$$->expr_1_ = $3; 
	 }
	|TAN OP_LP expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE20;

		$$->expr_1_ = $3; 
	 }
	|ATAN OP_LP expr OP_COMMA expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE21;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|FORMAT OP_LP expr OP_COMMA expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE22;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|LOG OP_LP expr OP_COMMA expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE23;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|MOD OP_LP expr OP_COMMA expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE24;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|POW OP_LP expr OP_COMMA expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE25;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|ROUND OP_LP expr OP_COMMA expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE26;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|TRUNCATE OP_LP expr OP_COMMA expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE27;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|CONV OP_LP expr OP_COMMA expr OP_COMMA expr OP_RP {
		$$ = new MathFunction();
		$$->case_idx_ = CASE28;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5;
		$$->expr_3_ = $7; 
	 }
	;

window_function:
	 ROW_NUMBER OP_LP OP_RP windowing_clause {
		$$ = new WindowFunction();
		$$->case_idx_ = CASE0;

		$$->windowing_clause_ = $4; 
	 }
	|RANK OP_LP OP_RP windowing_clause {
		$$ = new WindowFunction();
		$$->case_idx_ = CASE1;

		$$->windowing_clause_ = $4; 
	 }
	|DENSE_RANK OP_LP OP_RP windowing_clause {
		$$ = new WindowFunction();
		$$->case_idx_ = CASE2;

		$$->windowing_clause_ = $4; 
	 }
	|CUME_DIST OP_LP OP_RP windowing_clause {
		$$ = new WindowFunction();
		$$->case_idx_ = CASE3;

		$$->windowing_clause_ = $4; 
	 }
	|PERCENT_RANK OP_LP OP_RP windowing_clause {
		$$ = new WindowFunction();
		$$->case_idx_ = CASE4;

		$$->windowing_clause_ = $4; 
	 }
	|NTILE OP_LP iconst OP_RP windowing_clause {
		$$ = new WindowFunction();
		$$->case_idx_ = CASE5;

		$$->iconst_ = $3;
		$$->windowing_clause_ = $5; 
	 }
	|LEAD OP_LP expr opt_lead_lag_info OP_RP opt_null_treatment windowing_clause {
		$$ = new WindowFunction();
		$$->case_idx_ = CASE6;

		$$->expr_ = $3;
		$$->opt_lead_lag_info_ = $4;
		$$->opt_null_treatment_ = $6;
		$$->windowing_clause_ = $7; 
	 }
	|LAG OP_LP expr opt_lead_lag_info OP_RP opt_null_treatment windowing_clause {
		$$ = new WindowFunction();
		$$->case_idx_ = CASE7;

		$$->expr_ = $3;
		$$->opt_lead_lag_info_ = $4;
		$$->opt_null_treatment_ = $6;
		$$->windowing_clause_ = $7; 
	 }
	|FIRST_VALUE OP_LP expr OP_RP opt_null_treatment windowing_clause {
		$$ = new WindowFunction();
		$$->case_idx_ = CASE8;

		$$->expr_ = $3;
		$$->opt_null_treatment_ = $5;
		$$->windowing_clause_ = $6; 
	 }
	|LAST_VALUE OP_LP expr OP_RP opt_null_treatment windowing_clause {
		$$ = new WindowFunction();
		$$->case_idx_ = CASE9;

		$$->expr_ = $3;
		$$->opt_null_treatment_ = $5;
		$$->windowing_clause_ = $6; 
	 }
	|NTH_VALUE OP_LP expr OP_COMMA iconst OP_RP opt_from_first_last opt_null_treatment windowing_clause {
		$$ = new WindowFunction();
		$$->case_idx_ = CASE10;

		$$->expr_ = $3;
		$$->iconst_ = $5;
		$$->opt_from_first_last_ = $7;
		$$->opt_null_treatment_ = $8;
		$$->windowing_clause_ = $9; 
	 }
	;

opt_null_treatment:
	 	 {
		$$ = new OptNullTreatment();
		$$->case_idx_ = CASE0; 
	 }
	|RESPECT NULLS {
		$$ = new OptNullTreatment();
		$$->case_idx_ = CASE1; 
	 }
	|IGNORE NULLS {
		$$ = new OptNullTreatment();
		$$->case_idx_ = CASE2; 
	 }
	;

opt_from_first_last:
	 	 {
		$$ = new OptFromFirstLast();
		$$->case_idx_ = CASE0; 
	 }
	|FROM FIRST {
		$$ = new OptFromFirstLast();
		$$->case_idx_ = CASE1; 
	 }
	|FROM LAST {
		$$ = new OptFromFirstLast();
		$$->case_idx_ = CASE2; 
	 }
	;

opt_lead_lag_info:
	 	 {
		$$ = new OptLeadLagInfo();
		$$->case_idx_ = CASE0; 
	 }
	|lead_lag_info {
		$$ = new OptLeadLagInfo();
		$$->case_idx_ = CASE1;

		$$->lead_lag_info_ = $1; 
	 }
	;

lead_lag_info:
	 OP_COMMA iconst {
		$$ = new LeadLagInfo();
		$$->case_idx_ = CASE0;

		$$->iconst_ = $2; 
	 }
	|OP_COMMA iconst OP_COMMA expr {
		$$ = new LeadLagInfo();
		$$->case_idx_ = CASE1;

		$$->iconst_ = $2;
		$$->expr_ = $4; 
	 }
	;

aggregate_function:
	 AVG OP_LP opt_distinct expr OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE0;

		$$->opt_distinct_ = $3;
		$$->expr_ = $4;
		$$->opt_windowing_clause_ = $6; 
	 }
	|BIT_AND OP_LP expr OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE1;

		$$->expr_ = $3;
		$$->opt_windowing_clause_ = $5; 
	 }
	|BIT_OR OP_LP expr OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE2;

		$$->expr_ = $3;
		$$->opt_windowing_clause_ = $5; 
	 }
	|BIT_XOR OP_LP expr OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE3;

		$$->expr_ = $3;
		$$->opt_windowing_clause_ = $5; 
	 }
	|COUNT OP_LP OP_MUL OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE4;

		$$->opt_windowing_clause_ = $5; 
	 }
	|COUNT OP_LP expr OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE5;

		$$->expr_ = $3;
		$$->opt_windowing_clause_ = $5; 
	 }
	|COUNT OP_LP DISTINCT expr_list OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE6;

		$$->expr_list_ = $4;
		$$->opt_windowing_clause_ = $6; 
	 }
	|MIN OP_LP opt_distinct expr OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE7;

		$$->opt_distinct_ = $3;
		$$->expr_ = $4;
		$$->opt_windowing_clause_ = $6; 
	 }
	|MAX OP_LP opt_distinct expr OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE8;

		$$->opt_distinct_ = $3;
		$$->expr_ = $4;
		$$->opt_windowing_clause_ = $6; 
	 }
	|SUM OP_LP opt_distinct expr OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE9;

		$$->opt_distinct_ = $3;
		$$->expr_ = $4;
		$$->opt_windowing_clause_ = $6; 
	 }
	|STD OP_LP expr OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE10;

		$$->expr_ = $3;
		$$->opt_windowing_clause_ = $5; 
	 }
	|STDDEV_SAMP OP_LP expr OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE11;

		$$->expr_ = $3;
		$$->opt_windowing_clause_ = $5; 
	 }
	|VARIANCE OP_LP expr OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE12;

		$$->expr_ = $3;
		$$->opt_windowing_clause_ = $5; 
	 }
	|VAR_SAMP OP_LP expr OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE13;

		$$->expr_ = $3;
		$$->opt_windowing_clause_ = $5; 
	 }
	|GROUP_CONCAT OP_LP opt_distinct expr_list opt_order_clause opt_gconcat_separator OP_RP opt_windowing_clause {
		$$ = new AggregateFunction();
		$$->case_idx_ = CASE14;

		$$->opt_distinct_ = $3;
		$$->expr_list_ = $4;
		$$->opt_order_clause_ = $5;
		$$->opt_gconcat_separator_ = $6;
		$$->opt_windowing_clause_ = $8; 
	 }
	;

opt_gconcat_separator:
	 	 {
		$$ = new OptGconcatSeparator();
		$$->case_idx_ = CASE0; 
	 }
	|SEPARATOR sconst {
		$$ = new OptGconcatSeparator();
		$$->case_idx_ = CASE1;

		$$->sconst_ = $2; 
	 }
	;

opt_distinct:
	 	 {
		$$ = new OptDistinct();
		$$->case_idx_ = CASE0; 
	 }
	|DISTINCT {
		$$ = new OptDistinct();
		$$->case_idx_ = CASE1; 
	 }
	;

opt_windowing_clause:
	 	 {
		$$ = new OptWindowingClause();
		$$->case_idx_ = CASE0; 
	 }
	|windowing_clause {
		$$ = new OptWindowingClause();
		$$->case_idx_ = CASE1;

		$$->windowing_clause_ = $1; 
	 }
	;

windowing_clause:
	 OVER window_name_or_spec {
		$$ = new WindowingClause();
		$$->case_idx_ = CASE0;

		$$->window_name_or_spec_ = $2; 
	 }
	;

window_name_or_spec:
	 window_name {
		$$ = new WindowNameOrSpec();
		$$->case_idx_ = CASE0;

		$$->window_name_ = $1; 
	 }
	|window_spec {
		$$ = new WindowNameOrSpec();
		$$->case_idx_ = CASE1;

		$$->window_spec_ = $1; 
	 }
	;

control_function:
	 IF OP_LP expr OP_COMMA expr OP_COMMA expr OP_RP {
		$$ = new ControlFunction();
		$$->case_idx_ = CASE0;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5;
		$$->expr_3_ = $7; 
	 }
	|IFNULL OP_LP expr OP_COMMA expr OP_RP {
		$$ = new ControlFunction();
		$$->case_idx_ = CASE1;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	|NULLIF OP_LP expr OP_COMMA expr OP_RP {
		$$ = new ControlFunction();
		$$->case_idx_ = CASE2;

		$$->expr_1_ = $3;
		$$->expr_2_ = $5; 
	 }
	;

case_expr:
	 CASE opt_expr when_clasue_list opt_else END {
		$$ = new CaseExpr();
		$$->case_idx_ = CASE0;

		$$->opt_expr_ = $2;
		$$->when_clasue_list_ = $3;
		$$->opt_else_ = $4; 
	 }
	;

when_clasue_list:
	 when_list {
		$$ = new WhenClasueList();
		$$->case_idx_ = CASE0;

		$$->when_list_ = $1; 
	 }
	|when_list when_clasue_list {
		$$ = new WhenClasueList();
		$$->case_idx_ = CASE1;

		$$->when_list_ = $1;
		$$->when_clasue_list_ = $2; 
	 }
	;

when_list:
	 WHEN expr THEN expr {
		$$ = new WhenList();
		$$->case_idx_ = CASE0;

		$$->expr_1_ = $2;
		$$->expr_2_ = $4; 
	 }
	;

opt_expr:
	 	 {
		$$ = new OptExpr();
		$$->case_idx_ = CASE0; 
	 }
	|expr {
		$$ = new OptExpr();
		$$->case_idx_ = CASE1;

		$$->expr_ = $1; 
	 }
	;

opt_else:
	 	 {
		$$ = new OptElse();
		$$->case_idx_ = CASE0; 
	 }
	|ELSE expr {
		$$ = new OptElse();
		$$->case_idx_ = CASE1;

		$$->expr_ = $2; 
	 }
	;

type_cast:
	 CAST OP_LP expr AS cast_type OP_RP {
		$$ = new TypeCast();
		$$->case_idx_ = CASE0;

		$$->expr_ = $3;
		$$->cast_type_ = $5; 
	 }
	|CONVERT OP_LP expr OP_COMMA cast_type OP_RP {
		$$ = new TypeCast();
		$$->case_idx_ = CASE1;

		$$->expr_ = $3;
		$$->cast_type_ = $5; 
	 }
	;

cast_type:
	 BINARY opt_field_length {
		$$ = new CastType();
		$$->case_idx_ = CASE0;

		$$->opt_field_length_ = $2; 
	 }
	|CHAR opt_field_length {
		$$ = new CastType();
		$$->case_idx_ = CASE1;

		$$->opt_field_length_ = $2; 
	 }
	|NCHAR opt_field_length {
		$$ = new CastType();
		$$->case_idx_ = CASE2;

		$$->opt_field_length_ = $2; 
	 }
	|SIGNED {
		$$ = new CastType();
		$$->case_idx_ = CASE3; 
	 }
	|UNSIGNED {
		$$ = new CastType();
		$$->case_idx_ = CASE4; 
	 }
	|DECIMAL float_options {
		$$ = new CastType();
		$$->case_idx_ = CASE5;

		$$->float_options_ = $2; 
	 }
	|REAL {
		$$ = new CastType();
		$$->case_idx_ = CASE6; 
	 }
	|FLOAT opt_field_length {
		$$ = new CastType();
		$$->case_idx_ = CASE7;

		$$->opt_field_length_ = $2; 
	 }
	;

float_options:
	 	 {
		$$ = new FloatOptions();
		$$->case_idx_ = CASE0; 
	 }
	|field_length {
		$$ = new FloatOptions();
		$$->case_idx_ = CASE1;

		$$->field_length_ = $1; 
	 }
	|precision {
		$$ = new FloatOptions();
		$$->case_idx_ = CASE2;

		$$->precision_ = $1; 
	 }
	;

precision:
	 OP_LP iconst OP_COMMA iconst OP_RP {
		$$ = new Precision();
		$$->case_idx_ = CASE0;

		$$->iconst_1_ = $2;
		$$->iconst_2_ = $4; 
	 }
	;

opt_field_length:
	 	 {
		$$ = new OptFieldLength();
		$$->case_idx_ = CASE0; 
	 }
	|field_length {
		$$ = new OptFieldLength();
		$$->case_idx_ = CASE1;

		$$->field_length_ = $1; 
	 }
	;

field_length:
	 OP_LP iconst OP_RP {
		$$ = new FieldLength();
		$$->case_idx_ = CASE0;

		$$->iconst_ = $2; 
	 }
	;

literal:
	 NULL {
		$$ = new Literal();
		$$->case_idx_ = CASE0; 
	 }
	|FALSE {
		$$ = new Literal();
		$$->case_idx_ = CASE1; 
	 }
	|TRUE {
		$$ = new Literal();
		$$->case_idx_ = CASE2; 
	 }
	;

columnref:
	 column_name {
		$$ = new Columnref();
		$$->case_idx_ = CASE0;

		$$->column_name_ = $1; 
	 }
	|table_name OP_DOT column_name {
		$$ = new Columnref();
		$$->case_idx_ = CASE1;

		$$->table_name_ = $1;
		$$->column_name_ = $3; 
	 }
	;

comp_op:
	 OP_EQUAL {
		$$ = new CompOp();
		$$->case_idx_ = CASE0; 
	 }
	|OP_GREATEREQ {
		$$ = new CompOp();
		$$->case_idx_ = CASE1; 
	 }
	|OP_GREATERTHAN {
		$$ = new CompOp();
		$$->case_idx_ = CASE2; 
	 }
	|OP_LESSEQ {
		$$ = new CompOp();
		$$->case_idx_ = CASE3; 
	 }
	|OP_LESSTHAN {
		$$ = new CompOp();
		$$->case_idx_ = CASE4; 
	 }
	|OP_NOTEQUAL {
		$$ = new CompOp();
		$$->case_idx_ = CASE5; 
	 }
	;

all_subquery:
	 ALL subquery {
		$$ = new AllSubquery();
		$$->case_idx_ = CASE0;

		$$->subquery_ = $2; 
	 }
	|ANY subquery {
		$$ = new AllSubquery();
		$$->case_idx_ = CASE1;

		$$->subquery_ = $2; 
	 }
	|SOME subquery {
		$$ = new AllSubquery();
		$$->case_idx_ = CASE2;

		$$->subquery_ = $2; 
	 }
	;

in_subquery:
	 IN subquery {
		$$ = new InSubquery();
		$$->case_idx_ = CASE0;

		$$->subquery_ = $2; 
	 }
	|NOT IN subquery {
		$$ = new InSubquery();
		$$->case_idx_ = CASE1;

		$$->subquery_ = $3; 
	 }
	;

expr_list:
	 expr {
		$$ = new ExprList();
		$$->case_idx_ = CASE0;

		$$->expr_ = $1; 
	 }
	|expr OP_COMMA expr_list {
		$$ = new ExprList();
		$$->case_idx_ = CASE1;

		$$->expr_ = $1;
		$$->expr_list_ = $3; 
	 }
	;

iconst:
	 ICONST {
		$$ = new Iconst();
		$$->case_idx_ = CASE0;

		$$->long_val_ = $1; 
	 }
	;

fconst:
	 FCONST {
		$$ = new Fconst();
		$$->case_idx_ = CASE0;

		$$->float_val_ = $1; 
	 }
	;

sconst:
	 SCONST {
		$$ = new Sconst();
		$$->case_idx_ = CASE0;

		$$->str_val_ = $1;
		free($1); 
	 }
	;

ident:
	 IDENT {
		$$ = new Ident();
		$$->case_idx_ = CASE0;

		$$->str_val_ = $1;
		free($1); 
	 }
	;

alias:
	 ident {
		$$ = new Alias();
		$$->case_idx_ = CASE0;

		$$->ident_ = $1; 
	 }
	;

column_name:
	 ident {
		$$ = new ColumnName();
		$$->case_idx_ = CASE0;

		$$->ident_ = $1; 
	 }
	;

window_name:
	 ident {
		$$ = new WindowName();
		$$->case_idx_ = CASE0;

		$$->ident_ = $1; 
	 }
	;

table_name:
	 ident {
		$$ = new TableName();
		$$->case_idx_ = CASE0;

		$$->ident_ = $1; 
	 }
	;

constraint_name:
	 ident {
		$$ = new ConstraintName();
		$$->case_idx_ = CASE0;

		$$->ident_ = $1; 
	 }
	;

index_name:
	 ident {
		$$ = new IndexName();
		$$->case_idx_ = CASE0;

		$$->ident_ = $1; 
	 }
	;

view_name:
	 ident {
		$$ = new ViewName();
		$$->case_idx_ = CASE0;

		$$->ident_ = $1; 
	 }
	;

%%

