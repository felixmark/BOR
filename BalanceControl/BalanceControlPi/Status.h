#pragma once

enum class Status { ON, OFF };

bool status_to_bool(Status);
int status_to_highlow(Status);
Status int_to_status(int);