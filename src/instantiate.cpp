#include "../include/instantiate.h"

using namespace std;

vector<vector<TypeCompatibility>> Column::compatibility_ = {};
map<TERMINAL, ColumnType> Column::typeTranslation_ = {
	{TERMINAL::tInt, ColumnType::ctInt},
	{TERMINAL::tTinyint, ColumnType::ctTinyInt},
	{TERMINAL::tSmallint, ColumnType::ctSmallInt},
	{TERMINAL::tMediumint, ColumnType::ctMediumInt},
	{TERMINAL::tBigint, ColumnType::ctBigInt},
	{TERMINAL::tReal, ColumnType::ctReal},
	{TERMINAL::tDouble, ColumnType::ctDouble},
	{TERMINAL::tFloat, ColumnType::ctFloat},
	{TERMINAL::tDecimal, ColumnType::ctDecimal},
	{TERMINAL::tNumeric, ColumnType::ctNumeric},
	{TERMINAL::tFixed, ColumnType::ctFixed},

	{TERMINAL::tBool, ColumnType::ctBool},
	{TERMINAL::tBoolean, ColumnType::ctBoolean},

	{TERMINAL::tChar, ColumnType::ctChar},
	{TERMINAL::tBinary, ColumnType::ctBinary},
	{TERMINAL::tVarchar, ColumnType::ctVarchar},
	{TERMINAL::tVarbinary, ColumnType::ctVarbinary},
	{TERMINAL::tLongVarbinary, ColumnType::ctLongVarbinary},
	{TERMINAL::tTinyblob, ColumnType::ctTinyBlob},
	{TERMINAL::tMediumblob, ColumnType::ctMediumBlob},
	{TERMINAL::tLongblob, ColumnType::ctLongBlob},
	{TERMINAL::tBlob, ColumnType::ctBlob},
	{TERMINAL::tTinytext, ColumnType::ctTinyText},
	{TERMINAL::tMediumtext, ColumnType::ctMediumText},
	{TERMINAL::tLongtext, ColumnType::ctLongText},
	{TERMINAL::tText, ColumnType::ctText}
};
map<ColumnType, ColumnBasicType> Column::typeToBasicType_ = {
	{ColumnType::ctNull, ColumnBasicType::ctBool},

	{ColumnType::ctInt, ColumnBasicType::ctNumber},
	{ColumnType::ctTinyInt, ColumnBasicType::ctNumber},
	{ColumnType::ctSmallInt, ColumnBasicType::ctNumber},
	{ColumnType::ctMediumInt, ColumnBasicType::ctNumber},
	{ColumnType::ctBigInt, ColumnBasicType::ctNumber},
	{ColumnType::ctReal, ColumnBasicType::ctNumber},
	{ColumnType::ctDouble, ColumnBasicType::ctNumber},
	{ColumnType::ctFloat, ColumnBasicType::ctNumber},
	{ColumnType::ctDecimal, ColumnBasicType::ctNumber},
	{ColumnType::ctNumeric, ColumnBasicType::ctNumber},
	{ColumnType::ctFixed, ColumnBasicType::ctNumber},

	{ColumnType::ctBool, ColumnBasicType::ctBool},
	{ColumnType::ctBoolean, ColumnBasicType::ctBool},

	{ColumnType::ctChar, ColumnBasicType::ctString},
	{ColumnType::ctBinary, ColumnBasicType::ctString},
	{ColumnType::ctVarchar, ColumnBasicType::ctString},
	{ColumnType::ctVarbinary, ColumnBasicType::ctString},
	{ColumnType::ctLongVarbinary, ColumnBasicType::ctString},
	{ColumnType::ctTinyBlob, ColumnBasicType::ctString},
	{ColumnType::ctMediumBlob, ColumnBasicType::ctString},
	{ColumnType::ctLongBlob, ColumnBasicType::ctString},
	{ColumnType::ctBlob, ColumnBasicType::ctString},
	{ColumnType::ctTinyText, ColumnBasicType::ctString},
	{ColumnType::ctMediumText, ColumnBasicType::ctString},
	{ColumnType::ctLongText, ColumnBasicType::ctString},
	{ColumnType::ctText, ColumnBasicType::ctString}
};

Instantiator::Instantiator() {
	random_device rd;
	srand(rd());

	IRTrim_ = new IRTrim(this);
	exprInstantiator_ = new ExprInstantiator(this);
	globalStatusManger_ = new GlobalStatusManger();
	
	startTime = chrono::steady_clock::now();

	return;
}
Instantiator::~Instantiator() {
	delete IRTrim_;
	delete globalStatusManger_;
	delete exprInstantiator_;

	return;
}

unsigned int Instantiator::get_duration_ms(){
	chrono::time_point<chrono::steady_clock> curTime = chrono::steady_clock::now();

	return chrono::duration_cast<chrono::milliseconds>(curTime - startTime).count();
}

bool Instantiator::instantaite_sql(IR* root) {
	int maxCapacity = calc_capacity(root);
	int curCapacity = calc_node(root);

	if (curCapacity > maxCapacity) {
		return false;
	}

	instantiate_parse_toplevel(NT_check(root, IRTYPE::kParseToplevel));

	return true;
}
int Instantiator::calc_capacity(IR* root) {
	int tmp_node_upper_limit = 0;

	unsigned int curCapacity = CAPACITYBASE * pow(CAPACITYGROWTHRATE, static_cast<unsigned int>(get_duration_ms() / TIMETHRESHOLD));
	curCapacity = curCapacity > MAXCAPACITY ? MAXCAPACITY : curCapacity;

	if (root->type_ != IRTYPE::kParseToplevel)
		return 0;

	IR* stmtmulti = root->left_;

	while (stmtmulti) {
		tmp_node_upper_limit += curCapacity;
		stmtmulti = stmtmulti->right_;
	}

	return tmp_node_upper_limit;
}
int Instantiator::calc_node(IR* root) {
	int res = 0;

	if (root->left_) res += calc_node(root->left_);
	if (root->right_) res += calc_node(root->right_);

	return res + 1;
}

void Instantiator::instantiate_parse_toplevel(IR* parse_toplevel) {
	assert(parse_toplevel != nullptr && parse_toplevel->type_ == IRTYPE::kParseToplevel);
	IR* stmtmulti = NT_check(parse_toplevel->left_, IRTYPE::kStmtmulti);

	while (stmtmulti != nullptr) {
		IR* stmt = NT_check(stmtmulti->left_, IRTYPE::kStmt);		

		switch (stmt->left_->type_) {
			case IRTYPE::kSelectStmt: {
				instantiate_select_stmt(NT_check(stmt->left_, IRTYPE::kSelectStmt));
				break;
			}
			case IRTYPE::kCreateTableStmt: {
				instantiate_create_table_stmt(NT_check(stmt->left_, IRTYPE::kCreateTableStmt));
				break;
			}
			case IRTYPE::kCreateViewStmt: {
				instantiate_create_view_stmt(NT_check(stmt->left_, IRTYPE::kCreateViewStmt));
				break;
			}
			case IRTYPE::kCreateIndexStmt: {
				instantiate_create_index_stmt(NT_check(stmt->left_, IRTYPE::kCreateIndexStmt));
				break;
			}
			case IRTYPE::kDropIndexStmt: {
				instantiate_drop_index_stmt(NT_check(stmt->left_, IRTYPE::kDropIndexStmt));
				break;
			}
			case IRTYPE::kDropTableStmt: {
				instantiate_drop_table_or_view_stmt(NT_check(stmt->left_, IRTYPE::kDropTableStmt), IdentifierType::iTable);
				break;
			}
			case IRTYPE::kDropViewStmt: {
				instantiate_drop_table_or_view_stmt(NT_check(stmt->left_, IRTYPE::kDropViewStmt), IdentifierType::iView);
				break;
			}
			case IRTYPE::kDeleteStmt: {
				instantiate_delete_stmt(NT_check(stmt->left_, IRTYPE::kDeleteStmt));
				break;
			}
			case IRTYPE::kUpdateStmt: {
				instantiate_update_stmt(NT_check(stmt->left_, IRTYPE::kUpdateStmt));
				break;
			}
			case IRTYPE::kInsertStmt: {
				instantiate_insert_stmt(NT_check(stmt->left_, IRTYPE::kInsertStmt));
				break;
			}
			case IRTYPE::kAlterTableStmt: {
				instantiate_alter_table_stmt(NT_check(stmt->left_, IRTYPE::kAlterTableStmt));
				break;
			}
			default: {
				return;
			}
		}

		stmtmulti = stmtmulti->right_;
	}

	globalStatusManger_->reset_status();

	return;
}

void Instantiator::instantiate_alter_table_stmt(IR* alter_table_stmt) {
	IR* table_name = NT_check(alter_table_stmt->left_, IRTYPE::kTableName);
	IR* alter_list = NT_check(alter_table_stmt->right_, IRTYPE::kAlterList);

	vector<Table*> tables = globalStatusManger_->get_acce_table(IdentifierType::iTable);
	if (tables.empty()) {
		return;
	}

	Table* table = tables[rand() % tables.size()];
	instantiate_ident(table_name, IdentifierType::iTable, table->identifierName_);

	IR* curNode = alter_list;
	while (curNode != nullptr) {
		IR* alter_list_item = NT_check(curNode->left_, IRTYPE::kAlterListItem);
		instantiate_alter_list_item(alter_list_item, table);

		curNode = curNode->right_;
	}

	return;
}
void Instantiator::instantiate_alter_list_item(IR* alter_list_item, Table* table) {
	switch (alter_list_item->op_->prefix_) {
		case TERMINAL::tAddColumn: {
			IR* column_name = NT_check(alter_list_item->left_, IRTYPE::kColumnName);
			IR* field_def = NT_check(alter_list_item->right_->left_, IRTYPE::kFieldDef);
			IR* opt_place = NT_check(alter_list_item->right_->right_, IRTYPE::kOptPlace);

			IR* data_type = NT_check(field_def->left_, IRTYPE::kDataType);
			IR* opt_column_attribute_list = NT_check(field_def->right_, IRTYPE::kOptColumnAttributeList);

			ColumnType type = instantiate_data_type(data_type);

			name_t newColumnName = table->get_acce_column_name();
			Column* column = new Column(newColumnName, type);

			instantiate_ident(column_name, IdentifierType::iColumn, newColumnName);

			if (opt_place->left_ != nullptr) {
				IR* tmp_column_name = NT_check(opt_place->left_, IRTYPE::kColumnName);

				Column* tmp_column = table->columns_[rand() % table->columns_.size()];
				instantiate_ident(tmp_column_name, IdentifierType::iColumn, tmp_column->identifierName_);

				table->add_column(column, tmp_column);
			}
			else if(opt_place->op_->prefix_ == TERMINAL::tFirst) {
				table->add_column_first(column);
			}
			else {
				table->add_column(column);
			}

			if (opt_column_attribute_list->left_ != nullptr) {
				IR* curNode = NT_check(opt_column_attribute_list->left_, IRTYPE::kColumnAttributeList);

				while (curNode != nullptr) {
					IR* column_attribute = NT_check(curNode->left_, IRTYPE::kColumnAttribute);
					instantiate_column_attribute(column_attribute, column, table);

					curNode = curNode->right_;
				}
			}

			break;
		}
		case TERMINAL::tAddColumnOpLp: {
			IR* table_element_list = NT_check(alter_list_item->left_, IRTYPE::kTableElementList);

			instantiate_table_element_list(table_element_list, table);

			break;
		}
		case TERMINAL::tAdd: {
			IR* table_constraint_def = NT_check(alter_list_item->left_, IRTYPE::kTableConstraintDef);

			if (table_constraint_def->op_->prefix_ == TERMINAL::tIndex) {
				instantiate_table_constraint_def_index(table_constraint_def, table);
			}
			else if (table_constraint_def->op_->middle_ == tForeignKey) {
				instantiate_table_constraint_def_foreign_key(table_constraint_def, table);
			}
			else if (table_constraint_def->right_->type_ == IRTYPE::kCheckConstraint) {
				instantiate_table_constraint_def_check(table_constraint_def, table);
			}
			else {
				instantiate_table_constraint_def_key(table_constraint_def, table);
			}

			break;
		}
		case TERMINAL::tChangeColumn: {
			IR* column_name_1 = nullptr;
			IR* column_name_2 = nullptr;
			IR* field_def = nullptr;
			IR* opt_place = nullptr;

			IR* unknown = nullptr;

			column_name_1 = NT_check(alter_list_item->left_, IRTYPE::kColumnName);
			unknown = alter_list_item->right_;

			column_name_2 = NT_check(unknown->left_, IRTYPE::kColumnName);
			unknown = unknown->right_;

			field_def = NT_check(unknown->left_, IRTYPE::kFieldDef);
			opt_place = NT_check(unknown->right_, IRTYPE::kOptPlace);

			IR* data_type = NT_check(field_def->left_, IRTYPE::kDataType);
			IR* opt_column_attribute_list = NT_check(field_def->right_, IRTYPE::kOptColumnAttributeList);

			Column* column = table->columns_[rand() % table->columns_.size()];
			column->columnType_ = instantiate_data_type(data_type);

			name_t newColumnName = table->get_acce_column_name();
			instantiate_ident(column_name_1, IdentifierType::iColumn, column->identifierName_);
			instantiate_ident(column_name_2, IdentifierType::iColumn, newColumnName);

			table->column_rename(column, column->identifierName_, newColumnName);

			if (opt_place->left_ != nullptr) {
				IR* tmp_column_name = NT_check(opt_place->left_, IRTYPE::kColumnName);

				Column* tmp_column = table->columns_[rand() % table->columns_.size()];

				if (tmp_column != column) {
					instantiate_ident(tmp_column_name, IdentifierType::iColumn, tmp_column->identifierName_);
					table->move_column(column, tmp_column);
				}
				else {
					deep_delete(opt_place->left_);
					opt_place->left_ = nullptr;
					opt_place->op_->prefix_ = TERMINAL::tEmpty;
				}
			}
			else if (opt_place->op_->prefix_ == TERMINAL::tFirst) {
				table->move_column_first(column);
			}

			if (opt_column_attribute_list->left_ != nullptr) {
				IR* curNode = NT_check(opt_column_attribute_list->left_, IRTYPE::kColumnAttributeList);

				while (curNode != nullptr) {
					IR* column_attribute = NT_check(curNode->left_, IRTYPE::kColumnAttribute);
					instantiate_column_attribute(column_attribute, column, table);

					curNode = curNode->right_;
				}
			}

			break;
		}
		case TERMINAL::tModifyColumn: {
			IR* column_name = NT_check(alter_list_item->left_, IRTYPE::kColumnName);
			IR* field_def = NT_check(alter_list_item->right_->left_, IRTYPE::kFieldDef);
			IR* opt_place = NT_check(alter_list_item->right_->right_, IRTYPE::kOptPlace);

			IR* data_type = NT_check(field_def->left_, IRTYPE::kDataType);
			IR* opt_column_attribute_list = NT_check(field_def->right_, IRTYPE::kOptColumnAttributeList);

			Column* column = table->columns_[rand() % table->columns_.size()];
			column->columnType_ = instantiate_data_type(data_type);
			instantiate_ident(column_name, IdentifierType::iColumn, column->identifierName_);

			if (opt_place->left_ != nullptr) {
				IR* tmp_column_name = NT_check(opt_place->left_, IRTYPE::kColumnName);

				Column* tmp_column = table->columns_[rand() % table->columns_.size()];
				
				if (tmp_column != column) {
					instantiate_ident(tmp_column_name, IdentifierType::iColumn, tmp_column->identifierName_);
					table->move_column(column, tmp_column);
				}
				else {
					deep_delete(opt_place->left_);
					opt_place->left_ = nullptr;
					opt_place->op_->prefix_ = TERMINAL::tEmpty;
				}
			}
			else if (opt_place->op_->prefix_ == TERMINAL::tFirst) {
				table->move_column_first(column);
			}

			if (opt_column_attribute_list->left_ != nullptr) {
				IR* curNode = NT_check(opt_column_attribute_list->left_, IRTYPE::kColumnAttributeList);

				while (curNode != nullptr) {
					IR* column_attribute = NT_check(curNode->left_, IRTYPE::kColumnAttribute);
					instantiate_column_attribute(column_attribute, column, table);

					curNode = curNode->right_;
				}
			}

			break;
		}
		case TERMINAL::tDropColumn: {
			IR* column_name = NT_check(alter_list_item->left_, IRTYPE::kColumnName);

			if (table->columns_.size() <= 1) {
				return;
			}

			Column* column = table->columns_[rand() % table->columns_.size()];
			instantiate_ident(column_name, IdentifierType::iColumn, column->identifierName_);

			table->drop_column(column);

			break;
		}
		case TERMINAL::tDropForeignKey: {
			IR* constraint_name = NT_check(alter_list_item->left_, IRTYPE::kConstraintName);

			Constraint* constraint = nullptr;
			for (Constraint* c : table->constraints_) {
				if (c->constraintType_ != ConstraintType::ctForeignKey) {
					continue;
				}

				constraint = c;
				break;
			}

			if (constraint == nullptr) {
				return;
			}

			instantiate_ident(constraint_name, IdentifierType::iConstraint, constraint->identifierName_);

			table->drop_constraint(constraint);

			break;
		}
		case TERMINAL::tDropPrimaryKey: {
			if (table->hasPrimaryKey_ == false) {
				return;
			}

			Constraint* constraint = nullptr;
			for (Constraint* c : table->constraints_) {
				if (c->constraintType_ != ConstraintType::ctPrimaryKey) {
					continue;
				}

				constraint = c;
				break;
			}

			assert(constraint != nullptr);

			table->drop_constraint(constraint);
			table->hasPrimaryKey_ = false;

			break;
		}
		case TERMINAL::tDropIndex: {
			IR* index_name = NT_check(alter_list_item->left_, IRTYPE::kIndexName);

			if (table->indexes_.empty()) {
				return;
			}

			Index* index = table->indexes_[rand() % table->indexes_.size()];
			instantiate_ident(index_name, IdentifierType::iIndex, index->identifierName_);

			table->drop_index(index);

			break;
		}
		case TERMINAL::tDropCheck: {
			IR* constraint_name = NT_check(alter_list_item->left_, IRTYPE::kConstraintName);

			Constraint* constraint = nullptr;
			for (Constraint* c : table->constraints_) {
				if (c->constraintType_ != ConstraintType::ctCheck) {
					continue;
				}

				constraint = c;
				break;
			}

			if (constraint == nullptr) {
				return;
			}

			instantiate_ident(constraint_name, IdentifierType::iConstraint, constraint->identifierName_);

			table->drop_constraint(constraint);

			break;
		}
		case TERMINAL::tDropConstraint: {
			IR* constraint_name = NT_check(alter_list_item->left_, IRTYPE::kConstraintName);

			if (table->constraints_.empty()) {
				return;
			}

			Constraint* constraint = table->constraints_[rand() % table->constraints_.size()];
			instantiate_ident(constraint_name, IdentifierType::iConstraint, constraint->identifierName_);

			table->drop_constraint(constraint);

			break;
		}
		case TERMINAL::tAlterColumn: {
			IR* column_name = NT_check(alter_list_item->left_, IRTYPE::kColumnName);

			Column* column = table->columns_[rand() % table->columns_.size()];
			instantiate_ident(column_name, IdentifierType::iColumn, column->identifierName_);

			switch (alter_list_item->op_->middle_) {
				case TERMINAL::tSetDefaultOpLp: {
					IR* expr_root = NT_check(alter_list_item->right_, IRTYPE::kExprRoot);

					exprInstantiator_->instantiate_expr_root(expr_root, table->columns_, StmtLocation::slNotSubquery);
					
					break;
				}
				case TERMINAL::tDropDefault: {
					break;
				}
				case TERMINAL::tSet: {
					break;
				}
				default: {
					fprintf(stderr, "ERROR: Unknown ALTER COLUMN!!!\n");
					assert(0);
				}
			}

			break;
		}
		case TERMINAL::tAlterIndex: {
			IR* index_name = NT_check(alter_list_item->left_, IRTYPE::kIndexName);

			if (table->indexes_.empty()) {
				return;
			}

			Index* index = table->indexes_[rand() % table->indexes_.size()];
			instantiate_ident(index_name, IdentifierType::iIndex, index->identifierName_);

			break;
		}
		case TERMINAL::tRenameTo: {
			IR* table_name = NT_check(alter_list_item->left_, IRTYPE::kTableName);

			name_t newTableName = globalStatusManger_->get_new_table_name();

			instantiate_ident(table_name, IdentifierType::iTable, newTableName);

			table->identifierName_ = newTableName;

			break;
		}
		case TERMINAL::tRenameIndex: {
			IR* index_name_1 = NT_check(alter_list_item->left_, IRTYPE::kIndexName);
			IR* index_name_2 = NT_check(alter_list_item->right_, IRTYPE::kIndexName);

			if (table->indexes_.empty()) {
				return;
			}

			Index* index = table->indexes_[rand() % table->indexes_.size()];
			name_t newIndexName = globalStatusManger_->get_new_index_name();

			instantiate_ident(index_name_1, IdentifierType::iIndex, index->identifierName_);
			instantiate_ident(index_name_2, IdentifierType::iIndex, newIndexName);

			index->identifierName_ = newIndexName;

			break;
		}
		case TERMINAL::tRenameColumn: {
			IR* column_name_1 = NT_check(alter_list_item->left_, IRTYPE::kColumnName);
			IR* column_name_2 = NT_check(alter_list_item->right_, IRTYPE::kColumnName);

			Column* column = table->columns_[rand() % table->columns_.size()];
			name_t newColumnName = table->get_acce_column_name();

			instantiate_ident(column_name_1, IdentifierType::iColumn, column->identifierName_);
			instantiate_ident(column_name_2, IdentifierType::iColumn, newColumnName);

			table->column_rename(column, column->identifierName_, newColumnName);

			break;
		}
		default: {
			fprintf(stderr, "ERROR: Unknown alter_list_item!!!\n");
			assert(0);
		}
	}

	return;
}

