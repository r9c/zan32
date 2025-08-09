#pragma once
#include <string>

std::string generateTOTP(const std::string& secret, int interval = 30);
std::string getTOTPCode(const std::string& secret);
int secondsUntilNextCode();
