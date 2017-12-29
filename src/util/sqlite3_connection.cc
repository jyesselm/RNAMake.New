//
//  sqlite3_connection.cc
//  RNAMake
//
//  Created by Joseph Yesselman on 2/2/15.
//  Copyright (c) 2015 Joseph Yesselman. All rights reserved.
//

#include <string.h>
#include <fstream>

#include <util/sqlite3_connection.h>

namespace util {

/*


template<typename SqlitRowType>
Strings
Sqlite3Connection<SqlitRowType>::fetch_one(String const & query_statement) {
    query(query_statement);

    auto results = Strings();

    int col = sqlite3_column_count(stmt_);

    for (int i = 0; i < col; i++) {
        results.push_back(String(reinterpret_cast<const char *>(sqlite3_column_text(stmt_, i))));
    }

    sqlite3_finalize(stmt_);
    return results;

}*/

}

