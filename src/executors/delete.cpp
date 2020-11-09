#include "global.h"
/**
 * @brief 
 * SYNTAX: DELETE FROM <table_name> VALUES <value1>[,<value2>]*
 */
bool syntacticParseINSERT()
{
    logger.log("syntacticParseINSERT");
    if (tokenizedQuery.size() < 5 || *(tokenizedQuery.begin() + 1) != "INTO" || *(tokenizedQuery.begin() + 3) != "VALUES")
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = INSERT;
    parsedQuery.insertRelationName = tokenizedQuery[2];
    for (int i = 4; i < tokenizedQuery.size(); i++)
        parsedQuery.insertColumnList.emplace_back(tokenizedQuery[i]);
    return true;
} 

bool semanticParseINSERT()
{
    logger.log("semanticParseINSERT");

    if (tableCatalogue.isTable(parsedQuery.insertRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation doest't exists" << endl;
        return false;
    }

    Table table = *tableCatalogue.getTable(parsedQuery.insertRelationName);

    for (auto col : parsedQuery.insertColumnList)
    {
        if (!table.isColumn(col))
        {
            cout << "SEMANTIC ERROR: Column doesn't exist in relation";
            return false;
        }
    }
    return true;
}

void executeINSERTION(){
	
}