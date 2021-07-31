#pragma once

#include <string>

#ifdef NDEBUG
const std::string debugString ="";
#else
const std::string debugString = "[DEBUG]";
#endif

const std::string infoString = "Cloud Seed VST by Valdemar Eglingsson\nJUCE-based UI by Alex Nadzharov\nOriginal engine version: 1.0.1\nUI version: 0.0.3";// + debugString;
