/* Copyright 2013-2016 CyberTech Labs Ltd., Grigorii Zimin
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

#include "trikForwardOneCellBlock.h"
#include <utils/abstractTimer.h>
#include "utils/timelineInterface.h"

using namespace trik::blocks::details;
using namespace kitBase::robotModel::robotParts;

ForwardOneCellBlock::ForwardOneCellBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

void ForwardOneCellBlock::run() {
	const auto result = eval<int>("CellsNumber");
	if (!errorsOccured()) {
		mConnections << connect(&mRobotModel, &kitBase::robotModel::RobotModelInterface::endManual, this, &ForwardOneCellBlock::endMoving);
		emit mRobotModel.moveManually(result);
	}
}

void ForwardOneCellBlock::endMoving(bool success) {
	for (auto connection : mConnections) {
		disconnect(connection);
	}
	if (!success) emit warning(tr("Movement is impossible!"));
	emit done(mNextBlockId);
}
