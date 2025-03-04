#ifndef __DEFINE_H__
#define __DEFINE_H__

#define SWITCHSTART switch(case_idx_){ 
#define SWITCHEND default: assert(0); }

#define CASESTART(idx) case CASE##idx: {
#define CASEEND break; }

#define TRANSLATESTART IR *res = nullptr;
#define TRANSLATEEND return res;

#define SAFETRANSLATE(a) (assert(a != nullptr), a->translate())
#define SAFEDELETE(a) if(a != nullptr) a->deep_delete()

#define OP0() new IROperator(TERMINAL::tEmpty, TERMINAL::tEmpty, TERMINAL::tEmpty)
#define OP3(a,b,c) new IROperator(a,b,c)

#define ALLTYPE(V) \
	V(kNone) \
	V(kParseToplevel) \
	V(kStmtmulti) \
	V(kStmt) \
	V(kDeleteStmt) \
	V(kDeleteStmtTmp1) \
	V(kDeleteStmtTmp2) \
	V(kDeleteStmtTmp3) \
	V(kDeleteStmtTmp4) \
	V(kDeleteStmtTmp5) \
	V(kDeleteStmtTmp6) \
	V(kDeleteStmtTmp7) \
	V(kDeleteStmtTmp8) \
	V(kOptDeleteOptions) \
	V(kDeleteOptions) \
	V(kDeleteOption) \
	V(kAlterTableStmt) \
	V(kAlterList) \
	V(kAlterListItem) \
	V(kAlterListItemTmp1) \
	V(kAlterListItemTmp2) \
	V(kOptPlace) \
	V(kDropIndexStmt) \
	V(kDropTableStmt) \
	V(kIfExists) \
	V(kTableList) \
	V(kDropViewStmt) \
	V(kUpdateStmt) \
	V(kUpdateStmtTmp1) \
	V(kUpdateStmtTmp2) \
	V(kUpdateStmtTmp3) \
	V(kUpdateStmtTmp4) \
	V(kUpdateStmtTmp5) \
	V(kUpdateStmtTmp6) \
	V(kOptSimpleLimit) \
	V(kOptWithClause) \
	V(kOptLowPriority) \
	V(kInsertStmt) \
	V(kInsertStmtTmp1) \
	V(kInsertStmtTmp2) \
	V(kInsertStmtTmp3) \
	V(kInsertStmtTmp4) \
	V(kInsertStmtTmp5) \
	V(kInsertStmtTmp6) \
	V(kInsertStmtTmp7) \
	V(kInsertStmtTmp8) \
	V(kInsertStmtTmp9) \
	V(kInsertStmtTmp10) \
	V(kInsertQueryExpression) \
	V(kInsertFromConstructor) \
	V(kValuesList) \
	V(kRowValue) \
	V(kOptValues) \
	V(kValues) \
	V(kExprOrDefault) \
	V(kOptValuesReference) \
	V(kOptInsertUpdateList) \
	V(kUpdateList) \
	V(kUpdateElem) \
	V(kInsertLockOption) \
	V(kOptIgnore) \
	V(kCreateIndexStmt) \
	V(kCreateIndexStmtTmp1) \
	V(kCreateIndexStmtTmp2) \
	V(kOptUnique) \
	V(kCreateViewStmt) \
	V(kCreateViewStmtTmp1) \
	V(kCreateViewStmtTmp2) \
	V(kCreateViewStmtTmp3) \
	V(kOptOrReplace) \
	V(kOptViewAlgorithm) \
	V(kCreateTableStmt) \
	V(kCreateTableStmtTmp1) \
	V(kCreateTableStmtTmp2) \
	V(kCreateTableStmtTmp3) \
	V(kCreateTableStmtTmp4) \
	V(kCreateTableStmtTmp5) \
	V(kOptTemporary) \
	V(kOptIfNotExists) \
	V(kOptDuplicateAsQe) \
	V(kDuplicateAsQe) \
	V(kDuplicate) \
	V(kTableElementList) \
	V(kTableElement) \
	V(kColumnDef) \
	V(kFieldDef) \
	V(kDataType) \
	V(kStringType) \
	V(kBoolType) \
	V(kNumberType) \
	V(kNumberTypeTmp1) \
	V(kNumberTypeTmp2) \
	V(kNumericType) \
	V(kRealType) \
	V(kOptPrecision) \
	V(kIntType) \
	V(kOptFieldOptions) \
	V(kFieldOptionList) \
	V(kFieldOption) \
	V(kOptColumnAttributeList) \
	V(kColumnAttributeList) \
	V(kColumnAttribute) \
	V(kDefaultAttribute) \
	V(kNotNull) \
	V(kVisibility) \
	V(kOptConstraintName) \
	V(kCheckConstraint) \
	V(kTableConstraintDef) \
	V(kTableConstraintDefTmp1) \
	V(kTableConstraintDefTmp2) \
	V(kTableConstraintDefTmp3) \
	V(kTableConstraintDefTmp4) \
	V(kReferences) \
	V(kReferencesTmp1) \
	V(kOptOnUpdateDelete) \
	V(kOnUpdateOption) \
	V(kKeyList) \
	V(kKeyListWithExpression) \
	V(kKeyPartWithExpression) \
	V(kKeyPart) \
	V(kKeyPartTmp1) \
	V(kConstraintKeyType) \
	V(kOptIndexName) \
	V(kSelectStmt) \
	V(kQueryExpression) \
	V(kQueryExpressionTmp1) \
	V(kQueryExpressionTmp2) \
	V(kQueryExpressionBody) \
	V(kQueryExpressionBodyTmp1) \
	V(kQueryPrimary) \
	V(kQuerySpecification) \
	V(kQuerySpecificationTmp1) \
	V(kQuerySpecificationTmp2) \
	V(kQuerySpecificationTmp3) \
	V(kQuerySpecificationTmp4) \
	V(kQuerySpecificationTmp5) \
	V(kOptWindowClause) \
	V(kWindowDefinitionList) \
	V(kWindowDefinition) \
	V(kWindowSpec) \
	V(kWindowSpecDetails) \
	V(kWindowSpecDetailsTmp1) \
	V(kWindowSpecDetailsTmp2) \
	V(kOptExistingWindowName) \
	V(kOptPartitionClause) \
	V(kOptWindowOrderByClause) \
	V(kOrderList) \
	V(kOrderExpr) \
	V(kOptOrderingDirection) \
	V(kOptWindowFrameClause) \
	V(kWindowFrameUnits) \
	V(kWindowFrameExtent) \
	V(kWindowFrameStart) \
	V(kWindowFrameBetween) \
	V(kWindowFrameBound) \
	V(kOptHavingClause) \
	V(kOptGroupClause) \
	V(kOlapOpt) \
	V(kOptWhereClause) \
	V(kWhereClause) \
	V(kOptFromClause) \
	V(kFromClause) \
	V(kTableReferenceList) \
	V(kTableReference) \
	V(kTableFactor) \
	V(kTableReferenceListParens) \
	V(kJoinedTableParens) \
	V(kDerivedTable) \
	V(kDerivedTableTmp1) \
	V(kOptColumnList) \
	V(kColumnList) \
	V(kSubquery) \
	V(kSingleTable) \
	V(kOptAlias) \
	V(kJoinedTable) \
	V(kJoinedTableTmp1) \
	V(kJoinedTableTmp2) \
	V(kJoinedTableTmp3) \
	V(kJoinedTableTmp4) \
	V(kJoinedTableTmp5) \
	V(kJoinedTableTmp6) \
	V(kJoinedTableTmp7) \
	V(kJoinedTableTmp8) \
	V(kInnerJoinType) \
	V(kNaturalJoinType) \
	V(kOptInner) \
	V(kOptOuter) \
	V(kOuterJoinType) \
	V(kSelectItemList) \
	V(kSelectItem) \
	V(kTableWild) \
	V(kOptSelectOptions) \
	V(kSelectOptionList) \
	V(kSelectOption) \
	V(kQueryExpressionParens) \
	V(kOptUnionOption) \
	V(kOptOrderClause) \
	V(kOrderClause) \
	V(kOptLimitClause) \
	V(kLimitClause) \
	V(kLimitOptions) \
	V(kWithClause) \
	V(kWithList) \
	V(kCommonTableExpr) \
	V(kCommonTableExprTmp1) \
	V(kExprRootList) \
	V(kExprRoot) \
	V(kExpr) \
	V(kBoolPri) \
	V(kBoolPriTmp1) \
	V(kBoolPriTmp2) \
	V(kPredicate) \
	V(kPredicateTmp1) \
	V(kPredicateTmp2) \
	V(kBitExpr) \
	V(kSimpleExpr) \
	V(kFunction) \
	V(kStringFunction) \
	V(kStringFunctionTmp1) \
	V(kStringFunctionTmp2) \
	V(kStringFunctionTmp3) \
	V(kMathFunction) \
	V(kMathFunctionTmp1) \
	V(kWindowFunction) \
	V(kWindowFunctionTmp1) \
	V(kWindowFunctionTmp2) \
	V(kWindowFunctionTmp3) \
	V(kWindowFunctionTmp4) \
	V(kOptNullTreatment) \
	V(kOptFromFirstLast) \
	V(kOptLeadLagInfo) \
	V(kLeadLagInfo) \
	V(kAggregateFunction) \
	V(kAggregateFunctionTmp1) \
	V(kAggregateFunctionTmp2) \
	V(kAggregateFunctionTmp3) \
	V(kAggregateFunctionTmp4) \
	V(kOptGconcatSeparator) \
	V(kOptDistinct) \
	V(kOptWindowingClause) \
	V(kWindowingClause) \
	V(kWindowNameOrSpec) \
	V(kControlFunction) \
	V(kControlFunctionTmp1) \
	V(kCaseExpr) \
	V(kCaseExprTmp1) \
	V(kWhenClasueList) \
	V(kWhenList) \
	V(kOptExpr) \
	V(kOptElse) \
	V(kTypeCast) \
	V(kCastType) \
	V(kFloatOptions) \
	V(kPrecision) \
	V(kOptFieldLength) \
	V(kFieldLength) \
	V(kLiteral) \
	V(kColumnref) \
	V(kCompOp) \
	V(kAllSubquery) \
	V(kInSubquery) \
	V(kExprList) \
	V(kIconst) \
	V(kFconst) \
	V(kSconst) \
	V(kIdent) \
	V(kAlias) \
	V(kColumnName) \
	V(kWindowName) \
	V(kTableName) \
	V(kConstraintName) \
	V(kIndexName) \
	V(kViewName) \

