#pragma once
#include <string>

std::string generateBase32Secret();
std::string getTOTPCode(const std::string& secret);
int secondsUntilNextCode();
