/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include "qrutils/interpreter/block.h"

namespace qReal {
namespace interpretation {
namespace blocks {

/// Represents a block of a type which is unsupported by an interpreter.
/// Interpretation of this block will cause an error.
class QRUTILS_EXPORT UnsupportedBlock : public Block
{
	Q_OBJECT

public:
	void run() override;
};

}
}
}