#define ALLCLASS(V) \
	V(ParseToplevel) \
	V(Stmtmulti) \
	V(Stmt) \
	V(DeleteStmt) \
	V(OptDeleteOptions) \
	V(DeleteOptions) \
	V(DeleteOption) \
	V(AlterTableStmt) \
	V(AlterList) \
	V(AlterListItem) \
	V(OptPlace) \
	V(DropIndexStmt) \
	V(DropTableStmt) \
	V(IfExists) \
	V(TableList) \
	V(DropViewStmt) \
	V(UpdateStmt) \
	V(OptSimpleLimit) \
	V(OptWithClause) \
	V(OptLowPriority) \
	V(InsertStmt) \
	V(InsertQueryExpression) \
	V(InsertFromConstructor) \
	V(ValuesList) \
	V(RowValue) \
	V(OptValues) \
	V(Values) \
	V(ExprOrDefault) \
	V(OptValuesReference) \
	V(OptInsertUpdateList) \
	V(UpdateList) \
	V(UpdateElem) \
	V(InsertLockOption) \
	V(OptIgnore) \
	V(CreateIndexStmt) \
	V(OptUnique) \
	V(CreateViewStmt) \
	V(OptOrReplace) \
	V(OptViewAlgorithm) \
	V(CreateTableStmt) \
	V(OptTemporary) \
	V(OptIfNotExists) \
	V(OptDuplicateAsQe) \
	V(DuplicateAsQe) \
	V(Duplicate) \
	V(TableElementList) \
	V(TableElement) \
	V(ColumnDef) \
	V(FieldDef) \
	V(DataType) \
	V(StringType) \
	V(BoolType) \
	V(NumberType) \
	V(NumericType) \
	V(RealType) \
	V(OptPrecision) \
	V(IntType) \
	V(OptFieldOptions) \
	V(FieldOptionList) \
	V(FieldOption) \
	V(OptColumnAttributeList) \
	V(ColumnAttributeList) \
	V(ColumnAttribute) \
	V(DefaultAttribute) \
	V(NotNull) \
	V(Visibility) \
	V(OptConstraintName) \
	V(CheckConstraint) \
	V(TableConstraintDef) \
	V(References) \
	V(OptOnUpdateDelete) \
	V(OnUpdateOption) \
	V(KeyList) \
	V(KeyListWithExpression) \
	V(KeyPartWithExpression) \
	V(KeyPart) \
	V(ConstraintKeyType) \
	V(OptIndexName) \
	V(SelectStmt) \
	V(QueryExpression) \
	V(QueryExpressionBody) \
	V(QueryPrimary) \
	V(QuerySpecification) \
	V(OptWindowClause) \
	V(WindowDefinitionList) \
	V(WindowDefinition) \
	V(WindowSpec) \
	V(WindowSpecDetails) \
	V(OptExistingWindowName) \
	V(OptPartitionClause) \
	V(OptWindowOrderByClause) \
	V(OrderList) \
	V(OrderExpr) \
	V(OptOrderingDirection) \
	V(OptWindowFrameClause) \
	V(WindowFrameUnits) \
	V(WindowFrameExtent) \
	V(WindowFrameStart) \
	V(WindowFrameBetween) \
	V(WindowFrameBound) \
	V(OptHavingClause) \
	V(OptGroupClause) \
	V(OlapOpt) \
	V(OptWhereClause) \
	V(WhereClause) \
	V(OptFromClause) \
	V(FromClause) \
	V(TableReferenceList) \
	V(TableReference) \
	V(TableFactor) \
	V(TableReferenceListParens) \
	V(JoinedTableParens) \
	V(DerivedTable) \
	V(OptColumnList) \
	V(ColumnList) \
	V(Subquery) \
	V(SingleTable) \
	V(OptAlias) \
	V(JoinedTable) \
	V(InnerJoinType) \
	V(NaturalJoinType) \
	V(OptInner) \
	V(OptOuter) \
	V(OuterJoinType) \
	V(SelectItemList) \
	V(SelectItem) \
	V(TableWild) \
	V(OptSelectOptions) \
	V(SelectOptionList) \
	V(SelectOption) \
	V(QueryExpressionParens) \
	V(OptUnionOption) \
	V(OptOrderClause) \
	V(OrderClause) \
	V(OptLimitClause) \
	V(LimitClause) \
	V(LimitOptions) \
	V(WithClause) \
	V(WithList) \
	V(CommonTableExpr) \
	V(ExprRootList) \
	V(ExprRoot) \
	V(Expr) \
	V(BoolPri) \
	V(Predicate) \
	V(BitExpr) \
	V(SimpleExpr) \
	V(Function) \
	V(StringFunction) \
	V(MathFunction) \
	V(WindowFunction) \
	V(OptNullTreatment) \
	V(OptFromFirstLast) \
	V(OptLeadLagInfo) \
	V(LeadLagInfo) \
	V(AggregateFunction) \
	V(OptGconcatSeparator) \
	V(OptDistinct) \
	V(OptWindowingClause) \
	V(WindowingClause) \
	V(WindowNameOrSpec) \
	V(ControlFunction) \
	V(CaseExpr) \
	V(WhenClasueList) \
	V(WhenList) \
	V(OptExpr) \
	V(OptElse) \
	V(TypeCast) \
	V(CastType) \
	V(FloatOptions) \
	V(Precision) \
	V(OptFieldLength) \
	V(FieldLength) \
	V(Literal) \
	V(Columnref) \
	V(CompOp) \
	V(AllSubquery) \
	V(InSubquery) \
	V(ExprList) \
	V(Iconst) \
	V(Fconst) \
	V(Sconst) \
	V(Ident) \
	V(Alias) \
	V(ColumnName) \
	V(WindowName) \
	V(TableName) \
	V(ConstraintName) \
	V(IndexName) \
	V(ViewName) \

