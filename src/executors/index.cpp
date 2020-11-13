#include "global.h"

/**
 * @brief 
 * SYNTAX: INDEX ON column_name FROM relation_name USING indexing_strategy 
 *         [FANOUT <number_of_children_pointers>|BUCKETS <bucket_count>]
 * indexing_strategy: ASC | DESC | NOTHING
 * FANOUT: integer
 * BUCKETS: initial bucket count to start linear hash.
 */
bool syntacticParseINDEX()
{
    logger.log("syntacticParseINDEX");
    if (tokenizedQuery.size() != 9 || tokenizedQuery[1] != "ON" 
        || tokenizedQuery[3] != "FROM" || tokenizedQuery[5] != "USING"
        || (tokenizedQuery[7] != "FANOUT" && tokenizedQuery[7] != "BUCKETS")
        || not_digit(tokenizedQuery[8]))
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = INDEX;
    parsedQuery.indexColumnName = tokenizedQuery[2];
    parsedQuery.indexRelationName = tokenizedQuery[4];
    string indexingStrategy = tokenizedQuery[6];
    parsedQuery.indexInitialise = stoi(tokenizedQuery[8]);
    if (indexingStrategy == "BTREE")
        parsedQuery.indexingStrategy = BTREE;
    else if (indexingStrategy == "HASH")
        parsedQuery.indexingStrategy = HASH;
    else if (indexingStrategy == "NOTHING")
        parsedQuery.indexingStrategy = NOTHING;
    else
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    return true;
}

bool semanticParseINDEX()
{
    logger.log("semanticParseINDEX");
    if (!tableCatalogue.isTable(parsedQuery.indexRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }
    if (!tableCatalogue.isColumnFromTable(parsedQuery.indexColumnName, parsedQuery.indexRelationName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }
    Table* table = tableCatalogue.getTable(parsedQuery.indexRelationName);
    if(table->indexed){
        cout << "SEMANTIC ERROR: Table already indexed" << endl;
        return false;
    }
    return true;
}

void executeINDEX()
{
    logger.log("executeINDEX");
    Table* table = tableCatalogue.getTable(parsedQuery.indexRelationName);
    table->indexingStrategy = parsedQuery.indexingStrategy;
    table->indexedColumn = parsedQuery.indexColumnName;
    /* Create an instance of linear hash for this table 
    and initialize with given no. of buckets */
    if(table->indexingStrategy==HASH)
        table->index = static_cast<void *>(new Linearhash(2,parsedQuery.indexInitialise));
    // else
        /* bplus tree */
    table->buildIndex();
    return;
}