void Instantiator::instantiate_insert_stmt(IR* insert_stmt) {
	IR* table_name = nullptr;
	IR* insert_from_constructor = nullptr;
	IR* opt_values_reference = nullptr;
	IR* opt_insert_update_list = nullptr;
	IR* update_list = nullptr;
	IR* insert_query_expression = nullptr;

	IR* unknown = insert_stmt->right_->right_;

	table_name = NT_check(unknown->left_, IRTYPE::kTableName);
	unknown = unknown->right_;

	switch (unknown->left_->type_) {
		case IRTYPE::kInsertFromConstructor: {
			insert_from_constructor = NT_check(unknown->left_, IRTYPE::kInsertFromConstructor);
			unknown = unknown->right_;

			opt_values_reference = NT_check(unknown->left_, IRTYPE::kOptValuesReference);

			break;
		}
		case IRTYPE::kUpdateList: {
			update_list = NT_check(unknown->left_, IRTYPE::kUpdateList);
			unknown = unknown->right_;

			opt_values_reference = NT_check(unknown->left_, IRTYPE::kOptValuesReference);

			break;
		}
		case IRTYPE::kInsertQueryExpression: {
			insert_query_expression = NT_check(unknown->left_, IRTYPE::kInsertQueryExpression);
			break;
		}
		default: {
			fprintf(stderr, "ERROR: Unknown Insert Stmt!!!\n");
			assert(0);
		}
	}

	opt_insert_update_list = NT_check(unknown->right_, IRTYPE::kOptInsertUpdateList);

	vector<Table*> tables = globalStatusManger_->get_acce_table(IdentifierType::iTable);
	if (tables.empty()) {
		return;
	}

	globalStatusManger_->push_selectInfo(StmtLocation::slNotSubquery, DEFAULTROWNUMBER, DEFAULTCOLUMNNUMBER);

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	Table* chosenTable = tables[rand() % tables.size()];
	instantiate_ident(table_name, IdentifierType::iTable, chosenTable->identifierName_);

	Table* newTable = new Table(chosenTable, info);
	info->add_used_table(newTable);

	int chosenColumnNumber = chosenTable->columns_.size();
	Table* tmpTable = nullptr;

	info->update_acceColumns();

	if (insert_from_constructor != nullptr) {
		bool hasOptColumn = false;

		IR* values_list = nullptr;
		IR* column_list = nullptr;

		if (insert_from_constructor->right_ == nullptr) {
			values_list = NT_check(insert_from_constructor->left_, IRTYPE::kValuesList);
		}
		else {
			values_list = NT_check(insert_from_constructor->right_, IRTYPE::kValuesList);
			column_list = NT_check(insert_from_constructor->left_, IRTYPE::kColumnList);
		}

		if (column_list != nullptr) {
			hasOptColumn = true;

			vector<Column*> columnsSeq = chosenTable->get_column_seq();
			vector<Column*> chosenColumns = vector<Column*>(columnsSeq.begin(), columnsSeq.begin() + exprInstantiator_->get_random_integer(1, chosenTable->columns_.size()));

			chosenColumnNumber = chosenColumns.size();

			instantiate_column_list(column_list, chosenColumns);
		}

		int tmpChosenColumnNumber = chosenColumnNumber;
		if (hasOptColumn == false) {
			tmpChosenColumnNumber = (rand() % 20) == 0 ? 0 : chosenColumnNumber;
			chosenColumnNumber = tmpChosenColumnNumber;
		}

		instantiate_values_list(values_list, tmpChosenColumnNumber);
	}
	else if (update_list != nullptr) {
		chosenColumnNumber = instantiate_update_list(update_list, nullptr);
	}
	else {
		IR* query_expression = nullptr;
		IR* column_list = nullptr;

		if (insert_query_expression->right_ == nullptr) {
			query_expression = NT_check(insert_query_expression->left_, IRTYPE::kQueryExpression);
		}
		else {
			query_expression = NT_check(insert_query_expression->right_, IRTYPE::kQueryExpression);
			column_list = NT_check(insert_query_expression->left_, IRTYPE::kColumnList);
		}

		if (column_list != nullptr) {
			vector<Column*> columnsSeq = chosenTable->get_column_seq();
			vector<Column*> chosenColumns = vector<Column*>(columnsSeq.begin(), columnsSeq.begin() + exprInstantiator_->get_random_integer(1, chosenTable->columns_.size()));

			chosenColumnNumber = chosenColumns.size();

			instantiate_column_list(column_list, chosenColumns);
		}

		Table* tmp = instantiate_query_expression(query_expression, StmtLocation::slNotSubquery, DEFAULTROWNUMBER, chosenColumnNumber);
		delete tmp;
	}

	if (opt_values_reference != nullptr && opt_values_reference->left_ != nullptr) {
		if (chosenColumnNumber != 0) {
			IR* tmp_table_name = NT_check(opt_values_reference->left_, IRTYPE::kTableName);
			IR* opt_column_list = NT_check(opt_values_reference->right_, IRTYPE::kOptColumnList);

			name_t newTableName = globalStatusManger_->get_new_table_name();
			instantiate_ident(tmp_table_name, IdentifierType::iTable, newTableName);

			vector<name_t> chosenColumns;
			if (opt_insert_update_list->left_ != nullptr) {
				tmpTable = new Table(newTableName, IdentifierType::iTable, info);

				for (int i = 0; i < chosenColumnNumber; i++) {
					name_t newColumnName = tmpTable->get_acce_column_name();
					Column* newColumn = new Column(newColumnName, ColumnType::ctNull);

					tmpTable->add_column(newColumn);
					chosenColumns.push_back(newColumnName);
				}

				if (opt_column_list->left_ == nullptr) {
					opt_column_list->left_ = deep_copy(IRTrim_->simple_column_list_);
					opt_column_list->op_->prefix_ = TERMINAL::tOpLp;
					opt_column_list->op_->middle_ = TERMINAL::tOpRp;
				}
			}
			else {
				for (int i = 0; i < chosenColumnNumber; i++) {
					chosenColumns.push_back(static_cast<name_t>(i));
				}
			}

			if (opt_column_list->left_ != nullptr) {
				IR* column_list = NT_check(opt_column_list->left_, IRTYPE::kColumnList);
				instantiate_column_list(column_list, chosenColumns);
			}
		}
		else {
			deep_delete(opt_values_reference->left_);
			deep_delete(opt_values_reference->right_);

			opt_values_reference->left_ = nullptr;
			opt_values_reference->right_ = nullptr;

			opt_values_reference->op_->prefix_ = TERMINAL::tEmpty;
		}
	}

	if (opt_insert_update_list->left_ != nullptr) {
		IR* tmp_update_list = NT_check(opt_insert_update_list->left_, IRTYPE::kUpdateList);
		instantiate_update_list(tmp_update_list, tmpTable);
	}

	if (tmpTable != nullptr) {
		delete tmpTable;
	}

	globalStatusManger_->pop_selectInfo();

	return;
}
void Instantiator::instantiate_values_list(IR* values_list, int chosenColumnNumber) {
	IR* curNode = values_list;
	while (curNode != nullptr) {
		IR* opt_values = NT_check(curNode->left_->left_, IRTYPE::kOptValues);
		IR* values = opt_values->left_ == nullptr ? nullptr : NT_check(opt_values->left_, IRTYPE::kValues);

		if (chosenColumnNumber == 0 && values != nullptr) {
			deep_delete(opt_values->left_);
			opt_values->left_ = nullptr;
			values = nullptr;
		}
		else if (chosenColumnNumber != 0 && values == nullptr) {
			opt_values->left_ = deep_copy(IRTrim_->simple_values_);
			values = opt_values->left_;
		}

		if (values != nullptr) {
			assert(chosenColumnNumber > 0);
			instantiate_values(values, chosenColumnNumber);
		}

		curNode = curNode->right_;
	}

	return;
}
void Instantiator::instantiate_values(IR* values, int chosenColumnNumber) {
	int count = 0;
	IR* curNode = values;
	while (curNode != nullptr) {
		IR* expr_or_default = NT_check(curNode->left_, IRTYPE::kExprOrDefault);
		IR* expr_root = expr_or_default->left_ == nullptr ? nullptr : NT_check(expr_or_default->left_, IRTYPE::kExprRoot);

		if (expr_root != nullptr) {
			vector<Column*> tmp;
			exprInstantiator_->instantiate_expr_root(expr_root, tmp, StmtLocation::slNotSubquery);
		}

		count++;

		if (count == chosenColumnNumber && curNode->right_ != nullptr) {
			deep_delete(curNode->right_);

			curNode->right_ = nullptr;
			curNode->op_->middle_ = TERMINAL::tEmpty;
		}
		else if (count < chosenColumnNumber && curNode->right_ == nullptr) {
			curNode->right_ = deep_copy(IRTrim_->simple_values_);
			curNode->op_->middle_ = TERMINAL::tOpComma;
		}

		curNode = curNode->right_;
	}

	return;
}

