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
        parsedQuery.record.emplace_back(stoi(tokenizedQuery[i]));
    return true;
} 

bool semanticParseDELETE()
{
    logger.log("semanticParseDELETE");

    if (!tableCatalogue.isTable(parsedQuery.deleteRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exists" << endl;
        return false;
    }

    Table table = *tableCatalogue.getTable(parsedQuery.deleteRelationName);

    if(table.columnCount != parsedQuery.record.size()){
        cout<<" SEMANTIC ERROR: Relation column count not equal to no. of values."<<endl;
    }
    return true;
}

void executeDELETE(){
	
}