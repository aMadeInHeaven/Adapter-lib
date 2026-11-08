#pragma once

#include <optional>

#include "Pipe.h"
#include "Adapters/AsVector.h"
#include "Adapters/Transform.h"
#include "Adapters/AsDataFlow.h"
#include "Adapters/Filter.h"
#include "Adapters/DropNullopt.h"
#include "Adapters/Dir.h"
#include "Adapters/Out.h"
#include "Adapters/Write.h"
#include "Adapters/Split.h"
#include "Adapters/SplitExpected.h"
#include "Adapters/OpenFiles.h"
#include "Adapters/JoinResult.h"
#include "Adapters/Join.h"
#include "Adapters/AggregateByKey.h"
#include "Adapters/Take.h"
#include "Adapters/Enumerate.h"