void Instantiator::instantiate_update_stmt(IR* update_stmt) {
	IR* opt_with_clause = nullptr;
	IR* table_reference_list = nullptr;
	IR* update_list = nullptr;
	IR* opt_where_clause = nullptr;
	IR* opt_order_clause = nullptr;
	IR* opt_simple_limit = nullptr;

	IR* unknown = nullptr;

	opt_with_clause = NT_check(update_stmt->left_, IRTYPE::kOptWithClause);
	unknown = update_stmt->right_->right_->right_;

	table_reference_list = NT_check(unknown->left_, IRTYPE::kTableReferenceList);
	unknown = unknown->right_;

	update_list = NT_check(unknown->left_, IRTYPE::kUpdateList);
	unknown = unknown->right_;

	opt_where_clause = NT_check(unknown->left_, IRTYPE::kOptWhereClause);
	unknown = unknown->right_;

	opt_order_clause = NT_check(unknown->left_, IRTYPE::kOptOrderClause);
	opt_simple_limit = NT_check(unknown->right_, IRTYPE::kOptSimpleLimit);

	globalStatusManger_->push_selectInfo(StmtLocation::slNotSubquery, DEFAULTROWNUMBER, DEFAULTCOLUMNNUMBER);

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	if (opt_with_clause->left_ != nullptr) {
		IR* with_clause = NT_check(opt_with_clause->left_, IRTYPE::kWithClause);
		instantiate_with_clause(with_clause);
	}

	IR* curNode = table_reference_list;
	while (curNode != nullptr) {
		IR* table_reference = NT_check(curNode->left_, IRTYPE::kTableReference);
		instantiate_table_reference(table_reference);

		curNode = curNode->right_;
	}

	info->update_acceColumns();	

	if (opt_where_clause->left_ != nullptr) {
		IR* where_clause = NT_check(opt_where_clause->left_, IRTYPE::kWhereClause);
		instantiate_where_clause(where_clause);
	}

	if (opt_order_clause->left_ != nullptr) {
		IR* order_clause = NT_check(opt_order_clause->left_, IRTYPE::kOrderClause);
		instantiate_order_clause(order_clause);
	}

	if (opt_simple_limit->left_ != nullptr) {
		IR* iconst = NT_check(opt_simple_limit->left_, IRTYPE::kIconst);
		iconst->long_val_ = exprInstantiator_->get_random_integer(LIMITMIN, LIMITMAX);
	}

	instantiate_update_list(update_list, nullptr);

	globalStatusManger_->pop_selectInfo();

	return;
}
int Instantiator::instantiate_update_list(IR* update_list, Table* table) {
	int chosenColumnNumber = 0;

	vector<Column*> columns;
	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();	
	
	if (info->acceColumns_.empty()) {
		return 0;
	}

	if (table != nullptr) {
		vector<int> tmp = vector<int>(COLUMNNAMEMAX, 0);

		for (Column* c : table->columns_) {
			tmp[c->identifierName_]++;
		}

		for (Column* c : info->acceColumns_) {
			Column* tmpColumn = new Column(c);
			tmpColumn->parentTable_ = c->parentTable_;

			if (tmp[tmpColumn->identifierName_] != 0) {
				tmpColumn->duplicated_ = ColumnStatus::csDuplication;
			}

			tmp[tmpColumn->identifierName_]++;
			columns.push_back(tmpColumn);
		}

		for (Column* c : table->columns_) {
			Column* tmpColumn = new Column(c);
			tmpColumn->parentTable_ = c->parentTable_;

			if (tmp[tmpColumn->identifierName_] > 1) {
				tmpColumn->duplicated_ = ColumnStatus::csDuplication;
			}

			columns.push_back(tmpColumn);
		}
	}

	IR* curNode = update_list;
	while (curNode != nullptr) {
		chosenColumnNumber++;

		IR* columnref = NT_check(curNode->left_->left_, IRTYPE::kColumnref);
		IR* expr_root = (curNode->left_->right_->left_ == nullptr) ? nullptr : NT_check(curNode->left_->right_->left_, IRTYPE::kExprRoot);

		Column* chosenColumn = info->acceColumns_[rand() % info->acceColumns_.size()];

		IR* column_name = nullptr;
		IR* table_name = nullptr;

		if (chosenColumn->duplicated_ == ColumnStatus::csDuplication && columnref->right_ == nullptr) {
			IR* tmp1 = new IR(IRTYPE::kIdent, OP0(), nullptr, nullptr);
			IR* tmp2 = new IR(IRTYPE::kTableName, OP0(), tmp1, nullptr);

			columnref->right_ = columnref->left_;
			columnref->left_ = tmp2;
			columnref->op_->middle_ = TERMINAL::tOpDot;
		}

		if (columnref->right_ != nullptr) {
			table_name = NT_check(columnref->left_, IRTYPE::kTableName);
			column_name = NT_check(columnref->right_, IRTYPE::kColumnName);
		}
		else {
			column_name = NT_check(columnref->left_, IRTYPE::kColumnName);
		}

		instantiate_ident(column_name, IdentifierType::iColumn, chosenColumn->identifierName_);
		if (table_name != nullptr) {
			instantiate_ident(table_name, IdentifierType::iTable, chosenColumn->parentTable_->identifierName_);
		}

		if (expr_root != nullptr) {
			if (table != nullptr) {
				exprInstantiator_->instantiate_expr_root(expr_root, columns, StmtLocation::slNotSubquery);
			}
			else {
				exprInstantiator_->instantiate_expr_root(expr_root, info->acceColumns_, StmtLocation::slNotSubquery);
			}
		}

		curNode = curNode->right_;
	}

	for (Column* c : columns) {
		delete c;
	}

	return chosenColumnNumber;
}

void Instantiator::instantiate_delete_stmt(IR* delete_stmt) {
	IR* opt_with_clause = nullptr;
	IR* table_name = nullptr;
	IR* opt_alias = nullptr;
	IR* opt_where_clause = nullptr;
	IR* opt_order_clause = nullptr;
	IR* opt_simple_limit = nullptr;
	IR* table_list = nullptr;
	IR* table_reference_list = nullptr;

	IR* unknown = nullptr;

	opt_with_clause = NT_check(delete_stmt->left_, IRTYPE::kOptWithClause);
	unknown = delete_stmt->right_->right_;

	if (unknown->left_->type_ == IRTYPE::kTableName) {
		table_name = NT_check(unknown->left_, IRTYPE::kTableName);
		unknown = unknown->right_;

		opt_alias = NT_check(unknown->left_, IRTYPE::kOptAlias);
		unknown = unknown->right_;

		opt_where_clause = NT_check(unknown->left_, IRTYPE::kOptWhereClause);
		unknown = unknown->right_;

		opt_order_clause = NT_check(unknown->left_, IRTYPE::kOptOrderClause);
		opt_simple_limit = NT_check(unknown->right_, IRTYPE::kOptSimpleLimit);
	}
	else {
		table_list = NT_check(unknown->left_, IRTYPE::kTableList);
		unknown = unknown->right_;

		table_reference_list = NT_check(unknown->left_, IRTYPE::kTableReferenceList);
		opt_where_clause = NT_check(unknown->right_, IRTYPE::kOptWhereClause);
	}

	globalStatusManger_->push_selectInfo(StmtLocation::slNotSubquery, DEFAULTROWNUMBER, DEFAULTCOLUMNNUMBER);

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	if (table_name != nullptr) {
		if (opt_with_clause->left_ != nullptr) {
			deep_delete(opt_with_clause->left_);
			opt_with_clause->left_ = nullptr;
		}

		vector<Table*> tables = globalStatusManger_->get_acce_table(IdentifierType::iTable);
		if (tables.empty()) {
			globalStatusManger_->pop_selectInfo();
			return;
		}

		Table* chosenTable = tables[rand() % tables.size()];
		Table* newTable = new Table(chosenTable, info);
		info->add_used_table(newTable);

		instantiate_ident(table_name, IdentifierType::iTable, newTable->identifierName_);

		if (opt_alias->left_ != nullptr) {
			IR* alias = NT_check(opt_alias->left_, IRTYPE::kAlias);
			name_t newName = globalStatusManger_->get_new_table_name();

			instantiate_ident(alias, IdentifierType::iTable, newName);

			newTable->identifierName_ = newName;
		}

		info->update_acceColumns();

		if (opt_where_clause->left_ != nullptr) {
			IR* where_clause = NT_check(opt_where_clause->left_, IRTYPE::kWhereClause);
			instantiate_where_clause(where_clause);
		}

		if (opt_order_clause->left_ != nullptr) {
			IR* order_clause = NT_check(opt_order_clause->left_, IRTYPE::kOrderClause);
			instantiate_order_clause(order_clause);
		}

		if (opt_simple_limit->left_ != nullptr) {
			IR* iconst = NT_check(opt_simple_limit->left_, IRTYPE::kIconst);
			iconst->long_val_ = exprInstantiator_->get_random_integer(LIMITMIN, LIMITMAX);
		}
	}
	else if (table_list != nullptr) {
		if (opt_with_clause->left_ != nullptr) {
			IR* with_clause = NT_check(opt_with_clause->left_, IRTYPE::kWithClause);
			instantiate_with_clause(with_clause);
		}

		IR* curNode = table_reference_list;
		while (curNode != nullptr) {
			IR* table_reference = NT_check(curNode->left_, IRTYPE::kTableReference);
			instantiate_table_reference(table_reference);

			curNode = curNode->right_;
		}

		info->update_acceColumns();

		vector<Table*> tables = info->usedTables_;
		assert(!tables.empty());

		curNode = table_list;
		while (curNode != nullptr) {
			IR* table_name = NT_check(curNode->left_, IRTYPE::kTableName);

			int tmp = rand() % tables.size();
			Table* table = tables[tmp];
			tables.erase(tables.begin() + tmp);

			instantiate_ident(table_name, table->identifierType_, table->identifierName_);

			if (tables.empty() && curNode->right_ != nullptr) {
				deep_delete(curNode->right_);
				curNode->right_ = nullptr;
				curNode->op_->middle_ = TERMINAL::tEmpty;
			}

			curNode = curNode->right_;
		}

		if (opt_where_clause->left_ != nullptr) {
			IR* where_clause = NT_check(opt_where_clause->left_, IRTYPE::kWhereClause);
			instantiate_where_clause(where_clause);
		}
	}
	else {
		fprintf(stderr, "ERROR: Incorrect Delete Statement!!!\n");
		assert(0);
	}

	globalStatusManger_->pop_selectInfo();

	return;
}

void Instantiator::instantiate_drop_table_or_view_stmt(IR* drop_table_stmt, IdentifierType type) {
	bool existFlag = false;

	IR* if_exists = NT_check(drop_table_stmt->left_, IRTYPE::kIfExists);
	IR* table_list = NT_check(drop_table_stmt->right_, IRTYPE::kTableList);

	vector<Table*> tables = globalStatusManger_->get_acce_table(type);
	if (tables.empty()) {
		if_exists->op_->prefix_ = TERMINAL::tIfExists;
	}

	if (if_exists->op_->prefix_ == TERMINAL::tIfExists) {
		existFlag = true;
	}

	name_t nextTableName = UNREFERENCE;
	if (type == IdentifierType::iTable) {
		nextTableName = globalStatusManger_->get_new_table_name();
	}
	else if (type == IdentifierType::iView) {
		nextTableName = globalStatusManger_->get_new_view_name();
	}
	else {
		fprintf(stderr, "ERROR: IdentifierType isn't iTable or iView!!!\n");
		assert(0);
	}

	IR* curNode = table_list;
	while (curNode != nullptr) {
		name_t tmpTableName = UNREFERENCE;
		IR* table_name = NT_check(curNode->left_, IRTYPE::kTableName);

		if (existFlag == true && (rand() % 2 == 0 || tables.empty())) {
			tmpTableName = exprInstantiator_->get_random_integer(nextTableName, UCHAR_MAX);
		}
		else {
			int tmp = rand() % tables.size();
			Table* tmpTable = tables[tmp];

			tmpTableName = tmpTable->identifierName_;

			tables.erase(tables.begin() + tmp);
			globalStatusManger_->drop_table(tmpTable);
		}

		instantiate_ident(table_name, type, tmpTableName);

		if (existFlag == false && tables.empty() && curNode->right_ != nullptr) {
			deep_delete(curNode->right_);
			curNode->right_ = nullptr;
			curNode->op_->middle_ = TERMINAL::tEmpty;
		}

		curNode = curNode->right_;
	}

	return;
}

void Instantiator::instantiate_drop_index_stmt(IR* drop_index_stmt) {
	IR* index_name = NT_check(drop_index_stmt->left_, IRTYPE::kIndexName);
	IR* table_name = NT_check(drop_index_stmt->right_, IRTYPE::kTableName);

	vector<Table*> tables = globalStatusManger_->get_acce_table(IdentifierType::iTable);
	if (tables.empty()) {
		return;
	}

	Table* table = nullptr;
	for (Table* t : tables) {
		if (t->indexes_.empty()) {
			continue;
		}

		table = t;
		break;
	}

	if (table == nullptr) {
		return;
	}

	Index* index = table->indexes_[rand() % table->indexes_.size()];

	instantiate_ident(index_name, IdentifierType::iIndex, index->identifierName_);
	instantiate_ident(table_name, IdentifierType::iTable, table->identifierName_);

	table->drop_index(index);

	return;
}

void Instantiator::instantiate_create_index_stmt(IR* create_index_stmt) {
	IR* index_name = nullptr;
	IR* table_name = nullptr;
	IR* key_list_with_expression = nullptr;

	IR* unknown = create_index_stmt->right_;

	index_name = NT_check(unknown->left_, IRTYPE::kIndexName);
	unknown = unknown->right_;

	table_name = NT_check(unknown->left_, IRTYPE::kTableName);
	key_list_with_expression = NT_check(unknown->right_, IRTYPE::kKeyListWithExpression);

	vector<Table*> tables = globalStatusManger_->get_acce_table(IdentifierType::iTable);
	if (tables.empty()) {
		return;
	}

	Table* table = tables[rand() % tables.size()];

	name_t newIndexName = globalStatusManger_->get_new_index_name();
	Index* newIndex = new Index(newIndexName);

	instantiate_ident(index_name, IdentifierType::iIndex, newIndex->identifierName_);
	instantiate_ident(table_name, IdentifierType::iTable, table->identifierName_);

	newIndex->columns_ = instantiate_key_list_with_expression(key_list_with_expression, table, ConstraintType::ctNull);

	table->add_index(newIndex);

	return;
}

void Instantiator::instantiate_create_view_stmt(IR* create_view_stmt) {
	IR* opt_or_replace = nullptr;
	IR* view_name = nullptr;
	IR* opt_column_list = nullptr;
	IR* query_expression = nullptr;

	IR* unknown = nullptr;

	opt_or_replace = NT_check(create_view_stmt->left_, IRTYPE::kOptOrReplace);
	unknown = create_view_stmt->right_->right_;

	view_name = NT_check(unknown->left_, IRTYPE::kViewName);
	unknown = unknown->right_;

	opt_column_list = NT_check(unknown->left_, IRTYPE::kOptColumnList);
	query_expression = NT_check(unknown->right_, IRTYPE::kQueryExpression);

	name_t newViewName = globalStatusManger_->get_new_view_name();
	if (opt_or_replace->op_->prefix_ == TERMINAL::tOrReplace) {
		newViewName = (newViewName != 0 && (rand() % 2) == 0) ? (rand() % newViewName) : newViewName;
		globalStatusManger_->drop_table(newViewName, IdentifierType::iView);
	}

	instantiate_ident(view_name, IdentifierType::iView, newViewName);

	Table* view = instantiate_query_expression(query_expression, StmtLocation::slNotSubquery, DEFAULTROWNUMBER, DEFAULTCOLUMNNUMBER);
	
	view->identifierName_ = newViewName;
	view->identifierType_ = IdentifierType::iView;

	if (opt_column_list->left_ != nullptr) {
		IR* column_list = NT_check(opt_column_list->left_, IRTYPE::kColumnList);

		for (Column* column : view->columns_) {
			view->column_rename(column, column->identifierName_, view->get_acce_column_name());
		}

		instantiate_column_list(column_list, view->columns_);
	}

	assert(globalStatusManger_->add_table(view));

	return;
}

