/* Copyright 2016-2017 CyberTech Labs Ltd.
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

#include "trikKitInterpreterCommon/trikEmulation/trikkeysinterfacestub.h"

#include <kitBase/robotModel/commonRobotModel.h>
#include <kitBase/robotModel/robotModelUtils.h>
#include <kitBase/robotModel/robotParts/button.h>
#include <QsLog.h>

using namespace trik;
using namespace kitBase::robotModel;

TrikKeysInterfaceStub::TrikKeysInterfaceStub(const QSharedPointer<kitBase::robotModel::CommonRobotModel> &model)
	: mRobotModel(model)
{
	auto codes = mRobotModel->buttonCodes();
	for (auto it = codes.cbegin(); it != codes.cend(); ++it) {
		mKeycodeMap[it.value()] = it.key().left(it.key().length() - 6); // remove "Button" from the end
		mWasPressed[it.value()] = false;
	}
}

void TrikKeysInterfaceStub::init()
{
	auto codes = mRobotModel->buttonCodes();
	for (auto it = codes.cbegin(); it != codes.cend(); ++it) {
		registerButton(it.value());
	}
}

void TrikKeysInterfaceStub::start()
{
	init();
}

void TrikKeysInterfaceStub::reset() {
	for (auto &&button : mButtons) {
		button->read(); // hack to clear lastReadState
	}
	mWasPressed.clear();
}

bool TrikKeysInterfaceStub::wasPressed(int code)
{
	bool pressed = mWasPressed[code];
	mWasPressed[code] = false;
	return pressed;
}

bool TrikKeysInterfaceStub::isPressed(int code)
{
	return mButtons[code]->lastData();
}

int TrikKeysInterfaceStub::buttonCode(bool wait)
{
	Q_UNUSED(wait);
	return -1; /// @todo
}

void TrikKeysInterfaceStub::handleNewData(robotParts::Button *button, int value)
{
	int code = button->code();
	bool previousValue = mWasPressed[code];
	mWasPressed[code] = value;
	if (previousValue != value) {
		emit buttonPressed(code, value);
	}
}

bool TrikKeysInterfaceStub::registerButton(int code)
{
	if (!mButtons.contains(code)) {
		robotParts::Button * button =
				RobotModelUtils::findDevice<robotParts::Button>(*mRobotModel, mKeycodeMap[code]);
		if (button == nullptr) {
			/// TODO: propogate errors to trikbrick
			QLOG_FATAL() << "button not found for" << code << "/" << hex << code;
			//emit error(tr("No configured sensor on port: ") + port);
			return false;
		}

		mButtons[code] = button;
		using namespace std::placeholders;
		connect(button, &robotParts::Button::newData,
				this, std::bind(&TrikKeysInterfaceStub::handleNewData, this, button
								, std::bind(&QVariant::value<int>, _1))
				, Qt::UniqueConnection);
	}

	return true;
}
