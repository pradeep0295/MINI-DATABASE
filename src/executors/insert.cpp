#include "global.h"
/**
 * @brief 
 * SYNTAX: INSERT INTO <table_name> VALUES <value1>[,<value2>]*
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
