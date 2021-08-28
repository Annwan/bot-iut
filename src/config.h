#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <vector>
#include <string>
#include <iostream>

#include <sqlite3.h>

void setup_database();
std::vector<std::string> group_role_names();

#endif // __CONFIG_HPP__
