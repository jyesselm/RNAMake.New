//
// Created by Joseph Yesselman on 11/12/17.
//

#include <base/assertions.h>
#include <base/paths.h>

namespace base {

String
base_path() {

    char* base_path;
    base_path = std::getenv ("RNAMAKE_NEW");
    ensures<std::runtime_error>(
            base_path != NULL,
            "cannot find environemental path RNAMAKE_NEW");
    return String(base_path);
}

String
resources_path() {
    auto base_path_str = base_path();
    return base_path_str + "/resources/";
}


}