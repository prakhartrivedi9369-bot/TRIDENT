#pragma once

#include "crow.h"
#include <string>
#include "AuditLogger.h"

using namespace std;

void AuditLogger(AuditEvent event,const string &email,const string &IP,AuditStatus status,const string &timestamp);