//
// Created by LE, Duc Anh on 5/26/15.
//

#include "PredicateHelper.h"
#include "SqlHelper.h"
#include <Foundation/Enums.h>
#include <Foundation/EntityMap.h>
#include <Foundation/Query/Contract/IPredicateFormatter.h>
#include <Foundation/Query/Comparative.h>
#include <Foundation/Query/PredicateIterator.h>

namespace Cloude {
    namespace Foundation {
        namespace Query {
            namespace Helper {

                std::string SqlHelper::CreateGetPreparedQuery(const EntityMap &entityMap,
                                                              FPtrParamProcessor fptr) {

                    auto const &columnsForGet = entityMap.getColumnsForGet();
                    auto const &columnsForKey = entityMap.getColumnsForKey();

                    std::string strTableName = entityMap.TableName();
                    std::string strColumns;

                    std::for_each(columnsForGet.cbegin(), columnsForGet.cend(),
                                  [&strColumns](const std::shared_ptr<Column> &p) {
                                      if (strColumns.length() != 0)
                                          strColumns += ", ";

                                      strColumns += p->getDatasourceName();
                                  });


                    std::string strCondition;

                    int index = 0;

                    std::for_each(columnsForKey.cbegin(),
                                  columnsForKey.cend(),
                                  [&](const std::shared_ptr<Column> &column) {

                                      if (index != 0) {
                                          strCondition += " AND ";
                                      }

                                      strCondition += fptr(column, index);

                                      ++index;
                                  });

                    std::string strQuery;

                    strQuery += " SELECT " + strColumns;
                    strQuery += " FROM " + strTableName;
                    strQuery += " WHERE " + strCondition;

                    return strQuery;
                }

                std::string SqlHelper::CreateInsertPreparedQuery(const EntityMap &entityMap,
                                                                 FPtrParamProcessor fptr) {

                    auto const &columnsForKey = entityMap.getColumnsForKey();

                    std::string strColumns;
                    std::string strCondition;
                    std::string strQuery;

                    int index = 0;

                    std::for_each(columnsForKey.cbegin(),
                                  columnsForKey.cend(),
                                  [&](const std::shared_ptr<Column> &column) -> void {

                                      if (index != 0) {
                                          strColumns += ", ";
                                          strCondition += ", ";
                                      }

                                      strColumns += column->getDatasourceName();
                                      strCondition += fptr(column, index);

                                      ++index;
                                  });

                    strQuery += " INSERT INTO " + entityMap.TableName() + " (" + strColumns + ")";
                    strQuery += " VALUES (" + strCondition + ")";

                    return strQuery;
                }

                std::string SqlHelper::CreateUpdatePreparedQuery(const EntityMap &entityMap, FPtrParamProcessor fptr) {

                    auto const &columnsForUpdate = entityMap.getColumnsForUpdate();
                    auto const &columnsForKey = entityMap.getColumnsForKey();

                    std::string strColumns;
                    std::string strCondition;
                    std::string strQuery;

                    int index = 0;

                    bool isSetStart = true;
                    bool isWhereStart = true;

                    std::for_each(columnsForUpdate.cbegin(), columnsForUpdate.cend(),
                                  [&](const std::shared_ptr<Column> &column) -> void {

                                      if (!isSetStart) {
                                          strColumns += ", ";

                                      } else {
                                          isSetStart = false;
                                      }

                                      strColumns += fptr(column, index);

                                      ++index;
                                  });

                    std::for_each(columnsForKey.cbegin(), columnsForKey.cend(),
                                  [&](const std::shared_ptr<Column> &column) -> void {

                                      if (!isWhereStart) {
                                          strCondition += ", ";
                                      } else {
                                          isWhereStart = false;
                                      }

                                      strCondition += fptr(column, index);

                                      ++index;
                                  });

                    strQuery += " UPDATE " + entityMap.TableName();
                    strQuery += " SET " + strColumns;
                    strQuery += " WHERE (" + strCondition + ")";

                    return strQuery;
                }

                std::string SqlHelper::CreateDeletePreparedQuery(const EntityMap &entityMap, FPtrParamProcessor fptr) {

                    auto const &columnsForKey = entityMap.getColumnsForKey();

                    std::string strCondition;
                    std::string strQuery;

                    int i = 0;

                    std::for_each(columnsForKey.cbegin(), columnsForKey.cend(),
                                  [&](const std::shared_ptr<Column> &column) -> void {

                                      if (i != 0) {
                                          strCondition += ", ";
                                      }

                                      strCondition += fptr(column, i);

                                      ++i;
                                  });

                    strQuery += " DELETE FROM " + entityMap.TableName();
                    strQuery += " WHERE (" + strCondition + ")";

                    return strQuery;
                }