void Instantiator::instantiate_create_table_stmt(IR* create_table_stmt) {
	Table* res = nullptr;

	IR* opt_temporary = nullptr;
	IR* opt_if_not_exists = nullptr;
	IR* table_name = nullptr;
	IR* table_element_list = nullptr;
	IR* duplicate_as_qe = nullptr;
	IR* opt_duplicate_as_qe = nullptr;

	IR* unknown = nullptr;

	opt_temporary = NT_check(create_table_stmt->left_, IRTYPE::kOptTemporary);
	unknown = create_table_stmt->right_;

	opt_if_not_exists = NT_check(unknown->left_, IRTYPE::kOptIfNotExists);
	unknown = unknown->right_;

	table_name = NT_check(unknown->left_, IRTYPE::kTableName);
	unknown = unknown->right_;

	if (unknown->type_ == IRTYPE::kDuplicateAsQe) {
		duplicate_as_qe = NT_check(unknown, IRTYPE::kDuplicateAsQe);
	}
	else {
		table_element_list = NT_check(unknown->left_, IRTYPE::kTableElementList);
		opt_duplicate_as_qe = NT_check(unknown->right_, IRTYPE::kOptDuplicateAsQe);

		if (opt_duplicate_as_qe->left_ != nullptr) {
			duplicate_as_qe = NT_check(opt_duplicate_as_qe->left_, IRTYPE::kDuplicateAsQe);
		}
	}

	name_t newTableName = globalStatusManger_->get_new_table_name();
	instantiate_ident(table_name, IdentifierType::iTable, newTableName);

	if (table_element_list != nullptr) {
		res = new Table(newTableName, IdentifierType::iTable, nullptr);
		instantiate_table_element_list(table_element_list, res);
	}
	
	if (duplicate_as_qe != nullptr) {
		IR* query_expression = nullptr;

		if (duplicate_as_qe->left_->type_ == IRTYPE::kQueryExpression) {
			query_expression = NT_check(duplicate_as_qe->left_, IRTYPE::kQueryExpression);
		}
		else {
			query_expression = NT_check(duplicate_as_qe->right_, IRTYPE::kQueryExpression);
		}

		Table* tmpTable = instantiate_query_expression(query_expression, StmtLocation::slNotSubquery, DEFAULTROWNUMBER, DEFAULTCOLUMNNUMBER);

		if (res == nullptr) {
			res = tmpTable;
		}
		else {
			for (Column* c : tmpTable->columns_) {
				res->add_column(new Column(c), nullptr);
			}

			delete tmpTable;
		}

		res->identifierName_ = newTableName;
	}	

	if (opt_temporary->op_->prefix_ == TERMINAL::tTemporary) {
		res->isTemporary_ = true;
	}

	assert(globalStatusManger_->add_table(res));

	return;
}
void Instantiator::instantiate_table_element_list(IR* table_element_list, Table* table) {
	map<Column*, vector<IR*>> columnWithColumnAttributes;

	int column_def_count = 0;
	IR* curNode = table_element_list;
	while (curNode != nullptr) {
		IR* table_element = NT_check(curNode->left_, IRTYPE::kTableElement);

		if (table_element->left_->type_ == IRTYPE::kColumnDef) {
			column_def_count++;
		}

		curNode = curNode->right_;
	}

	if (table->columns_.empty() && column_def_count == 0) {
		deep_delete(table_element_list->left_);
		table_element_list->left_ = deep_copy(IRTrim_->simple_table_element_);
	}

	curNode = table_element_list;
	vector<IR*> tableConstraintDefs;
	while (curNode != nullptr) {
		IR* table_element = curNode->left_;

		if (table_element->left_->type_ == IRTYPE::kColumnDef) {
			pair<Column*, vector<IR*>> tmp = instantiate_column_def(NT_check(table_element->left_, IRTYPE::kColumnDef), table);
			columnWithColumnAttributes[tmp.first] = tmp.second;
		}
		else {
			tableConstraintDefs.push_back(NT_check(table_element->left_, IRTYPE::kTableConstraintDef));			
		}

		curNode = curNode->right_;
	}

	assert(!table->columns_.empty());

	for (auto& it : columnWithColumnAttributes) {
		for (IR* column_attribute : it.second) {
			instantiate_column_attribute(column_attribute, it.first, table);
		}
	}

	vector<IR*> waitForDelete;
	for (IR* tmp : tableConstraintDefs) {
		bool flag = false;

		if (tmp->left_->type_ == IRTYPE::kOptIndexName) {
			flag = instantiate_table_constraint_def_index(tmp, table);
		}
		else if (tmp->right_->type_ == IRTYPE::kCheckConstraint) {
			flag = instantiate_table_constraint_def_check(tmp, table);
		}
		else if (tmp->op_->middle_ == TERMINAL::tForeignKey) {
			flag = instantiate_table_constraint_def_foreign_key(tmp, table);
		}
		else {
			flag = instantiate_table_constraint_def_key(tmp, table);
		}

		if (flag == false) {
			waitForDelete.push_back(tmp);
		}
	}

	curNode = table_element_list;
	IR* preNode = curNode;
	while (curNode != nullptr && !waitForDelete.empty()) {
		IR* table_element = curNode->left_;
		
		auto it = find(waitForDelete.begin(), waitForDelete.end(), table_element->left_);
		if (it != waitForDelete.end()) {
			if (curNode->right_ == nullptr) {
				deep_delete(preNode->right_);
				preNode->right_ = nullptr;
				preNode->op_->middle_ = TERMINAL::tEmpty;

				curNode = preNode;

				waitForDelete.erase(it);
			}
			else {
				IR* tmp = curNode->right_;

				deep_delete(curNode->left_);
				curNode->left_ = tmp->left_;
				curNode->right_ = tmp->right_;
				curNode->op_->middle_ = tmp->op_->middle_;

				tmp->left_ = nullptr;
				tmp->right_ = nullptr;
				deep_delete(tmp);

				waitForDelete.erase(it);

				continue;
			}			
		}

		preNode = curNode;
		curNode = curNode->right_;
	}

	assert(waitForDelete.empty());

	return;
}
pair<Column*, vector<IR*>> Instantiator::instantiate_column_def(IR* column_def, Table* table) {
	vector<IR*> res;

	IR* column_name = NT_check(column_def->left_, IRTYPE::kColumnName);
	IR* data_type = NT_check(column_def->right_->left_, IRTYPE::kDataType);
	IR* opt_column_attribute_list = NT_check(column_def->right_->right_, IRTYPE::kOptColumnAttributeList);

	name_t columnName = table->get_acce_column_name();
	ColumnType type = instantiate_data_type(data_type);

	Column* column = new Column(columnName, type);
	instantiate_ident(column_name, IdentifierType::iColumn, column->identifierName_);

	if (opt_column_attribute_list->left_ != nullptr) {
		IR* column_attribute_list = NT_check(opt_column_attribute_list->left_, IRTYPE::kColumnAttributeList);

		while (column_attribute_list != nullptr) {
			res.push_back(NT_check(column_attribute_list->left_, IRTYPE::kColumnAttribute));
			column_attribute_list = column_attribute_list->right_;
		}
	}

	table->add_column(column);

	return { column, res };
}
ColumnType Instantiator::instantiate_data_type(IR* data_type) {
	ColumnType res = ColumnType::ctNull;

	switch (data_type->left_->type_) {
		case IRTYPE::kNumberType: {
			IR* number_type = NT_check(data_type->left_, IRTYPE::kNumberType);

			IR* type = number_type->left_;
			IR* precision = nullptr;
			IR* float_options = nullptr;

			if (type->type_ == IRTYPE::kRealType) {
				IR* opt_precision = NT_check(number_type->right_->left_, IRTYPE::kOptPrecision);
				precision = (opt_precision->left_ == nullptr) ? nullptr : NT_check(opt_precision->left_, IRTYPE::kPrecision);
			}
			else if (type->type_ == IRTYPE::kNumericType) {
				float_options = NT_check(number_type->right_->left_, IRTYPE::kFloatOptions);
			}

			res = Column::typeTranslation_[type->op_->prefix_];

			if (float_options != nullptr && float_options->left_ != nullptr) {
				if (float_options->left_->type_ == IRTYPE::kPrecision) {
					precision = NT_check(float_options->left_, IRTYPE::kPrecision);
				}
				else {
					IR* field_length = NT_check(float_options->left_, IRTYPE::kFieldLength);
					IR* iconst = NT_check(field_length->left_, IRTYPE::kIconst);

					int v1 = exprInstantiator_->get_random_integer(0, 53);
					iconst->long_val_ = v1;
				}
			}

			if (precision != nullptr) {
				IR* iconst_1 = NT_check(precision->left_, IRTYPE::kIconst);
				IR* iconst_2 = NT_check(precision->right_, IRTYPE::kIconst);

				int v1 = exprInstantiator_->get_random_integer(1, 255);
				int v2 = exprInstantiator_->get_random_float(0, min(v1, 30));

				iconst_1->long_val_ = v1;
				iconst_2->long_val_ = v2;
			}

			break;
		}
		case IRTYPE::kBoolType: {
			res = Column::typeTranslation_[data_type->left_->op_->prefix_];
			break;
		}
		case IRTYPE::kStringType: {
			IR* string_type = NT_check(data_type->left_, IRTYPE::kStringType);

			IR* field_length = nullptr;
			if (string_type->left_ != nullptr) {
				field_length = (string_type->left_->type_ == IRTYPE::kFieldLength) ? NT_check(string_type->left_, IRTYPE::kFieldLength) : string_type->left_->left_;
			}

			res = Column::typeTranslation_[string_type->op_->prefix_];

			if (field_length != nullptr) {
				IR* iconst = NT_check(field_length->left_, IRTYPE::kIconst);

				int v1 = exprInstantiator_->get_random_integer(32, 255);
				iconst->long_val_ = v1;
			}

			break;
		}
		default: {
			fprintf(stderr, "Unknown Data Type!!!\n");
			assert(0);
		}
	}

	return res;
}
void Instantiator::instantiate_column_attribute(IR* column_attribute, Column* column, Table* table) {
	switch (column_attribute->left_->type_) {
		case IRTYPE::kNotNull: {
			column->notNull_ = true;
			break;
		}
		case IRTYPE::kDefaultAttribute: {
			IR* expr_root = NT_check(column_attribute->left_->left_, IRTYPE::kExprRoot);
			exprInstantiator_->instantiate_expr_root(expr_root, table->columns_, StmtLocation::slNotSubquery);

			column->hasDefault_ = true;

			break;
		}
		case IRTYPE::kVisibility: {
			column->isVisible_ = (column_attribute->left_->op_->prefix_ == TERMINAL::tVisible) ? true : false;
			break;
		}
		default: {
			fprintf(stderr, "Unknown Column Attribute!!!\n");
			assert(0);
		}
	}

	return;
}
map<Column*, IndexType> Instantiator::instantiate_key_list_with_expression(IR* key_list_with_expression, Table* table, ConstraintType type) {
	map<Column*, IndexType> res;

	IR* curNode = key_list_with_expression;
	while (curNode != nullptr) {
		IR* key_part_with_expression = NT_check(curNode->left_, IRTYPE::kKeyPartWithExpression);

		if (type == ConstraintType::ctPrimaryKey && key_part_with_expression->left_->type_ != IRTYPE::kKeyPart) {
			deep_delete(key_part_with_expression->left_);
			key_part_with_expression->left_ = deep_copy(IRTrim_->simple_key_part_);
		}

		if (key_part_with_expression->left_->type_ == IRTYPE::kKeyPart) {
			IR* key_part = NT_check(key_part_with_expression->left_, IRTYPE::kKeyPart);
			IR* iconst = nullptr;

			Column* columnChosen = table->get_column();
			instantiate_ident(NT_check(key_part->left_, IRTYPE::kColumnName), IdentifierType::iColumn, columnChosen->identifierName_);

			if (res.find(columnChosen) == res.end()) {
				res[columnChosen] = IndexType::itNormal;
			}			

			if (key_part->right_->type_ != IRTYPE::kOptOrderingDirection) {
				if (Column::typeToBasicType_[columnChosen->columnType_] != ColumnBasicType::ctString) {
					deep_delete(key_part->right_);
					key_part->right_ = new IR(IRTYPE::kOptOrderingDirection, OP0(), nullptr, nullptr);
				}
				else {
					iconst = NT_check(key_part->right_->left_, IRTYPE::kIconst);
				}
			}

			if (iconst != nullptr) {
				int v1 = exprInstantiator_->get_random_integer(1, 32);
				iconst->long_val_ = v1;
			}
		}
		else {
			IR* expr_root = NT_check(key_part_with_expression->left_, IRTYPE::kExprRoot);
			exprInstantiator_->instantiate_expr_root(expr_root, table->columns_, StmtLocation::slNotSubquery);

			for (Column* column : exprInstantiator_->usedColumnsBuffer_) {
				res[column] = IndexType::itFunctional;
			}
		}

		curNode = curNode->right_;
	}

	return res;
}
bool Instantiator::instantiate_table_constraint_def_index(IR* table_constraint_def, Table* table) {
	IR* opt_index_name = NT_check(table_constraint_def->left_, IRTYPE::kOptIndexName);
	IR* key_list_with_expression = NT_check(table_constraint_def->right_, IRTYPE::kKeyListWithExpression);

	if (opt_index_name->left_ == nullptr) {
		opt_index_name->left_ = deep_copy(IRTrim_->simple_index_name_);
	}

	name_t newIndexName = globalStatusManger_->get_new_index_name();
	Index* newIndex = new Index(newIndexName);
	instantiate_ident(NT_check(opt_index_name->left_, IRTYPE::kIndexName), IdentifierType::iIndex, newIndex->identifierName_);

	newIndex->columns_ = instantiate_key_list_with_expression(key_list_with_expression, table, ConstraintType::ctNull);

	table->add_index(newIndex);

	return true;
}
bool Instantiator::instantiate_table_constraint_def_key(IR* table_constraint_def, Table* table) {
	ConstraintType type = ConstraintType::ctNull;

	IR* opt_constraint_name = nullptr;
	IR* constraint_key_type = nullptr;
	IR* opt_index_name = nullptr;
	IR* key_list_with_expression = nullptr;

	IR* unknown = nullptr;

	opt_constraint_name = NT_check(table_constraint_def->left_, IRTYPE::kOptConstraintName);
	unknown = table_constraint_def->right_;

	constraint_key_type = NT_check(unknown->left_, IRTYPE::kConstraintKeyType);
	unknown = unknown->right_;

	opt_index_name = NT_check(unknown->left_, IRTYPE::kOptIndexName);
	key_list_with_expression = NT_check(unknown->right_, IRTYPE::kKeyListWithExpression);

	if (opt_constraint_name->left_ == nullptr) {
		opt_constraint_name->left_ = deep_copy(IRTrim_->simple_constraint_name_);
		opt_constraint_name->op_->prefix_ = TERMINAL::tConstraint;
	}

	if (opt_index_name->left_ == nullptr) {
		opt_index_name->left_ = deep_copy(IRTrim_->simple_index_name_);
	}

	if (table->hasPrimaryKey_ == true) {
		if (constraint_key_type->op_->prefix_ == TERMINAL::tPrimaryKey); {
			return false;
		}

		type = ConstraintType::ctUnique;
	}
	else {
		type = (constraint_key_type->op_->prefix_ == TERMINAL::tPrimaryKey) ? ConstraintType::ctPrimaryKey : ConstraintType::ctUnique;
		if (type == ConstraintType::ctPrimaryKey) {
			table->hasPrimaryKey_ = true;
		}
	}

	name_t newConstraintName = globalStatusManger_->get_new_constraint_name();
	name_t newIndexName = globalStatusManger_->get_new_index_name();

	Constraint* newConstraint = new Constraint(newConstraintName, type);
	Index* newIndex = new Index(newIndexName);

	instantiate_ident(NT_check(opt_constraint_name->left_, IRTYPE::kConstraintName), IdentifierType::iConstraint, newConstraint->identifierName_);
	instantiate_ident(NT_check(opt_index_name->left_, IRTYPE::kIndexName), IdentifierType::iIndex, newIndex->identifierName_);

	newConstraint->columns_ = instantiate_key_list_with_expression(key_list_with_expression, table, type);
	newIndex->columns_ = newConstraint->columns_;

	table->add_constraint(newConstraint, newIndex);

	return true;
}
bool Instantiator::instantiate_table_constraint_def_check(IR* table_constraint_def, Table* table) {
	IR* opt_constraint_name = NT_check(table_constraint_def->left_, IRTYPE::kOptConstraintName);
	IR* check_constraint = NT_check(table_constraint_def->right_, IRTYPE::kCheckConstraint);

	if (opt_constraint_name->left_ == nullptr) {
		opt_constraint_name->left_ = deep_copy(IRTrim_->simple_constraint_name_);
		opt_constraint_name->op_->prefix_ = TERMINAL::tConstraint;
	}

	name_t newConstraintName = globalStatusManger_->get_new_constraint_name();
	Constraint* newConstraint = new Constraint(newConstraintName, ConstraintType::ctCheck);
	instantiate_ident(NT_check(opt_constraint_name->left_, IRTYPE::kConstraintName), IdentifierType::iConstraint, newConstraint->identifierName_);

	exprInstantiator_->instantiate_expr_root(NT_check(check_constraint->left_, IRTYPE::kExprRoot), table->columns_, StmtLocation::slNotSubquery);
	for (Column* column : exprInstantiator_->usedColumnsBuffer_) {
		newConstraint->columns_[column] = IndexType::itFunctional;
	}

	table->add_constraint(newConstraint);

	return true;
}
bool Instantiator::instantiate_table_constraint_def_foreign_key(IR* table_constraint_def, Table* table) {
	IR* opt_constraint_name = nullptr;
	IR* opt_index_name = nullptr;
	IR* key_list = nullptr;
	IR* references = nullptr;
	IR* table_name = nullptr;
	IR* column_list = nullptr;

	IR* unknown = nullptr;

	opt_constraint_name = NT_check(table_constraint_def->left_, IRTYPE::kOptConstraintName);
	unknown = table_constraint_def->right_;

	opt_index_name = NT_check(unknown->left_, IRTYPE::kOptIndexName);
	unknown = unknown->right_;

	key_list = NT_check(unknown->left_, IRTYPE::kKeyList);
	references = NT_check(unknown->right_, IRTYPE::kReferences);

	table_name = NT_check(references->left_, IRTYPE::kTableName);
	column_list = NT_check(references->right_->left_, IRTYPE::kColumnList);

	vector<Column*> curTableColumn;
	vector<Column*> otherTableColumn;
	if (check_foreign_key(table, curTableColumn, otherTableColumn) == false) {
		return false;
	}

	assert(!curTableColumn.empty() && curTableColumn.size() == otherTableColumn.size());

	if (opt_constraint_name->left_ == nullptr) {
		opt_constraint_name->left_ = deep_copy(IRTrim_->simple_constraint_name_);
		opt_constraint_name->op_->prefix_ = TERMINAL::tConstraint;
	}

	if (opt_index_name->left_ == nullptr) {
		opt_index_name->left_ = deep_copy(IRTrim_->simple_index_name_);
	}

	name_t newConstraintName = globalStatusManger_->get_new_constraint_name();
	name_t newIndexName = globalStatusManger_->get_new_index_name();

	Constraint* newConstraint = new Constraint(newConstraintName, ConstraintType::ctForeignKey);

	IR* constraint_name = NT_check(opt_constraint_name->left_, IRTYPE::kConstraintName);
	IR* index_name = NT_check(opt_index_name->left_, IRTYPE::kIndexName);

	instantiate_ident(constraint_name, IdentifierType::iConstraint, newConstraint->identifierName_);
	instantiate_ident(index_name, IdentifierType::iIndex, newIndexName);

	instantiate_ident(table_name, IdentifierType::iTable, otherTableColumn[0]->parentTable_->identifierName_);

	instantiate_key_list(key_list, curTableColumn);
	instantiate_column_list(column_list, otherTableColumn);

	for (int i = 0; i < curTableColumn.size(); i++) {
		newConstraint->refToOtherColumns_[curTableColumn[i]].push_back(otherTableColumn[i]);
	}

	table->add_constraint(newConstraint);

	return true;
}

