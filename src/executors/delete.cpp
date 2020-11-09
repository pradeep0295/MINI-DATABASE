#include "global.h"
/**
 * @brief 
 * SYNTAX: DELETE FROM <table_name> VALUES <value1>[,<value2>]*
 */
bool syntacticParseDELETE()
{
    logger.log("syntacticParseDELETE");
    if (tokenizedQuery.size() < 5 || *(tokenizedQuery.begin() + 1) != "FROM" || *(tokenizedQuery.begin() + 3) != "VALUES")
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = DELETE;
    parsedQuery.deleteRelationName = tokenizedQuery[2];
    for (int i = 4; i < tokenizedQuery.size(); i++)
        parsedQuery.insertColumnList.emplace_back(tokenizedQuery[i]);
    return true;
} 

bool semanticParseDELETE()
{
    logger.log("semanticParseDELETE");

    if (tableCatalogue.isTable(parsedQuery.deleteRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation doest't exists" << endl;
        return false;
    }

    Table table = *tableCatalogue.getTable(parsedQuery.deleteRelationName);

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

void executeDELETION(){
	
}