                std::pair<std::string, std::vector<SqlHelper::SPtrPredicate>> SqlHelper::CreateSelectPreparedQuery(
                        const EntityMap &entityMap,
                        const SPtrPredicate &sptrPredicate,
                        const FPtrParamProcessor fptrParamProcessor) {

                    using PredicateComposite = Foundation::Query::PredicateComposite;

                    /// Internal (recursive) Predicate to string copy parser lambda.
                    /// FPtrParamProcessor is forwarded from outter scope, while SPtrPredicate is not.
                    /// This forward approach is due to FPtrParamProcess does not change at (each) predicate parsing.
                    ///
                    /// Note: This lambda is RECURSIVE.
                    ///
                    std::function<std::string(const SPtrPredicate, int &)>
                            fptr = [&fptr, &fptrParamProcessor](const SPtrPredicate &fptrSPtrPredicate,
                                                                int &index) -> std::string {

                        using PredicateComposite = Foundation::Query::PredicateComposite;

                        std::string result;
                        result.reserve(Cloude::Foundation::THIRTYTWO);

                        switch (fptrSPtrPredicate->getComparativeType()) {
                            case Cloude::Foundation::Query::Enumeration::ComparativeType::And: {
                                auto sptrComposite = std::dynamic_pointer_cast<PredicateComposite>(fptrSPtrPredicate);
                                result += "(" + fptr(sptrComposite->getLhs(), index) + ")";
                                result += " AND ";
                                result += "(" + fptr(sptrComposite->getRhs(), index) + ")";
                                break;
                            }
                            case Cloude::Foundation::Query::Enumeration::ComparativeType::Or: {
                                auto sptrComposite = std::dynamic_pointer_cast<PredicateComposite>(fptrSPtrPredicate);
                                result += "(" + fptr(sptrComposite->getLhs(), index) + ")";
                                result += " OR ";
                                result += "(" + fptr(sptrComposite->getRhs(), index) + ")";
                                break;
                            };
                            case Cloude::Foundation::Query::Enumeration::ComparativeType::Like: {
                                result += fptrSPtrPredicate->getColumn()->getDatasourceName();
                                result += " LIKE ";
                                result += fptrParamProcessor(fptrSPtrPredicate->getColumn(), index);
                                ++index;
                                break;
                            };
                            case Cloude::Foundation::Query::Enumeration::ComparativeType::NotLike: {
                                result += fptrSPtrPredicate->getColumn()->getDatasourceName();
                                result += " NOT LIKE ";
                                result += fptrParamProcessor(fptrSPtrPredicate->getColumn(), index);
                                ++index;
                                break;
                            };
                            case Cloude::Foundation::Query::Enumeration::ComparativeType::IsNull: {
                                result += fptrSPtrPredicate->getColumn()->getDatasourceName();
                                result += " IS NULL ";
                                ++index;
                                break;
                            };
                            case Cloude::Foundation::Query::Enumeration::ComparativeType::IsNotNull: {
                                result += fptrSPtrPredicate->getColumn()->getDatasourceName();
                                result += " IS NOT NULL ";
                                ++index;
                                break;
                            };
                            case Cloude::Foundation::Query::Enumeration::ComparativeType::Equal: {
                                result += fptrSPtrPredicate->getColumn()->getDatasourceName();
                                result += " = ";
                                result += fptrParamProcessor(fptrSPtrPredicate->getColumn(), index);
                                ++index;
                                break;
                            };
                            case Cloude::Foundation::Query::Enumeration::ComparativeType::NotEqual: {
                                result += fptrSPtrPredicate->getColumn()->getDatasourceName();
                                result += " != ";
                                result += fptrParamProcessor(fptrSPtrPredicate->getColumn(), index);
                                ++index;
                                break;
                            };
                            case Cloude::Foundation::Query::Enumeration::ComparativeType::Greater: {
                                result += fptrSPtrPredicate->getColumn()->getDatasourceName();
                                result += " > ";
                                result += fptrParamProcessor(fptrSPtrPredicate->getColumn(), index);
                                ++index;
                                break;
                            };
                            case Cloude::Foundation::Query::Enumeration::ComparativeType::GreaterOrEqual: {
                                result += fptrSPtrPredicate->getColumn()->getDatasourceName();
                                result += " >= ";
                                result += fptrParamProcessor(fptrSPtrPredicate->getColumn(), index);
                                ++index;
                                break;
                            };
                            case Cloude::Foundation::Query::Enumeration::ComparativeType::Lesser: {
                                result += fptrSPtrPredicate->getColumn()->getDatasourceName();
                                result += " < ";
                                result += fptrParamProcessor(fptrSPtrPredicate->getColumn(), index);
                                ++index;
                                break;
                            };
                            case Cloude::Foundation::Query::Enumeration::ComparativeType::LesserOrEqual: {
                                result += fptrSPtrPredicate->getColumn()->getDatasourceName();
                                result += " <= ";
                                result += fptrParamProcessor(fptrSPtrPredicate->getColumn(), index);
                                ++index;
                                break;
                            };
                        }

                        return result;
                    };

                    std::string statement;
                    std::vector<SPtrPredicate> values;

                    auto const &columnsForSelect = entityMap.getColumnsForSelect();

                    // Prepare Columns segment
                    std::string strColumns;
                    strColumns.reserve(Foundation::BufferSize::EIGHTY);

                    std::for_each(columnsForSelect.cbegin(), columnsForSelect.cend(),
                                  [&strColumns](const std::shared_ptr<Column> &p) {
                                      if (strColumns.length() != 0)
                                          strColumns += ", ";

                                      strColumns += p->getDatasourceName();
                                  });

                    strColumns.shrink_to_fit();

                    // Prepare Conditions segment
                    int x = 0;
                    std::string strCondition = fptr(sptrPredicate, x);
                    strCondition.reserve(Foundation::BufferSize::EIGHTY);

                    int y = 0;
                    SPtrPredicateIterator iter(new PredicateIterator(sptrPredicate));
                    while (iter) {

                        if (!iter->isVisited() && iter->isLeaf()) {
                            values.push_back(iter->getPredicate());
                            ++y;
                        }

                        iter = iter->operator++();
                    }

                    strCondition.shrink_to_fit();

                    statement += " SELECT " + strColumns;
                    statement += " FROM " + entityMap.TableName();
                    statement += " WHERE " + strCondition;

                    return std::make_pair(statement, values);
                }

            }
        }
    }
}