void Instantiator::instantiate_select_stmt(IR* select_stmt) {
	globalStatusManger_->push_selectInfo(StmtLocation::slNotSubquery, DEFAULTROWNUMBER, DEFAULTCOLUMNNUMBER);

	Table* t = instantiate_query_expression(NT_check(select_stmt->left_, IRTYPE::kQueryExpression));

	globalStatusManger_->pop_selectInfo();
	delete t;

	return;
}
Table* Instantiator::instantiate_query_expression(IR* query_expression, StmtLocation location, int rowNumber, int columnNumber) {
	globalStatusManger_->push_selectInfo(location, rowNumber, columnNumber);

	Table* res = instantiate_query_expression(NT_check(query_expression, IRTYPE::kQueryExpression));
	globalStatusManger_->pop_selectInfo();

	res->inSelectStmtInfo_ = globalStatusManger_->get_cur_selectInfo();

	return res;
}
Table* Instantiator::instantiate_query_expression(IR* query_expression) {
	Table* res = nullptr;

	IR* with_clause = nullptr;
	IR* query_expression_body = nullptr;
	IR* opt_order_clause = nullptr;
	IR* opt_limit_clause = nullptr;

	IR* unknown = nullptr;

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	if (query_expression->left_->type_ == IRTYPE::kQueryExpressionBody) {
		query_expression_body = NT_check(query_expression->left_, IRTYPE::kQueryExpressionBody);
		unknown = query_expression->right_;
	}
	else {
		with_clause = NT_check(query_expression->left_, IRTYPE::kWithClause);

		unknown = query_expression->right_;
		query_expression_body = NT_check(unknown->left_, IRTYPE::kQueryExpressionBody);
	
		unknown = unknown->right_;
	}

	opt_order_clause = NT_check(unknown->left_, IRTYPE::kOptOrderClause);
	opt_limit_clause = NT_check(unknown->right_, IRTYPE::kOptLimitClause);

	if (with_clause != nullptr) {
		instantiate_with_clause(with_clause);
	}

	switch (query_expression_body->left_->type_) {
		case IRTYPE::kQueryPrimary: {
			res = instantiate_query_primary(NT_check(query_expression_body->left_, IRTYPE::kQueryPrimary));
			break;
		}
		case IRTYPE::kQueryExpressionParens: {
			IR* query_expression_parens = NT_check(query_expression_body->left_, IRTYPE::kQueryExpressionParens);
			while (query_expression_parens->left_->type_ == IRTYPE::kQueryExpressionParens) {
				query_expression_parens = NT_check(query_expression_parens->left_, IRTYPE::kQueryExpressionParens);
			}

			res = instantiate_query_expression(NT_check(query_expression_parens->left_, IRTYPE::kQueryExpression));

			break;
		}
		case IRTYPE::kQueryExpressionBody: {
			res = instanitate_union(query_expression_body);
			break;
		}
		default: {
			fprintf(stderr, "ERROR: Unknown query_expression_body->left_->type_: %d\n", static_cast<int>(query_expression_body->left_->type_));
			assert(0);
		}
	}

	assert(info->usedTables_.empty());

	if (opt_order_clause->left_ != nullptr) {
		IR* order_clause = NT_check(opt_order_clause->left_, IRTYPE::kOrderClause);

		vector<Table*> usedTables = { res };
		vector<int> duplicationInfo = vector<int>(COLUMNNAMEMAX, 0);
		info->update_acceColumns(usedTables, duplicationInfo);

		instantiate_order_clause(order_clause);

		info->acceColumns_.clear();
	}

	if (info->has_row_limit() && opt_limit_clause->left_ == nullptr) {
		opt_limit_clause->left_ = deep_copy(IRTrim_->simple_limit_);
	}
	
	if (opt_limit_clause->left_ != nullptr) {
		IR* limit_clause = NT_check(opt_limit_clause->left_, IRTYPE::kLimitClause);
		instantiate_limit_clause(limit_clause);
	}

	return res;
}
Table* Instantiator::instantiate_query_primary(IR* query_primary) {
	Table* res = nullptr;

	IR* query_specification = nullptr;

	IR* opt_select_options = nullptr;
	IR* select_item_list = nullptr;
	IR* opt_from_clause = nullptr;
	IR* opt_where_clause = nullptr;
	IR* opt_group_clause = nullptr;
	IR* opt_having_clause = nullptr;
	IR* opt_window_clause = nullptr;

	IR* unknown = nullptr;

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();
	info->clear_local_info();

	query_specification = NT_check(query_primary->left_, IRTYPE::kQuerySpecification);

	opt_select_options = NT_check(query_specification->left_, IRTYPE::kOptSelectOptions);
	unknown = query_specification->right_;

	select_item_list = NT_check(unknown->left_, IRTYPE::kSelectItemList);
	unknown = unknown->right_;

	opt_from_clause = NT_check(unknown->left_, IRTYPE::kOptFromClause);
	unknown = unknown->right_;

	opt_where_clause = NT_check(unknown->left_, IRTYPE::kOptWhereClause);
	unknown = unknown->right_;

	opt_group_clause = NT_check(unknown->left_, IRTYPE::kOptGroupClause);
	unknown = unknown->right_;

	opt_having_clause = NT_check(unknown->left_, IRTYPE::kOptHavingClause);
	opt_window_clause = NT_check(unknown->right_, IRTYPE::kOptWindowClause);

	if (opt_from_clause->left_) {
		int acceTableNumber = globalStatusManger_->get_acce_table_number();

		if (acceTableNumber == 0) {
			deep_delete(opt_from_clause->left_);
			opt_from_clause->left_ = nullptr;
		}
		else {
			IR* from_clause = NT_check(opt_from_clause->left_, IRTYPE::kFromClause);
			instantiate_from_clause(from_clause);
		}		
	}

	info->update_acceColumns();

	if (opt_where_clause->left_ != nullptr) {
		IR* where_clause = NT_check(opt_where_clause->left_, IRTYPE::kWhereClause);
		instantiate_where_clause(where_clause);
	}

	if (opt_having_clause->left_ != nullptr) {
		instantiate_having_clause(opt_having_clause);
	}

	if (opt_window_clause->left_ != nullptr) {
		instantiate_window_clause(opt_window_clause);
	}

	res = instantiate_select_item_list(select_item_list);

	if (info->hasAggregate && !info->usedInnerColumns_.empty() && opt_group_clause->left_ == nullptr) {
		IR* olap_opt = new IR(IRTYPE::kOlapOpt, OP0(), nullptr, nullptr);
		IR* expr_root_list = deep_copy(IRTrim_->simple_expr_root_list_);

		opt_group_clause->left_ = expr_root_list;
		opt_group_clause->right_ = olap_opt;
		opt_group_clause->op_->prefix_ = TERMINAL::tGroupBy;
	}

	if (opt_group_clause->left_ != nullptr) {
		instantiate_group_clause(opt_group_clause);
	}

	info->clear_local_info();

	return res;
}
Table* Instantiator::instanitate_union(IR* query_expression_body) {
	Table* res = nullptr;

	IR* query_expression_body_1 = nullptr;
	IR* query_expression_body_2 = nullptr;

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();
	StmtLocation oldLocation = info->location_;

	stack<IR*> stk;
	vector<IR*> queue;

	query_expression_body_1 = NT_check(query_expression_body->left_, IRTYPE::kQueryExpressionBody);
	query_expression_body_2 = NT_check(query_expression_body->right_->right_, IRTYPE::kQueryExpressionBody);

	stk.push(query_expression_body_2);
	stk.push(query_expression_body_1);

	while (!stk.empty()) {
		IR* curIR = stk.top();
		stk.pop();

		switch (curIR->left_->type_) {
			case IRTYPE::kQueryPrimary: {
				IR* query_primary = NT_check(curIR->left_, IRTYPE::kQueryPrimary);
				queue.push_back(query_primary);

				break;
			}
			case IRTYPE::kQueryExpressionParens: {
				IR* query_expression_parens = NT_check(curIR->left_, IRTYPE::kQueryExpressionParens);
				queue.push_back(query_expression_parens);

				break;
			}
			case IRTYPE::kQueryExpressionBody: {
				IR* t_query_expression_body_1 = NT_check(curIR->left_, IRTYPE::kQueryExpressionBody);
				IR* t_query_expression_body_2 = NT_check(curIR->right_->right_, IRTYPE::kQueryExpressionBody);

				stk.push(t_query_expression_body_2);
				stk.push(t_query_expression_body_1);

				break;
			}
			default: {
				fprintf(stderr, "ERROR: Unknown query_expression_body->left_->type_: %d\n", static_cast<int>(curIR->left_->type_));
				assert(0);
			}
		}
	}

	assert(queue.size() >= 2);

	for (int i = 0; i < queue.size(); i++) {
		if (i == 0 && (oldLocation == StmtLocation::slRecursiveCTE || oldLocation == StmtLocation::slRecursivePart1)) {
			info->location_ = StmtLocation::slRecursivePart1;
		}
		else if (i == queue.size() - 1 && (oldLocation == StmtLocation::slRecursiveCTE || oldLocation == StmtLocation::slRecursivePart2)) {
			info->location_ = StmtLocation::slRecursivePart2;
		}
		else if(oldLocation == StmtLocation::slRecursiveCTE || oldLocation == StmtLocation::slRecursivePart1 || oldLocation == StmtLocation::slRecursivePart2) {
			info->location_ = StmtLocation::slCTE;
		}

		IR* curIR = queue[i];
		Table* tmp = nullptr;

		switch (curIR->type_) {
			case IRTYPE::kQueryPrimary: {
				tmp = instantiate_query_primary(NT_check(curIR, IRTYPE::kQueryPrimary));
				break;
			}
			case IRTYPE::kQueryExpressionParens: {
				IR* query_expression_parens = NT_check(curIR, IRTYPE::kQueryExpressionParens);
				while (query_expression_parens->left_->type_ == IRTYPE::kQueryExpressionParens) {
					query_expression_parens = NT_check(query_expression_parens->left_, IRTYPE::kQueryExpressionParens);
				}

				tmp = instantiate_query_expression(NT_check(query_expression_parens->left_, IRTYPE::kQueryExpression));

				break;
			}
			default: {
				fprintf(stderr, "ERROR: Unknown curIR->type_: %d\n", static_cast<int>(curIR->type_));
				assert(0);
			}
		}

		if (i == 0) {
			res = tmp;

			if (curIR->type_ == IRTYPE::kQueryPrimary) {
				if (!info->has_column_limit()) {
					info->columnNumber_ = res->columns_.size();
				}

				if (info->recursiveCteName_ != UNREFERENCE && info->recursiveCte_ == nullptr) {
					res->identifierName_ = info->recursiveCteName_;
					info->recursiveCte_ = res;
				}				
			}
		}
		else {
			delete tmp;
		}
	}

	info->location_ = oldLocation;

	return res;
}