#define ALLTERMINAL(V) \
	V(tOpSemi) \
	V(tFrom) \
	V(tDelete) \
	V(tEmpty) \
	V(tQuick) \
	V(tLowPriority) \
	V(tIgnore) \
	V(tAlterTable) \
	V(tOpComma) \
	V(tAddColumn) \
	V(tOpRp) \
	V(tAddColumnOpLp) \
	V(tAdd) \
	V(tChangeColumn) \
	V(tModifyColumn) \
	V(tDropColumn) \
	V(tDropForeignKey) \
	V(tDropPrimaryKey) \
	V(tDropIndex) \
	V(tDropCheck) \
	V(tDropConstraint) \
	V(tSetDefaultOpLp) \
	V(tAlterColumn) \
	V(tDropDefault) \
	V(tSet) \
	V(tAlterIndex) \
	V(tRenameTo) \
	V(tTo) \
	V(tRenameIndex) \
	V(tRenameColumn) \
	V(tAfter) \
	V(tFirst) \
	V(tOn) \
	V(tDropTable) \
	V(tIfExists) \
	V(tDropView) \
	V(tUpdate) \
	V(tLimit) \
	V(tInto) \
	V(tInsert) \
	V(tOpLp) \
	V(tValues) \
	V(tOpRpValues) \
	V(tDefault) \
	V(tAs) \
	V(tOnDuplicateKeyUpdate) \
	V(tOpEqual) \
	V(tDelayed) \
	V(tHighPriority) \
	V(tIndex) \
	V(tCreate) \
	V(tUnique) \
	V(tView) \
	V(tOrReplace) \
	V(tAlgorithmOpEqualUndefined) \
	V(tAlgorithmOpEqualMerge) \
	V(tAlgorithmOpEqualTemptable) \
	V(tTable) \
	V(tTemporary) \
	V(tIfNotExists) \
	V(tReplace) \
	V(tChar) \
	V(tBinary) \
	V(tVarchar) \
	V(tVarbinary) \
	V(tLongVarbinary) \
	V(tTinyblob) \
	V(tMediumblob) \
	V(tLongblob) \
	V(tBlob) \
	V(tTinytext) \
	V(tMediumtext) \
	V(tLongtext) \
	V(tText) \
	V(tBool) \
	V(tBoolean) \
	V(tFloat) \
	V(tDecimal) \
	V(tNumeric) \
	V(tFixed) \
	V(tReal) \
	V(tDouble) \
	V(tInt) \
	V(tTinyint) \
	V(tSmallint) \
	V(tMediumint) \
	V(tBigint) \
	V(tSigned) \
	V(tUnsigned) \
	V(tZerofill) \
	V(tDefaultOpLp) \
	V(tNotNull) \
	V(tVisible) \
	V(tInvisible) \
	V(tConstraint) \
	V(tCheckOpLp) \
	V(tForeignKey) \
	V(tReferences) \
	V(tOnUpdate) \
	V(tOnDelete) \
	V(tCascade) \
	V(tSetNull) \
	V(tNoAction) \
	V(tSetDefault) \
	V(tPrimaryKey) \
	V(tUnion) \
	V(tExcept) \
	V(tIntersect) \
	V(tSelect) \
	V(tWindow) \
	V(tPartitionBy) \
	V(tOrderBy) \
	V(tAsc) \
	V(tDesc) \
	V(tRows) \
	V(tRange) \
	V(tUnboundedPreceding) \
	V(tPreceding) \
	V(tCurrentRow) \
	V(tAnd) \
	V(tBetween) \
	V(tUnboundedFollowing) \
	V(tFollowing) \
	V(tHaving) \
	V(tGroupBy) \
	V(tWithRollup) \
	V(tWhere) \
	V(tLateral) \
	V(tUsingOpLp) \
	V(tJoin) \
	V(tInnerJoin) \
	V(tCrossJoin) \
	V(tStraightJoin) \
	V(tNatural) \
	V(tNaturalRight) \
	V(tNaturalLeft) \
	V(tInner) \
	V(tOuter) \
	V(tLeft) \
	V(tRight) \
	V(tOpMul) \
	V(tOpDotOpMul) \
	V(tSqlNoCache) \
	V(tDistinct) \
	V(tSqlSmallResult) \
	V(tSqlBigResult) \
	V(tSqlBufferResult) \
	V(tSqlCalcFoundRows) \
	V(tAll) \
	V(tWith) \
	V(tWithRecursive) \
	V(tOr) \
	V(tXor) \
	V(tNot) \
	V(tIsTrue) \
	V(tIsNotTrue) \
	V(tIsFalse) \
	V(tIsNotFalse) \
	V(tIsUnknown) \
	V(tIsNotUnknown) \
	V(tIsNull) \
	V(tIsNotNull) \
	V(tInOpLp) \
	V(tNotInOpLp) \
	V(tNotBetween) \
	V(tSoundsLike) \
	V(tLike) \
	V(tNotLike) \
	V(tRegexp) \
	V(tNotRegexp) \
	V(tOpOr) \
	V(tOpAnd) \
	V(tOpShl) \
	V(tOpShr) \
	V(tOpAdd) \
	V(tOpSub) \
	V(tOpDivide) \
	V(tOpMod) \
	V(tOpXor) \
	V(tOpNot) \
	V(tExists) \
	V(tAsciiOpLp) \
	V(tBinOpLp) \
	V(tBitLengthOpLp) \
	V(tCharLengthOpLp) \
	V(tLengthOpLp) \
	V(tLowerOpLp) \
	V(tLtrimOpLp) \
	V(tOctOpLp) \
	V(tOrdOpLp) \
	V(tQuoteOpLp) \
	V(tReverseOpLp) \
	V(tRtrimOpLp) \
	V(tSpaceOpLp) \
	V(tTrimOpLp) \
	V(tTrimOpLpLeadingFrom) \
	V(tTrimOpLpTrailingFrom) \
	V(tUnhexOpLp) \
	V(tUpperOpLp) \
	V(tFindInSetOpLp) \
	V(tInstrOpLp) \
	V(tLeftOpLp) \
	V(tLocateOpLp) \
	V(tRepeatOpLp) \
	V(tRightOpLp) \
	V(tSubstringOpLp) \
	V(tTrimOpLpLeading) \
	V(tTrimOpLpTrailing) \
	V(tExportSetOpLp) \
	V(tLpadOpLp) \
	V(tReplaceOpLp) \
	V(tRpadOpLp) \
	V(tSubstringIndexOpLp) \
	V(tInsertOpLp) \
	V(tCharOpLp) \
	V(tConcatOpLp) \
	V(tConcatWsOpLp) \
	V(tEltOpLp) \
	V(tFieldOpLp) \
	V(tMakeSetOpLp) \
	V(tPiOpLpOpRp) \
	V(tRandOpLpOpRp) \
	V(tAbsOpLp) \
	V(tAcosOpLp) \
	V(tAsinOpLp) \
	V(tAtanOpLp) \
	V(tCeilingOpLp) \
	V(tCosOpLp) \
	V(tCotOpLp) \
	V(tDegreesOpLp) \
	V(tExpOpLp) \
	V(tFloorOpLp) \
	V(tHexOpLp) \
	V(tLnOpLp) \
	V(tRadiansOpLp) \
	V(tRandOpLp) \
	V(tRoundOpLp) \
	V(tSignOpLp) \
	V(tSinOpLp) \
	V(tSqrtOpLp) \
	V(tTanOpLp) \
	V(tFormatOpLp) \
	V(tLogOpLp) \
	V(tModOpLp) \
	V(tPowOpLp) \
	V(tTruncateOpLp) \
	V(tConvOpLp) \
	V(tRowNumberOpLpOpRp) \
	V(tRankOpLpOpRp) \
	V(tDenseRankOpLpOpRp) \
	V(tCumeDistOpLpOpRp) \
	V(tPercentRankOpLpOpRp) \
	V(tNtileOpLp) \
	V(tLeadOpLp) \
	V(tLagOpLp) \
	V(tFirstValueOpLp) \
	V(tLastValueOpLp) \
	V(tNthValueOpLp) \
	V(tRespectNulls) \
	V(tIgnoreNulls) \
	V(tFromFirst) \
	V(tFromLast) \
	V(tAvgOpLp) \
	V(tBitAndOpLp) \
	V(tBitOrOpLp) \
	V(tBitXorOpLp) \
	V(tCountOpLpOpMulOpRp) \
	V(tCountOpLp) \
	V(tCountOpLpDistinct) \
	V(tMinOpLp) \
	V(tMaxOpLp) \
	V(tSumOpLp) \
	V(tStdOpLp) \
	V(tStddevSampOpLp) \
	V(tVarianceOpLp) \
	V(tVarSampOpLp) \
	V(tGroupConcatOpLp) \
	V(tSeparator) \
	V(tOver) \
	V(tIfOpLp) \
	V(tIfnullOpLp) \
	V(tNullifOpLp) \
	V(tEnd) \
	V(tCase) \
	V(tThen) \
	V(tWhen) \
	V(tElse) \
	V(tCastOpLp) \
	V(tConvertOpLp) \
	V(tNchar) \
	V(tNull) \
	V(tFalse) \
	V(tTrue) \
	V(tOpDot) \
	V(tOpGreatereq) \
	V(tOpGreaterthan) \
	V(tOpLesseq) \
	V(tOpLessthan) \
	V(tOpNotequal) \
	V(tAny) \
	V(tSome) \
	V(tIn) \
	V(tNotIn) \
	V(tIconst) \
	V(tFconst) \
	V(tSconst) \
	V(tIdent) \

#define IRNUMBER 273

#endif