void Instantiator::instantiate_with_clause(IR* with_clause) {
	StmtLocation location = StmtLocation::slCTE;

	IR* with_list = NT_check(with_clause->left_, IRTYPE::kWithList);

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();
	info->clear_cte();

	if (with_clause->op_->prefix_ == TERMINAL::tWithRecursive) {
		location = StmtLocation::slRecursiveCTE;
	}

	while (with_list != nullptr) {
		IR* common_table_expr = NT_check(with_list->left_, IRTYPE::kCommonTableExpr);

		IR* table_name = NT_check(common_table_expr->left_, IRTYPE::kTableName);
		IR* opt_column_list = NT_check(common_table_expr->right_->left_, IRTYPE::kOptColumnList);
		IR* subquery = NT_check(common_table_expr->right_->right_, IRTYPE::kSubquery);

		name_t tableName = globalStatusManger_->get_new_table_name();
		if (location == StmtLocation::slRecursiveCTE) {
			info->recursiveCteName_ = tableName;
		}

		info->update_acceColumns();

		IR* curNode = subquery->left_;
		while (curNode->type_ != IRTYPE::kQueryExpression) {
			curNode = curNode->left_;
		}

		Table* table = instantiate_query_expression(curNode, location, DEFAULTROWNUMBER, DEFAULTCOLUMNNUMBER);
		table->isSubQuery_ = true;
		table->identifierName_ = tableName;

		instantiate_ident(table_name, IdentifierType::iTable, tableName);

		if (opt_column_list->left_) {
			IR* column_list = NT_check(opt_column_list->left_, IRTYPE::kColumnList);
			instantiate_column_list(column_list, table->columns_);
		}

		info->innerCteTables_.push_back(table);

		info->recursiveCteName_ = UNREFERENCE;
		info->acceColumns_.clear();

		with_list = with_list->right_;
	}

	return;
}
void Instantiator::instantiate_order_clause(IR* order_clause) {
	IR* order_list = NT_check(order_clause->left_, IRTYPE::kOrderList);

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	while (order_list != nullptr) {
		IR* expr_root = NT_check(order_list->left_->left_, IRTYPE::kExprRoot);

		exprInstantiator_->instantiate_expr_root(expr_root, info->acceColumns_, StmtLocation::slOrderClause);	
		info->add_out_usedColumns(exprInstantiator_->usedColumnsBuffer_);

		order_list = order_list->right_;
	}

	return;
}
void Instantiator::instantiate_limit_clause(IR* limit_clause) {
	IR* limit_options = NT_check(limit_clause->left_, IRTYPE::kLimitOptions);
	IR* iconst_1 = NT_check(limit_options->left_, IRTYPE::kIconst);
	IR* iconst_2 = (limit_options->right_ == nullptr) ? nullptr : NT_check(limit_options->right_, IRTYPE::kIconst);

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	if (info->has_row_limit()) {
		iconst_1->long_val_ = exprInstantiator_->get_random_integer(info->rowNumber_, info->rowNumber_);
		info->rowNumber_ = DEFAULTROWNUMBER;
	}
	else {
		iconst_1->long_val_ = exprInstantiator_->get_random_integer(LIMITMIN, LIMITMAX);
	}
	
	if (iconst_2 != nullptr) {
		iconst_2->long_val_ = exprInstantiator_->get_random_integer(OFFSETMIN, OFFSETMAX);
	}

	return;
}

Table* Instantiator::instantiate_select_item_list(IR* select_item_list) {
	IR* t_select_item_list = select_item_list;

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();
	Table* res = new Table(UNREFERENCE, IdentifierType::iTable, info);

	int columnCount = 0;
	while (t_select_item_list != nullptr) {
		IR* select_item = NT_check(t_select_item_list->left_, IRTYPE::kSelectItem);

		if (info->location_ == StmtLocation::slRecursivePart1 && columnCount == 0) {
			deep_delete(select_item->left_);
			deep_delete(select_item->right_);

			select_item->left_ = deep_copy(IRTrim_->simple_expr_root_);
			select_item->right_ = new IR(IRTYPE::kOptAlias, OP3(TERMINAL::tAs, TERMINAL::tEmpty, TERMINAL::tEmpty), deep_copy(IRTrim_->simple_alias_), nullptr);

			IR* alias = NT_check(select_item->right_->left_, IRTYPE::kAlias);
			instantiate_ident(alias, IdentifierType::iColumn, RESERVEDCOLUMNNAME);

			bool tmp = res->add_column(new Column(RESERVEDCOLUMNNAME, ColumnType::ctInt));
			assert(tmp);

			columnCount++;
		}
		else {
			if (select_item->left_->type_ == IRTYPE::kTableWild) {
				bool success = false;

				if (!info->usedTables_.empty()) {
					IR* table_wild = NT_check(select_item->left_, IRTYPE::kTableWild);
					IR* table_name = nullptr;

					if (table_wild->left_ != nullptr) {
						table_name = NT_check(table_wild->left_, IRTYPE::kTableName);
					}

					if (table_name != nullptr) {
						int index = exprInstantiator_->get_random_integer(0, info->usedTables_.size() - 1);
						Table* table = info->usedTables_[index];

						if (!info->exist_duplicated_column(res->columns_, table->columns_) && (!info->has_column_limit() || (columnCount + table->columns_.size()) <= info->columnNumber_)) {
							instantiate_ident(table_name, IdentifierType::iTable, table->identifierName_);

							for (Column* c : table->columns_) {
								bool tmp = res->add_column(new Column(c));
								assert(tmp);
							}

							columnCount += table->columns_.size();
							success = true;
						}
					}
					else {
						vector<Column*> columns;

						for (Table* t : info->usedTables_) {
							for (Column* c : t->columns_) {
								columns.push_back(c);
							}
						}

						if (!info->exist_duplicated_column(res->columns_, columns) && (!info->has_column_limit() || (columnCount + columns.size()) <= info->columnNumber_)) {
							for (Column* c : columns) {
								bool tmp = res->add_column(new Column(c));
								assert(tmp);
							}

							columnCount += columns.size();
							success = true;
						}
					}
				}

				if (!success) {
					deep_delete(select_item->left_);

					select_item->left_ = deep_copy(IRTrim_->simple_expr_root_);
					select_item->right_ = new IR(IRTYPE::kOptAlias, OP3(TERMINAL::tAs, TERMINAL::tEmpty, TERMINAL::tEmpty), deep_copy(IRTrim_->simple_alias_), nullptr);
				}
			}

			if (select_item->left_->type_ == IRTYPE::kExprRoot) {
				IR* expr_root = NT_check(select_item->left_, IRTYPE::kExprRoot);
				IR* opt_alias = NT_check(select_item->right_, IRTYPE::kOptAlias);

				if (opt_alias->left_ == nullptr) {
					opt_alias->left_ = deep_copy(IRTrim_->simple_alias_);
					opt_alias->op_->prefix_ = TERMINAL::tAs;
				}

				IR* alias = NT_check(opt_alias->left_, IRTYPE::kAlias);

				name_t newColumnName = res->get_acce_column_name();
				instantiate_ident(alias, IdentifierType::iColumn, newColumnName);

				exprInstantiator_->instantiate_expr_root(expr_root, info->acceColumns_, StmtLocation::slTargetList);
				info->add_all_usedColumns(exprInstantiator_->usedColumnsBuffer_);

				bool tmp = res->add_column(new Column(newColumnName, ColumnType::ctNull));
				assert(tmp);

				columnCount++;
			}
		}		

		if (info->has_column_limit()) {
			assert(info->columnNumber_ > 0);

			if (columnCount >= info->columnNumber_ && t_select_item_list->right_ != nullptr) {
				deep_delete(t_select_item_list->right_);

				t_select_item_list->right_ = nullptr;
				t_select_item_list->op_->middle_ = TERMINAL::tEmpty;
			}
			else if (columnCount < info->columnNumber_ && t_select_item_list->right_ == nullptr) {
				t_select_item_list->right_ = deep_copy(IRTrim_->simple_select_item_list_);
				t_select_item_list->op_->middle_ = TERMINAL::tOpComma;
			}
		}		

		t_select_item_list = t_select_item_list->right_;
	}

	assert(res->columns_.size() == columnCount);

	return res;
}
void Instantiator::instantiate_from_clause(IR* from_clause) {
	IR* table_reference_list = NT_check(from_clause->left_, IRTYPE::kTableReferenceList);

	while (table_reference_list != nullptr) {
		IR* table_reference = NT_check(table_reference_list->left_, IRTYPE::kTableReference);
		instantiate_table_reference(table_reference);

		table_reference_list = table_reference_list->right_;
	}

	return;
}
vector<Table*> Instantiator::instantiate_table_reference(IR* table_reference) {
	vector<Table*> res;
	vector<IR*> queue;

	if (table_reference->left_->type_ == IRTYPE::kJoinedTable) {
		IR* joined_table = NT_check(table_reference->left_, IRTYPE::kJoinedTable);
		queue.push_back(joined_table);
	}
	else {
		IR* table_factor = NT_check(table_reference->left_, IRTYPE::kTableFactor);

		switch (table_factor->left_->type_) {
			case IRTYPE::kSingleTable: {
				IR* single_table = NT_check(table_factor->left_, IRTYPE::kSingleTable);
				queue.push_back(single_table);

				break;
			}
			case IRTYPE::kDerivedTable: {
				IR* derived_table = NT_check(table_factor->left_, IRTYPE::kDerivedTable);
				queue.push_back(derived_table);

				break;
			}
			case IRTYPE::kJoinedTableParens: {
				IR* joined_table = NT_check(table_factor->left_->left_, IRTYPE::kJoinedTable);
				queue.push_back(joined_table);

				break;
			}
			case IRTYPE::kTableReferenceListParens: {
				IR* table_reference_list_parens = NT_check(table_factor->left_, IRTYPE::kTableReferenceListParens);

				while (table_reference_list_parens != nullptr) {
					IR* t_table_reference = NT_check(table_reference_list_parens->left_, IRTYPE::kTableReference);
					queue.push_back(t_table_reference);

					table_reference_list_parens = table_reference_list_parens->right_;
				}

				break;
			}
			default: {
				fprintf(stderr, "ERROR: Unknown table_factor->left_->type_: %d\n", static_cast<int>(table_factor->left_->type_));
				assert(0);
			}
		}
	}

	assert(!queue.empty());
	for (int i = 0; i < queue.size(); i++) {
		IR* curIR = queue[i];

		switch (curIR->type_) {
			case IRTYPE::kSingleTable: {
				Table* table = instantiate_single_table(curIR);
				res.push_back(table);

				break;
			}
			case IRTYPE::kDerivedTable: {
				Table* table = instantiate_derived_table(curIR);
				res.push_back(table);

				break;
			}
			case IRTYPE::kJoinedTable: {
				vector<Table*> tables = instantiate_joined_table(curIR);

				for (Table* table : tables) {
					res.push_back(table);
				}

				break;
			}
			case IRTYPE::kTableReference: {
				vector<Table*> tables = instantiate_table_reference(curIR);

				for (Table* table : tables) {
					res.push_back(table);
				}

				break;
			}
			default: {
				fprintf(stderr, "ERROR: Unknown curIR->type_: %d\n", static_cast<int>(curIR->type_));
				assert(0);
			}
		}
	}

	return res;
}
Table* Instantiator::instantiate_single_table(IR* single_table) {
	Table* res = nullptr;
	Table* chosenTable = nullptr;

	IR* table_name = NT_check(single_table->left_, IRTYPE::kTableName);
	IR* opt_alias = NT_check(single_table->right_, IRTYPE::kOptAlias);

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	if (info->location_ == StmtLocation::slRecursivePart2 && info->recursiveCteName_ != UNREFERENCE && info->recursiveCte_ != nullptr) {
		chosenTable = info->recursiveCte_;

		info->recursiveCteName_ = UNREFERENCE;
		info->recursiveCte_ = nullptr;
	}
	else {
		int index = globalStatusManger_->get_acce_table_number();
		assert(index > 0);

		int randomNumber = exprInstantiator_->get_random_integer(0, index - 1);
		chosenTable = globalStatusManger_->get_acce_table(randomNumber);
	}

	res = new Table(chosenTable, info);
	instantiate_ident(table_name, res->identifierType_, res->identifierName_);

	if (info->is_duplicated_table(res) && opt_alias->left_ == nullptr) {
		opt_alias->left_ = deep_copy(IRTrim_->simple_alias_);
		opt_alias->op_->prefix_ = TERMINAL::tAs;
	}

	if (opt_alias->left_ != nullptr) {
		name_t newName = globalStatusManger_->get_new_table_name();
		IR* alias = NT_check(opt_alias->left_, IRTYPE::kAlias);

		instantiate_ident(alias, IdentifierType::iTable, newName);

		res->identifierName_ = newName;
	}

	info->add_used_table(res);

	return res;
}
Table* Instantiator::instantiate_derived_table(IR* derived_table) {
	Table* res = nullptr;

	IR* subquery = NT_check(derived_table->left_, IRTYPE::kSubquery);
	IR* alias = NT_check(derived_table->right_->left_, IRTYPE::kAlias);
	IR* opt_column_list = NT_check(derived_table->right_->right_, IRTYPE::kOptColumnList);

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	if (derived_table->op_->prefix_ == TERMINAL::tLateral) {
		info->update_acceColumns();
	}
	else {
		info->acceColumns_ = info->outReferences_;
	}

	IR* curNode = subquery->left_;
	while (curNode->type_ != IRTYPE::kQueryExpression) {
		curNode = curNode->left_;
	}

	res = instantiate_query_expression(curNode, StmtLocation::slTableReference, DEFAULTROWNUMBER, DEFAULTCOLUMNNUMBER);
	res->isSubQuery_ = true;

	name_t newName = globalStatusManger_->get_new_table_name();
	instantiate_ident(alias, IdentifierType::iTable, newName);
	res->identifierName_ = newName;

	if (opt_column_list->left_ != nullptr) {
		IR* column_list = NT_check(opt_column_list->left_, IRTYPE::kColumnList);

		for (Column* c: res->columns_) {
			name_t name = res->get_acce_column_name();
			res->remove_column_name(name);
			
			if (name != UNREFERENCE) {
				res->add_column_name(c->identifierName_);
				c->identifierName_ = name;
			}
		}
		
		instantiate_column_list(column_list, res->columns_);
	}

	info->acceColumns_.clear();
	info->add_used_table(res);

	return res;
}
vector<Table*> Instantiator::instantiate_joined_table(IR* joined_table) {
	bool isRightJoin = false;
	vector<Table*> res;

	IR* table_reference_left = nullptr;
	IR* table_reference_right = nullptr;
	IR* table_factor = nullptr;
	IR* expr_root = nullptr;
	IR* column_list = nullptr;
	IR* joined_table_tmp = nullptr;

	IR* unknown = nullptr;

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	table_reference_left = NT_check(joined_table->left_, IRTYPE::kTableReference);
	unknown = joined_table->right_;

	if (unknown->left_->type_ == IRTYPE::kNaturalJoinType) {
		IR* natural_join_type = NT_check(unknown->left_, IRTYPE::kNaturalJoinType);

		if (natural_join_type->op_->prefix_ == TERMINAL::tNaturalRight) {
			isRightJoin = true;
		}
	}
	else if (unknown->left_->type_ == IRTYPE::kOuterJoinType) {
		IR* outer_join_type = NT_check(unknown->left_, IRTYPE::kOuterJoinType);

		if (outer_join_type->op_->prefix_ == TERMINAL::tRight) {
			isRightJoin = true;
		}
	}

	if (unknown->right_->type_ == IRTYPE::kTableReference) {
		table_reference_right = NT_check(unknown->right_, IRTYPE::kTableReference);
	}
	else if (unknown->right_->type_ == IRTYPE::kTableFactor) {
		table_factor = NT_check(unknown->right_, IRTYPE::kTableFactor);
	}
	else {
		joined_table_tmp = unknown->right_;

		table_reference_right = NT_check(joined_table_tmp->left_, IRTYPE::kTableReference);

		if (joined_table_tmp->right_->type_ == IRTYPE::kExprRoot) {
			expr_root = NT_check(joined_table_tmp->right_, IRTYPE::kExprRoot);
		}
		else {
			column_list = NT_check(joined_table_tmp->right_, IRTYPE::kColumnList);
		}
	}

	if (table_factor != nullptr) {
		table_reference_right = new IR(IRTYPE::kTableReference, OP0(), table_factor, nullptr);
	}

	vector<Table*> left;
	vector<Table*> right;
	if (isRightJoin) {
		right = instantiate_table_reference(table_reference_right);
		left = instantiate_table_reference(table_reference_left);
	}
	else {
		left = instantiate_table_reference(table_reference_left);
		right = instantiate_table_reference(table_reference_right);
	}

	if (table_factor != nullptr && table_reference_right->left_ == table_factor) {
		table_reference_right->left_ = nullptr;
		deep_delete(table_reference_right);
		table_reference_right = nullptr;
	}

	vector<int> columnNames = vector<int>(COLUMNNAMEMAX, 0);
	for (Table* t : left) {
		for (Column* c : t->columns_) {
			columnNames[c->identifierName_]++;
		}

		res.push_back(t);
	}

	for (Table* t : right) {
		for (Column* c : t->columns_) {
			columnNames[c->identifierName_]++;
		}

		res.push_back(t);
	}

	if (column_list != nullptr) {
		vector<name_t> duplicatedColumnNames;
		for (int i = 0; i < COLUMNNAMEMAX; i++) {
			if (columnNames[i] > 1) {
				duplicatedColumnNames.push_back(static_cast<name_t>(i));
			}
		}

		if (!duplicatedColumnNames.empty()) {
			vector<name_t> usedColumns;

			random_device rd;
			mt19937 r_eng(rd());
			uniform_int_distribution<int> dis(1, duplicatedColumnNames.size());

			int max = dis(r_eng);
			for (int i = 0; i < max; i++) {
				usedColumns.push_back(duplicatedColumnNames[i]);
			}

			instantiate_column_list(column_list, usedColumns);
		}
		else {
			deep_delete(joined_table_tmp->right_);
			joined_table_tmp->right_ = deep_copy(IRTrim_->simple_expr_root_true_);

			joined_table_tmp->op_->middle_ = TERMINAL::tOn;
			joined_table_tmp->op_->suffix_ = TERMINAL::tEmpty;

			column_list = nullptr;
		}
	}
	else if (expr_root != nullptr) {
		info->update_acceColumns(res, columnNames);

		exprInstantiator_->instantiate_expr_root(expr_root, info->acceColumns_, StmtLocation::slTableReference);
		info->add_out_usedColumns(exprInstantiator_->usedColumnsBuffer_);

		info->acceColumns_.clear();
	}

	return res;
}
void Instantiator::instantiate_where_clause(IR* where_clause) {
	IR* expr_root = NT_check(where_clause->left_, IRTYPE::kExprRoot);
	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	exprInstantiator_->instantiate_expr_root(expr_root, info->acceColumns_, StmtLocation::slWhereClause);
	info->add_out_usedColumns(exprInstantiator_->usedColumnsBuffer_);

	if (info->location_ == StmtLocation::slRecursivePart2) {
		IRTrim_->set_recursive_limit(expr_root);
	}

	return;
}
void Instantiator::instantiate_group_clause(IR* group_clause) {
	IR* expr_root_list = NT_check(group_clause->left_, IRTYPE::kExprRootList);

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	int i = 0;
	while (expr_root_list != nullptr && i < info->usedInnerColumns_.size()) {
		IR* expr_root = IRTrim_->get_expr_root_columnref(info->usedInnerColumns_[i]);

		deep_delete(expr_root_list->left_);
		expr_root_list->left_ = expr_root;

		i++;

		if (i < info->usedInnerColumns_.size() && expr_root_list->right_ == nullptr) {
			expr_root_list->right_ = deep_copy(IRTrim_->simple_expr_root_list_);
			expr_root_list->op_->middle_ = TERMINAL::tOpComma;
		}

		expr_root_list = expr_root_list->right_;
	}

	vector<Column*> usedColumns;
	while (expr_root_list != nullptr && !info->acceColumns_.empty()) {
		int index = exprInstantiator_->get_random_integer(0, info->acceColumns_.size() - 1);
		Column* randomColumn = info->acceColumns_[index];

		IR* expr_root = IRTrim_->get_expr_root_columnref(randomColumn);

		deep_delete(expr_root_list->left_);
		expr_root_list->left_ = expr_root;

		if (find(usedColumns.begin(), usedColumns.end(), randomColumn) == usedColumns.end()) {
			usedColumns.push_back(randomColumn);
		}		

		expr_root_list = expr_root_list->right_;
	}

	if (expr_root_list != nullptr) {
		deep_delete(expr_root_list->left_);
		expr_root_list->left_ = deep_copy(IRTrim_->simple_expr_root_);

		deep_delete(expr_root_list->right_);

		expr_root_list->right_ = nullptr;
		expr_root_list->op_->middle_ = TERMINAL::tEmpty;
	}

	info->add_out_usedColumns(usedColumns);

	return;
}
void Instantiator::instantiate_having_clause(IR* having_clause) {
	IR* expr_root = NT_check(having_clause->left_, IRTYPE::kExprRoot);
	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	exprInstantiator_->instantiate_expr_root(expr_root, info->acceColumns_, StmtLocation::slHavingClause);
	info->add_all_usedColumns(exprInstantiator_->usedColumnsBuffer_);

	return;
}
void Instantiator::instantiate_window_clause(IR* window_clause) {
	IR* window_definition_list = NT_check(window_clause->left_, IRTYPE::kWindowDefinitionList);

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	while (window_definition_list != nullptr) {
		IR* window_definition = NT_check(window_definition_list->left_, IRTYPE::kWindowDefinition);
		instantiate_window_definition(window_definition);

		window_definition_list = window_definition_list->right_;
	}

	return;
}
void Instantiator::instantiate_window_definition(IR* window_definition) {
	IR* window_name = NT_check(window_definition->left_, IRTYPE::kWindowName);
	IR* window_spec = NT_check(window_definition->right_, IRTYPE::kWindowSpec);

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	name_t newWindowName = globalStatusManger_->get_new_window_name();
	instantiate_ident(window_name, IdentifierType::iWindow, newWindowName);

	WindowStatus windowStatus = instantiate_window_spec(window_spec);

	info->windowStatusRecord_[newWindowName] = windowStatus;

	return;
}
WindowStatus Instantiator::instantiate_window_spec(IR* window_spec) {
	WindowStatus windowStatus = WindowStatus::wsNoInherit;

	IR* window_spec_details = nullptr;

	IR* opt_existing_window_name = nullptr;
	IR* opt_partition_clause = nullptr;
	IR* opt_window_order_by_clause = nullptr;
	IR* opt_window_frame_clause = nullptr;

	IR* unknown = nullptr;

	SelectStmtInfo* info = globalStatusManger_->get_cur_selectInfo();

	window_spec_details = NT_check(window_spec->left_, IRTYPE::kWindowSpecDetails);

	opt_existing_window_name = NT_check(window_spec_details->left_, IRTYPE::kOptExistingWindowName);
	unknown = window_spec_details->right_;

	opt_partition_clause = NT_check(unknown->left_, IRTYPE::kOptPartitionClause);
	unknown = unknown->right_;

	opt_window_order_by_clause = NT_check(unknown->left_, IRTYPE::kOptWindowOrderByClause);
	opt_window_frame_clause = NT_check(unknown->right_, IRTYPE::kOptWindowFrameClause);

	if (opt_existing_window_name->left_ != nullptr) {
		vector<name_t> inheritedWindows;

		for (auto &it : info->windowStatusRecord_) {
			if (it.second != WindowStatus::wsCantBeInherited) {
				inheritedWindows.push_back(it.first);
			}
		}

		if (inheritedWindows.empty()) {
			deep_delete(opt_existing_window_name->left_);
			opt_existing_window_name->left_ = nullptr;
		}
		else {
			int index = exprInstantiator_->get_random_integer(0, inheritedWindows.size() - 1);
			name_t inheritedWindowName = inheritedWindows[index];

			IR* t_window_name = NT_check(opt_existing_window_name->left_, IRTYPE::kWindowName);
			instantiate_ident(t_window_name, IdentifierType::iWindow, inheritedWindowName);

			windowStatus = info->windowStatusRecord_[inheritedWindowName] == WindowStatus::wsWithOrder ? WindowStatus::wsWithOrder : WindowStatus::wsInherit;
		}
	}

	if (opt_partition_clause->left_ != nullptr) {
		if (windowStatus == WindowStatus::wsNoInherit) {
			IR* expr_root_list = NT_check(opt_partition_clause->left_, IRTYPE::kExprRootList);

			while (expr_root_list != nullptr) {
				IR* expr_root = NT_check(expr_root_list->left_, IRTYPE::kExprRoot);

				exprInstantiator_->instantiate_expr_root(expr_root, info->acceColumns_, StmtLocation::slWindowClause);
				info->add_all_usedColumns(exprInstantiator_->usedColumnsBuffer_);

				expr_root_list = expr_root_list->right_;
			}
		}
		else {
			deep_delete(opt_partition_clause->left_);

			opt_partition_clause->left_ = nullptr;
			opt_partition_clause->op_->prefix_ = TERMINAL::tEmpty;
		}
	}

	if (opt_window_order_by_clause->left_ != nullptr) {
		if (windowStatus != WindowStatus::wsWithOrder) {
			IR* order_list = NT_check(opt_window_order_by_clause->left_, IRTYPE::kOrderList);

			while (order_list != nullptr) {
				IR* expr_root = NT_check(order_list->left_->left_, IRTYPE::kExprRoot);

				exprInstantiator_->instantiate_expr_root(expr_root, info->acceColumns_, StmtLocation::slWindowClause);
				info->add_all_usedColumns(exprInstantiator_->usedColumnsBuffer_);

				order_list = order_list->right_;
			}

			windowStatus = WindowStatus::wsWithOrder;
		}
		else {
			deep_delete(opt_window_order_by_clause->left_);

			opt_window_order_by_clause->left_ = nullptr;
			opt_window_order_by_clause->op_->prefix_ = TERMINAL::tEmpty;
		}
	}

	if (opt_window_frame_clause->left_ != nullptr) {
		IR* window_frame_units = NT_check(opt_window_frame_clause->left_, IRTYPE::kWindowFrameUnits);
		IR* window_frame_extent = NT_check(opt_window_frame_clause->right_, IRTYPE::kWindowFrameExtent);

		if (windowStatus != WindowStatus::wsWithOrder) {
			window_frame_units->op_->prefix_ = TERMINAL::tRows;
		}

		stack<IR*> IRStack;
		vector<IR*> iconsts;
		IRStack.push(window_frame_extent);

		while (!IRStack.empty()) {
			IR* curIR = IRStack.top();
			IRStack.pop();

			if (curIR == nullptr) {
				continue;
			}
			else if (curIR->type_ == IRTYPE::kIconst) {
				iconsts.push_back(curIR);
			}
			else {
				IRStack.push(curIR->right_);
				IRStack.push(curIR->left_);
			}
		}

		for (IR* curIR : iconsts) {
			curIR->long_val_ = exprInstantiator_->get_random_integer(LIMITMIN, LIMITMAX);
		}

		windowStatus = WindowStatus::wsCantBeInherited;
	}

	return windowStatus;
}

void Instantiator::instantiate_column_list(IR* column_list, vector<name_t>& columns) {
	int i = 0;

	while (column_list != nullptr && i < columns.size()) {
		IR* column_name = NT_check(column_list->left_, IRTYPE::kColumnName);
		instantiate_ident(column_name, IdentifierType::iColumn, columns[i]);

		i++;

		if (i == columns.size() && column_list->right_ != nullptr) {
			deep_delete(column_list->right_);

			column_list->right_ = nullptr;
			column_list->op_->middle_ = TERMINAL::tEmpty;
		}
		else if (i != columns.size() && column_list->right_ == nullptr) {
			column_list->right_ = deep_copy(IRTrim_->simple_column_list_);
			column_list->op_->middle_ = TERMINAL::tOpComma;
		}

		column_list = column_list->right_;
	}

	return;
}
void Instantiator::instantiate_column_list(IR* column_list, vector<Column*>& columns) {
	vector<name_t> columnNames;

	for (Column* c : columns) {
		columnNames.push_back(c->identifierName_);
	}

	instantiate_column_list(column_list, columnNames);

	return;
}
void Instantiator::instantiate_key_list(IR* key_list, vector<Column*>& columns) {
	int count = 0;
	IR* curNode = key_list;
	while (curNode != nullptr && count < columns.size()) {
		IR* key_part = NT_check(curNode->left_, IRTYPE::kKeyPart);
		IR* column_name = NT_check(key_part->left_, IRTYPE::kColumnName);
		IR* iconst = nullptr;

		instantiate_ident(column_name, IdentifierType::iColumn, columns[count]->identifierName_);
		
		if (key_part->right_->type_ != IRTYPE::kOptOrderingDirection) {
			if (Column::typeToBasicType_[columns[count]->columnType_] != ColumnBasicType::ctString) {
				deep_delete(key_part->right_);
				key_part->right_ = new IR(IRTYPE::kOptOrderingDirection, OP0(), nullptr, nullptr);
			}
			else {
				iconst = NT_check(key_part->right_->left_, IRTYPE::kIconst);
			}
		}

		if (iconst != nullptr) {
			int v1 = exprInstantiator_->get_random_integer(1, 32);
			iconst->long_val_ = v1;
		}

		count++;

		if (count == columns.size() && curNode->right_ != nullptr) {
			deep_delete(curNode->right_);

			curNode->right_ = nullptr;
			curNode->op_->middle_ = TERMINAL::tEmpty;
		}
		else if (count != columns.size() && curNode->right_ == nullptr) {
			curNode->right_ = deep_copy(IRTrim_->simple_key_part_list_);
			curNode->op_->middle_ = TERMINAL::tOpComma;
		}

		curNode = curNode->right_;
	}

	return;
}
void Instantiator::instantiate_ident(IR* identifier_name, IdentifierType type, name_t name) {
	IR* ident = NT_check(identifier_name->left_, IRTYPE::kIdent);

	switch(type) {
		case IdentifierType::iColumn: {
			ident->str_val_ = "c" + to_string(name);
			break;
		}
		case IdentifierType::iTable: {
			ident->str_val_ = "t" + to_string(name);
			break;
		}
		case IdentifierType::iWindow: {
			ident->str_val_ = "w" + to_string(name);
			break;
		}
		case IdentifierType::iIndex: {
			ident->str_val_ = "i" + to_string(name);
			break;
		}
		case IdentifierType::iConstraint: {
			ident->str_val_ = "cc" + to_string(name);
			break;
		}
		case IdentifierType::iView: {
			ident->str_val_ = "v" + to_string(name);
			break;
		}
		default: {
			fprintf(stderr, "ERROR: Unknown Ident Type: %d\n", static_cast<int>(type));
			assert(0);
		}
	}

	return;
}

bool Instantiator::check_foreign_key(Table* table, vector<Column*>& curTableColumn, vector<Column*>& otherTableColumn) {
	vector<ColumnType> types;

	vector<Table*> acceTable = globalStatusManger_->get_acce_table(IdentifierType::iTable);
	acceTable.push_back(table);

	for (Column* c : table->columns_) {
		types.push_back(c->columnType_);
	}

	Table* chosenTable = acceTable[rand() % acceTable.size()];
	for (Index* index : chosenTable->indexes_) {
		bool flag = false;

		for (auto& it : index->columns_) {
			if (find(types.begin(), types.end(), it.first->columnType_) == types.end() || it.second == IndexType::itFunctional) {
				continue;
			}

			for (Column* column : table->columns_) {
				if (column->columnType_ != it.first->columnType_ || it.first == column) {
					continue;
				}

				if (find(curTableColumn.begin(), curTableColumn.end(), column) == curTableColumn.end()) {
					curTableColumn.push_back(column);
					otherTableColumn.push_back(it.first);

					flag = true;
					break;
				}
			}
		}

		if (flag == true) {
			break;
		}
	}

	if (curTableColumn.empty() || curTableColumn.size() != otherTableColumn.size()) {
		return false;
	}

	return true;
}

//-----------------------------ExprInstantiator-----------------------------
void ExprInstantiator::instantiate_expr_root(IR* expr_root, vector<Column*>& acceColumns, StmtLocation location) {
	usedColumnsBuffer_.clear();

	IR* expr = NT_check(expr_root->left_, IRTYPE::kExpr);
	instantiate_expr(expr, acceColumns, location);

	return;
}
void ExprInstantiator::instantiate_expr(IR* expr, vector<Column*>& acceColumns, StmtLocation location) {
	stack<IR*> IRStack;
	vector<IR*> queue;

	IRStack.push(expr);

	while (!IRStack.empty()) {
		IR* curIR = IRStack.top();
		IRStack.pop();

		if (curIR == nullptr) {
			continue;
		}

		switch (curIR->type_) {
			case IRTYPE::kAllSubquery:
			case IRTYPE::kInSubquery: {
				queue.push_back(curIR);
				break;
			}
			case IRTYPE::kSimpleExpr: {
				if (find(ignoreSimpleExpr_.begin(), ignoreSimpleExpr_.end(), curIR->left_->type_) != ignoreSimpleExpr_.end()) {
					IRStack.push(curIR->left_);
				}
				else {
					queue.push_back(curIR);
				}

				break;
			}
			default: {
				IRStack.push(curIR->left_);
				IRStack.push(curIR->right_);

				break;
			}
		}
	}

	for (IR* curIR : queue) {
		assert(curIR->type_ == IRTYPE::kInSubquery || curIR->type_ == IRTYPE::kAllSubquery || curIR->type_ == IRTYPE::kSimpleExpr);

		if (curIR->type_ == IRTYPE::kSimpleExpr) {
			instantiate_simple_expr(curIR, acceColumns, location);
		}
		else {
			while (curIR->type_ != IRTYPE::kQueryExpression) {
				curIR = curIR->left_;
			}

			vector<Column*> tmpUsedColumns = usedColumnsBuffer_;
			Table* tmp = instantiator_->instantiate_query_expression(curIR, location, DEFAULTROWNUMBER, 1);

			delete tmp;
			usedColumnsBuffer_ = tmpUsedColumns;
		}
	}

	return;
}
void ExprInstantiator::instantiate_simple_expr(IR* simple_expr, vector<Column*>& acceColumns, StmtLocation location) {
	assert(find(ignoreSimpleExpr_.begin(), ignoreSimpleExpr_.end(), simple_expr->left_->type_) == ignoreSimpleExpr_.end());

	switch (simple_expr->left_->type_) {
		case IRTYPE::kColumnref: {
			if (!acceColumns.empty()) {
				IR* columnref = NT_check(simple_expr->left_, IRTYPE::kColumnref);

				IR* table_name = nullptr;
				IR* column_name = nullptr;

				if (columnref->right_ == nullptr) {
					column_name = NT_check(columnref->left_, IRTYPE::kColumnName);
				}
				else {
					table_name = NT_check(columnref->left_, IRTYPE::kTableName);
					column_name = NT_check(columnref->right_, IRTYPE::kColumnName);
				}

				int index = get_random_integer(0, acceColumns.size() - 1);
				Column* column = acceColumns[index];

				if (column->duplicated_ == ColumnStatus::csDuplication && table_name == nullptr) {
					IR* tmp1 = new IR(IRTYPE::kIdent, OP0(), nullptr, nullptr);
					IR* tmp2 = new IR(IRTYPE::kTableName, OP0(), tmp1, nullptr);

					table_name = tmp2;

					columnref->left_ = table_name;
					columnref->right_ = column_name;
					columnref->op_->middle_ = TERMINAL::tOpDot;					
				}

				if (table_name != nullptr) {
					name_t tableName = column->parentTable_->identifierName_;
					instantiator_->instantiate_ident(table_name, IdentifierType::iTable, tableName);
				}

				name_t columnName = column->identifierName_;
				instantiator_->instantiate_ident(column_name, IdentifierType::iColumn, columnName);

				if (find(usedColumnsBuffer_.begin(), usedColumnsBuffer_.end(), column) == usedColumnsBuffer_.end()) {
					usedColumnsBuffer_.push_back(column);
				}
			}
			else {
				deep_delete(simple_expr->left_);

				simple_expr->left_ = new IR(IRTYPE::kIconst, OP0(), nullptr, nullptr);
				simple_expr->left_->long_val_ = get_random_integer();
			}			

			break;
		}
		case IRTYPE::kIconst: {
			IR* iconst = NT_check(simple_expr->left_, IRTYPE::kIconst);
			iconst->long_val_ = (rand() % 10) == 0 ? get_random_integer() : get_random_integer(-128, 128);

			break;
		}
		case IRTYPE::kFconst: {
			IR* fconst = NT_check(simple_expr->left_, IRTYPE::kFconst);
			fconst->float_val_ = (rand() % 10) == 0 ? get_random_float() : get_random_float(-128, 128);

			break;
		}
		case IRTYPE::kSconst: {
			IR* sconst = NT_check(simple_expr->left_, IRTYPE::kSconst);
			sconst->str_val_ = get_random_string(MINSTRINGLENGTH, MAXSTRINGLENGTH);

			break;
		}
		case IRTYPE::kSubquery: {
			IR* subquery = NT_check(simple_expr->left_, IRTYPE::kSubquery);

			IR* curNode = subquery->left_;
			while (curNode->type_ != IRTYPE::kQueryExpression) {
				curNode = curNode->left_;
			}
			
			Table* tmp = nullptr;
			vector<Column*> tmpUsedColumns = usedColumnsBuffer_;

			if (simple_expr->op_->prefix_ == TERMINAL::tExists) {
				tmp = instantiator_->instantiate_query_expression(curNode, StmtLocation::slTargetList, DEFAULTROWNUMBER, DEFAULTCOLUMNNUMBER);				
			}
			else {
				tmp = instantiator_->instantiate_query_expression(curNode, StmtLocation::slTargetList, 1, 1);
			}

			delete tmp;
			usedColumnsBuffer_ = tmpUsedColumns;

			break;
		}
		case IRTYPE::kTypeCast: {
			IR* type_cast = NT_check(simple_expr->left_, IRTYPE::kTypeCast);
			instantiate_type_cast(type_cast, acceColumns, location);

			break;
		}
		case IRTYPE::kFunction: {
			IR* function = NT_check(simple_expr->left_, IRTYPE::kFunction);
			instantiate_function(function, acceColumns, location);

			break;
		}
		default: {
			fprintf(stderr, "ERROR: Unknown simple_expr->left_->type_: %d\n", static_cast<int>(simple_expr->left_->type_));
			assert(0);
		}
	}

	return;
}
void ExprInstantiator::instantiate_type_cast(IR* type_cast, vector<Column*>& acceColumns, StmtLocation location) {
	IR* expr = NT_check(type_cast->left_, IRTYPE::kExpr);
	IR* cast_type = NT_check(type_cast->right_, IRTYPE::kCastType);

	if (cast_type->left_ != nullptr && cast_type->left_->left_ != nullptr) {
		IR* tmp = cast_type->left_;

		if (tmp->left_->type_ == IRTYPE::kFieldLength) {
			IR* field_length = NT_check(tmp->left_, IRTYPE::kFieldLength);
			IR* iconst = NT_check(field_length->left_, IRTYPE::kIconst);

			iconst->long_val_ = get_random_integer(1, LONG_MAX);
		}
		else if (tmp->left_->type_ == IRTYPE::kPrecision) {
			IR* precision = NT_check(tmp->left_, IRTYPE::kPrecision);

			IR* iconst_left = NT_check(precision->left_, IRTYPE::kIconst);
			IR* iconst_right = NT_check(precision->right_, IRTYPE::kIconst);

			iconst_left->long_val_ = get_random_integer(1, 65);
			iconst_right->long_val_ = get_random_integer(1, min(static_cast<int>(iconst_left->long_val_), 30));
		}
		else {
			fprintf(stderr, "ERROR: Unknown cast_type->left_->left_->type_: %d\n", static_cast<int>(cast_type->left_->left_->type_));
			assert(0);
		}
	}

	instantiate_expr(expr, acceColumns, location);

	return;
}
void ExprInstantiator::instantiate_function(IR* function, vector<Column*>& acceColumns, StmtLocation location) {
	switch (function->left_->type_) {
		case IRTYPE::kControlFunction:
		case IRTYPE::kMathFunction:
		case IRTYPE::kStringFunction: {
			stack<IR*> IRStack;
			vector<IR*> queue;

			IRStack.push(function->left_);
			while (!IRStack.empty()) {
				IR* curIR = IRStack.top();
				IRStack.pop();

				if (curIR == nullptr) {
					continue;
				}

				if (curIR->type_ == IRTYPE::kExpr) {
					queue.push_back(curIR);
				}
				else {
					IRStack.push(curIR->left_);
					IRStack.push(curIR->right_);
				}
			}

			for (IR* expr : queue) {
				assert(expr->type_ == IRTYPE::kExpr);

				instantiate_expr(expr, acceColumns, location);
			}

			break;
		}
		case IRTYPE::kAggregateFunction: {
			if (location != StmtLocation::slTargetList && location != StmtLocation::slHavingClause && location != StmtLocation::slWindowClause) {
				deep_delete(function->left_);
				function->left_ = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tRandOpLpOpRp, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);

				break;
			}

			IR* aggregate_function = NT_check(function->left_, IRTYPE::kAggregateFunction);
			instantiate_aggregate_function(aggregate_function, acceColumns, location);

			break;
		}
		case IRTYPE::kWindowFunction: {
			if (location != StmtLocation::slTargetList) {
				deep_delete(function->left_);
				function->left_ = new IR(IRTYPE::kMathFunction, OP3(TERMINAL::tRandOpLpOpRp, TERMINAL::tEmpty, TERMINAL::tEmpty), nullptr, nullptr);

				break;
			}

			IR* window_function = NT_check(function->left_, IRTYPE::kWindowFunction);
			instantiate_window_function(window_function, acceColumns, location);

			break;
		}
		default: {
			fprintf(stderr, "ERROR: Unknown function->left_->type_: %d\n", static_cast<int>(function->left_->type_));
			assert(0);
		}
	}

	return;
}
void ExprInstantiator::instantiate_aggregate_function(IR* aggregate_function, vector<Column*>& acceColumns, StmtLocation location) {	
	stack<IR*> IRStack;
	vector<IR*> queue;
	vector<IRTYPE> captureType = { IRTYPE::kExpr, IRTYPE::kWindowingClause, IRTYPE::kOrderClause, IRTYPE::kSconst };

	SelectStmtInfo* info = instantiator_->globalStatusManger_->get_cur_selectInfo();

	IRStack.push(aggregate_function);
	while (!IRStack.empty()) {
		IR* curIR = IRStack.top();
		IRStack.pop();

		if (curIR == nullptr) {
			continue;
		}

		if (find(captureType.begin(), captureType.end(), curIR->type_) != captureType.end()) {
			queue.push_back(curIR);
		}
		else {
			IRStack.push(curIR->left_);
			IRStack.push(curIR->right_);
		}
	}

	for (IR* curIR : queue) {
		switch (curIR->type_) {
			case IRTYPE::kExpr: {
				instantiate_expr(curIR, acceColumns, location);
				break;
			}
			case IRTYPE::kWindowingClause: {
				IR* window_name_or_spec = NT_check(curIR->left_, IRTYPE::kWindowNameOrSpec);

				if (info->windowStatusRecord_.empty() && window_name_or_spec->left_->type_ == IRTYPE::kWindowName) {
					deep_delete(window_name_or_spec->left_);
					window_name_or_spec->left_ = deep_copy(instantiator_->IRTrim_->simple_window_spec_);
				}

				if (window_name_or_spec->left_->type_ == IRTYPE::kWindowName) {
					IR* window_name = NT_check(window_name_or_spec->left_, IRTYPE::kWindowName);

					vector<name_t> windowNames;
					for (auto &it : info->windowStatusRecord_) {
						windowNames.push_back(it.first);
					}

					assert(!windowNames.empty());

					int index = get_random_integer(0, windowNames.size() - 1);
					name_t windowName = windowNames[index];

					instantiator_->instantiate_ident(window_name, IdentifierType::iWindow, windowName);
				}
				else {
					IR* window_spec = NT_check(window_name_or_spec->left_, IRTYPE::kWindowSpec);
					instantiator_->instantiate_window_spec(window_spec);
				}

				break;
			}
			case IRTYPE::kOrderClause: {
				instantiator_->instantiate_order_clause(curIR);
				break;
			}
			case IRTYPE::kSconst: {
				curIR->str_val_ = get_random_string(MINSTRINGLENGTH, MAXSTRINGLENGTH);
				break;
			}
			default: {
				fprintf(stderr, "ERROR: Unknown curIR->type_: %d\n", static_cast<int>(curIR->type_));
				assert(0);
			}
		}
	}

	info->hasAggregate = true;

	return;
}
void ExprInstantiator::instantiate_window_function(IR* window_function, vector<Column*>& acceColumns, StmtLocation location) {
	stack<IR*> IRStack;
	vector<IR*> queue;
	vector<IRTYPE> captureType = { IRTYPE::kExpr, IRTYPE::kWindowingClause, IRTYPE::kIconst };

	SelectStmtInfo* info = instantiator_->globalStatusManger_->get_cur_selectInfo();

	IRStack.push(window_function);

	while (!IRStack.empty()) {
		IR* curIR = IRStack.top();
		IRStack.pop();

		if (curIR == nullptr) {
			continue;
		}

		if (find(captureType.begin(), captureType.end(), curIR->type_) != captureType.end()) {
			queue.push_back(curIR);
		}
		else {
			IRStack.push(curIR->left_);
			IRStack.push(curIR->right_);
		}
	}

	for (IR* curIR : queue) {
		switch (curIR->type_) {
			case IRTYPE::kExpr: {
				instantiate_expr(curIR, acceColumns, location);
				break;
			}
			case IRTYPE::kWindowingClause: {
				IR* window_name_or_spec = NT_check(curIR->left_, IRTYPE::kWindowNameOrSpec);

				if (info->windowStatusRecord_.empty() && window_name_or_spec->left_->type_ == IRTYPE::kWindowName) {
					deep_delete(window_name_or_spec->left_);
					window_name_or_spec->left_ = deep_copy(instantiator_->IRTrim_->simple_window_spec_);
				}

				if (window_name_or_spec->left_->type_ == IRTYPE::kWindowName) {
					IR* window_name = NT_check(window_name_or_spec->left_, IRTYPE::kWindowName);

					vector<name_t> windowNames;
					for (auto &it : info->windowStatusRecord_) {
						windowNames.push_back(it.first);
					}

					assert(!windowNames.empty());

					int index = get_random_integer(0, windowNames.size() - 1);
					name_t windowName = windowNames[index];

					instantiator_->instantiate_ident(window_name, IdentifierType::iWindow, windowName);
				}
				else {
					IR* window_spec = NT_check(window_name_or_spec->left_, IRTYPE::kWindowSpec);
					instantiator_->instantiate_window_spec(window_spec);
				}

				break;
			}
			case IRTYPE::kIconst: {
				curIR->long_val_ = get_random_integer(1, LONG_MAX);
				break;
			}
			default: {
				fprintf(stderr, "ERROR: Unknown curIR->type_: %d\n", static_cast<int>(curIR->type_));
				assert(0);
			}
		}
	}

	return;
}

long int ExprInstantiator::get_random_integer(long int min, long int max) {
	assert(min <= max);

	random_device rd;
	mt19937 r_eng(rd());
	uniform_int_distribution<long int> dis(min, max);

	return dis(r_eng);
}
double ExprInstantiator::get_random_float(int min, int max) {
	random_device rd;
	mt19937 r_eng(rd());
	uniform_real_distribution<double> dis(min, max);

	return dis(r_eng);
}
string ExprInstantiator::get_random_string(int minLength, int maxLength) {
	random_device rd;
	mt19937 r_eng(rd());
	uniform_int_distribution<int> charNumber(32, 126);
	uniform_int_distribution<int> stringLength(minLength, maxLength);

	int length = stringLength(r_eng);
	string res = "\'";

	for (int i = 0; i < length; i++) {
		char tmp = charNumber(r_eng);

		if (tmp == '\'' || tmp == '\\') {
			res += "\\";			
		}
		
		res += tmp;
	}

	res += "\'";

	return res;
}
